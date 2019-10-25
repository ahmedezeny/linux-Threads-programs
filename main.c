#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h> 

#define Max 100

int m,n,l;
int a[Max][Max],b[Max][Max],c[Max][Max];


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

	for(int i=0;i<n;i++){
		c[elem->row][elem->column]+=
				a[elem->row][i]*b[i][elem->column];
	}
	pthread_exit(0);
}

//Thread function to calculate the value of row c[row][]
void* rowMul(void* arg)
{
	struct matrixRow *row =(struct matrixRow*) arg;
	for(int i=0;i<l;i++){
		for(int j=0;j<n;j++){
			c[row->rowNum][i]+=a[row->rowNum][j]*b[j][i];
		}
	}
	
	pthread_exit(0);
}

//initiating matrices after reading them from file
void initMatrs(){
	FILE *infile;
    infile = fopen("infile.txt","r");

    if (infile == NULL)
        exit(1);

    fscanf( infile, "%d %d", &m ,&n);
    int check=n;

    printf("%d %d\n",m,n );

    for(int i = 0; i < m;i++) {
    	for(int j = 0; j < n;j++) {
    		fscanf( infile, "%d", &a[i][j]);
    		printf("%d ",a[i][j]);
    	}
    	printf("\n");	
    }

    fscanf( infile, "%d %d", &n ,&l);
    printf("%d %d\n",n,l );
    if(n!=check){
    	printf("can't perform multiplication on these matrices\n");
    	exit(0);
    }

    for(int i = 0; i < n;i++) {
    	printf("	");
    	for(int j = 0; j < l;j++) {
    		fscanf( infile, "%d", &b[i][j]);
    		printf("%d ",b[i][j]);
    	}
    	printf("\n");
    }

    fclose(infile);

}

int main(int argc, char **argv)
{
	
	//initializing the matrices
	initMatrs();

	struct  matrixElement elem[m*l];
	
	// Calculate the time taken by fun() 
    clock_t t; 
    t = clock(); 
    
	// Launch thread
	pthread_t tids[m*l];
	int k=0;
	for (int i = 0; i < m; i++) {
		for(int j=0;j<l;j++){
			elem[k].row=i;
			elem[k].column=j;
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_create(&tids[k], &attr, elemMul, &elem[k]);
			k++;
		}
		
	}

	// Wait until thread is done its work
	for (int i = 0; i < (m*l); i++) {
		pthread_join(tids[i], NULL);

	}
	t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
  	
  	FILE * outfile;
    /* open the file for writing*/
    outfile = fopen ("outfile.txt","w+");
	
	//printing output
	for(int i = 0; i < m;i++) {
    	for(int j = 0; j < l;j++) {
    		fprintf (outfile, "%d ",c[i][j]);
    		printf("%d ",c[i][j]);
    	}
    	printf("\n");
    	fprintf (outfile, "\n");
    }

    printf("a took %f seconds to execute \n", time_taken); 
    fprintf (outfile, "time: %f\n",time_taken); 

    printf("*******************************************************\n");

    for(int i=0;i<m;i++){
		for(int j=0;j<l;j++){
			c[i][j]=0;
		}
	}

	struct  matrixRow row[m];

    t = clock(); 
	// Launch thread
	pthread_t tids2[m];

	for (int i = 0; i < m; i++) {
		row[i].rowNum=i;
		pthread_create(&tids2[i] ,NULL ,rowMul ,&row[i]);
				
	}

	// Wait until threads are done
	for (int i = 0; i < (m); i++) {
		pthread_join(tids2[i], NULL);

	}
	t = clock() - t; 
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
  
	//printing output
	for(int i = 0; i < m;i++) {
    	for(int j = 0; j < l;j++) {
    		fprintf (outfile, "%d ",c[i][j]);
    		printf("%d ",c[i][j]);
    	}
    	printf("\n");
    	fprintf (outfile, "\n");
    }

    printf("a took %f seconds to execute \n", time_taken); 
    fprintf (outfile, "time: %f\n",time_taken); 

    fclose (outfile);
}
