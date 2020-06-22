# ELEN-4020 PROJECT
**Group Members:**
- Ntladi Mohajane - 1599953
- Phetolo Malele - 1412331
- Zwavhudi Mulelu - 1110574


## Indexing the Hansard dataset using parallel programming 
This project is written in C++, the OpenMP library was used for parallel processing and the bitmap technique was used to generate indexes.

## Compile Instructions
In order to emulate the results recorded in the project report as close as possible, please note the following:

* Intel Core i7-6700 CPU @ 3.4GHz (4 CPU Cores, 8 Logical Threads due to HyperThreading).
* 8.00 GB 2133MHz Dual Channel DDR4 RAM
* Linux Ubuntu 18.04 LTS (64-bit)
 

In order to produce the execution time results, clone the repository and run the `benchmark.sh` script in the root folder.
The script will compile all the source code located in the **src** folder.
Once compiled, the executable files can be found in the **executables** folder.
The input Hansard file can be found in the **executables** folder.
The MakeFile can also be found in the **executables** folder.

In order to get the output of the program, run the `make` command and then run the following command:


`./project`

The results will be shown in the terminal window or console.

 
