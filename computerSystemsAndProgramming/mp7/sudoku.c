#include "sudoku.h"

//-------------------------------------------------------------------------------------------------
// Start here to work on your MP7
//-------------------------------------------------------------------------------------------------

/*
Intro: 
The work completed on this file includes the functions is_val_in_row, is_val_in_col, is_val_in_3x3_zone, is_val_valid, and solve_sudoku.
The functions is_val_in_row, is_val_in_col, and is_val_in_3x3_zone have a purpose to check if a value exists in a given row, col, or 3x3
section on the sudoku board. is_val_valid is used to check whether a given val would fit in a location based on the exisiting entries
in its row, col, and 3x3 section. solve_sudoku is used to compile all of the aformention functions to solve an inputted sudoku board.
*/

// You are free to declare any private functions if needed.

// Function: is_val_in_row
// Return true if "val" already existed in ith row of array sudoku.
int is_val_in_row(const int val, const int i, const int sudoku[9][9]) {

  assert(i>=0 && i<9);

  // BEG TODO
  //loops through the designated row and checks if the value appears
  for (int j=0; j<9; j++){
    //if the val appears, the function returns 1
    if(sudoku[i][j]==val){
      return 1;
    }
  }
  return 0;
  // END TODO
}

// Function: is_val_in_col
// Return true if "val" already existed in jth column of array sudoku.
int is_val_in_col(const int val, const int j, const int sudoku[9][9]) {

  assert(j>=0 && j<9);

  // BEG TODO
  //loops through the designated col and checks if the value appears
  for (int i=0; i<9; i++){
    //if the val appears, the function returns 1
    if(sudoku[i][j]==val){
      return 1;
    }
  }
  return 0;
  // END TODO
}

// Function: is_val_in_3x3_zone
// Return true if val already existed in the 3x3 zone corresponding to (i, j)
int is_val_in_3x3_zone(const int val, const int i, const int j, const int sudoku[9][9]) {
   
  assert(i>=0 && i<9 && j>=0 && j<9);

  // BEG TODO
  int rs, cs;
  //the function starts by designating the start row(rs) and start col(cs) positons
  //for the loop to check, essentially determines which section to look at
  if(i>=0 && i<3)
    rs=0;
  else if(i>=3 & i<6)
    rs=3;
  else
    rs=6;

  if(j>=0 && j<3)
    cs=0;
  else if(j>=3 && j<6)
    cs=3;
  else
    cs=6;

  //the loop uses the determined start row and col to loop through a 3x3 space
  //starting with rc and cs
  for (int r=rs; r<rs+3; r++){
    for (int c=cs; c<cs+3; c++){
      //if the val appears, the function returns 1
      if(sudoku[r][c]==val){
        return 1;
      }
    }
  }

  return 0;
  // END TODO
}

// Function: is_val_valid
// Return true if the val is can be filled in the given entry.
int is_val_valid(const int val, const int i, const int j, const int sudoku[9][9]) {

  //assert(i>=0 && i<9 && j>=0 && j<9);
  assert(i>=0 && i<9 && j>=0 && j<9);
    
  // BEG TODO
  //checks the row, col, and 3x3 section to ensure the val is valid
  if(is_val_in_3x3_zone(val, i, j, sudoku) || is_val_in_col(val, j, sudoku) || is_val_in_row(val, i ,sudoku)){
    return 0;
  }

  return 1;
  // END TODO
}

// Procedure: solve_sudoku
// Solve the given sudoku instance.
int solve_sudoku(int sudoku[9][9]) {

  // BEG TODO.
  int i, j;
  int done=1;
  //checks if all of the cells are occupied and if a location is found, 
  //the index is saved and the loop is broken out of
  for (i=0; i<9; i++){
    for (j=0; j<9; j++){
      //if an empty location is found on the board, it is broken out of the loop
      if(sudoku[i][j]==0){
	      done=0;
        break;
      }
    }
    if(done==0){
        break;
    }
  }
  //if done was not marked as false, 1 is returned by the function
  if(done==1){
    return 1;
  }

  //numbers 1-9 are checked at the empty location until the value is found to be valid
  for(int num=1; num<=9; num++){
    //if it is valid, the number is assigned and solve_sudoku is called again
    //until the board is solved
    if(is_val_valid(num, i, j, sudoku)){
      sudoku[i][j] = num;
      if(solve_sudoku(sudoku)){
        return 1;
      }
      sudoku[i][j] = 0;
    }
  }

  return 0;
  // END TODO.
}

// Procedure: print_sudoku
void print_sudoku(int sudoku[9][9])
{
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      printf("%2d", sudoku[i][j]);
    }
    printf("\n");
  }
}

// Procedure: parse_sudoku
void parse_sudoku(const char fpath[], int sudoku[9][9]) {
  FILE *reader = fopen(fpath, "r");
  assert(reader != NULL);
  int i, j;
  for(i=0; i<9; i++) {
    for(j=0; j<9; j++) {
      fscanf(reader, "%d", &sudoku[i][j]);
    }
  }
  fclose(reader);
}





