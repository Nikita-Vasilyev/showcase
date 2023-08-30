;
; This program is intended to allow user input of numbers and common operators (+, -, *, /, and ^) in
; postfix notation to output an answer from a stack calculator. Once the program runs, the string
; "INPUT: " is printed to the screen, waiting for the user to input numbers or operators. If the user
; inputs a number, the number is PUSH-ed onto the top of the stack. If the user inputs an operator,
; 2 numbers are POP-ed off the top of the stack and chronologically stored in R3 and R4, ran through
; the respective subroutines that correspond to the operator (i.e: "+" calls IFPLUS, which then JSR's
; to PLUS, etc), and the output is stored in R0, which is then PUSH-ed back to the top of the stack.
; The program halts and POP-s out the single output stored in the stack once the "=" operator is called,
; and the output runs through another algorithm to print the stored decimal value in a 4-digit hex code
; onto the screen, for practical use. If the number of operators inputed does not equal to the number
; numbers inputed subtracted by 1, the calculator will output an "Invalid Expression" comment. This is
; due to either anything greater than one number stored in the stack when "=" is called, or if the stack
; underflows from an operator call with zero numbers or one number stored in the stack.
; Partner (NetID): sunnyc3
.ORIG x3000
	
;your code goes here
LD R1, EQUAL
NOT R1, R1
ADD R1, R1, #1

LEA R0, INSTART		;print "INPUT: "
PUTSP

INLOOP			;input loop
GETC			;waits for keyboard
OUT

JSR EVALUATE

BRnzp INLOOP



DONE	HALT

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R3- value to print in hexadecimal
PRINT_HEX
	ST R0, R0SAVER			;used for printing
	ST R1, R1SAVER			;counter
	ST R3, R3SAVER			;to be printed
	ST R4, R4SAVER			;for operations
	ST R5, R5SAVER			;for operations
	ST R7, R7SAVER			
	
	AND R0, R0, #0
	AND R1, R1, #0
	ADD R1, R1, #4
	AND R4, R4, #0			;clears R4, R5, and R6 for temp usage
	AND R5, R5, #0

PRINTLOOP

	LD R5, ISOVAL			;loads the value to isolate the last four digits to R5

	ADD R3, R3, #0
	BRzp #3				;circular left shifts R6 4 times
	ADD R3, R3, R3
	ADD R3, R3, #1
	BRnzp #1
	ADD R3, R3, R3			

	BRzp #3				;circular left shifts R6 4 times
	ADD R3, R3, R3
	ADD R3, R3, #1
	BRnzp #1
	ADD R3, R3, R3	

	BRzp #3				;circular left shifts R6 4 times
	ADD R3, R3, R3
	ADD R3, R3, #1
	BRnzp #1
	ADD R3, R3, R3	

	BRzp #3				;circular left shifts R6 4 times
	ADD R3, R3, R3
	ADD R3, R3, #1
	BRnzp #1
	ADD R3, R3, R3	
				
	AND R0, R3, R5			;sets R0 to the 4 least sig figs of R6
	ADD R0, R0, #-9 		;checks if a letter or number is to be printed

	BRp LETTER
	AND R0, R3, R5			;sets R0 to the 4 least sig figs of R6		
	LD R4, NUMOFF
	ADD R0, R0, R4			;adds the offset for numbers
	BRnzp POUT

LETTER	
	AND R0, R3, R5			;sets R0 to the 4 least sig figs of R6
	LD R4, HEXOFF			
	ADD R0, R0, R4			;adds the offset for letters
POUT
	OUT
	ADD R1, R1, #-1
	BRp PRINTLOOP

	LD R0, R0SAVER			;used for printing
	LD R1, R1SAVER			;counter
	LD R3, R3SAVER			;to be printed
	LD R4, R4SAVER			;for operations
	LD R5, R5SAVER			;for operations
	LD R7, R7SAVER	

RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R0 - character input from keyboard
;R6 - current numerical output
;
;
EVALUATE
;your code goes here

ST R0, R0SAVER
ST R7, R7SAVER

ADD R0, R0, #-10		;R0 starts with a ascii value and is subracted from until 
ADD R0, R0, #-10		;it is found that the value matches to a number or an operator
ADD R0, R0, #-10	
ADD R0, R0, #-2
BRz IFSPACE			;R0 would be the value of ' '

