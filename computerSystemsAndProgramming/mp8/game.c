#include "game.h"

/*
Intro:
This file contains the functions make_game, remake_game, get_cell, move_w, move_s, move_a, move_d, and legal_move_check. 
make_game is used to create a game struct and assign the values within based on the inputted row and col values while
assigning -1 to each cell to start the game. remake_game takes an existing game struct and resets given the inputted 
row and col values while setting each cell to -1. get_cell is used to access a given cell's pointer if the inputted 
row and col is in bounds of the games board. move_w through move_d are used to shift the board in a given direction
to progress the game and combine cells. legal_move_check is used to check whether any moves are possible given the 
current game board, if there are no moves left 0 is returned.
*/

game * make_game(int rows, int cols)
/*! Create an instance of a game structure with the given number of rows
    and columns, initializing elements to -1 and return a pointer
    to it. (See game.h for the specification for the game data structure) 
    The needed memory should be dynamically allocated with the malloc family
    of functions.
*/
{
    //Dynamically allocate memory for game and cells (DO NOT modify this)
    game * mygame = malloc(sizeof(game));
    mygame->cells = malloc(rows*cols*sizeof(cell));

    //YOUR CODE STARTS HERE:  Initialize all other variables in game struct
    //assigns the row number and col number
    mygame->rows = rows;
    mygame->cols = cols;

    //loops through the cells and assigns each to a value of -1
    for(int i=0; i<(rows*cols); i++){
        mygame->cells[i] = -1;
    }

    return mygame;
}

void remake_game(game ** _cur_game_ptr,int new_rows,int new_cols)
/*! Given a game structure that is passed by reference, change the
	game structure to have the given number of rows and columns. Initialize
	the score and all elements in the cells to -1. Make sure that any 
	memory previously allocated is not lost in this function.	
*/
{
	/*Frees dynamically allocated memory used by cells in previous game,
	 then dynamically allocates memory for cells in new game.  DO NOT MODIFY.*/
	free((*_cur_game_ptr)->cells);
	(*_cur_game_ptr)->cells = malloc(new_rows*new_cols*sizeof(cell));

	 //YOUR CODE STARTS HERE:  Re-initialize all other variables in game struct
     //uses the pointer instead of creating a new game
    (*_cur_game_ptr)->rows = new_rows;
    (*_cur_game_ptr)->cols = new_cols;

    //loops through the cells and assigns each to -1
    for(int i=0; i<(new_rows*new_cols); i++){
        (*_cur_game_ptr)->cells[i] = -1;
    }

	return;	
}

void destroy_game(game * cur_game)
/*! Deallocate any memory acquired with malloc associated with the given game instance.
    This includes any substructures the game data structure contains. Do not modify this function.*/
{
    free(cur_game->cells);
    free(cur_game);
    cur_game = NULL;
    return;
}

cell * get_cell(game * cur_game, int row, int col)
/*! Given a game, a row, and a column, return a pointer to the corresponding
    cell on the game. (See game.h for game data structure specification)
    This function should be handy for accessing game cells. Return NULL
	if the row and col coordinates do not exist.
*/
{
    //YOUR CODE STARTS HERE
    //checks if the row and col is in bounds of the game board
   if(row>=0 && row<cur_game->rows && col>=0 && col<cur_game->cols){
    //if they are, the pointer to the location requested is returned
     return cur_game->cells + (row*(cur_game->cols)+col) ;
   } 

    return NULL;
}

