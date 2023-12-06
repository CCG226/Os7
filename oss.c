#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "oss.h"
//Author: Connor Gilmore
//Purpose: Program task is to launch x amount of worker processors with specfied params
//takes an arguments -h for help,
//-n for number of child processes to run
//-s for number of child processes to run simultanously
//-t is the numeric value passed to worker child processes 


int main(int argc, char** argv) {
printf("\n\n\n");
//default values for argument variables are 0
int workerAmount = 0;//-n proc value (number of workers to launch)
int simultaneousLimit = 0;//-s simul value (number of workers to run in sync)
int workerArg = 0;//-t iter value (number to pass to worker as parameter)

//get CL arguments
//pass reference memory address value for above variables so we can assign 
//variables in ArgumentParser function
ArgumentParser(argc, argv,&workerAmount, &simultaneousLimit, &workerArg);

//use argument information to control Worker behavior
WorkerHandler(workerAmount, simultaneousLimit, workerArg);

printf("\n\n\n");
return EXIT_SUCCESS;

}
//use getopt to parse through CL arguments
//and assign argument values to workerAmount, simultaneousLimit, workerArg variables in main using ptrs 
void ArgumentParser(int argc, char** argv, int* workerAmount, int* workerSimLimit, int* workerArgument) {

int option;
//getopt to iterate over CL options
while((option = getopt(argc, argv, "hn:s:t:")) != -1)
{
switch(option)
{//junk values (non-numerics) will default to 0 with atoi which is ok
 case 'h'://if -h
  Help();
  break;
 case 'n'://if -n int_value
  *(workerAmount) = atoi(optarg);
  break;
 case 's'://if -s int_value
  *(workerSimLimit) = atoi(optarg);
  break;
 case 't'://if t int_value
  *(workerArgument) = atoi(optarg);
  break;
 default://give help info
   Help();
   break; 
}

}
//check if arguments are valid 
int isValid = ValidateInput(*workerAmount, *workerSimLimit, *workerArgument);

if(isValid == 0)
{//valid arguments 
 return;	
}
else
{
exit(1);
}
}
//check iv command line arguments provided are not negative values, if they are end program
int ValidateInput(int workerAmount, int workerSimLimit, int workerArgument)
{
	
 if(workerAmount < 1 || workerSimLimit < 1 || workerArgument < 1)
 {
 printf("\nInput Arguments Cannot Be Zero Or A Negative Number!\n");	 
  return 1;	
 }
 else {
 return 0;
 } 
}
//handles worker launching behavior bassed on command line parameters
//workerAmount = -n
//workerSimLimit = -s
//workerArgument = -t
void WorkerHandler(int workerAmount, int workerSimLimit,int workerArgument)
{
 

	//tracks amount of workers finished with task
	int workersComplete = 0;

	//tracks amount of workers left to be launched
	int workersLeft = 0;

	if(workerSimLimit > workerAmount)
	{
		//if (-s) values is greater than (-n) value, then launch all workers at once	
		WorkerLauncher(workerAmount, workerArgument);
		//no more workers left to be launched
		workersLeft = 0;

	}
	else
	{

		//workerAmount (-n) is greater than or equal to  WorkerSimLimit (-s), so launch (-s)  amount of workers	
		WorkerLauncher(workerSimLimit, workerArgument);	

		//subtract simultanoues limit from amount of workers (n) to get amount of workers left to launch
		workersLeft = workerAmount - workerSimLimit;

	}
	//keep looping until all workers (-n) have finished working
	while(workersComplete !=  workerAmount)
	{
		int WorkerFinishedId = AwaitWorker();//check to see if a worker is done, returns 0 if none are done, returns id of worker done if a worker is done

		if(WorkerFinishedId != 0)//if a 0 is returned, then no workers are currently done at them moement
		{//if a worker is finished
			workersComplete++;
			//worker no longer occupied

			if(workersLeft != 0)
			{ //if we are allowed to, launch another worker
				WorkerLauncher(1,workerArgument);
				workersLeft--;
			}
		}
	}
}

//pass in amount of workers that were launched by launcher
//wait for all workers launched to finish their task here
////done by oss program (parent)
int AwaitWorker()
{
	
 int stat = 0;
int pid = waitpid(-1, &stat, WNOHANG);

 if(WIFEXITED(stat)) {
 if(WEXITSTATUS(stat) != 0)
 {	 
  exit(1);
 }
 }

return pid;
}
//takes in -s simultanous limit as limit value
////takes in -t argument to send to worker as workerArgument
void WorkerLauncher(int limit,int workerArgument){
//convert workerArgument to char array using sprint technique
char workerArgumentString[10];
sprintf(workerArgumentString, "%d", workerArgument);

//keep launching workers until limit (is reached
//to create workers, first create a new copy process (child)
//then replace that child with a worker program using execlp
//execlp:, the l lets us pass in n amount of arguments so we can pass workerArgument
////the p handles the directory PATH to our worker.c for us 

for(int i = 0 ; i < limit; i++)
{
pid_t newProcess = fork();
 if(newProcess < 0)
 {
  printf("\nFailed To Create New Process\n");
  exit(1);
 }
 if(newProcess == 0)
 {//child process (workers launching and running their software)
 execlp("./worker", "./worker", workerArgumentString, NULL);

 printf("\nFailed To Launch Worker\n");
 exit(1);
 }

}
	

}
//help info
void Help() {
printf("When executing this program, please provide three numeric arguments");
printf("Set-Up: oss [-h] [-n proc] [-s simul] [-t iter]");
printf("The [-h] argument is to get help information");
printf("The [-n int_value] argument to specify the amount of workers to launch");
printf("The [-s int_value] argument to specify how many workers are allowed to run simultaneously");
printf("The [-t int_value] argument is for the worker process to know how many times it should run its task");
printf("Example: ./oss -n 5 -s 3 -t 7");
printf("\n\n\n");
}
