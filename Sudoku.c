/*
Project 1 - Sistemas Operacionais v1.3

Pedro Grijó
Felipe Rodrigues
Matheus Figueiredo

SUDOKU
- Checker: checks if a complete sudoku given as input
  is correct. In case it isn't, the occurrences of error are printed.
- Tips: checks the given sudoku, finding possible numbers that fit each missing
  cell. These numbers are stored in the 3 dimensional sudoTable, making it easier
  to manage it.
- Solver: Solves the given sudoku, completing it correctly.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int sudoTable[9][9][10]; //Global variable which represents the sudoku.

/*Checks the sudoku for errors*/

void checkSudoku ();

/*Hints possible combinations to finish the sudoku*/

void hintSudoku();

/*The solver uses the hints of the hintSudoku function, then it creates recursive threads
to check for all possible options, in the case that the
hint gives you 2 possible numbers in a cell. */

void solveSudoku ();
void* solver_thread();

/*Prints the hints or the solved sudoku*/
void print_sudoku();

/*Thread checks the lines, given as parameter v. If an error is found
  it is printed.*/

void* check_Line_C(void *v);
void* check_Line_H(void *v);

/*Thread checks the columns given as parameter v. If an error is found
  it is printed.*/

void* check_Column_C(void *v);
void* check_Column_H(void *v);

/*Thread checks the blocks given as parameter v, 9 blocks in total. If an error is found
  it is printed. */
void* check_Block_C(void *v);
void* check_Block_H(void *v);

int main() {
	int menu = 0;

	while(menu==0) {

		printf("Choose your option\n 1: Check Sudoku\n 2: Hint Sudoku\n 3: Solve puzzle\n 4: Exit\n");

		scanf("%d", &menu);

		switch(menu) {
		case 1:
			checkSudoku();
			menu = 0;
			break;

		case 2:
			hintSudoku();
			print_sudoku();
			menu = 0;
			break;

		case 3:
			solveSudoku();
			menu = 0;
			break;
		case 4:
			menu = -1;
			break;
		}
	}

	printf("Program ends!\n");
	return 0;
}

void checkSudoku () {
	pthread_t thr[27]; /*A total of 27 threads are used to solve the problem( 9 lines + 9 columns + 9 blocks ).*/
	int id[9];


	/*Gets the sudoku, input line by line*/
	int i;
	int j;
	for(i=0; i < 9; i++) {
		for(j=0; j < 9; j++) {
			scanf("%d", &sudoTable[i][j][0]);
		}
	}

	printf("We'll create threads..\n");
	/*Create the threads(27 threads to solve)*/
	for(i= 0; i < 9; i++) {
		id[i] = i;

		if (pthread_create(&thr[i], NULL, check_Line_C, &id[i])) /*Threads to solve lines (indexed from 0 to 8).*/
			fprintf(stderr, "Erro na cria??o da thread. \n");

		if (pthread_create(&thr[i+9], NULL, check_Column_C, &id[i])) /*Thread to solve columns (indexed from 0 to 8).*/
			fprintf(stderr, "Erro na cria??o da thread. \n");

		if (pthread_create(&thr[i+18], NULL, check_Block_C, &id[i])) /*Thread to solve blocks (indexed from 0 to 8).*/
			fprintf(stderr, "Erro na cria??o da thread. \n");
	}

	/*Makes the main thread wait for the others*/
	for(i= 0; i < 9; i++) {

		if (pthread_join(thr[i], NULL)) /*Wait for the line threads.*/
			fprintf(stderr, "Erro na cria??o da thread. \n");

		if (pthread_join(thr[i+9], NULL)) /*Wait for the column threads*/
			fprintf(stderr, "Erro na cria??o da thread. \n");

		if (pthread_join(thr[i+18], NULL)) /*Wait for the block threads*/
			fprintf(stderr, "Erro na cria??o da thread. \n");
	}

	return ;

}

