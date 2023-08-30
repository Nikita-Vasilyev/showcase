#include <stdio.h>
#include <stdlib.h>
#include "maze.h"

/*
Intro: this file includes the following functions; createMaze, destroyMaze, printMaze, and solveMazeDFS.
For createMaze, given the name of the file containing the maze, this function allocates memory for the 
maze_t structure and the cells. This function then parses the given file and fills in all the parameters 
of the maze structure, including the cells parameter. This function returns a pointer to the maze 
structure that was allocated and filled. For destroyMaze, given a pointer to a maze structure this 
function frees all memory associated with the maze. For printMaze, Given a structure that 
represents a maze, it prints out the maze in a human readable format to the console. For solveMazeDFS,
given a maze structure, row, and col, this recursive function will attempt to find a solution for 
the given maze. If the maze is unsolvable this function returns 0 otherwise it returns 1.
*/

/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None 
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */
maze_t * createMaze(char * fileName)
{
    // Your code here. Make sure to replace following line with your own code.
    //opens the designated maze file and initializes the maze_t structure
    FILE* file = fopen(fileName, "r");
    maze_t* maze;
    maze = (maze_t *)malloc(sizeof(maze_t));

    //the two dimensions are extracted from the file and assigend to the struct
    fscanf(file, "%d %d", &maze->height, &maze->width);
    int row = maze->height; int col = maze->width; char a;

    //allocates spaces for the maze cells
    maze->cells = (char **)malloc(sizeof(char *)*row);
    for(int i=0; i<col; i++){
      maze->cells[i] = (char *)malloc(col*sizeof(char));
    }

    //removes the first new line
    fscanf(file, "%c", &a);
    
    //loops through all the rows and cols of the maze and assigns them to the 
    //corresponding cell in maze
    for(int i=0; i<row; i++){
        for(int j=0; j<col; j++){
            //extracts one character at a time while looking for the start and end
	        fscanf(file, "%c", &maze->cells[i][j]);
	        if(maze->cells[i][j] == START){;
                maze->startColumn = j;
                maze->startRow = i;
            }if(maze->cells[i][j] == END){
                maze->endColumn = j;
                maze->endRow = i;
            }
        }
    //skips the new line char at the end of each row
	fscanf(file, "%c", &a);
    }

    fclose(file);
    return maze;
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information 
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */
void destroyMaze(maze_t * maze)
{
    // Your code here.
    //loops through each col and deallocates them
    for(int i=0; i<maze->width; i++){
      free(maze->cells[i]);
    }
    //deallocates the remainder of the maze struct
    free(maze->cells);
    free(maze);
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information 
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */
void printMaze(maze_t * maze)
{
    // Your code here.
    //loops through the maze and prints the character at each location
    for(int i=0; i<maze->height; i++){
        for(int j=0; j<maze->width; j++){
            printf("%c", maze->cells[i][j]);
        }
        //at the end of each row a new line is printed
        printf("\n");
    }
}

/*
 * solveMazeManhattanDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */ 
int solveMazeDFS(maze_t * maze, int col, int row)
{
    // Your code here. Make sure to replace following line with your own code.
    //base cases
    //if the (col, row) is out of bounds return 0
    if(col>=maze->width || col<0 || row>=maze->height || row<0){
        return 0;
    }//if the end is found the start is reverted to S and 1 is returned
    if(maze->cells[row][col] == END){
        maze->cells[maze->startRow][maze->startColumn] = START;
        return 1;
    }//if the cell is anything other than empty or start, return 0
    if(maze->cells[row][col] == WALL || maze->cells[row][col] == PATH || maze->cells[row][col] == VISITED){
        return 0;
    }

    //sets (col, row) as part of the solution path in the maze
    maze->cells[row][col] = PATH;

    //checks each direction starting from left going to down, if they are part of the solution, return 1 
    if(solveMazeDFS(maze, col-1, row) == 1){
        return 1;
    }if(solveMazeDFS(maze, col+1, row) == 1){
        return 1;
    }if(solveMazeDFS(maze, col, row-1) == 1){
        return 1;
    }if(solveMazeDFS(maze, col, row+1) == 1){
        return 1;
    }
    
    //if this cell is not part of the solution, it is marked at visited and 0 is returned
    maze->cells[row][col] = VISITED;
       
    return 0;
}
