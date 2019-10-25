#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h> 
#include <unistd.h>
#define Max 1000000

int n,a[Max];

struct arrayBound
{
	int left;
	int right;
};


//merge function used in merge sort
void merge(int low, int mid, int high) 
{ 
    int left[mid - low + 1]; 
    int right[high - mid]; 
  
    // n1 is size of left part and n2 is size 
    // of right part 
    int n1 = mid - low + 1, n2 = high - mid, i, j; 
  
    // storing values in left part 
    for (i = 0; i < n1; i++)
        left[i] = a[i + low];
    	

    // storing values in right part 
    for (i = 0; i < n2; i++)
        right[i] = a[i + mid + 1]; 
  		
    int k = low; 
    i = j = 0; 
  
    // merge left and right in ascending order 
    while (i < n1 && j < n2) { 
        if (left[i] <= right[j]){
            a[k++] = left[i++];
            //printf("%d ", left[i-1]);
        }
        
        else{
            a[k++] = right[j++]; 
            //printf("%d ", right[i-1]);
        }

    } 
  	//printf("\n");
    // insert remaining values from left 
    while (i < n1) { 
        a[k++] = left[i++]; 
    } 
    // insert remaining values from right 
    while (j < n2) { 
        a[k++] = right[j++]; 
    } 
} 

// merge sort thread function 
void* merge_sort(void* arg) 
{ 

	struct arrayBound *ab = (struct arrayBound*) arg;
	int low=ab->left;
	int high=ab-> right;
    // calculating mid point of array 
    int mid = low + (high - low) / 2; 
    
    if (low < high) {
    	//printf("%d\n",z++);
		struct arrayBound ab1;
		struct arrayBound ab2;

    	ab1.left=low;
    	ab1.right=mid;

    	ab2.left=mid+1;
    	ab2.right=high;
    
		// Launch threads
    	pthread_t tid1;
    	pthread_t tid2;

  		// calling first half 
  		pthread_create(&tid1, NULL, merge_sort, &ab1);
  
        // calling second half 
        pthread_create(&tid2, NULL, merge_sort, &ab2);
  
        // merging the two halves 
         
        pthread_join(tid1, NULL);
        pthread_join(tid2,NULL);
        merge(low, mid, high);

    } 

    pthread_exit(0);
} 
  
//a function that reads the array of enteries
void readArray(){
	FILE *infile;
    infile = fopen("input.txt","r");

    if (infile == NULL)
        exit(1);
    fscanf( infile, "%d" ,&n);

    printf("%d\n",n );

    for(int i = 0; i < n;i++) {
    	fscanf( infile, "%d", &a[i]);
    	printf("%d ",a[i]);
    }
    printf("\n");

    fclose(infile);
}

int main(int argc, char **argv)
{
	
	//int num_args = argc - 1;

	//initializing the matrices
	readArray();

	// Calculate the time taken by fun() 
    clock_t t; 
    t = clock(); 
    
    struct arrayBound ab;

    ab.left=0;
    ab.right=n-1;
    
	// Launch thread
    pthread_t tid;

    pthread_create(&tid, NULL, merge_sort, &ab);
	//sleep(1);


	pthread_join(tid,NULL);
	
	t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
  	
  	FILE * outfile;
    /* open the file for writing*/
    outfile = fopen ("outfile.txt","w+");
	
	//printing output
	for(int i = 0; i < n;i++) {
    	
    	fprintf (outfile, "%d ",a[i]);
    	printf("%d ",a[i]);
    	
    	
    	
    }
    printf("\n");
    fprintf (outfile, "\n");

    printf("a took %f seconds to execute \n", time_taken); 
    fprintf (outfile, "time: %f\n",time_taken); 

    
    fclose (outfile);
}