void hintSudoku() {
	pthread_t thr[27]; /*A total of 27 threads are used to solve the problem( 9 lines + 9 columns + 9 blocks ).*/
	int id[9];


	/*Gets the sudoku, input line by line*/
	int i;
	int j;
	char input[0];
	for(i=0; i < 9; i++) {
		for(j=0; j < 9; j++) {
			do {
				input[0] = getchar();
			} while( input[0] == '\n' || input[0] == ' ');

			if( input[0] == 'X')
				sudoTable[i][j][0] = -1;
			else if(input[0] != '\n' && input[0] != ' ')
				sudoTable[i][j][0] = atoi(input);

		}
	}

	/*Finds all available numbers(set -1).*/
	int k;
	for(i=0; i < 9; i++) {
		for(j=0; j < 9; j++) {
			for(k=1; k < 10; k++) {
				sudoTable[i][j][k] = -1;
			}
		}
	}

	/*  PEDA?O DE CODIGO PARA DEBUGAR: PRINTA PISO 9 => REMOVER
	for(i=0; i < 9; i++){
	      for(j=0; j < 9; j++){
	                    printf("%d ",sudoTable[i][j][9]);
	              }

	      printf("\n");
	}
	*/

	printf("We'll create threads..\n");
	/*Create the threads(27 threads to solve)*/
	for(i= 0; i < 9; i++) {
		id[i] = i;

		if (pthread_create(&thr[i], NULL, check_Line_H, &id[i])) //Thread to solve line (note: we indexed sudo matriz 0..8 lines).
			fprintf(stderr, "Erro na cria??o da thread. \n");

		if (pthread_create(&thr[i+9], NULL, check_Column_H, &id[i])) //Thread to solve column (note: we indexed sudo matriz 0..8 column).
			fprintf(stderr, "Erro na cria??o da thread. \n");

		if (pthread_create(&thr[i+18], NULL, check_Block_H, &id[i])) //Thread to solve block (note: we index sudo matriz 0..8 block).
			fprintf(stderr, "Erro na cria??o da thread. \n");

	}


	/*Makes the main thread wait for the others*/
	for(i= 0; i < 9; i++) {

		if (pthread_join(thr[i], NULL)) //Thread to solve line (note: we indexed sudo matriz 0..8 lines).
			fprintf(stderr, "Erro na cria??o da thread. \n");

		if (pthread_join(thr[i+9], NULL)) //Thread to solve column (note: we indexed sudo matriz 0..8 column).
			fprintf(stderr, "Erro na cria??o da thread. \n");

		if (pthread_join(thr[i+18], NULL)) //Thread to solve block (note: we index sudo matriz 0..8 block).
			fprintf(stderr, "Erro na cria??o da thread. \n");
	}

	return ;
}

void solveSudoku () {
	pthread_t thr; //First solver option
	//void *sudoku = (*int) 9*9*10*malloc(sizeof(int));//To copy the sudoTable to the thread scope

	hintSudoku();

	//memcpy(sudoku, (*void)&sudoTable, 9*9*10*sizeof(int));//Copies the sudoTable in the scope of the thread

	printf("We'll create threads to solve...\n");

	if (pthread_create(&thr, NULL, solver_thread, NULL)) //Thread to solve first options
		fprintf(stderr, "Erro na cria??o da thread. \n");

	if (pthread_join(thr, NULL)) //Thread wait
		fprintf(stderr, "Erro na cria??o da thread. \n");

	return ;
}

void* solver_thread() {
	//pthread_t *thr;
	//int matrix[9][9][10] = *(int *) v;
	//void *sudoku = (*int) 9*9*10*malloc(sizeof(int));//To copy the sudoTable to the thread scope
	int i, j, k, l, n = -1, flag = 1;

	while(flag) {
		flag = 0;
		for(i=0; i < 9; i++) {
			for(j=0; j < 9; j++) {
				if(sudoTable[i][j][0] == -1) {
                    n = -1;
					for(k=1; k < 10; k++) {
						if(sudoTable[i][j][k] == -1 && n != -1) {
							k = 10;   //If there is a possible number, but it is not unique, break
						} else if(sudoTable[i][j][k] == -1) {
							n = k;   //If it is unique, register
						}
						if(k == 9 && k != 10 && n != -1) {
						    printf("Put %d in the position (%d, %d)\n", n, i, j);
							sudoTable[i][j][0] = n; //at the end, apply
							flag = 1;
							for(l=0; l < 9; l++) { //remove possibility to put in line, column and block
								sudoTable[i][l][n] = 0; //Set not available for use.
								sudoTable[l][j][n] = 0; //Set not available for use.
								int a;
								for(a=0; a<3; a++)sudoTable[(i/3)*3+l%3][(j/3)*3+a][n] = 0; //Set not available for use.
							}
						}
					}
				}
			}
		}
	}

	print_sudoku();

	return NULL;

}

void print_sudoku() {

	int i, j, m;
	for(i=0; i < 9; i++) {
		for(j=0; j < 9; j++) {
			if(sudoTable[i][j][0] != -1 && j == 0) /*Case that the clue does not need to be printed.*/
				printf("%d",sudoTable[i][j][0]);
			else if(sudoTable[i][j][0] != -1)
				printf("\t%d",sudoTable[i][j][0]);
			else { /*Case that the clue needs to be printed.*/
				if(j == 0)
					printf("("); /*Open(output formatting).*/
				else
					printf("\t("); /*Open(output formatting).*/
				for(m=1; m < 10; m++) { //looks for number available(set with "-1")
					if(sudoTable[i][j][m] == -1) /*Case when "m" is available.*/
						printf("%d", m);
				}
				printf(")"); /*Close(output formatting).*/
			}
		}
		printf("\n");
	}

	return;

}