int move_w(game * cur_game)
/*!Slides all of the tiles in cur_game upwards. If a tile matches with the 
   one above it, the tiles are merged by adding their values together. When
   tiles merge, increase the score by the value of the new tile. A tile can 
   not merge twice in one turn. If sliding the tiles up does not cause any 
   cell to change value, w is an invalid move and return 0. Otherwise, return 1. 
*/
{
    //YOUR CODE STARTS HERE
    //var to determine if the board changes
    int change=0;
    //loops through the board starting from top of each col
    for (int n=0; n<cur_game->cols; n++){
        for (int m=0; m<cur_game->rows; m++){
            //if the location being checked is empty, the next occupied location is moved into that spot
            if(cur_game->cells[m*(cur_game->cols)+n] != -1){
                int min=m;
                //loops through the remaining locations in the col
                for (int i=m; i>=0; i--){
                    //if a location higher up is found to have an empty spot, the min is replaced
                    if(cur_game->cells[i*(cur_game->cols)+n] == -1){
                        min=i;
                    }
                }
                //if a better location was found, the value is set and the old location is set to empty
                if(min<m){
                    cur_game->cells[min*(cur_game->cols)+n] = cur_game->cells[m*(cur_game->cols)+n];
                    cur_game->cells[m*(cur_game->cols)+n] = -1;
                    //since a change was made, the change counter is incremented
                    change++;
                }
            }

        }
    }

    //loops through the board to find whether any spots need to be combined
    for (int n=0; n<cur_game->cols; n++){
        for (int m=0; m<cur_game->rows; m++){
            //if two locations that were pushed together have the same value and exist, they are combined and put into one location
            if(cur_game->cells[m*(cur_game->cols)+n] != -1 && get_cell(cur_game, m+1, n) != NULL && cur_game->cells[m*(cur_game->cols)+n] == cur_game->cells[(m+1)*(cur_game->cols)+n]){
                *get_cell(cur_game, m, n) = *get_cell(cur_game, m, n) * 2;
                *get_cell(cur_game, m+1, n) = -1;
                //since a point was scored, the score is added to and the change counter is incremented
		        cur_game->score+=*get_cell(cur_game, m, n);
			    change++;
            }

        }
    }        


    //loops through the board starting from top of each col
    for (int n=0; n<cur_game->cols; n++){
        for (int m=0; m<cur_game->rows; m++){
            //if the location being checked is empty, the next occupied location is moved into that spot
            if(cur_game->cells[m*(cur_game->cols)+n] != -1){
                int min=m;
                //loops through the remaining locations in the col
                for (int i=m; i>=0; i--){
                    //if a location higher up is found to have an empty spot, the min is replaced
                    if(cur_game->cells[i*(cur_game->cols)+n] == -1){
                        min=i;
                    }
                }
                //if a better location was found, the value is set and the old location is set to empty
                if(min<m){
                    cur_game->cells[min*(cur_game->cols)+n] = cur_game->cells[m*(cur_game->cols)+n];
                    cur_game->cells[m*(cur_game->cols)+n] = -1;
                    //since a change was made, the change counter is incremented
                    change++;
                }
            }

        }
    }
    //if no changes were made, 0 is returned
    if(change == 0){
      return 0;
    }
    return 1;
};

int move_s(game * cur_game) //slide down
{
    //YOUR CODE STARTS HERE
    //var to determine if the board changes
    int change=0;
    //loops through the board starting from bottom of each col
    for (int n=0; n<cur_game->cols; n++){
      for (int m=cur_game->rows-1; m>=0; m--){
            //if the location being checked is empty, the next occupied location is moved into that spot
            if(cur_game->cells[m*(cur_game->cols)+n] != -1){
                int max=m;
                //loops through the remaining locations in the col
                for (int i=m; i<cur_game->cols; i++){
                    //if a location lower down is found to have an empty spot, the max is replaced
                    if(cur_game->cells[i*(cur_game->cols)+n] == -1){
                        max=i;
                    }
                }
                //if a better location was found, the value is set and the old location is set to empty
                if(max>m){
                    cur_game->cells[max*(cur_game->cols)+n] = cur_game->cells[m*(cur_game->cols)+n];
                    cur_game->cells[m*(cur_game->cols)+n] = -1;
                    //since a change was made, the change counter is incremented
                    change++;
                }
            }

        }
    }

    //loops through the board to find whether any spots need to be combined
    for (int n=cur_game->cols-1; n>=0; n--){
        for (int m=cur_game->rows-1; m>=0; m--){
            //if two locations that were pushed together have the same value and exist, they are combined and put into one location
	        if(cur_game->cells[m*(cur_game->cols)+n] != -1 && get_cell(cur_game, m-1, n) != NULL && cur_game->cells[m*(cur_game->cols)+n] == cur_game->cells[(m-1)*(cur_game->cols)+n]){
                *get_cell(cur_game, m, n) = *get_cell(cur_game, m, n) * 2;
                *get_cell(cur_game, m-1, n) = -1;
                //since a point was scored, the score is added to and the change counter is incremented
		        cur_game->score+=*get_cell(cur_game, m, n);
			    change++;
            }

        }
    }        


    //loops through the board starting from bottom of each col
    for (int n=0; n<cur_game->cols; n++){
      for (int m=cur_game->rows-1; m>=0; m--){
            //if the location being checked is empty, the next occupied location is moved into that spot
            if(cur_game->cells[m*(cur_game->cols)+n] != -1){
                int max=m;
                //loops through the remaining locations in the col
                for (int i=m; i<cur_game->cols; i++){
                    //if a location lower down is found to have an empty spot, the max is replaced
                    if(cur_game->cells[i*(cur_game->cols)+n] == -1){
                        max=i;
                    }
                }
                //if a better location was found, the value is set and the old location is set to empty
                if(max>m){
                    cur_game->cells[max*(cur_game->cols)+n] = cur_game->cells[m*(cur_game->cols)+n];
                    cur_game->cells[m*(cur_game->cols)+n] = -1;
                    //since a change was made, the change counter is incremented
                    change++;
                }
            }

        }
    }
    if(change == 0){
      return 0;
    }
    return 1;
};

