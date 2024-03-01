# Assignment Hamming Code
Programming assignment from 2021

encode.c
This program reads a file and encodes the data in the file into Hamming(8,4) code. The encoded data is then outputted to a desired location.
The program takes the following options: -h help,-i infile(file to read from- defualt stdin) -o outfile(file to print to- default stdout).

decode.c
This program reads a file containing data encoded in Hamming(8,4) and decodes it back into its orignal message. If the data contains errors, it will try to fix them if possible.
The program takes the following options: -h help,-i infile(file to read from- defualt stdin) -o outfile(file to print to- default stdout), -v print out stats(bytes processsed, corrected errors, uncorrected errors, error rate(uncorrected/bytes processed)).

## Build
	$ make all
## Running
	$ ./encode [-i file name] [-o file name]
	$ ./decode [-v] [-i file name] [-o file name]
## Cleaning
	$ make clean
## Other files
	bv.c		- contains the implmentation of the bit vector
	bm.c		- contains the implmentation of the bit matrix
	hamming.c	- contains the code to encode data into Hamming(8,4) and decode it.	
	error.c		- adds errors to a file
	entropy.c	- calculates the entropy of a file
	DESIGN.PDF	- psudeocode for bv.c, bm.c, hamming.c, encode.c, decode.c
	Makefile	- builds encode, decode, error, entropy
	WRITEUP.PDF	- analysis of the entropy of files with errors