void* check_Line_C(void *v) {
	int line = *(int *) v; /*line to process.*/
	int test[9] = {-1,-1,-1,-1,-1,-1,-1,-1,-1}; //Array that if test[i-1] = 1, so i've already seen this i number of sudoku, otherwise test[i-1] = -1.

	int j;
	for(j =0; j < 9; j++) {
		int numberUsed = sudoTable[line][j][0];
		if(test[numberUsed - 1] == -1) { /*Case that "numberUsed" has not yet been found*/
			test[numberUsed - 1] = 1; /*Marks that it was found.*/
		} else { /*ERROR case: at least two of the same values have been found.*/
			printf("Line %d contains two occurrences of the number %d.\n", line, numberUsed); /*Prints error information.*/
		}
	}

	return NULL;
}

void* check_Column_C(void *v) {
	int column = *(int *) v;
	int test[9] = {-1,-1,-1,-1,-1,-1,-1,-1,-1}; //Array that if test[i-1] = 1, so i've already seen this i number of sudoku, otherwise test[i-1] = -1.

	int j;
	for(j =0; j < 9; j++) {
		int numberUsed = sudoTable[j][column][0];
		if(test[numberUsed - 1] == -1) { /*Case that "numberUsed" has not yet been found*/
			test[numberUsed - 1] = 1; /*Marks that it was found.*/
		} else { /*ERROR case: at least two of the same values have been found.*/
			printf("Column %d contains two occurrences of the number %d.\n", column, numberUsed); /*Prints error information.*/
		}
	}

	return NULL;
}

void* check_Block_C(void *v) {
	int block = *(int *) v;
	int test[9] = {-1,-1,-1,-1,-1,-1,-1,-1,-1}; //Array that if test[i-1] = 1, so i've already seen this i number of sudoku, otherwise test[i-1] = -1.
	int i,j;

	/* First gets the pair of values which are the beginning of the block. */
	switch(block) {
	case 0:
		i = 0;
		j = 0;
		break;
	case 1:
		i = 0;
		j = 3;
		break;
	case 2:
		i = 0;
		j = 6;
		break;
	case 3:
		i = 3;
		j = 0;
		break;
	case 4:
		i = 3;
		j = 3;
		break;
	case 5:
		i = 3;
		j = 6;
		break;

	case 6:
		i = 6;
		j = 0;
		break;
	case 7:
		i = 6;
		j = 3;
		break;

	case 8:
		i = 6;
		j = 6;
		break;
	default:
		printf("Error: %d doesnt exist\n", block);
		break;
	}

	//Intera the block to check
	int x;
	int y;
	for(x = 0; x < 3; x++) {
		for(y=0; y < 3; y++) {
			int numberUsed = sudoTable[i+x][j+y][0];
			if(test[numberUsed - 1] == -1) { /*Case that "numberUsed" has not yet been found*/
				test[numberUsed - 1] = 1; /*Marks that it was found.*/
			} else { /*ERROR case: at least two of the same values have been found.*/
				printf("Block %d contains two occurrences of the number %d.\n", block, numberUsed); /*Prints error information.*/
			}
		}

	}

	return NULL;
}

void* check_Line_H(void *v) {
	int line = *(int *) v;
	int j;

	for(j=0; j < 9; j++) {
		if(sudoTable[line][j][0] != -1) { /*Sets all numbers of this line as not available*/
			int number =  sudoTable[line][j][0];
			int m;
			for( m = 0; m < 9; m++) {
				sudoTable[line][m][number] = 0; //Set to all in line that isn't avaible for use.
			}
		}
	}

	return NULL;

}

void* check_Column_H(void *v) {
	int column = *(int *)v;
	int i;

	for(i=0; i < 9; i++) {
		if(sudoTable[i][column][0] != -1) { /*Sets all numbers of this column as not available*/
			int number =  sudoTable[i][column][0];
			int m;
			for( m = 0; m < 9; m++) {
				sudoTable[m][column][number] = 0; //Set isn't avaible for use.
			}
		}
	}

	return NULL;

}

void* check_Block_H (void *v) {
	int block = *(int *) v;
	int i,j;

	/* First gets the pair of values which are the beginning of the block. */
	switch(block) {
	case 0:
		i = 0;
		j = 0;
		break;
	case 1:
		i = 0;
		j = 3;
		break;
	case 2:
		i = 0;
		j = 6;
		break;
	case 3:
		i = 3;
		j = 0;
		break;
	case 4:
		i = 3;
		j = 3;
		break;
	case 5:
		i = 3;
		j = 6;
		break;

	case 6:
		i = 6;
		j = 0;
		break;
	case 7:
		i = 6;
		j = 3;
		break;

	case 8:
		i = 6;
		j = 6;
		break;
	default:
		printf("Error: %d doesnt exist\n", block);
		break;
	}

	//Intera the block to check
	int x;
	int y;
	for(x = 0; x < 3; x++) {
		for(y=0; y < 3; y++) {
			if(sudoTable[i+x][j+y][0] != -1) { //*Sets all numbers of this block as not available*/
				int number =  sudoTable[i+x][j+y][0];
				int m,n;
				for( m = 0; m < 3; m++) {
					for(n =0; n < 3; n++) {
						sudoTable[i+m][j+n][number] = 0; //Set isn't avaible for use.
					}
				}
			}

		}
	}

	return NULL;
}
