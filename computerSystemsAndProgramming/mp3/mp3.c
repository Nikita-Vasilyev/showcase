/*
Intro: 
This program prints the desired row of pascal's triangle based on the inputted row number. Two for-loops are  used to loop over the 
number of entries in a row. Within the for loops, variable "val" calculates the forumla for a given position in the triangle and 
prints it to the console. 
 */

#include <stdio.h>
#include <stdlib.h>

int main()
{
  int row;
  unsigned long val;

  printf("Enter a row index: ");
  scanf("%d",&row);

  // Write your code here

  //loops through based on the number of entries in the desired row
  for (int k=0; k<=row; k++){
    val = 1;
    //loops through the product formula and saves to val
    for(int i=1; i<=k; i++){
      val =val*(row+1-i)/i;
    }
    
    printf("%lu ", val);  
  }  
  printf("\n");
  return 0;
}
