#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>
//#include "generate3.h"
#include <errno.h>



void print_usage()
{
  printf("%s %s\n", "Usage: Lab1 : [-u] [-g] [-n <num-integers>] [-m <min-int>] [-M <max-int>] [-s <seed>]",
         "[-i <input-file-name>] [-o <output-file-name>] [-c <count-file-name>]");
  exit(2);
}


void swap_ints(int* left, int* right)
{
    int tmp = *left;
    *left = *right;
    *right = tmp;
}

size_t find_min_index(int* number_array, size_t number_array_size)
{
    assert(number_array_size > 0);
    
    int best_value = INT_MAX;
    size_t best_index = (size_t)(-1);
    for (int i = 0; i < number_array_size; ++i)
    {
        int value = number_array[i];
        if (value < best_value)
        {
          best_value = value;
          best_index = i;
        }
    }
    
    return best_index;
}

void sort_nums(int* number_array, size_t number_array_size)
{
    if (number_array_size == 0)
        return;
  
  
    ///5, 7, 8, 1, 7, 10
    ///5, 7, 8, 1, 7, 10
    ///^--------------^ find the minimum in this range
    ///         ^ minimum
    ///1, 7, 8, 5, 7, 10
    ///   ^-----------^ find the minimum in this range
    ///     ....
  
    for (int i = 0; i < number_array_size - 1; ++i)
    {
        ///loop through everything until the last item
        
        ///pick the item at i
        
        ///find the item in the range [i,end), that is the minimum
        size_t minimum_value_index = i + find_min_index(number_array + i, number_array_size - i);
        
        swap_ints(number_array+i, number_array+minimum_value_index);

        //fprintf(stdout, "%d\n", number_array[i]);

    }
    
}


void Output_File()
{
   FILE *fp = fopen("FN.txt", "w");
   
   if (!fp)
   {
     printf("error opening %s\n", "FN.txt");
     return;
   }
   
   
   fprintf(fp, "This is testing for fprintf...\n");
   
   fputs("This is testing for fputs...\n", fp);
   fclose(fp);
   
   
   /////////////////////////////////////////
   
   
   
   int c;
   
   while ( (c = fgetc(fp)) != EOF)
   {
     char character = c;
     
     
     
   }
   
   
   
}

int* get_input_from_file(const char* ifile_name, size_t* number_array_size)
{
  
    FILE *fp = fopen(ifile_name, "r");
    if (!fp)
    {
      printf("error opening %s\n", ifile_name);
      exit(-1);
    }
    int count = 0;
    while (true)
    {
      int n;
      int ret = fscanf(fp, "%i", &n); 
      if (ret == EOF)
        break;
      ++count;
    }
    printf("count: %d\n", count);
    ///start at the beginning again
    fseek(fp, 0, SEEK_SET);
    int* number_array = malloc(sizeof(int)*count);
    if (!number_array){
        printf("error allocating, something is wrong\n");
        exit(-1);
    }
    size_t size = 0;
    while (true)
    {
      int n;
      int ret = fscanf(fp, "%i", &n); 
      if (ret == EOF)
        break;
      number_array[size] = n;
      size++;
    }
    assert(size == count);
    *number_array_size = size;
    return number_array;
}



int random_num_generator(long i) ///////////////////////////////////////////////////// Done 
{
    int random_num, count;
    if(i<=1000000 && i>-1)
    {
      srand(time(NULL));
      int* ptr=(int*)malloc(i*sizeof(int));  //memory allocated using malloc
      if(ptr==NULL)                     
      {
          printf("Error! memory not allocated.");
          exit(0);
      }
        printf(" The Total Number is %ld \n", i);  
        for(count = 0; count < i; count++) {
          *(ptr+count) = (rand() % (255-1+1))+1;
      }

      for(count = 0; count < i; count++) {
          printf("%d\t", *(ptr+count));
            printf("\n");
      }
      printf("\n");
    }
    else
    fprintf(stderr, "You are Trying to generate a number outside the range 0 to 1000000: %ld\t\n", i);
    exit(2);
    return 0;
}


int random_num_generator_nd()   /////////////////////////////////////////////////////// Done 
{
    int random_num, count;
      srand(time(NULL));
      int* ptr=(int*)malloc(sizeof(int));  //memory allocated using malloc
      if(ptr==NULL)                     
      {
          printf("Error! memory not allocated.");
          exit(0);
      }

      for(count = 0; count < 100; count++) {
          *(ptr+count) = (rand() % (255-1+1))+1;
      }

      for(count = 0; count < 100; count++) {
          printf("%d\t", *(ptr+count));
            printf("\n");
      }
      free(ptr);
    return 0;
}





int random_case_generator_seed(int i, int seed) //////////////////////////////////////////////////////// Done 
{
    if(i<1000000 && i>-1)
    {
      int random_num, count;

      srand(seed);
      int* ptr=(int*)malloc(sizeof(int));  //memory allocated using malloc
      if(ptr==NULL)                     
      {
          printf("Error! memory not allocated.");
          exit(0);
      }

      for(count = 0; count < i; count++) {
          *(ptr+count) = (rand() % (100-1+1))+1;
      }

      for(count = 0; count < i; count++) {
          printf("%d\t", *(ptr+count));
            printf("\n");
      }

      free(ptr);
    }
    else
      fprintf(stderr, "You are Trying to generate a number outside the range 0 to 1000000: %d\t\n", i);
      exit(2);

    return 0;
}