int move_a(game * cur_game) //slide left
{
    //YOUR CODE STARTS HERE
    //var to determine if the board changes
    int change=0;
    //loops through the board starting from left of each row
    for (int n=0; n<cur_game->rows; n++){
        for (int m=0; m<cur_game->cols; m++){
            //if the location being checked is empty, the next occupied location is moved into that spot
            if(cur_game->cells[n*(cur_game->cols)+m] != -1){
                int min=m;
                //loops through the remaining locations in the row
                for (int i=m; i>=0; i--){
                    //if a location lower is found to have an empty spot, the min is replaced
                    if(cur_game->cells[n*(cur_game->cols)+i] == -1){
                        min=i;
                    }
                }
                //if a better location was found, the value is set and the old location is set to empty
                if(min<m){
                    cur_game->cells[n*(cur_game->cols)+min] = cur_game->cells[n*(cur_game->cols)+m];
                    cur_game->cells[n*(cur_game->cols)+m] = -1;
                    //since a change was made, the change counter is incremented
                    change++;
                }
            }

        }
    }

    //loops through the board to find whether any spots need to be combined
    for (int n=0; n<cur_game->rows; n++){
        for (int m=0; m<cur_game->cols; m++){
            //if two locations that were pushed together have the same value and exist, they are combined and put into one location
            if(*get_cell(cur_game, n, m) != -1 && get_cell(cur_game, n, m+1) != NULL && *get_cell(cur_game, n, m) == *get_cell(cur_game, n, m+1)){
                *get_cell(cur_game, n, m) = *get_cell(cur_game, n, m) * 2;
                *get_cell(cur_game, n, m+1) = -1;
                //since a point was scored, the score is added to and the change counter is incremented
		        cur_game->score+=*get_cell(cur_game, n, m);
			    change++;
            }

        }
    }        


    //loops through the board starting from left of each row
    for (int n=0; n<cur_game->rows; n++){
        for (int m=0; m<cur_game->cols; m++){
            //if the location being checked is empty, the next occupied location is moved into that spot
            if(cur_game->cells[n*(cur_game->cols)+m] != -1){
                int min=m;
                //loops through the remaining locations in the row
                for (int i=m; i>=0; i--){
                    //if a location lower is found to have an empty spot, the min is replaced
                    if(cur_game->cells[n*(cur_game->cols)+i] == -1){
                        min=i;
                    }
                }
                //if a better location was found, the value is set and the old location is set to empty
                if(min<m){
                    cur_game->cells[n*(cur_game->cols)+min] = cur_game->cells[n*(cur_game->cols)+m];
                    cur_game->cells[n*(cur_game->cols)+m] = -1;
                    //since a change was made, the change counter is incremented
                    change++;
                }
            }

        }
    }
    if(change == 0){
      return 0;
    }
    return 1;
};

