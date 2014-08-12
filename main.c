/*2048 console-based game by johnny.tifosi
Written in June-August 2014
Free and open source software

Copyright (C) 2014  johnny.tifosi

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <dos.h>
#define size 4
#define undos 5

/* Random integer generator within a semi-open interval [min, max) */
int random_in_range (unsigned int min, unsigned int max)
{
  int base_random = rand(); /* in [0, RAND_MAX] */
  if (RAND_MAX == base_random) return random_in_range(min, max);
  /* now guaranteed to be in [0, RAND_MAX) */
  int range       = max - min,
      remainder   = RAND_MAX % range,
      bucket      = RAND_MAX / range;
  /* There are range buckets, plus one smaller interval
     within remainder of RAND_MAX */
  if (base_random < RAND_MAX - remainder) {
    return min + base_random/bucket;
  } else {
    return random_in_range (min, max);
  }
}

/*function to insert numbers to empty slots*/
void insert(int array[size][size]) {
	/*printf("Entered function insert\n");*/
	int i,j,newi,newj,newnum,flag;
	/*check if there is an empty cell to insert number*/
	flag=0;
	for (i=0; i<size; i++) {
		for (j=0; j<size; j++) {
			if (array[i][j]==0) {
				flag=1;
			}
		}
	}
	if (flag==1) {
		do {
			newi=random_in_range(0,size);
			newj=random_in_range(0,size);
			/*printf("I'm in the loop!\n");*/
		} while (array[newi][newj]!=0);
		/*insert new number: 90% chance of 2, 10% chance of 4*/
		i=rand() %10;
		if (i==9) {
			newnum=4;
		}
		else {
			newnum=2;
		}
		array[newi][newj]=newnum; 
	}
}

/*function to print colored numbers with correct spacing*/
void print(int x) {
	HANDLE  hConsole;
    int k;
	if (x==0) {
		k=15;}
	else if (x==2) {
		k=112;}
	else if (x==4) {
		k=48;}
	else if (x==8) {
		k=32;}
	else if (x==16) {
		k=208;}
	else if (x==32) {
		k=192;}
	else if (x==64) {
		k=64;}
	else {
		k=224;}
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, k);
    if (x<10) {
    	printf("  %d  ",x);}
    else if (x<100) {
    	printf("  %d ",x);}
    else if (x<1000) {
    	printf(" %d ",x);}
    else {
    	printf(" %d",x);}
    SetConsoleTextAttribute(hConsole, 7);
}