int random_case_generator_seed_nd(int seed) ///////////////////////////////////////////////////////// Done 
{
      int random_num, count;

      srand(seed);
      int* ptr=(int*)malloc(sizeof(int));  //memory allocated using malloc
      if(ptr==NULL)                     
      {
          printf("Error! memory not allocated.");
          exit(0);
      }

      for(count = 0; count < 100; count++) {
          *(ptr+count) = (rand() % (255-1+1))+1;
      }

      for(count = 0; count < 100; count++) {
          printf("%d\t", *(ptr+count));
            printf("\n");
      }

      free(ptr);

    return 0;
}


int random_case_generator()  /////////////////////////////////////////////////////// Done 
{

    int random_num, count;

    srand(time(NULL));
    int* ptr=(int*)malloc(sizeof(int));  //memory allocated using malloc
    if(ptr==NULL)                     
    {
        printf("Error! memory not allocated.");
        exit(0);
    }

    for(count = 0; count < 100; count++) {
        *(ptr+count) = (rand() % (255-1+1))+1;
    }

    for(count = 0; count < 100; count++) {
        printf("%d\t", *(ptr+count));
            printf("\n");

    }
    free(ptr);

    return 0;
}


int min_num_Max(int n, int m, int M) ////////////////////////////////////////////// Done 
{
      int random_num, count;
  if((m<M)&&(m>=1)&&(M<=1000000))
  { 
    srand(time(NULL));
    int* ptr=(int*)malloc(sizeof(int));  //memory allocated using malloc
    if(ptr==NULL)                     
    {
        printf("Error! memory not allocated.");
        exit(0);
    }

    for(count = 0; count < n; count++) {
        *(ptr+count) = (rand() % (M-m+1))+m;
    }

    for(count = 0; count < n; count++) {
        printf("%d\t", *(ptr+count));
            printf("\n");

    }
    free(ptr);
  }
    else
    { 
      fprintf(stderr, " The Min is greater than 0 : %d and Max should be %d  to 1000000 : %d\t\n", m,m,M);
      exit(2);
    }
    return 0;
}



int min_num_Max_nd(int m, int M) /////////////////////////////////////////////////////// Done
{

    int random_num, count;

    if((m<=M) && (m>=1) && (M<=1000000))
    {
    
    srand(time(NULL));
    int* ptr=(int*)malloc(sizeof(int));  //memory allocated using malloc
    if(ptr==NULL)                     
    {
        printf("Error! memory not allocated.");
        exit(0);
    }

      for(count = 0; count < 100; count++) {
        *(ptr+count) = (rand() % (M-m+1))+m;
    }


      for(count = 0; count < 100; count++) {
        printf("%d\t", *(ptr+count));
            printf("\n");
      }
    }
    else 
      fprintf(stderr, " The Min is greater than 0 : %d and Max should be %d  to 1000000 : %d\t\n", m,m,M);
      exit(2);

    return 0;
}



int min_num_Max_seed_Nf(int n, int m, int M, int seed) ///////////////////////////////////////////////Done 
{

    if((m<=M) && (m>=1) && (M<=1000000))
    {

    int random_num;

    if(m<=M)
    {
        srand(seed);
        
        int* nums = malloc(n*sizeof(int));
        
        for(int i = 0; i < n; i++) 
        {
          nums[i] = (rand() % (M-m+1))+m;
        }

        


        for(int i = 0; i < n; i++) 
        {
          

            printf("%d\t", nums[i]);
            printf("\n");
        }
    }
  }
    else {
      fprintf(stderr, "The max number %d smaller than  the min number:%d \n", M, m);
      exit(2);
    }
    return 0;
}



int min_num_Max_seed_Nf_nd(int m, int M, int seed) ////////////////////////////////////////////////////////////////// Done
{
    if((m<=M) && (m>=1) && (M<=1000000))
    {

      int random_num;

    if(m<=M)
    {
        srand(seed);
        
        int* nums = malloc(sizeof(int));
        
        for(int i = 0; i < 100; i++) 
        {
          nums[i] = (rand() % (M-m+1))+m;
        }

        


        for(int i = 0; i < 100; i++) 
        {
          

            printf("%d\t", nums[i]);
            printf("\n");
        }
    }
  }
    else {
      fprintf(stderr, "The max number %d smaller than  the min number:%d \n", M, m);
      exit(2);
    }
    return 0;
}



