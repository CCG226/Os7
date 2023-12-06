PROJECT 1 REDO

example compile: make all
example run: ./oss -n 1 -s 1 -t 1

FIXES:

-Added needed lib imports to fix warnings

-improved formatting

-replaced workerHandler and AwaitWorkers method with project 2 versions of those methods  as they were better optimized 

-in ValidateInput(), prevented launching oss with 0 worker amount(-n) or 0 workers simultanously (-s) or having workers run for 0 seconds (-t)
 
-makeFile is no longer broken

NOTES:
No changes to worker program
