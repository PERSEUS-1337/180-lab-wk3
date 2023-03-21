# CMSC 180 - Week 3 - Lab 02 Part 1
## Author
Author: Aron Resty Ramillano | 2020-01721
Section: T3L

## App Description
This is a new implementation of the c file froom my lab 01 part 02 progress, and it features multi-threading with the use of the C library `<pthreads.h>`

## Features
 1. It can calculate matrices that is divisible by 10 with high performance using the C language
 2. This revision has a benchmark that can test the program's performance with 3 passes on calculating matrices ranging from 100 to 20000
 3. It can calculate high numbers of matrices with the help of threading to speedup the calculation by subdividing it into smaller submatrices
 4. It can also benchmark itself automatically with predetermined n and t values

## Files included:
 - lab02-1.c
 - README.md
  
## How to setup
 - Must have GCC
 - Run this command:
	 - `gcc -o a lab02.c -lm -pthread; ./a`
	 - The `-lm` enables the `<math.h>` library for use of the `ceil` and `floor` functions
     - The `-pthread` enables the `<pthread.h>` library in my machine. It may work perfectly on your machine, but my machine requires these arguments

# Resources
- https://www.geeksforgeeks.org/multidimensional-arrays-c-cpp/
- https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/
- https://stackoverflow.com/questions/5201708/how-to-return-a-2d-array-from-a-function-in-c
- https://stackoverflow.com/questions/2150291/how-do-i-measure-a-time-interval-in-c
