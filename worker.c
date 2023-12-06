#include <stdio.h>
#include <stdlib.h>
//Author: Connor Gilmore
//Purpose: takes in a numeric argument
//Program will iterate x amount of times, x being numeric argument
//each iteration the program will print to the console its process id, 
//its parent(caller) process id, and the loop iteration.
//the program will print this info twice each loop before and after a sleep command

int main(int argc, char** argv)
{

int argValue = 0;	
if(argc == 2)//only one arg allowrd
{//it is -t arg from os
argValue = atoi(argv[1]);
}
else{
return EXIT_FAILURE;
}

Task(argValue);//do worker task


return EXIT_SUCCESS;
}
//workers console print task
void Task(int argValue)
{
pid_t os_id = getppid();//parent process id (OS ID)
pid_t worker_id = getpid();//child process id (WORKER ID)
for(int i = 1; i <= argValue;i++)
{
printf("\nPID: %d PPID: %d Iteration: %d before sleeping\n", worker_id, os_id, i);
sleep(1);
printf("\nPID: %d PPID: %d Iteration: %d after sleeping\n", worker_id, os_id, i);
}
}