int min_num(int n, int m) ////////////////////////////////////////////////////////////////// Done
{
  printf("salem\n");
    int random_num, count;

    if((m>=1) && (m<255))
    {

      int* ptr=(int*)malloc(sizeof(int));  //memory allocated using malloc
      if(ptr==NULL)                     
      {
          printf("Error! memory not allocated.");
          exit(0);
      }

      srand(time(NULL));
      char nums[n];
      for(count = 0; count < n; count++) {
          *(ptr+count) = (rand() % (255-m+1))+m;
      }

      for(count = 0; count < n; count++) {
        printf("%d\t", *(ptr+count));
            printf("\n");
      }
    }
      fprintf(stderr, "The min number outside the range vale 1 to 255: %d\t\n", m);
      exit(2);

    return 0;
}



int min_num_nd( int m) /////////////////////////////////////////////////////////////// Done 
{
  
    int random_num, count;

    if(m>=1 && m<255)
    {

      int* ptr=(int*)malloc(sizeof(int));  //memory allocated using malloc
      if(ptr==NULL)                     
      {
          printf("Error! memory not allocated.");
          exit(0);
      }

      srand(time(NULL));
      char nums[100];
      for(count = 0; count < 100; count++) {
          *(ptr+count) = (rand() % (255-m+1))+m;
      }

      for(count = 0; count < 100; count++) {
        printf("%d\t", *(ptr+count));
            printf("\n");
      }
    }
      else
      fprintf(stderr, "The min number outside the range vale 1 to 255: %d\t\n", m);
      exit(2);

    return 0;
}


int max_num(int n, int M) /////////////////////////////////////////////////////////////////////////// Done 
{
    int random_num, count;

    if(M>=1 && M<=1000000)
    {


      int* ptr=(int*)malloc(sizeof(int));  //memory allocated using malloc
      if(ptr==NULL)                     
      {
          printf("Error! memory not allocated.");
          exit(0);
      }

      srand(time(NULL));
      char nums[n];
      for(count = 0; count < n; count++) {
          *(ptr+count) = (rand() % (M-1+1))+1;
      }

      for(count = 0; count < n; count++) {
        printf("%d\t", *(ptr+count));
            printf("\n");
      }

    }
    else
      fprintf(stderr, "The max number outside the range vale 1 to 1000000: %d\t\n", M);
      exit(2);
    return 0;
}



int max_num_nd( int M) //////////////////////////////////////////////////////////////// Done 
{
    
    int random_num, count;

    if(M>=1 && M<=1000000)
    {


      int* ptr=(int*)malloc(sizeof(int));  //memory allocated using malloc
      if(ptr==NULL)                     
      {
          printf("Error! memory not allocated.");
          exit(0);
      }

      srand(time(NULL));
      char nums[100];
      for(count = 0; count < 100; count++) {
          *(ptr+count) = (rand() % (M-1+1))+1;
      }

      for(count = 0; count < 100; count++) {
        printf("%d\t", *(ptr+count));
            printf("\n");
      }

    }
    else
      fprintf(stderr, "The max number outside the range vale 1 to 1000000: %d\t\n", M);
      exit(2);
    return 0;
}




int min_num_seed(int n, int m,int seed) /////////////////////////////////////////////////////////////////// Done 
{
    int random_num, count;

    if(m>=1&& m<=255)
    {


       int* ptr=(int*)malloc(sizeof(int));  //memory allocated using malloc
      if(ptr==NULL)                     
      {
          printf("Error! memory not allocated.");
          exit(0);
      }

      srand(seed);
      char nums[n];
      for(count = 0; count < n; count++) {
          *(ptr+count) = (rand() % (255-m+1))+m;
      }

      for(count = 0; count < n; count++) {
        printf("%d\t", *(ptr+count));
            printf("\n");
      }

    }
    else       
      fprintf(stderr, "The min number outside the range vale 1 to 1000000: %d\t\n", m);
      exit(2);

    return 0;
}



int min_num_seed_nd( int m,int seed) //////////////////////////////////////////////////////////////// Done 
{
   
    int random_num, count;

    if(m>=1&& m<=255)
    {


       int* ptr=(int*)malloc(sizeof(int));  //memory allocated using malloc
      if(ptr==NULL)                     
      {
          printf("Error! memory not allocated.");
          exit(0);
      }

      srand(seed);
      char nums[100];
      for(count = 0; count < 100; count++) {
          *(ptr+count) = (rand() % (255-m+1))+m;
      }

      for(count = 0; count < 100; count++) {
        printf("%d\t", *(ptr+count));
            printf("\n");
      }

    }
    else       
      fprintf(stderr, "The min number outside the range vale 1 to 255: %d\t\n", m);
      exit(2);

    return 0;
}




int min_num_of(int n, int m,FILE* ofp) ///////////////////////////////////////////////////////////// Done 
{
    int random_num, count;
    
    if((m>=1) && (m<=255))
    {
        srand(time(NULL));
        
        int* nums = malloc(n*sizeof(int));
        
        for(int i = 0; i < n; i++) 
        {
          nums[i] = (rand() % (255-m+1))+m;
        }

        for(int i = 0; i < n; i++) 
        {
          

         //   printf("%d\n", nums[i]);

            if (ofp) 
                fprintf(ofp, "%d \n", nums[i]);

        }

      if (ofp) fwrite("\n", sizeof(char), 1, ofp);
        printf("\n");
    }
    else 
      fprintf(stderr, "The min number outside the range vale 1 to 255 : %d\t\n", m);
      exit(2);

    return 0;
}



