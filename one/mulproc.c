/*
 This program will fork two child processes running the two programs generated in programming zero in parallel.
 Hint: You mant need to use fork(), exec() family, wait(), exit(), getpid() and etc ...
 
 Requirements:
 
 1) Exactly two child processes are created, one to run testspecial program and the other is to run testalphabet program;
 2) When a program starts to run, print a message to the output screen showing which process (with PID) is running which program, for example:
    "CHILD <16741> process is executing testalphabet program!"
    
 3) When a program is done, the process exits and at the same time, a message should be print to the output screen showing which  process (with PID) is done with the program, for example:
    "CHILD <16741> process has done with testalphabet program !"
    
 4) The messages should match the real execution orders, i.e. when the testspecial program starts/ends, the right message should be print out. So you need to figure out how to get the starting/ending time of each 
 process.
   
   
 The expected screen print out should be similar as follows:
 
 CHILD <16741> process is executing testalphabet program!
 CHILD <16742> process is executing testspecial program!
, -> 745668
. -> 798072
... ...

CHILD <16742> process has done with testspecial program !
a -> 2973036
b -> 556908
... ...

CHILD <16741> process has done with testalphabet program !
 */

/*
Reymond Pamelar
CS 570-02
Programming Assignment #1
July 18, 2021
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {

   //signed int to represent process id. One for each process.
   pid_t a, b;

   //Run two seperate processes at the same time
   a = fork();

   if (a < 0)
   {
        perror("Fork Unsuccessful");  
   }
   if(a == 0){
      printf("CHILD <PID: %d> process is executing testalphabet program!\n", getpid());

      //Execute testalphabet in first process
      char *args[]={"./testalphabet",NULL};
      execv(args[0],args);

      exit(1);
   }
   else if(a > 0){
      b = fork();

      if (b < 0)
      {
        perror("Fork Unsuccessful");  
      }

      if(b == 0){
         printf("CHILD <PID: %d> process is executing testspecial program!\n", getpid());

         //Execute testspecial in second process
         char *args1[]={"./testspecial",NULL};
         execv(args1[0],args1);

         exit(1);
      }

      //Wait for process "b" to be finished then print statement with the returned pid
      waitpid(b, NULL, 0);
      printf("CHILD <PID: %d> process has done with testspecial program! See the results above!\n", b);
   }
   //Wait for process "a" to be finished then print statement with the returned pid
   waitpid(a, NULL, 0);
   printf("CHILD <PID: %d> process has done with testalphabet program! See the results above!\n", a);
	
	return 0;
	
}