/*--------------------------------------------------------------------------------------------------------------------------------------------*/
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {
	int a[size][size]; /*the 4x4 board matrix*/
	int u[size][size][undos+1]; /*undo stack containing the 5 previous board states*/
	int points[undos]; /*undo stack containing the points added in eack move*/
	int temp[size];
	int i,j,l/*array indices*/,flag/*game over*/,flag2/*valid move*/,score,highscore,newi,newj,newnum/*indices and magnitude of new inserted number*/,move/*next move*/,undos_left/*undo moves available*/,current_points/*points scored in the last move*/;
	FILE *hs/*highscore file*/, *save/*save file*/;
	score=0;
	flag=0;
	undos_left=undos;
	srand( time( NULL ) ); /*seed the pseudo random number generator with current time*/
	
	/*open highscore file, create one if it doesn't exist*/
	hs=fopen("highscore.dat", "ab+");
	if (hs==NULL) {
		highscore=0;
	}
	else {
		fscanf(hs, "%d", &highscore);
	}
	fclose(hs);

	/*array initialization*/
	for (i=0; i<=size-1; i++) {
		for (j=0; j<=size-1; j++) {
			a[i][j]=0;
		}
	}
	
	/*insert first number*/
	insert(a);
	
	/*insert second number*/
	insert(a);
	
	/*undo array initialization*/
	for (i=0; i<=size-1; i++) {
		for (j=0; j<=size-1; j++) {
			for (l=0; l<=undos; l++) {
				if (l==0) {
					u[i][j][l]=a[i][j];
				}
				else {
					u[i][j][l]=0;
				}
				if (l!=undos) {
					points[l]=0;
				}
			}
		}
	}	
	
	/*print first output*/
	printf("2048 game by johnny.tifosi.\n\n");
	for (i=0; i<=size-1; i++) {
		for (j=0; j<=size-1; j++) {
			if (j<3) {
		        print(a[i][j]);
			}
			else {
				print(a[i][j]);
				printf("\n");
				printf("\n");
			}
 
		}
	}
	printf("Score: %d	High Score: %d\n\n",score,highscore);
	printf("Play using arrow keys. R:reset U:undo (%d available) S:save L:load game\n", undos_left);
	/*-------------------------------------------------------------------------------------------------------------------*/
	while (flag==0) {
		flag=1; /*if this won't change to 0 inside the loop, game is over*/
		flag2=0; /*if this changes to 1, it was a valid move*/
		current_points=0;
		
		/*read next move with getch() function to use arrow keys without pressing ENTER*/
		move=getch();
		if (move==0 || move==0xE0) move=getch();
		
		/*UP*/
		if (move==72) {
			for (j=0; j<size; j++){
				for (i=0; i<size; i++) {
					temp[i]=0;
				}
				/*first check for valid move*/
				for (i=0; i<size-1; i++) {
					if (a[i][j]==0 && a[i+1][j]!=0) {
						flag2=1;
					}
				}
				/*keep non-zero cells, put them in row with temp array and delete them from a*/
				l=0;
				for (i=0;i<size;i++) {
					if (a[i][j]!=0) {
						temp[l]=a[i][j];
						a[i][j]=0;
						l++;
					}
				}
				/*add adjacent equal cells and add to score*/
				for (i=0; i<size-1; i++) {
					if (temp[i]==temp[i+1] && temp[i]!=0) {
						temp[i]=temp[i]+temp[i+1];
						current_points=current_points+temp[i];
						temp[i+1]=0;
						flag2=1; /*second check for valid move*/
					}
				}
				/* keep non-zero cells again and transfer them back to a*/
				l=0;
				for (i=0; i<size; i++) {
					if (temp[i]!=0){
						a[l][j]=temp[i];
						l++;
					}
				}
			}
		}
		
		/*DOWN*/
		if (move==80) {
			for (j=0; j<size; j++){
				for (i=0; i<size; i++) {
					temp[i]=0;
				}
				/*first check for valid move*/
				for (i=3; i>=1; i=i-1) {
					if (a[i][j]==0 && a[i-1][j]!=0) {
						flag2=1;
					}
				}
				/*keep non-zero cells, put them in row with temp array and delete them from a*/
				l=3;
				for (i=3; i>=0; i=i-1) {
					if (a[i][j]!=0) {
						temp[l]=a[i][j];
						a[i][j]=0;
						l=l-1;
					}
				}
				/*add adjacent equal cells and add to score*/
				for (i=2; i>=0; i=i-1) {
					if (temp[i]==temp[i+1] && temp[i]!=0){
						temp[i+1]=temp[i]+temp[i+1];
						current_points=current_points+temp[i+1];
						temp[i]=0;
						flag2=1;
					}
				}
				/* keep non-zero cells again and transfer them back to a*/
				l=3;
				for (i=3; i>=0; i=i-1) {
					if (temp[i]!=0){
						a[l][j]=temp[i];
						l=l-1;
					}
				}
			}
		}
		
		/*LEFT*/
		if (move==75) {
			for (i=0; i<size; i++){
				for (l=0; l<size; l++) {
					temp[l]=0;
				}
				/*first check for valid move*/
				for (j=0; j<size-1; j++) {
					if (a[i][j]==0 && a[i][j+1]!=0) {
						flag2=1;
					}
				}
				/*keep non-zero cells, put them in row with temp array and delete them from a*/
				l=0;
				for (j=0;j<size;j++) {
					if (a[i][j]!=0) {
						temp[l]=a[i][j];
						a[i][j]=0;
						l++;
					}
				}
				/*add adjacent equal cells and add to score*/
				for (j=0; j<size-1; j++) {
					if (temp[j]==temp[j+1] && temp[j]!=0){
						temp[j]=temp[j]+temp[j+1];
						current_points=current_points+temp[j];
						temp[j+1]=0;
						flag2=1;
					}
				}
				/* keep non-zero cells again and transfer them back to a*/
				l=0;
				for (j=0; j<size; j++) {
					if (temp[j]!=0){
						a[i][l]=temp[j];
						l++;
					}
				}
			}
		}
		
		/*RIGHT*/
		if (move==77) {
			for (i=0; i<size; i++){
				for (l=0; l<size; l++) {
					temp[l]=0;
				}
				/*first check for valid move*/
				for (j=3; j>=1; j=j-1) {
					if (a[i][j]==0 && a[i][j-1]!=0) {
						flag2=1;
					}
				}
				/*keep non-zero cells, put them in row with temp array and delete them from a*/
				l=3;
				for (j=3; j>=0; j=j-1) {
					if (a[i][j]!=0) {
						temp[l]=a[i][j];
						a[i][j]=0;
						l=l-1;
					}
				}
				/*add adjacent equal cells and add to score*/
				for (j=2; j>=0; j=j-1) {
					if (temp[j]==temp[j+1] && temp[j]!=0){
						temp[j+1]=temp[j]+temp[j+1];
						current_points=current_points+temp[j+1];
						temp[j]=0;
						flag2=1;
					}
				}
				/* keep non-zero cells again and transfer them back to a*/
				l=3;
				for (j=3; j>=0; j=j-1) {
					if (temp[j]!=0){
						a[i][l]=temp[j];
						l=l-1;
					}
				}
			}
		}
		
		
		if (flag2==1) {
			/*insert new number if there was a valid move*/
			insert(a);
		
			/*save previous score changes if there was a valid move*/		
			for (l=undos-1; l>=1; l=l-1) {
				points[l]=points[l-1];
			}
			points[0]=current_points;
			
			/*update score and save highscore*/
			score=score+current_points;			
			if (highscore<score) {
				highscore=score;
				hs=fopen("highscore.dat","w");
				fprintf(hs, "%d", score);
				fclose(hs);
			}
		}
				
		/*RESET, R button press*/
		if (move==114) {
			for (i=0; i<=3; i++) {
				for (j=0; j<=3; j++) {
					a[i][j]=0;
				}
			}
			score=0;
			undos_left=5;
			insert(a);
			insert(a);
			/*undo array initialization*/
			for (i=0; i<=size-1; i++) {
				for (j=0; j<=size-1; j++) {
					for (l=0; l<=undos; l++) {
						if (l==0) {
							u[i][j][l]=a[i][j];
						}
						else {
							u[i][j][l]=0;
						}
						if (l!=undos) {
							points[l]=0;
						}
					}
				}
			}	
		}
				
		/*UNDO, U button press*/
		if (move==117 && undos_left>0) {
			score=score-points[0];
			/*restore previous board state*/
			for (i=0; i<=size-1; i++) {
				for (j=0; j<=size-1; j++) {
					a[i][j]=u[i][j][1];
				}
			}
			/*move previous board states up in the stack*/
			for (i=0; i<=size-1; i++) {
				for (j=0; j<=size-1; j++) {
					for (l=0; l<=undos; l++) {
						u[i][j][l]=u[i][j][l+1];
					}
				}
			}
			for (l=0; l<=undos-2; l++) {						
				points[l]=points[l+1];						
			}
			undos_left=undos_left-1;
		}
		
		/*SAVE, S button press*/
		if (move==115) {
			save=fopen("save.dat","w");
			for (i=0; i<=size-1; i++) {
				for (j=0; j<=size-1; j++) {
					if (j<3) {
				        fprintf(save,"%d ",a[i][j]);
					}
					else {
						fprintf(save,"%d",a[i][j]);
						fprintf(save,"\n");
					}
				}
			}
			fprintf(save,"%d\n",score);
			fprintf(save,"%d",undos_left);
			fclose(save);
		}
		
		/*LOAD GAME, L button press*/
		if (move==108) {
			/*open save file, display error message if it doesn't exist*/
			save=fopen("save.dat", "r");
			if (save!=NULL) {
				for (i=0; i<=size-1; i++) {
					for (j=0; j<=size-1; j++) {
						if (j<3) {
					        fscanf(save,"%d ",&a[i][j]);
						}
						else {
							fscanf(save,"%d\n",&a[i][j]);
						}
					}
				}
				fscanf(save,"%d\n",&score);
				fscanf(save,"%d\n",&undos_left);
				/*undo array initialization*/
				for (i=0; i<=size-1; i++) {
					for (j=0; j<=size-1; j++) {
						for (l=0; l<=undos; l++) {
							if (l==0) {
								u[i][j][l]=a[i][j];
							}
							else {
								u[i][j][l]=0;
							}
							if (l!=undos) {
								points[l]=0;
							}
						}
					}
				}	
			}
			fclose(save);
		}
		
		/*printf("loop ended\n");*/
		if (move==72 || move==80 || move==75 || move==77 || move==114 || move==117 || move==115 || move==108) {
			if (flag2==1 && undos_left>0) {
				/*move previous states down in the stack*/
				for (i=0; i<=size-1; i++) {
					for (j=0; j<=size-1; j++) {
						for (l=undos; l>=1; l=l-1) {
							u[i][j][l]=u[i][j][l-1];
						}
					}
				}
				/*save current board state*/
				for (i=0; i<=size-1; i++) {
					for (j=0; j<=size-1; j++) {
						u[i][j][0]=a[i][j];
					}
				}
			}
			/*print output*/
	    	system("cls"); /*clear screen to print new output*/
	    	printf("2048 game by johnny.tifosi.\n\n");
			for (i=0; i<=size-1; i++) {
				for (j=0; j<=size-1; j++) {
					if (j<3) {
				        print(a[i][j]);
					}
					else {
						print(a[i][j]);
						printf("\n");
						printf("\n");
					}
				}
			}
			printf("Score: %d	High Score: %d\n\n",score,highscore);
			printf("Play using arrow keys. R:reset U:undo (%d available) S:save L:load game\n", undos_left);
			if (move==115) {
				printf("Game saved.\n");
			}
			if (move==108) {
				if (save==NULL) {
					printf("There isn't any saved game.\n");
				}
				else {
					printf("Game loaded.\n");
				}
			}
		}
		/*if there are no zero cells or equal adjacent cells, game over*/
		for (i=0; i<size; i++) {
			for (j=0; j<size; j++) {
				if (a[i][j]==0) {
					flag=0;
				}
			}
		}
		for (i=0; i<size; i++) {
			for (j=0; j<size-1; j++) {
				if (a[i][j]==a[i][j+1]) {
					flag=0;
				}
			}
		}
		for (j=0; j<size; j++) {
			for (i=0; i<size-1; i++) {
				if (a[i][j]==a[i+1][j]) {
					flag=0;
				}
			}
		}
	}
	printf("GAME OVER\n");
	system("pause");
	return 0;	
}
