/* 
 * Reymond Pamelar
 * CS570 - 02
 * July 25, 2021 
 * charcount.h - This header file include type definitions (including function prototypes) and macros 
   used for the programing assignment two.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#define ALPHABETSIZE 26     //The total number of alphabetical letter from a - z (case insensitive)

void alphabetcountmulthreads(char *path, char *filetowrite, long alphabetfreq[], int num_threads);
/*
  The alphabetcountmulthreads function counts the frequency of each alphabet letter (a-z, case insensitive) in all the .txt files under
  directory of the given path and write the results to a file named as filetowrite. Different with programming assignment#0, you need to implement the program using mutithreading.
  
  Input: 
      path - a pointer to a char string [a character array] specifying the path of the directory; and
      filetowrite - a pointer to a char string [a character array] specifying the file where results should be written in.
      alphabetfreq - a pointer to a long array storing the frequency of each alphabet letter from a - z, which should be already up-to-date;
      num_threads - number of the threads running in parallel to process the files
      
       
  Output: a new file named as filetowrite with the frequency of each alphabet letter written in
  */