int min_num_of_nd(int m,FILE* ofp) ////////////////////////////////////////////////////////////////////// Done  
{
    
    int random_num, count;
    
    if((m>=1) && m<=255)
    {
        srand(time(NULL));
        
        int* nums = malloc(sizeof(int));
        
        for(int i = 0; i < 100; i++) 
        {
          nums[i] = (rand() % (255-m+1))+m;
        }

        for(int i = 0; i < 100; i++) 
        {
          

        //    printf("%d\t", nums[i]);

            if (ofp) fprintf(ofp, "%d \n", nums[i]);
              
       //         printf("\n");

        }

      if (ofp) fwrite("\n", sizeof(char), 1, ofp);
        printf("\n");
    }
    else 
      fprintf(stderr, "The min number outside the range vale 1 to 255 : %d\t\n", m);
      exit(2);

    return 0;
}


int min_num_Max_seed(int n, int m, int M, int seed, FILE* ofp) ////////////////////////////////////////////////////////// Done
{


    int random_num;

    if((m>=1) && (m<=M) && (M<=1000000))
    {
        srand(seed);
        
        int* nums = malloc(n*sizeof(int));
        
        for(int i = 0; i < n; i++) 
        {
          nums[i] = (rand() % (M-m+1))+m;
        }

        for(int i = 0; i < n; i++) 
        {
          

  //          printf("%d\t", nums[i]);

            if (ofp) fprintf(ofp, "%d \n", nums[i]);
              
//                printf("\n");

        }
      
        if (ofp) fwrite("\n", sizeof(char), 1, ofp);
              printf("\n");
    }
    else {
      fprintf(stderr, "The min: %d or max number : %d outside the range vale 1 to 1000000 \t\n", m, M);
      exit(2);
    }
    return 0;
}





int min_num_Max_seed_nd(int m, int M, int seed, FILE* ofp) //////////////////////////////////////////////////// Done  
{

    
    int random_num;

    if((m>=1) && (m<=M) && (M<=1000000))
    {
        srand(seed);
        
        int* nums = malloc(sizeof(int));
        
        for(int i = 0; i < 100; i++) 
        {
          nums[i] = (rand() % (M-m+1))+m;
        }

        for(int i = 0; i < 100; i++) 
        {
            if (ofp) fprintf(ofp, "%d \n", nums[i]);
              
        }

      
        if (ofp) fwrite("\n", sizeof(char), 1, ofp);
              printf("\n");
    }
    else {
      fprintf(stderr, "The min: %d or max number : %d outside the range vale 1 to 1000000 \t\n", m, M);
      exit(2);
    }
    return 0;
}



int max_num_of(int n,int M,FILE* ofp) ///////////////////////////////////////////////////////////////////////////////////Done 
{
    int random_num, count;

    if((M<=1000000) && (M>=1))
    {

      srand(time(NULL));
        
        int* nums = malloc(n*sizeof(int));
        
        for(int i = 0; i < n; i++) 
        {
          nums[i] = (rand() % (M-1+1))+1;
        }

        for(int i = 0; i < n; i++) 
        {
            if (ofp) fprintf(ofp, "%d \n", nums[i]);
              
        }

      if (ofp) fwrite("\n", sizeof(char), 1, ofp);
        printf("\n");
    }
    else 
      fprintf(stderr, "The Max number is outside the range 1 to 1000000 : %d\t\n", M);
      exit(2);

    return 0;
}



int max_num_of_nd(int M,FILE* ofp) ////////////////////////////////////////////////////////////////////// Done 
{
    
    int random_num, count;

    if((M>=1) && (M<=1000000))             
    {
      srand(time(NULL));
      
      int* nums= malloc(sizeof(int));

      for(count = 0; count < 100; count++) {
        nums[count] = (rand() % (M-1+1))+1;
      }


      for(int count = 0; count < 100; count++) 
        {
          
            //printf("%d\t", nums[count]);

            if (ofp) fprintf(ofp, "%d\t \n", nums[count]);
        }
      if (ofp) fwrite("\n", sizeof(char), 1, ofp);
        printf("\n");
    }
    else 
      fprintf(stderr, "The Max number is outside the range 0 to 1000000 : %d\t\n", M);
      exit(2);

    return 0;
}



int min_max_num_of(int n, int m, int M, FILE* ofp) ////////////////////////////////////////////////////////////////////// Done 
{


    int random_num;

    if((m>=1) && (m<=M) && (M<=1000000))
    {
        srand(time(NULL));
        
        int* nums = malloc(n*sizeof(int));
        
        for(int i = 0; i < n; i++) 
        {
          nums[i] = (rand() % (M-m+1))+m;
        }

        for(int i = 0; i < n; i++) 
        {
          

           // printf("%d\t", nums[i]);

            if (ofp) fprintf(ofp, "%d \n", nums[i]);
              
              //  printf("\n");

        }

      
        if (ofp) fwrite("\n", sizeof(char), 1, ofp);
              printf("\n");
    }
    else {
      fprintf(stderr, "The Max or min number is not in range 0 to 1000000 : %d\t\n", M);
      exit(2);
    }
    return 0;
}





