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
void insert(int a[size][size]) {
	int newi,newj,newnum;
	do {
		newi=random_in_range(0,size);
		newj=random_in_range(0,size);
	} while (a[newi][newj]!=0);
	
	newnum=random_in_range(1,3);
	a[newi][newj]=2*newnum; /*new number must be 2 or 4*/
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
	int i,j,l/*array indices*/,flag/*game over*/,score,newi,newj,newnum/*indices and magnitude of new inserted number*/,move/*next move*/;
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
		/*read next move with getch() function to use arrow keys without pressing ENTER*/
		move=getch();
		if (move==0 || move==0xE0) move=getch();
		
		/*up*/
		if (move==72) {
			for (j=0; j<size; j++){
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
					if (temp[i]==temp[i+1]){
						temp[i]=temp[i]+temp[i+1];
						score=score+temp[i];
						temp[i+1]=0;
						flag=0; /*there are two equal adjacent cells, so game is not over*/
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
					if (temp[i]==temp[i+1]){
						temp[i+1]=temp[i]+temp[i+1];
						score=score+temp[i+1];
						temp[i]=0;
						flag=0; /*there are two equal adjacent cells, so game is not over*/
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
			
		}
		
		/*right*/
		if (move==77) {
			
		}
		
		if (move==72 || move==80 || move==75 || move==77) {
			srand(time(NULL));
			insert(a);
			
			/*print output*/
	    	system("cls"); /*clear screen to print new output*/
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
	}
	printf("GAME OVER\n");
	return 0;	
}
