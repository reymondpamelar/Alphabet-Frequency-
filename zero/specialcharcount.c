/* Reymond Pamelar
 * CS570 - 02
 * June 8, 2021
 * specialcharcount.c - this file implements the specialcharcount function.
 */


#include <stdio.h> 
#include "count.h"
#include <sys/stat.h>
#include <sys/types.h>

char filelist[1000][256];

void specialcharcount(char *path, char *filetowrite, long charfreq[])
{
   //Open Path directory and grab input file names
   int i = 0;
   struct dirent *dir;
   DIR *d = opendir(path);
   char filePath[1000];
   if(d){
       while ((dir = readdir(d)) != NULL){
              size_t len = strlen(dir -> d_name);

              //concatenate full path for file then store in array
              if(len > 4 && strcmp(dir -> d_name + len - 4, ".txt") == 0){
                    filePath[0]='\0';
                    strcat(filePath, path);
                    strcat(filePath,"/");
                    strcat(filePath, dir -> d_name);
                    strcpy(filelist[i], filePath);
                    i++;
              }
       }
   }
   closedir(d);

   //Array to store special characters
   char charList[4];
   charList[0] = ',';
   charList[1] = '.';
   charList[2] = ':';
   charList[3] = ';';
   charList[4] = '!';

   int CLength = sizeof(charList) / sizeof(charList[0]);
   int dirLength = sizeof(filelist) / sizeof(filelist[0]);

   //iterate through all files
   for (int i = 0; i < dirLength; i++){
        FILE *file = fopen(filelist[i], "r");
        if(file == NULL){
            continue;
        }

        //read character one by one and compare with each special character
        char c;
        c = fgetc(file);
        while (c != EOF){
            for (int a = 0; a < CLength+1; a++){
                //Increment special character frequency if current char matches
                if(c == charList[a]){
                    charfreq[a]++;
                }
            }
            c = fgetc(file);
        }
        fclose(file);
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
        for (int i = 0; i < CLength+1; i++){
            fprintf(fp, "%c -> %d\n", charList[i], charfreq[i]);
        }
   }
}
/**
  The specialcharcount function counts the frequency of the following 5 special characters:
  ','   '.'   ':'    ';'    '!'
   
  in all the .txt files under directory of the given path and write the results to a file named as filetowrite.
  
  Input: 
  
      path - a pointer to a char string [a character array] specifying the path of the directory; and
      filetowrite - a pointer to a char string [a character array] specifying the file where results should be written in.
      alphabetfreq - a pointer to a long array storing the frequency of the above 5 special characters
      
      charfreq[0]: the frequency of ','
      charfreq[1]: the frequency of '.'
      charfreq[2]: the frequency of ':'
      charfreq[3]: the frequency of ';'
      charfreq[4]: the frequency of '!'

  
  Output: a new file named as filetowrite with the frequency of the above special characters written in
  
  Steps recommended to finish the function:
  1) Find all the files ending with .txt and store in filelist.
  2) Read all files in the filelist one by one and count the frequency of each alphabet letter only (a - z). The array 
  long alphabetfreq[] always has the up-to-date frequencies of alphabet letters counted so far. If the letter is upper case, convert
  it to lower case first. 
  3) Write the result in the output file: filetowrite in following format: 
  
     character -> frequency
     
     example:
     , -> 20
     . -> 11
     : -> 20
     ; -> 11
     ! -> 12     
     
  Assumption:  
  1) You can assume there is no sub-directory under the given path so you don't have to search the files 
  recursively.    
  2) Only .txt files are counted and other types of files should be ignored.
  
*/

