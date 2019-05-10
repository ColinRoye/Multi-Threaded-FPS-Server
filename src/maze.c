





 extern void maze_init(char **template);
 extern void maze_fini();
 extern int maze_get_rows();
 extern int maze_get_cols();
 extern int maze_set_player(OBJECT avatar, int row, int col);
 extern int maze_set_player_random(OBJECT avatar, int *rowp, int *colp);
 extern void maze_remove_player(OBJECT avatar, int row, int col);
 extern int maze_move(int row, int col, int dir);
 extern OBJECT maze_find_target(int row, int col, DIRECTION dir);
 extern int maze_get_view(VIEW *view, int row, int col, DIRECTION gaze, int depth);
 extern void show_view(VIEW *view, int depth);
 extern void show_maze();

int ROWS;
int COLS
OBJECT** maze;
pthread_mutex_t mutex;

void maze_init(char **template){
	pthread_mutex_init(&mutex, NULL);
	COLS = strlen(*template);
	char** temp = template;

	for(ROWS = 0; temp != NULL ;temp++, ROWS++);


	maze = Malloc(ROWS);
	for(i = 0; i < ROWS; i++){
		maze[i] = Malloc(COLS);
		strcpy(maze[i], template[i]);
	}
}

/*
* Finalize the maze.
* This should be called when the maze is no longer required.
*/
void maze_fini(){
	pthread_mutex_destroy(&mutex);
	for(int i = 0; i < ROWS; i++){
		free(maze[i]);
	}
	free(maze);
}

/*
* Get the number of rows of the maze.
*
* @return the number of rows of the maze.
*/
int maze_get_rows(){
	return ROWS;
}

/*
* Get the number of columns of the maze.
*
* @return the number of columns of the maze.
*/
int maze_get_cols(){
	return COLS;
}

/*
* Place a player's avatar in the maze at a specified row/column location.
*
* @param avatar  The avatar to be placed in the maze.
* @param row  The row in which the avatar is to be placed.
* @param col  The column in which the avatar is to be placed.
* @return zero if the placement was successful, nonzero otherwise.
*
* Unsuccessful placement will occur if the specified location in the maze
* is not empty.  If placement is successful, then the views of all
* players will be updated.
*/
int maze_set_player(OBJECT avatar, int row, int col){
	//fail if not EMPTY
	pthread_mutex_lock(&mutex);
	if(!IS_EMPTY(maze[row][col])){
		pthread_mutex_unlock(&mutex);
		return -1;
		//*****Success update views of all players
	}
	maze[row][col] = avatar;
	pthread_mutex_unlock(&mutex);
	return 0;

	//maze[i][j] = avatar;
}

/*
* Place a player's avatar in the maze at a random unoccupied location.
*
* @param avatar  The avatar to be placed in the maze.
* @param row  Pointer to a variable into which will be stored the row
* at which the avatar was placed.
* @param col  Pointer to a variable into which will be stored the column
* at which the avatar was placed.
* @return zero if the placement was successful, nonzero otherwise.
*
* The placement can fail if after a large number of attempts an unoccupied
* location has not been found.  If placement is successful, then the views
* of all players will be updated.
*/
int maze_set_player_random(OBJECT avatar, int *rowp, int *colp){
	int MAX_ATTEMPTS = ROWS*COLS;
	for(int i = 0; i < MAX_ATTEMPTS; i++){
		int ind = (int)(rand(1)*rows*cols);
	 	int row = (int)ind/ROWS;
	 	int col = ind%COLS;
		if(!maze_set_player(avatar, row, col)){
			*rowp = row;
			*colp = col;
			return 0;
		}
	}
	return -1
}

/*
* Remove a specified player's avatar from a specified location in the maze.
*
* @param avatar  The avatar to be removed from the maze.
* @param row  The row from which the avatar is to be removed.
* @param col  The column from which the avatar is to be removed.
*
* The views of all players are updated after the removal has been performed.
*/
void maze_remove_player(OBJECT avatar, int row, int col){

	pthread_mutex_lock(&lock);
	maze[row][col] = EMPTY;
	pthread_mutex_unlock(&lock);

}