ADD R0, R0, #-10		
BRz IFMUL			;R0 would be the value of '*'

ADD R0, R0, #-1
BRz IFPLUS			;R0 would be the value of '+'

ADD R0, R0, #-2
BRz IFMIN			;R0 would be the value of '-'

ADD R0, R0, #-2
BRz IFDIV			;R0 would be the value of '/'

ST R2, R2SAVER
AND R2, R2, #0
ADD R2, R2, #10
NUMLOOP
ADD R0, R0, #-1
BRz IFNUMBER			;R0 would be the value of a number from '0'-'9'
ADD R2, R2, #-1
BRp NUMLOOP
LD R2, R2SAVER

ADD R0, R0, #-4
BRz IFEQUAL			;R0 would be the value of '='

ADD R0, R0, #-10
ADD R0, R0, #-10
ADD R0, R0, #-10
ADD R0, R0, #-3
BRz IFEXP			;R0 would be the value of '^'

BRnzp IFINVALID			;if none of the conditions are met then the INVALID state is thrown



IFSPACE				;all of the states that are possible when evaluating
BRnzp FINISH			;including if space, plus, minus, multiplication, division, 
				;exponent, or equal are inputs, as well as invalid state
IFPLUS
JSR POP
ADD R4, R0, #0
JSR POP
ADD R3, R0, #0
ADD R5, R5, #0
BRp IFINVALID			;pops twice, assigns registers, and checks if the pops are valid 
JSR PLUS
JSR PUSH			;pushes the result of the operation back to the stack
BRnzp FINISH

IFMIN
JSR POP
ADD R4, R0, #0
JSR POP
ADD R3, R0, #0
ADD R5, R5, #0
BRp IFINVALID			;pops twice, assigns registers, and checks if the pops are valid 
JSR MIN
JSR PUSH			;pushes the result of the operation back to the stack
BRnzp FINISH

IFMUL
JSR POP
ADD R4, R0, #0
JSR POP
ADD R3, R0, #0
ADD R5, R5, #0
BRp IFINVALID			;pops twice, assigns registers, and checks if the pops are valid 
JSR MUL
JSR PUSH			;pushes the result of the operation back to the stack
BRnzp FINISH

IFDIV
JSR POP
ADD R4, R0, #0
JSR POP
ADD R3, R0, #0
ADD R5, R5, #0
BRp IFINVALID			;pops twice, assigns registers, and checks if the pops are valid 
JSR DIV
JSR PUSH			;pushes the result of the operation back to the stack
BRnzp FINISH

IFEXP
JSR POP
ADD R4, R0, #0
JSR POP
ADD R3, R0, #0
ADD R5, R5, #0
BRp IFINVALID			;pops twice, assigns registers, and checks if the pops are valid 
JSR EXP
JSR PUSH			;pushes the result of the operation back to the stack
BRnzp FINISH

IFNUMBER			;if a number is inputted, the value is loaded again and ascii character '0' is subtracted
LD R0, R0SAVER
ST R2, R2SAVER
LD R2, NUMOFF
NOT R2, R2
ADD R2, R2, #1
ADD R0, R0, R2
LD R2, R2SAVER
JSR PUSH			;pushes the true value of the inputted value into the stack
BRnzp FINISH

IFEQUAL
ST R6, R6SAVER
ST R2, R2SAVER
LD R6, STACK_TOP
LD R2, STACK_START
NOT R6, R6
ADD R2, R2, R6
BRnp IFINVALID			;loads then compares the values of stack top and stack start to insure there is only one value in the stack
JSR POP
ADD R3, R0, #0
JSR PRINT_HEX 			;uses hex print to print the value remaining in the stack
LD R6, R6SAVER
LD R2, R2SAVER
BRnzp DONE

IFINVALID 
LEA R0, INVAL
PUTSP
BRnzp DONE

FINISH					;sends the program back to the input loop
LD R0, R0SAVER
LD R7, R7SAVER

RET 

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4 (R3+R4)
;out R0
PLUS	
;your code goes here

ADD R0, R3, R4

RET
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4 (R3-R4)
;out R0
MIN	
;your code goes here

ST R3, R3SAVER			;stores registers
ST R4, R4SAVER			;for operations
AND R0, R0, #0

NOT R4, R4			;twos comp
ADD R4, R4, #1
ADD R0, R3, R4			;subtracts