int move_d(game * cur_game){ //slide to the right
    //YOUR CODE STARTS HERE
    //var to determine if the board changes
    int change=0;
     //loops through the board starting from right of each row
    for (int n=0; n<cur_game->rows; n++){
      for (int m=cur_game->cols-1; m>=0; m--){
            //if the location being checked is empty, the next occupied location is moved into that spot
            if(cur_game->cells[n*(cur_game->cols)+m] != -1){
                int max=m;
                //loops through the remaining locations in the row
                for (int i=m; i<cur_game->cols; i++){
                    //if a location higher is found to have an empty spot, the max is replaced
                    if(cur_game->cells[n*(cur_game->cols)+i] == -1){
                        max=i;
                    }
                }
                //if a better location was found, the value is set and the old location is set to empty
                if(max>m){
                    cur_game->cells[n*(cur_game->cols)+max] = cur_game->cells[n*(cur_game->cols)+m];
                    cur_game->cells[n*(cur_game->cols)+m] = -1;
                    //since a change was made, the change counter is incremented
                    change++;
                }
            }

        }
    }

    //loops through the board starting from left of each row
    for (int n=0; n<cur_game->rows; n++){
      for (int m=cur_game->cols-1; m>=0; m--){
            //if two locations that were pushed together have the same value and exist, they are combined and put into one location
            if(*get_cell(cur_game, n, m) != -1 && get_cell(cur_game, n, m-1) != NULL && *get_cell(cur_game, n, m) == *get_cell(cur_game, n, m-1)){
                *get_cell(cur_game, n, m) = *get_cell(cur_game, n, m) * 2;
                *get_cell(cur_game, n, m-1) = -1;
                //since a point was scored, the score is added to and the change counter is incremented
		        cur_game->score+=*get_cell(cur_game, n, m);
			    change++;
            }

        }
    }        


     //loops through the board starting from right of each row
    for (int n=0; n<cur_game->rows; n++){
      for (int m=cur_game->cols-1; m>=0; m--){
            //if the location being checked is empty, the next occupied location is moved into that spot
            if(cur_game->cells[n*(cur_game->cols)+m] != -1){
                int max=m;
                //loops through the remaining locations in the row
                for (int i=m; i<cur_game->cols; i++){
                    //if a location higher is found to have an empty spot, the max is replaced
                    if(cur_game->cells[n*(cur_game->cols)+i] == -1){
                        max=i;
                    }
                }
                //if a better location was found, the value is set and the old location is set to empty
                if(max>m){
                    cur_game->cells[n*(cur_game->cols)+max] = cur_game->cells[n*(cur_game->cols)+m];
                    cur_game->cells[n*(cur_game->cols)+m] = -1;
                    //since a change was made, the change counter is incremented
                    change++;
                }
            }

        }
    }
    if(change == 0){
      return 0;
    }

    return 1;
};

int legal_move_check(game * cur_game)
/*! Given the current game check if there are any legal moves on the board. There are
    no legal moves if sliding in any direction will not cause the game to change.
	Return 1 if there are possible legal moves, 0 if there are none.
 */
{
    //YOUR CODE STARTS HERE
    //creates 4 deep copies of the current game board, one for each test
    int r = cur_game->rows;
    int c = cur_game->cols;
    game * dcopyw = make_game(r, c);
    game * dcopys = make_game(r, c);
    game * dcopya = make_game(r, c);
    game * dcopyd = make_game(r, c);

    //the cells are looped through to copy each value
     for (int i=0; i<(cur_game->rows*cur_game->cols); i++){
        int v=cur_game->cells[i];
        dcopyw->cells[i] = v;
        dcopys->cells[i] = v;
        dcopya->cells[i] = v;
        dcopyd->cells[i] = v;
     }
    
    //result of each shift is saved by its corresponding var
    int w, s, a, d;
    w = move_w(dcopyw);
    s = move_s(dcopys);
    a = move_a(dcopya);
    d = move_d(dcopyd);

    //if all of the shifts failed, it means that there are no more moves left, so 0 is returned
    if (w==0 && s==0 && a==0 && d==0){
        return 0;
    }
    return 1;
}


/*! code below is provided and should not be changed */

