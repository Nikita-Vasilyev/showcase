/*
Intro:
This file contains the functions countLiveNeighbor, updateBoard, and aliveStable. countLiveNeighbor uses the inputted 
board in order to find the number of live neighbors of the selected position. It returns the number of alive
neighbors at that position. updateBoard takes the inputted board and uses the game logic in order to determine
whether a cell will be alive in the next iteration and outputs the updated board. aliveStable checks 
if the next iteration is identical to the current one or if the next iteration is all zeros. If the next
iteration is identical or it is all zeros then a value of 1 is returned otherwise 0 is returned. 

*/



/*
 * countLiveNeighbor
 * Inputs:
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * row: the row of the cell that needs to count alive neighbors.
 * col: the col of the cell that needs to count alive neighbors.
 * Output:
 * return the number of alive neighbors. There are at most eight neighbors.
 * Pay attention for the edge and corner cells, they have less neighbors.
 */

int countLiveNeighbor(int* board, int boardRowSize, int boardColSize, int row, int col){

    int liveCount=0;
    //loops through every position of the game board using two for loops
    for (int i=row-1; i<=row+1; i++){
        for (int j=col-1; j<=col+1; j++){
            //if the current coordinate is that of the position in question than the loop continues
	        if(i==row && j==col){
	           continue;
	        }
            //if the i an j are in the bounds of the board and the value is 1, then 
            //the livecount is increased
            if(i<boardRowSize && j<boardColSize && board[i*boardColSize+j]==1 && i>=0 && j>=0){
                liveCount++;
            }
        }
    }
    return liveCount;
}
/*
 * Update the game board to the next step.
 * Input: 
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: board is updated with new values for next step.
 */
void updateBoard(int* board, int boardRowSize, int boardColSize) {
    //a deep copy if the game board is created in order to isolate the changes
    //from effecting the state of the next iteration of the game
    int dcopy[boardRowSize*boardColSize];
    for(int i=0; i<boardRowSize*boardColSize; i++){
        dcopy[i]=board[i];
    }

    //loops through the board using two for loops to determine whether a cell will 
    //be alive in the next iteration
    for(int i=0; i<boardRowSize; i++){
        for(int j=0; j<boardColSize; j++){
            int live = countLiveNeighbor(board, boardRowSize, boardColSize, i, j);
            // if the current cell is alive and has 2 or 3 neighbors, the cell is alive otherwise the cell dies
            if(board[i*boardColSize+j]==1){
                if(live==2 || live==3){
                    dcopy[i*boardColSize+j] = 1;
                }else{
                    dcopy[i*boardColSize+j] = 0;
                }
            }
            //if the cell is not alive and has 3 live neighbors, the cell becomes alive
            else{
                if(live==3){
                    dcopy[i*boardColSize+j] = 1;
                }
            }
        }   
    }
    //assigns the deep copy of the array to the output array
    for(int i=0; i<boardRowSize*boardColSize; i++){
        board[i]=dcopy[i];
    }
}

/*
 * aliveStable
 * Checks if the alive cells stay the same for next step
 * board: 1-D array of the current game board. 1 represents a live cell.
 * 0 represents a dead cell
 * boardRowSize: the number of rows on the game board.
 * boardColSize: the number of cols on the game board.
 * Output: return 1 if the alive cells for next step is exactly the same with 
 * current step or there is no alive cells at all.
 * return 0 if the alive cells change for the next step.
 */ 
int aliveStable(int* board, int boardRowSize, int boardColSize){
    //creates a deep copy of the board array in order to prevent 
    //any changes to the original board
    int dcopy[boardRowSize*boardColSize];
    for(int i=0; i<boardRowSize*boardColSize; i++){
        dcopy[i]=board[i];
    }
    
    //calls the updateBoard function using the copy
    updateBoard(dcopy, boardRowSize, boardColSize);

    //using counters live and change, to determine whether the board has 
    //any changes and if the entire board is 0s
    int live=0;
    int change=0;
    for(int i=0; i<boardRowSize*boardColSize; i++){
        //if a live cell is found, increment live
        if(dcopy[i]==1){
            live++;
        }
        //if a change is detected, increment change
        if(dcopy[i]!=board[i]){
            change++;
        }
    }
    //if the live or change counters were incremented, 1 is returned otherwise 0 is returned
    if(live==0 || change==0){
        return 1;
    }
    return 0;
}

				
				
			