int min_max_num_of_nd( int m, int M, FILE* ofp) //////////////////////////////////////////////////////////////////////// Done 
{

    
    int random_num;

    if((m>=1) && (m<1000000) &&(m<M) &&(M<=1000000))             
    {
        srand(time(NULL));
        
        int* nums = malloc(sizeof(int));
        
        for(int i = 0; i < 100; i++) 
        {
          nums[i] = (rand() % (M-m+1))+m;
        }

        for(int i = 0; i < 100; i++) 
        {
          

           // printf("%d\t", nums[i]);

            if (ofp) fprintf(ofp, "%d \n", nums[i]);
              
                //printf("\n");

        }
      
        if (ofp) fwrite("\n", sizeof(char), 1, ofp);
              printf("\n");
    }
    else {
      fprintf(stderr, "The Max or min number is not in range 1 to 1000000 : %d\t\n", M);
      exit(2);
    }
    return 0;
}




int num_seed_of(int n, int seed,FILE* ofp) ///////////////////////////////////////////////////////////////////////////// Done 
{
    int random_num, count;

      srand(seed);
      int* nums = malloc(n*sizeof(int));
      for(count = 0; count < n; count++) {
        nums[count] = (rand() % (255-1+1))+1;
      }

        for(int count = 0; count < n; count++) 
        {
          

            //printf("%d\t", nums[count]);

            if (ofp) fprintf(ofp, "%d \n", nums[count]);
              
        }

      if (ofp) fwrite("\n", sizeof(char), 1, ofp);
        printf("\n");

    return 0;
}




int num_seed_of_nd( int seed,FILE* ofp) //////////////////////////////////////////////////////////////////////////// Done 
{
    
    int random_num, count;

      srand(seed);
      char nums[100];
      for(count = 0; count < 100; count++) {
        nums[count] = (rand() % (100-1+1))+1;
      }

      for(int count = 0; count < 100; count++) 
        {
          

            //printf("%d\t", nums[count]);

            if (ofp) fprintf(ofp, "%d \n", nums[count]);
              
              //  printf("\n");

        }

      if (ofp) fwrite("\n", sizeof(char), 1, ofp);
        printf("\n");

    return 0;
}


int max_num_of_seed(int n, int M,int seed,FILE* ofp) /////////////////////////////////////////////////////////////////////////////// Done 
{
    int random_num, count;

    if((M<=1000000) && (M>=1))
    {
      srand(seed);
        
        int* nums = malloc(n*sizeof(int)); //
        
        for(int i = 0; i < n; i++) 
        {
          nums[i] = (rand() % (M-1+1))+1;
        }

        for(int i = 0; i < n; i++) 
        {
            if (ofp) fprintf(ofp, "%d \n", nums[i]);
              
        }

      if (ofp) fwrite("\n", sizeof(char), 1, ofp);
        printf("\n");
    }
    else 
      fprintf(stderr, "The Max  number is not in range 1 to 1000000 : %d\t\n", M);
      exit(2);

    return 0;
}



int max_num_of_seed_nd(int M,int seed,FILE* ofp) /////////////////////////////////////////////////////////////////////////////////////// Done 
{
  
    int random_num, count;

    if(M<=1000000)
    {
        srand(seed);
        
        int* nums = malloc(sizeof(int));
        
        for(int i = 0; i < 100; i++) 
        {
          nums[i] = (rand() % (M-1+1))+1;
        }

        for(int i = 0; i < 100; i++) 
        {
            if (ofp) fprintf(ofp, "%d \n", nums[i]);
              
        }

      if (ofp) fwrite("\n", sizeof(char), 1, ofp);
        printf("\n");
    }
    else 
      fprintf(stderr, "The Max  number is not in range 1 to 1000000 : %d\t\n", M);
      exit(2);

    return 0;
}


int num_of(int n, FILE* ofp)////////////////////////////////////////////////////////////////////////////////// Done 
{
    int random_num, count;

      srand(time(NULL));
        
        int* nums = malloc(n*sizeof(int));
        
        for(int i = 0; i < n; i++) 
        {
          nums[i] = (rand() % (255-1+1))+1;
        }

        for(int i = 0; i < n; i++) 
        {
          

           // printf("%d\t", nums[i]);

            if (ofp) fprintf(ofp, "%d \n", nums[i]);
              
                //printf("\n");

        }
      
        if (ofp) fwrite("\n", sizeof(char), 1, ofp);
              printf("\n");

    return 0;
}



int num_of_nd(FILE* ofp) /////////////////////////////////////////////////////////////////////// Done  
{

    int random_num, count;

      srand(time(NULL));

      int* nums = malloc(sizeof(int));

      for(count = 0; count < 100; count++) {
        nums[count] = (rand() % (255-1+1))+1;
      }


        for(int count = 0; count < 100; count++) 
        {
          

           // printf("%d\t", nums[count]);

            if (ofp) fprintf(ofp, "%d \n", nums[count]);
              
        }

      if (ofp) fwrite("\n", sizeof(char), 1, ofp);
        printf("\n");

    return 0;
}