void rand_new_tile(game * cur_game)
/*! insert a new tile into a random empty cell. First call rand()%(rows*cols) to get a random value between 0 and (rows*cols)-1.
*/
{
	
	cell * cell_ptr;
    cell_ptr = 	cur_game->cells;
	
    if (cell_ptr == NULL){ 	
        printf("Bad Cell Pointer.\n");
        exit(0);
    }
	
	
	//check for an empty cell
	int emptycheck = 0;
	int i;
	
	for(i = 0; i < ((cur_game->rows)*(cur_game->cols)); i++){
		if ((*cell_ptr) == -1){
				emptycheck = 1;
				break;
		}		
        cell_ptr += 1;
	}
	if (emptycheck == 0){
		printf("Error: Trying to insert into no a board with no empty cell. The function rand_new_tile() should only be called after tiles have succesfully moved, meaning there should be at least 1 open spot.\n");
		exit(0);
	}
	
    int ind,row,col;
	int num;
    do{
		ind = rand()%((cur_game->rows)*(cur_game->cols));
		col = ind%(cur_game->cols);
		row = ind/cur_game->cols;
    } while ( *get_cell(cur_game, row, col) != -1);
        //*get_cell(cur_game, row, col) = 2;
	num = rand()%20;
	if(num <= 1){
		*get_cell(cur_game, row, col) = 4; // 1/10th chance
	}
	else{
		*get_cell(cur_game, row, col) = 2;// 9/10th chance
	}
}

int print_game(game * cur_game) 
{
    cell * cell_ptr;
    cell_ptr = 	cur_game->cells;

    int rows = cur_game->rows;
    int cols = cur_game->cols;
    int i,j;
	
	printf("\n\n\nscore:%d\n",cur_game->score); 
	
	
	printf("\u2554"); // topleft box char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // top box char
	printf("\u2557\n"); //top right char 
	
	
    for(i = 0; i < rows; i++){
		printf("\u2551"); // side box char
        for(j = 0; j < cols; j++){
            if ((*cell_ptr) == -1 ) { //print asterisks
                printf(" **  "); 
            }
            else {
                switch( *cell_ptr ){ //print colored text
                    case 2:
                        printf("\x1b[1;31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4:
                        printf("\x1b[1;32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8:
                        printf("\x1b[1;33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 16:
                        printf("\x1b[1;34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 32:
                        printf("\x1b[1;35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 64:
                        printf("\x1b[1;36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 128:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 256:
                        printf("\x1b[32m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 512:
                        printf("\x1b[33m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 1024:
                        printf("\x1b[34m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 2048:
                        printf("\x1b[35m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 4096:
                        printf("\x1b[36m%04d\x1b[0m ",(*cell_ptr));
                        break;
                    case 8192:
                        printf("\x1b[31m%04d\x1b[0m ",(*cell_ptr));
                        break;
					default:
						printf("  X  ");

                }

            }
            cell_ptr++;
        }
	printf("\u2551\n"); //print right wall and newline
    }
	
	printf("\u255A"); // print bottom left char
	for(i = 0; i < cols*5;i++)
		printf("\u2550"); // bottom char
	printf("\u255D\n"); //bottom right char
	
    return 0;
}

int process_turn(const char input_char, game* cur_game) //returns 1 if legal move is possible after input is processed
{ 
	int rows,cols;
	char buf[200];
	char garbage[2];
    int move_success = 0;
	
    switch ( input_char ) {
    case 'w':
        move_success = move_w(cur_game);
        break;
    case 'a':
        move_success = move_a(cur_game);
        break;
    case 's':
        move_success = move_s(cur_game);
        break;
    case 'd':
        move_success = move_d(cur_game);
        break;
    case 'q':
        destroy_game(cur_game);
        printf("\nQuitting..\n");
        return 0;
        break;
	case 'n':
		//get row and col input for new game
		dim_prompt: printf("NEW GAME: Enter dimensions (rows columns):");
		while (NULL == fgets(buf,200,stdin)) {
			printf("\nProgram Terminated.\n");
			return 0;
		}
		
		if (2 != sscanf(buf,"%d%d%1s",&rows,&cols,garbage) ||
		rows < 0 || cols < 0){
			printf("Invalid dimensions.\n");
			goto dim_prompt;
		} 
		
		remake_game(&cur_game,rows,cols);
		
		move_success = 1;
		
    default: //any other input
        printf("Invalid Input. Valid inputs are: w, a, s, d, q, n.\n");
    }

	
	
	
    if(move_success == 1){ //if movement happened, insert new tile and print the game.
         rand_new_tile(cur_game); 
		 print_game(cur_game);
    } 

    if( legal_move_check(cur_game) == 0){  //check if the newly spawned tile results in game over.
        printf("Game Over!\n");
        return 0;
    }
    return 1;
}