LD R3, R3SAVER			;restores registers
LD R4, R4SAVER			;for operations

RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

R0SAVER		.BLKW #1
R1SAVER		.BLKW #1
R2SAVER		.BLKW #1
R3SAVER		.BLKW #1
R4SAVER		.BLKW #1
R5SAVER		.BLKW #1
R6SAVER		.BLKW #1
R7SAVER		.BLKW #1
R7EXP		.BLKW #1
R7DIV		.BLKW #1

EQUAL		.FILL x003D
ISOVAL 		.FILL x000F	;value to isolate ascii number
NUMOFF		.FILL x0030	;number offset for ascii number
HEXOFF		.FILL x0037	;offset for ascii chars after 9
NEWLINE		.FILL x000A	;new line ascii value

POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;
STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
STACK_TOP	.FILL x4000	;

INSTART		.FILL x4E49
		.FILL x5550
		.FILL x3A54
		.FILL x0020
		.FILL x0000

INVAL		.FILL x4E49
		.FILL x4156
		.FILL x494C
		.FILL x0044
		.FILL x0000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;input R3, R4 (R3*R4)
;out R0
MUL	
;your code goes here

ST R3, R3SAVER			;stores registers
ST R4, R4SAVER			;for operations
AND R0, R0, #0

MULTILOOP			;loops through using R4
ADD R0, R0, R3
ADD R4, R4, #-1
BRp MULTILOOP

LD R3, R3SAVER			;restores registers
LD R4, R4SAVER			;for operations

RET
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4 (R3/R4)
;out R0
DIV	
;your code goes here

ST R2, R2SAVER			;increment (solution)
ST R3, R3SAVER			;stores registers
ST R4, R4SAVER			;for operations
ST R7, R7DIV			
AND R2, R2, #0

AND R0, R0, #0

DIVLOOP				;loops through and increments R2 every time R4 is subtracted
JSR MIN
ADD R0, R0, #0
BRn RESTORE
ADD R2, R2, #1
ADD R3, R0, #0
BRnzp DIVLOOP

RESTORE
ADD R0, R2, #0			;change R0 to output
LD R2, R2SAVER
LD R3, R3SAVER			;restores registers
LD R4, R4SAVER			;for operations
LD R7, R7DIV	

RET
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4 (R3^R4)
;out R0
EXP
;your code goes here
	
ST R3, R3SAVER			;stores registers
ST R4, R4SAVER			;for operations
ST R5, R5SAVER			;for operations
ST R7, R7EXP			
ADD R5, R4, #-1			;saves the exponent to r5
ADD R4, R3, #0			;for the first interation, it will be R3 times itself

EXPLOOP				;loops through using R5
JSR MUL
ADD R4, R0, #0
ADD R5, R5, #-1
BRp EXPLOOP

LD R3, R3SAVER			;restores registers
LD R4, R4SAVER			;for operations
LD R5, R5SAVER			;for operations
LD R7, R7EXP	

RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH	
	ST R3, PUSH_SaveR3	;save R3
	ST R4, PUSH_SaveR4	;save R4
	AND R5, R5, #0		;
	LD R3, STACK_END	;
	LD R4, STACk_TOP	;
	ADD R3, R3, #-1		;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz OVERFLOW		;stack is full
	STR R0, R4, #0		;no overflow, store value in the stack
	ADD R4, R4, #-1		;move top of the stack
	ST R4, STACK_TOP	;store top of stack pointer
	BRnzp DONE_PUSH		;
OVERFLOW
	ADD R5, R5, #1		;
DONE_PUSH
	LD R3, PUSH_SaveR3	;
	LD R4, PUSH_SaveR4	;
	RET


PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP	
	ST R3, POP_SaveR3	;save R3
	ST R4, POP_SaveR4	;save R3
	AND R5, R5, #0		;clear R5
	LD R3, STACK_START	;
	LD R4, STACK_TOP	;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz UNDERFLOW		;
	ADD R4, R4, #1		;
	LDR R0, R4, #0		;
	ST R4, STACK_TOP	;
	BRnzp DONE_POP		;
UNDERFLOW
	ADD R5, R5, #1		;
DONE_POP
	LD R3, POP_SaveR3	;
	LD R4, POP_SaveR4	;
	RET







.END
