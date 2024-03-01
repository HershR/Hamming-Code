#include "bm.h"
#include "bv.h"
#include "hamming.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS "hi:o:"
uint8_t lower_nibble(uint8_t val) {
    return val & 0xF;
}
uint8_t upper_nibble(uint8_t val) {
    return val >> 4;
}

int main(int argc, char **argv) {

    int opt = 0; //store options
    FILE *infile = stdin; //default input file
    FILE *outfile = stdout; //default output file
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': //help
            fprintf(stderr, "SYNOPSIS\nA Hamming(8, 4) systematic code generator\n");
            fprintf(stderr, "USAGE\n\t./encode [-h] [-i infile] [-o outfile]\n");
            fprintf(stderr, "OPTIONS\n\t-h\tProgram usage and help\n\t-i\t Input data to "
                            "encode\n\t-o\t Output of encoded dat\n");
            return 0;
            break;
        case 'i': //infile
            infile = fopen(optarg, "r");
            if (infile == NULL) {
                fprintf(stderr, "fail to open infile");
                return 1;
            }
            break;
        case 'o': //outfile
            outfile = fopen(optarg, "w");
            if (outfile == NULL) {
                fprintf(stderr, "fail to open outfile");
                return 1;
            }
            break;
        default: //error
            fprintf(stderr, "SYNOPSIS\nA Hamming(8, 4) systematic code generator\n");
            fprintf(stderr, "USAGE\n\t./encode [-h] [-i infile] [-o outfile]\n");
            fprintf(stderr, "OPTIONS\n\t-h\tProgram usage and help\n\t-i\t Input data to "
                            "encode\n\t-o\t Output of encoded dat\n");
            return 1;
            break;
        }
    }
    struct stat statbuf;
    fstat(fileno(infile), &statbuf); //get infile permisions
    fchmod(fileno(outfile), statbuf.st_mode); //set outfile permision

    BitMatrix *G = bm_create(4, 8); //create generator matrix
    bm_set_bit(G, 0, 0);
    bm_set_bit(G, 0, 5);
    bm_set_bit(G, 0, 6);
    bm_set_bit(G, 0, 7);
    bm_set_bit(G, 1, 1);
    bm_set_bit(G, 1, 4);
    bm_set_bit(G, 1, 6);
    bm_set_bit(G, 1, 7);
    bm_set_bit(G, 2, 2);
    bm_set_bit(G, 2, 4);
    bm_set_bit(G, 2, 5);
    bm_set_bit(G, 2, 7);
    bm_set_bit(G, 3, 3);
    bm_set_bit(G, 3, 4);
    bm_set_bit(G, 3, 5);
    bm_set_bit(G, 3, 6);

    int byte = 0;
    while ((byte = fgetc(infile)) != EOF) {
        uint8_t msg1 = lower_nibble((uint8_t) byte); //get lower nibble of byte
        uint8_t msg2 = upper_nibble((uint8_t) byte); //get upper nibble
        uint8_t code1 = ham_encode(G, msg1); //encode lower nibble
        uint8_t code2 = ham_encode(G, msg2); //encode upper nibble
        fputc(code1, outfile); //put code in file
        fputc(code2, outfile);
    }
    bm_delete(&G); //delete matrix and close files
    fclose(infile);
    fclose(outfile);
    return 0;
}