int max_num_seed(int n, int M,int seed) /////////////////////////////////////////////////////////////////// Done 
{
    int random_num, count;

    if((1<=M) && (M<= 1000000))
    {
      srand(seed);
      int*  nums = malloc(n*sizeof(int));
      for(count = 0; count < n; count++) {
        nums[count] = (rand() % (M-1+1))+1;
      }

      for(count = 0; count < n; count++) {
        printf("%d\t", nums[count]);
              printf("\n");

      }
    }
    else       
      fprintf(stderr, "The Max  number is not in range 1 to 1000000 : %d\t\n", M);
      exit(2);

    return 0;
}




int max_num_seed_nd( int M,int seed) ///////////////////////////////////////////////////////////// Done  
{
    
    int random_num, count;

    if((1<=M) && (M<=1000000))
    {
      srand(seed);
      int* nums = malloc(sizeof(100));
      for(count = 0; count < 100; count++) {
        nums[count] = (rand() % (M-1+1))+1;
      }

      for(count = 0; count < 100; count++) {
        printf("%d\t", nums[count]);
              printf("\n");

      }
    }
    else 
      fprintf(stderr, "The Max  number is not in range 1 to 1000000 : %d\t\n", M);
      exit(2);

    return 0;
}


bool arg_is_not_zero(const char* arg)
{
    const char* zero = "0";
    return memcmp(arg, zero, 2) != 0;
}
bool arg_is_zero(const char* arg)
{
    const char* zero = "0";
    return memcmp(arg, zero, 2) == 0;
}


int calculate_frequency(char c, const int* number_array, size_t number_array_size)
{
  int result = 0;
  for (int i = 0; i < number_array_size; ++i)
    if (number_array[i] == (int)c)
      result++;
  return result;
}

