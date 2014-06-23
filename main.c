/*2048 console-based game by johnny.tifosi
Written in June 2014
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
		/*printf("newi=%d, newj=%d\n",newi,newj);*/
		newnum=random_in_range(1,3);
		/*printf("newnum=%d\n",2*newnum);*/
		array[newi][newj]=2*newnum; /*new number must be 2 or 4*/
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
	int a[size][size],temp[size]; /*the 4x4 matrix*/
	int i,j,l/*array indices*/,flag/*game over*/,flag2/*valid move*/,score,newi,newj,newnum/*indices and magnitude of new inserted number*/,move/*next move*/;
	score=0;
	flag=0;
	srand( time( NULL ) ); /*seed the pseudo random number generator with current time*/

	/*array initialization*/
	for (i=0; i<=3; i++) {
		for (j=0; j<=3; j++) {
			a[i][j]=0;
		}
	}
	
	/*insert first number*/
	insert(a);
	
	/*insert second number*/
	insert(a);
	
	/*print first output*/
	printf("2048 game by johnny.tifosi. Play using arrow keys.\n\n");
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
	printf("Score: %d\n",score);
	/*-------------------------------------------------------------------------------------------------------------------*/
	while (flag==0) {
		flag=1; /*if this won't change to 0 inside the loop, game is over*/
		flag2=0; /*if this changes to 1, it was a valid move*/
		
		/*read next move with getch() function to use arrow keys without pressing ENTER*/
		move=getch();
		if (move==0 || move==0xE0) move=getch();
		
		/*up*/
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
						score=score+temp[i];
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
		
		/*down*/
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
						score=score+temp[i+1];
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
		}204
205
206
207
208
209
210
211
212
213
214
215
216
217
218
219
220
221
222
223
224
225
226
227
228
229
230
231
232
233
234
235
236
237
238
239
240
241
242
243
244
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
                                                score=score+temp[i+1];
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
                
                /*left*/
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
johnnytifosi
Commit changes

		
		/*left*/
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
						score=score+temp[j];
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
		
		/*right*/
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
						score=score+temp[j+1];
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
		/*printf("loop ended\n");*/
		if (move==72 || move==80 || move==75 || move==77) {
			/*insert new number if there was a valid move*/
			if (flag2==1) {
				insert(a);
			}
			/*print output*/
	    		system("cls"); /*clear screen to print new output*/
	    		printf("2048 game by johnny.tifosi. Play using arrow keys.\n\n");
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
			printf("Score: %d\n",score);
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
