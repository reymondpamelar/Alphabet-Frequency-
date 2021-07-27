/*
 * Reymond Pamelar
 * CS570 - 02
 * July 25, 2021 
 * alphabetcountmulthreads.c - this file implements the alphabetcountmulthreads function.
 */

#include <stdio.h> 
#include "count.h"


/**
  The alphabetcountmulthreads function counts the frequency of each alphabet letter (a-z, case insensitive) in all the .txt files under
  directory of the given path and write the results to a file named as filetowrite. Different with programming assignment#0, you need to implement the program using mutithreading.
  
  Input: 
      path - a pointer to a char string [a character array] specifying the path of the directory; and
      filetowrite - a pointer to a char string [a character array] specifying the file where results should be written in.
      alphabetfreq - a pointer to a long array storing the frequency of each alphabet letter from a - z, which should be already up-to-date;
      num_threads - number of the threads running in parallel to process the files
      
       
  Output: a new file named as filetowrite with the frequency of each alphabet letter written in
  
  Requirements:
1)	Multiple threads are expected to run in parallel to share the workload, i.e. suppose 3 threads to process 30 files, then each thread should process 10 files;
2)	When a thread is created, a message should be print out showing which files this thread will process, for example:
Thread id = 274237184 starts processing files with index from 0 to 10!
3)	When a file is being processed, a message should be print out showing which thread (thread_id = xxx) is processing this file, for example:
Thread id = 265844480 is processing file input_11.txt
4)	When a thread is done with its workload, a message should be print out showing which files this thread has done with work, for example:
      Thread id = 274237184 is done !
5)	The array: long alphabetfreq[ ] should always be up-to-date, i.e. it always has the result of all the threads counted so far.  [You may need to use mutexes to protect this critical region.]


You should have the screen printing should be similar as follows:

 Thread id = 274237184 starts processing files with index from 0 to 10!
 Thread id = 265844480 starts processing files with index from 11 to 22!
 Thread id = 257451776 starts processing files with index from 23 to 31!

 Thread id = 265844480 is processing file input_11.txt
 Thread id = 257451776 is processing file input_22.txt
 Thread id = 274237184 is processing file input_00.txt
  � � 

 Thread id = 274237184 is done !
 Thread id = 265844480 is done !
 Thread id = 257451776 is done !

 The results are counted as follows:
 a -> 2973036
 b -> 556908
 c -> 765864
 �  � 
*/

char filelist[1000][256];
char fileName[1000][256];
char alphabetList[ALPHABETSIZE];

long alphabetfreq_Local[ALPHABETSIZE] = {0};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int start = 0;
int end = 0;

typedef struct{
  int filesPerThreadIndex;
  int startIndex;
  int endIndex;
}arguments;

void *alphabetCount_thread(void *args){
  arguments *actual_args = args;
  int indexStart = actual_args[0].startIndex;
  int indexEnd = actual_args[0].endIndex;
  printf("Thread id = %d starts processing files with index from %d to %d!\n", pthread_self(), indexStart, indexEnd);
  
  //iterate through all files
  for (int i = indexStart; i <= indexEnd; i++){
    FILE *file = fopen(filelist[i], "r");
    printf("Thread id = %d is processing file %s \n", pthread_self(), fileName[i]);
    if(file == NULL){
      continue;
    }

    //read character one by one and compare with each letter of alphabet
    char c;
    c = fgetc(file);
    while (c != EOF){
      for (int a = 0; a < ALPHABETSIZE; a++){

        //Make lowercase
        if(c >= 65 && c <= 90){
          c += 32;
        }

        //Increment letter frequency
        if(c == alphabetList[a]){
          
          //Critical Region Start
          pthread_mutex_lock(&mutex);
          alphabetfreq_Local[a]++;
          pthread_mutex_unlock(&mutex);
          //Critical Region End

        }
      }
      c = fgetc(file);
    }
    fclose(file);
  }
  printf("Thread id = %d is done !\n", pthread_self());
  pthread_exit(NULL);
}

void alphabetcountmulthreads(char *path, char *filetowrite, long alphabetfreq[], int num_threads)
{	

  //Open Path directory and grab input file names
  int i = 0;
  struct dirent *dir;
  DIR *d = opendir(path);
  char filePath[1000];
  int dirLength = 0;
  if(d){
    while ((dir = readdir(d)) != NULL){
      size_t len = strlen(dir -> d_name);

      //concatenate full path for file then store in array
      if(len > 4 && strcmp(dir -> d_name + len - 4, ".txt") == 0){
        filePath[0]='\0';
        strcat(fileName[i], dir -> d_name);
        strcat(filePath, path);
        strcat(filePath,"/");
        strcat(filePath, dir -> d_name);
        strcpy(filelist[i], filePath);
        i++;
        dirLength++;
      }
    }
  }
  closedir(d);

  //Array for all alphabet ascii values
  for(int i = 0; i < ALPHABETSIZE; i++)
  {
      alphabetList[i] = (char)(i+97);
  }
  
  //Array for number of files per thread
  int filesPerThread[num_threads];
  for(int i = 0; i < num_threads; i++){
    filesPerThread[i] = 0;
  }

  //Evenly distribute amount of files per thread
  int y = 0;
  for(int x = 0; x < dirLength; x++){
      if(y == num_threads){
        y = 0;
      }
      filesPerThread[y] ++;
      y++;
  }

  arguments *args = malloc(num_threads * sizeof (arguments));

  //Create start and end point within files for each thread to be sent as an argument of structs later
  for(int i = 0; i < num_threads; i++){
    if(i == 0){
      end += filesPerThread[i]-1;
    }
    else{
      end += filesPerThread[i];
    }
    args[i].endIndex = end;
    args[i].startIndex = start;
    start += filesPerThread[i];
  }

  pthread_t threads[num_threads];
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  void *status;

  //Create threads and send "arguments" struct as an argument
  for(int i = 0; i < num_threads; i++){
    args[i].filesPerThreadIndex = i;
    pthread_create(&threads[i], &attr, alphabetCount_thread, &args[i]);
  }

  
  //Wait for each thread to terminate
  for(int a = 0; a < num_threads; a++){
    pthread_join(threads[a], &status);

    //Update alphabet frequencies after threads counted so far
    for(int b = 0; b < ALPHABETSIZE; b++){
      alphabetfreq[b] = alphabetfreq_Local[b];
    }
  }

     //Make "result" directory if no directory is already created
   int chk = mkdir("../result",0777);
   if (!chk)
        printf("Directory created\n");
   else {
        printf("Unable to create directory\n");
   }

   //Write frequencies to filetowrite path
   FILE *fp = fopen(filetowrite, "w");
   if(fp != NULL){
        for (int i = 0; i < ALPHABETSIZE; i++){
            fprintf(fp, "%c -> %d\n", (char)(i+97), alphabetfreq[i]);
        }
   }
}
