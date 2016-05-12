#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>


// A normal C function that is executed as a thread when its name
// is specified in pthread_create()

int num_threads = 0;
float initial_market_value = 0;
float total_market_value = 0;
float P = 0;
pthread_mutex_t mut;
pthread_cond_t cv;
pthread_t* stock_threads;

pthread_t watch_top_thread_handle;
pthread_t watch_bottom_thread_handle;

bool should_terminate = false;

void print_usage()
{
  fprintf(stderr, "./lab6 <num-threads> <percent-terminate>\n\n");
  fprintf(stderr, "   <num-threads>  num-threads must be between 1 and 1000 inclusive\n");
  fprintf(stderr, "   <percent-terminate>  percent-terminate must be between 1 and 20 inclusive\n");
  fflush(stderr);
}

void check_pthread_error(int retcode){
  if (retcode != 0)
  {
    fprintf(stderr, "pthread error happened, code: %d\n", retcode);
    fflush(stderr);
    exit(-1);
  }
}

float randfloat(int low, int high)
{
    return (rand()/(double)(RAND_MAX))*abs(low-high)+low;
}


void* stock_thread(void* t){
  float stock_price = 100;

  while (true)
  {
    float change = randfloat(-1,1);

    stock_price += change;


    pthread_mutex_lock(&mut);
    {
      total_market_value += change;
     // printf("t: %d, change: %.2f\n", (int)t, change);
      fflush(stdout);
      ///signal to the watcher threads to wake up; as soon as we exit this mutex,
      /// the watcher thread should steal it from us.
      pthread_cond_broadcast(&cv);

    }//exit mutex
    pthread_mutex_unlock(&mut);
  }
  return 0;
}

void* watch_bottom_thread(void* param)
{
  int ret = 0;

  while (true){


    ret = pthread_mutex_lock(&mut); check_pthread_error(ret);
    { 

    //open mutex


      ret = pthread_cond_wait(&cv, &mut); check_pthread_error(ret);

      ///once pthread_cond_wait() returns (after cv was signaled), the mutex IS already locked.


      float bottom_market_value = initial_market_value - (initial_market_value * P);

      //printf("watch_bottom_thread loop\n");
      fflush(stdout);

      if (total_market_value <= bottom_market_value)
      {

        printf("Market Down to %.2f\n", total_market_value);
        printf("Total Market Price of  %d Stocks: %.2f\n",num_threads, total_market_value);
        fflush(stdout);
        exit(0);
      }

    } 
    //close mutex
    
    ret = pthread_mutex_unlock(&mut); check_pthread_error(ret);
  }
  return 0;
}

void* watch_top_thread(void* param)
{

  int ret = 0;

  while (true){


    ret = pthread_mutex_lock(&mut); check_pthread_error(ret);
    { //open mutex



      ret = pthread_cond_wait(&cv, &mut); check_pthread_error(ret);

      ///once pthread_cond_wait() returns (after cv was signaled), the mutex IS already locked.



      float top_market_value = initial_market_value + (initial_market_value * P);


      if (total_market_value >= top_market_value)
      {

        printf("Market Up to %.2f\n", total_market_value);
        printf("Total Market Price of  %d Stocks: %.2f\n",num_threads, total_market_value);

        fflush(stdout);
        exit(0);
      }

    } //close mutex
    ret = pthread_mutex_unlock(&mut); check_pthread_error(ret);
  }

  return 0;
}


int main( int argc, char *argv[] )
{

  // read the number of the std input.
  if (argc != 3)
  {
    print_usage();
    exit(-1);
  }

  num_threads = atoi(argv[1]);

  if (num_threads < 1 || num_threads > 1000)
  {
      ///"where <num-threads> will be a well-formed integer between 1 and 1000, inclusive."
      fprintf(stderr, "num-threads must be between 1 and 1000 inclusive\n");
      print_usage();
      exit(-1);
  }



  int Pint = atoi(argv[2]);

  if (Pint < 1 || Pint > 20)
  {
      fprintf(stderr, "percent-terminate must be between 1 and 20 inclusive\n");
      print_usage();
      exit(-1);
  }

  initial_market_value = num_threads*100.0;
  total_market_value = initial_market_value;
  P = ((float)Pint) / 100.0;



  int ret = 0, t;
  ret = pthread_mutex_init(&mut, NULL); 
  check_pthread_error(ret);
  ret = pthread_cond_init(&cv, NULL);
  check_pthread_error(ret);



  stock_threads = malloc(num_threads*sizeof(pthread_t));


  for(t = 0; t<num_threads; t++)
  {
    //printf("In main: creating thread %d\n", t);
    fflush(stdout);
    ret = pthread_create(&stock_threads[t], NULL, stock_thread, (void*)t); check_pthread_error(ret);
  }


  ret = pthread_create(&watch_top_thread_handle, NULL, watch_top_thread, 0); 
  check_pthread_error(ret);
  ret = pthread_create(&watch_bottom_thread_handle, NULL, watch_bottom_thread, 0); 
  check_pthread_error(ret);

  for(t=0; t<num_threads; t++)
  {
    pthread_join(stock_threads[t], NULL);
  }

  pthread_join(watch_top_thread_handle, NULL);
  pthread_join(watch_bottom_thread_handle, NULL);
printf("I am here "); 
  free(stock_threads);

  return 0;
}
