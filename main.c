#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h> 
#define M 5
#define N 4
#define L 8

long long a[M][N],b[N][L],c[M][L];

/*struct sum_runner_struct {
	long long limit;
	long long answer;
};
*/
struct matrixElement {
	int row;
	int column;
};



struct matrixRow {
	int rowNum;
	
};


//Thread function to calculate the value of element c[row][column]
void* elemMul(void* arg)
{
	struct matrixElement *elem =(struct matrixElement*) arg;

	for(int i=0;i<N;i++){
		c[elem->row][elem->column]+=
				a[elem->row][i]*b[i][elem->column];
	}
	pthread_exit(0);
}

void* rowMul(void* arg)
{
	struct matrixRow *row =(struct matrixRow*) arg;
	for(int i=0;i<L;i++){
		for(int j=0;j<N;j++){
			c[row->rowNum][i]+=a[row->rowNum][j]*b[j][i];
		}
	}
	
	pthread_exit(0);
}


void initMatrs(){
	for(int i = 0; i < M;i++) {
    	for(int j = 0; j < N;j++) {

    		a[i][j]= rand()%10;
    		//b[i][j]= rand()%10;
    		//a[i][j]=(j+i);
    		printf("%lld ",a[i][j]);
    	}
    	printf("\n");
    }
	
    for(int i = 0; i < N;i++) {
    	printf("	");
    	for(int j = 0; j < L;j++) {
    		//b[i][j]=(j+i);
    		//a[i][j]= rand()%10;
    		b[i][j]= rand()%10;
    		printf("%lld ",b[i][j]);
    	}
    	printf("\n");
    }
}

int main(int argc, char **argv)
{
	
	//int num_args = argc - 1;

	//initializing the matrices
	initMatrs();


	//struct sum_runner_struct args[num_args];
	struct  matrixElement elem[M*L];
	
	// Calculate the time taken by fun() 
    clock_t t; 
    t = clock(); 
    
	// Launch thread
	pthread_t tids[M*L];
	int k=0;
	for (int i = 0; i < M; i++) {
		for(int j=0;j<L;j++){
			elem[k].row=i;
			elem[k].column=j;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_create(&tids[k], &attr, elemMul, &elem[k]);
			k++;
		}
		
	}

	// Wait until thread is done its work
	for (int i = 0; i < (M*L); i++) {
		pthread_join(tids[i], NULL);

	}
	t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
  
    printf("a took %f seconds to execute \n", time_taken); 
	//printing output
	for(int i = 0; i < M;i++) {
    	for(int j = 0; j < L;j++) {

    		printf("%lld ",c[i][j]);
    	}
    	printf("\n");
    }
    printf("*******************************************************\n");
    for(int i=0;i<M;i++){
		for(int j=0;j<L;j++){
			c[i][j]=0;
		}
	}

	struct  matrixRow row[M];

    t = clock(); 
	// Launch thread
	pthread_t tids2[M];

	for (int i = 0; i < M; i++) {
		row[i].rowNum=i;
		pthread_create(&tids2[i] ,NULL ,rowMul ,&row[i]);
				
	}

	// Wait until threads are done
	for (int i = 0; i < (M); i++) {
		pthread_join(tids2[i], NULL);

	}
	t = clock() - t; 
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
  
    printf("b took %f seconds to execute \n", time_taken); 
	//printing output
	for(int i = 0; i < M;i++) {
    	for(int j = 0; j < L;j++) {

    		printf("%lld ",c[i][j]);
    	}
    	printf("\n");
    }
}