/*
* Attempt to move a player's avatar at a specified location one unit
* of distance in a specified direction.
*
* @param row  The row at which the avatar to be moved is located.
* @param col  The column at which the avatar to be moved is located.
* @param dir  The direction in which the avatar is to be moved.
* @return zero if movement was successful, nonzero otherwise.
*
* Movement is not possible if it would cause the avatar to occupy
* a location already occupied by some other object, or if it would
* result in moving outside the bounds of the maze.
* If movement is successful, then the views of all players are updated.
*/
int maze_move(int row, int col, int dir){
	if(dir == NORTH)
		return maze_set_player(row-1, col);
	if(dir == SOUTH)
		return maze_set_player(row+1, col);
	if(dir == EAST)
		return maze_set_player(row, col+1);
	if(dir == WEST)
		return maze_set_player(row, col-1);

	return -1;


}

/*
* Search from a specified target location in a specified direction,
* and return the first avatar, if any, that is found.
*
* @param row  The starting row for the search.
* @param col  The starting column for the search.
* @param dir  The direction for the search.
* @return the first avatar found, or EMPTY if the search terminated
* without finding an avatar.
*
* The search terminates when a non-empty location is reached,
* or the search would go beyond the maze boundaries.
*/
OBJECT maze_find_target(int row, int col, DIRECTION dir){
	pthread_mutex_lock(&lock);

	while(IS_EMPTY(maze[row][col]) && (row >= 0 && row < ROWS) && (col >= 0 && col < COLS)){
		if(dir == NORTH)
			row = row-1;
		if(dir == SOUTH)
		 	row = row+1;
		if(dir == EAST)
			col = col+1;
		if(dir == WEST)
			col = col-1;
		if((row >= 0 && row < ROWS) && (col >= 0 && col < COLS) && IS_AVATAR(maze[row][col])){
			pthread_mutex_unlock(&lock);
			return maze[row][col];
		}
	}
	pthread_mutex_unlock(&lock);

	return EMPTY;
}

/*
* Get the view from a specified location in the maze, with the gaze
* in a specified direction.
*
* @param view  A pointer to a view of maximum depth that is to be filled
* in as a result of the call.
* @param row  Row of the maze that contains the view origin.
* @param col  Column of the maze that contains the view origin.
* @param gaze  Direction of gaze for the view.
* @param depth  Maximum depth of the view.  This must be less than or
* equal to the depth of the view that is passed.
* @return the depth to which the view was filled in.
*
* The view array is populated with a "patch" of the maze contents,
* as described above.  The returned value could be less than the
* maximum depth, as described above.  Entries of the view at depths
* greater than the returned depth should be regarded as invalid.
*/

// #define VIEW_DEPTH 16    // Maximum view depth
//
// #define LEFT_WALL 0
// #define CORRIDOR 1
// #define RIGHT_WALL 2
int isValid(int row, int col){
	if(row >= 0 && row < ROWS) && (col >= 0 && col < COLS)
		return 1;
	return 0;
}
int maze_get_view(VIEW *view, int row, int col, DIRECTION gaze, int depth){
	int i = 0;
	for(i = 0; i < depth && depth < VIEW_DEPTH; i++){
		if(gaze == NORTH){
			if(isValid(row, col))
				view[i][CORRIDOR] = maze[row][col]
			if(isValid(row, col-1))
				view[i][LEFT_WALL] = maze[row][col-1]
			if(isValid(row, col+1))
				view[i][RIGHT_WALL] = maze[row][col+1]
			row = row-1;
		}
		if(gaze == SOUTH){
			if(isValid(row, col))
				view[i][CORRIDOR] = maze[row][col]
			if(isValid(row, col+1))
				view[i][LEFT_WALL] = maze[row][col+1]
			if(isValid(row, col-1))
				view[i][RIGHT_WALL] = maze[row][col-1]
			row = row+1;
		}
		if(gaze == EAST){
			if(isValid(row, col))
				view[i][CORRIDOR] = maze[row][col]
			if(isValid(row, col+1))
				view[i][LEFT_WALL] = maze[row-1][col]
			if(isValid(row, col-1))
				view[i][RIGHT_WALL] = maze[row+1][col]
			col = col+1;
		}
		if(gaze == WEST){
			if(isValid(row, col))
				view[i][CORRIDOR] = maze[row][col]
			if(isValid(row, col-1))
				view[i][LEFT_WALL] = maze[row+1][col]
			if(isValid(row, col+1))
				view[i][RIGHT_WALL] = maze[row-1][col]
			col = col-1;
		}
		if(IS_WALL(view[i][CORRIDOR])){
			break;
		}

	}
	return i;
}

/*
* Print a view on stderr, for debugging.
*
* @param view  A pointer to the view to be shown.
* @param depth  Depth to which the view should be shown.
*/
void show_view(VIEW *view, int depth){

}

/*
* Print the maze on stderr, for debugging.
*/
void show_maze(){

}