int main( int argc, char *argv[] )
{
    
    
    bool print_usage_and_exit = false;
    bool generate = false;
    long number_count = 100;
    bool has_number_count = false;
    int number_min = 1;
    bool has_number_min = false;
    int number_max = 255;
    bool has_number_max = false;
    int seed = 0;
    bool has_seed = false;
    
    const char* ifile_name = NULL;
    const char* ofile_name = NULL;
    const char* cfile_name = NULL;
    
    ///overallocate the numbers a bit, just in case
    int* number_array = malloc(sizeof(int)*argc);
    size_t number_array_size = 0;
    
    if (!number_array)
    {
      printf("error allocating, somethings wrong\n");
      exit(-1);
    }
    
    for (int i = 1; i < argc; ++i)
    {
        const char* argi = argv[i];
        
        
        //if (atoi(argi) != 0 || arg_is_zero(argi))

        if (atoi(argi) != 0 || arg_is_zero(argi))

        {
          ///this was a number input
          
          
          number_array[number_array_size] = atoi(argi);
          number_array_size++;
          
          continue;
        }
        
        char dash = argi[0];
        
        
        if (strlen(argi) != 2 || dash != '-')
        {
            print_usage();
            return -1;
        }
        
        
        char flag = argi[1];
        
        if (flag == 'u')
        {
            if (print_usage_and_exit) {
            
                ///Example: ./Lab1 -u -u
                print_usage();
                return -1;
            }
            print_usage_and_exit = true;
            continue;
        }
        
        
        if (flag == 'g')
        {
            if (generate || number_array_size > 0) {
            
                ///Example: ./Lab1 -g -g
                ///Example: ./Lab1 -g 5 5
                print_usage();
                return -1;
            }
            generate = true;
            continue;
        }
        
        if (!(i + 1 < argc))
        {
            ///Example: ./Lab1 -m
            print_usage();
            return -1;
        }
        
        
        if (flag == 'n')
        {
            if (has_number_count)
            {
                ///Example: ./Lab1 -n <number> -n <number>
                print_usage();
                return -1;
              //continue;
            }
            
            const char* next_arg_i = argv[i+1];
            char* next_arg_i_end = NULL;
            
            has_number_count = true;

            number_count = strtol( /*first character in string*/next_arg_i
                              , /*this will change the end pointer point to the last character strtol parses as a number*/
                                &next_arg_i_end
                              , /*base*/10);

            if (number_count == 0 && arg_is_not_zero(next_arg_i))
            {
                ///Example: ./Lab1 -n NOTANUMBER
                print_usage();
                return -1;
            }
            
            i++;
            continue;
        }
        
        
        if (flag == 'm')
        {
            if (has_number_min)
            {
                ///Example: ./Lab1 -m <min> -m <min>
                print_usage();
                return -1;
            }
            
            
            const char* next_arg_i = argv[i+1];
            char* next_arg_i_end = NULL;
            
            has_number_min = true;
            number_min = strtol( /*first character in string*/next_arg_i
                              , /*this will change the end pointer point to the last character strtol parses as a number*/
                                &next_arg_i_end
                              , /*base*/10);
            
            if (number_min == 0 && arg_is_not_zero(next_arg_i))
            {
                ///Example: ./Lab1 -m NOTANUMBER
                print_usage();
                return -1;
            }
            i++;
            continue;
        }
        
        
        if (flag == 'M')
        {
            if (has_number_max)
            {
                ///Example: ./Lab1 -M <max> -M <max>
                print_usage();
                return -1;
            }
            
            
            const char* next_arg_i = argv[i+1];
            char* next_arg_i_end = NULL;
            
            has_number_max = true;
            number_max = strtol( /*first character in string*/next_arg_i
                              , /*this will change the end pointer point to the last character strtol parses as a number*/
                                &next_arg_i_end
                              , /*base*/10);
            
            if (number_max == 0 && arg_is_not_zero(next_arg_i))
            {
                ///Example: ./Lab1 -M NOTANUMBER
                print_usage();
                return -1;
            }
            
            i++;
            continue;
        }
        
        if (flag == 's')
        {
            if (has_seed)
            {
                ///Example: ./Lab1 -s <seed> -s <seed>
                print_usage();
                return -1;
            }
            
            
            const char* next_arg_i = argv[i+1];
            char* next_arg_i_end = NULL;
            
            has_seed = true;
            
            
            seed = strtol( /*first character in string*/next_arg_i
                              , /*this will change the end pointer point to the last character strtol parses as a number*/
                                &next_arg_i_end
                              , /*base*/10);
            
            if (seed == 0 && arg_is_not_zero(next_arg_i))
            {
                ///Example: ./Lab1 -s NOTANUMBER
                print_usage();
                return -1;
            }
            
            
            i++;
            continue;
        }
        
        
        
        if (flag == 'i')
        {
            if (ifile_name != NULL)
            {
                ///Example: ./Lab1 -i INPUTFILE -i INPUTFILE
                print_usage();
                return -1;
            }
            
            
            ifile_name = argv[i+1];
            
            i++;
            continue;
        }
        
        if (flag == 'o')
        {
            if (ofile_name != NULL)
            {
                ///Example: ./Lab1 -o OUTPUTFILE -o OUTPUTFILE
                print_usage();
                return -1;
            }
            


            
            ofile_name = argv[i+1];
            
            i++;
            continue;
        }
        
        if (flag == 'c')
        {
            if (cfile_name != NULL)
            {
                ///Example: ./Lab1 -c OUTPUTFILE -c OUTPUTFILE
                print_usage();
                return -1;
            }
            
            
            cfile_name = argv[i+1];
            
            i++;
            continue;
        }
    }
    
    if (print_usage_and_exit)
    {
        print_usage();
        exit(0);
    }
    
    
    if (ifile_name)
    {
        free(number_array);
        
        number_array = get_input_from_file(ifile_name, &number_array_size);
      
    }
    
    
    FILE* ofile = NULL;
    if (ofile_name)
    {
        ofile = fopen(ofile_name, "w");
        if (!ofile)
        {
            printf("Cannot open %s for writing\n", ofile_name);
            exit(-1);
        }
        
    }
    
    FILE* cfile = NULL;
    if (cfile_name)
    {
        cfile = fopen(cfile_name, "w");
        if (!cfile)
        {
            printf("Cannot open %s for writing\n", cfile_name);
            exit(-1);
        }
        
    }
    
    
    if (!generate && number_array_size == 0)
    {
        fprintf(stdout, "Enter the number count\n");
        fflush(stdout);
        
        free(number_array);
        
        int ret = fscanf(stdin, "%zi", &number_array_size);
        fgetc(stdin);
            
        
        if (ret == EOF)
        {
            fprintf(stdout, "Error reading from command line\n");
            exit(-1);
        }
        
        number_array = (int*)malloc(sizeof(int)*number_array_size);
        
        for (int i = 0; i < number_array_size; ++i)
        {
            int value;
            int ret = fscanf(stdin, "%i", &value);
            
            if (ret == EOF)
            {
                fprintf(stdout, "Error reading from command line\n");
                exit(-1);
            }
            fgetc(stdin);
            
            number_array[i] = value;
        }
    }
    
    
    
    
    
    
    
    
    
    
    
    
    sort_nums(number_array, number_array_size);

    
    
    
    
    
    
    
    
    
    
    
    
    if (generate) 
    {
        if(has_number_count)
        {   
            if(has_number_max && has_number_min && !(has_seed) && !(ofile)) //3
                min_num_Max(number_count, number_min, number_max);

            if(has_number_max && has_number_min && has_seed && ofile) // 1
                min_num_Max_seed(number_count, number_min, number_max, seed, ofile);

            if(has_number_max && has_number_min && has_seed && !(ofile)) // 2 //7
                min_num_Max_seed_Nf(number_count, number_min, number_max, seed);

            else if(has_number_max && !(has_number_min) && !(has_seed) && !(ofile)) // 15
                max_num(number_count,number_max);

            else if (has_number_min &&!(has_number_max) && !(has_seed) && !(ofile)) //4
                min_num(number_count,number_min);


            else if(has_number_max && !(has_number_min) && (has_seed)&& !(ofile)) //12
                max_num_seed(number_count,number_max,seed);

            else if (has_number_min &&!(has_number_max) && (has_seed) && !(ofile) ) //5
                min_num_seed(number_count,number_min,seed);

            else if (has_number_min &&!(has_number_max) && !(has_seed) && (ofile) ) //6
                min_num_of(number_count,number_min,ofile);


            else if (has_number_min && (has_number_max) && !(has_seed) && (ofile) ) //8         // I have problem here
                min_max_num_of(number_count,number_min,number_max,ofile);

            else if (!(has_number_min) && (has_number_max) && !(has_seed) && (ofile) ) //9   // I am heve problem 
                max_num_of(number_count,number_max,ofile);


            else if (!(has_number_min) && !(has_number_max) && (has_seed) && (ofile) ) //10    //  completed 
                num_seed_of(number_count,seed,ofile);

            else if (!(has_number_min) && !(has_number_max) && !(has_seed) && (ofile) ) //11  // result in negative 
                num_of(number_count,ofile);

            else if(has_seed && !(has_number_min) && !(has_number_max) && !(ofile)) //14  // good 
               random_case_generator_seed(number_count,seed);

            else if (!(has_number_min) && (has_number_max) && (has_seed) && (ofile) ) //13  // good 
                max_num_of_seed(number_count,number_max,seed,ofile);
            

            if(has_number_count && !(has_seed) && !(has_number_max) && !(has_number_min) && !(ofile)) //16
                random_num_generator(number_count);
            
            if (has_number_count)
            {
                goto here;
            }
        }

        if(!(has_number_count))
        {   
            if(has_number_max && has_number_min && !(has_seed) && !(ofile)) //3         /////////////// Done
                min_num_Max_nd(number_min, number_max);

            if(has_number_max && has_number_min && has_seed && ofile) // 1             /////////////////Done
                min_num_Max_seed_nd( number_min, number_max, seed, ofile);  //done 

            if(has_number_max && has_number_min && has_seed && !(ofile)) // 2 //7      ////////////// Done 
                min_num_Max_seed_Nf_nd( number_min, number_max, seed);

            else if(has_number_max && !(has_number_min) && !(has_seed) && !(ofile)) // 15  /////////// Done 
                max_num_nd(number_max);

            else if (has_number_min &&!(has_number_max) && !(has_seed) && !(ofile)) //4   //////////// Done 
                min_num_nd(number_min);


            else if(has_number_max && !(has_number_min) && (has_seed)&& !(ofile)) //12    /////////// Done 
                max_num_seed_nd(number_max,seed);

            else if (has_number_min &&!(has_number_max) && (has_seed) && !(ofile) ) //5   /////////// Done 
                min_num_seed_nd(number_min,seed);

            else if (has_number_min &&!(has_number_max) && !(has_seed) && (ofile) ) //6  //////////// Done 
                min_num_of_nd(number_min,ofile);


            else if (has_number_min && (has_number_max) && !(has_seed) && (ofile) ) //8  //////////// Done 
                min_max_num_of_nd(number_min,number_max,ofile);

            else if (!(has_number_min) && (has_number_max) && !(has_seed) && (ofile) ) //9   //////////// Done  
                max_num_of_nd(number_max,ofile);


            else if (!(has_number_min) && !(has_number_max) && (has_seed) && (ofile) ) //10    //////////// Done
                num_seed_of_nd(seed,ofile);

            else if (!(has_number_min) && !(has_number_max) && !(has_seed) && (ofile) ) //11  //////////// Problem 
                num_of_nd(ofile);

            else if(has_seed && !(has_number_min) && !(has_number_max) && !(ofile)) //14  // good //////// Done 
               random_case_generator_seed_nd(seed);

            else if (!(has_number_min) && (has_number_max) && (has_seed) && (ofile) ) //13  // good /////// Done 
                max_num_of_seed_nd(number_max,seed,ofile);
            
            if (!(has_number_count) && !(has_seed) && !(has_number_max) && !(has_number_min) && !(ofile)) //// Done
            {
                random_case_generator();
            }
        }



        here:
            printf("");

    }
    else {
        ///get input from user
        
        
        
        
        
        
        
        
        
        const char* userid = getenv("USER");
        size_t useridlen = strlen(userid);
        
        Begin_clock();

        for (int i = 0; i < useridlen; ++i)
        {
          char c = userid[i];
          
          int freq = calculate_frequency(c, number_array, number_array_size);
          
          if (cfile) fprintf(cfile, "%c  %d\t%d\n", c, (int)c, freq);
          if (!cfile) fprintf(stdout, "%c  %d\t%d\n", c, (int)c, freq);  
        }
        
        if (ofile) {
            for (int i = 0; i < number_array_size; ++i)
            {
                fprintf(ofile, "%d\n", number_array[i]);

            }

            fprintf(ofile, "\n");
        }

        if (!ofile) {
            for (int i = 0; i < number_array_size; ++i)
            {
              printf("\n");
              fprintf(stdout, "%d\t", number_array[i]);

            }

            printf("\n");

        }

            
        Eng_clock();

        Calculate_Time();
    }
    
        
    
    return 0;
}