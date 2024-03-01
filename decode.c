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

#define OPTIONS "hvi:o:"

uint8_t pack_byte(uint8_t upper, uint8_t lower) {
    return (upper << 4) | (lower & 0xF);
}

int main(int argc, char **argv) {

    bool stats = false; //deterimines if we print stats
    uint32_t bproccessed = 0; //bytes proccessed
    uint32_t uerrors = 0; //uncorrected errors
    uint32_t cerrors = 0; //corrected errors
    double errate = 0; //error rate
    int opt = 0; //store options
    FILE *infile = stdin; //default input file
    FILE *outfile = stdout; //default output file
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': //help
            fprintf(stderr, "SYNOPSIS\nA Hamming(8, 4) systematic code decoder\n");
            fprintf(stderr, "USAGE\n\t./encode [-h] [-v] [-i infile] [-o outfile]\n");
            fprintf(stderr, "OPTIONS\n\t-h\tProgram usage and help\n\t-v\tPrint statistics\n\t-i\t "
                            "Input data to encode\n\t-o\t Output of encoded dat\n");
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
        case 'v': stats = true; break;
        default: //error
            fprintf(stderr, "SYNOPSIS\nA Hamming(8, 4) systematic code decoder\n");
            fprintf(stderr, "USAGE\n\t./encode [-h] [-v] [-i infile] [-o outfile]\n");
            fprintf(stderr, "OPTIONS\n\t-h\tProgram usage and help\n\t-v\tPrint statistics\n\t-i\t "
                            "Input data to encode\n\t-o\t Output of encoded dat\n");
            return 0;
            break;
        }
    }
    struct stat statbuf;
    fstat(fileno(infile), &statbuf); //get infile permisions
    fchmod(fileno(outfile), statbuf.st_mode); //set outfile permision

    BitMatrix *Ht = bm_create(8, 4); //parity-checker matrix transpose
    bm_set_bit(Ht, 0, 1);
    bm_set_bit(Ht, 0, 2);
    bm_set_bit(Ht, 0, 3);
    bm_set_bit(Ht, 1, 0);
    bm_set_bit(Ht, 1, 2);
    bm_set_bit(Ht, 1, 3);
    bm_set_bit(Ht, 2, 0);
    bm_set_bit(Ht, 2, 1);
    bm_set_bit(Ht, 2, 3);
    bm_set_bit(Ht, 3, 0);
    bm_set_bit(Ht, 3, 1);
    bm_set_bit(Ht, 3, 2);
    bm_set_bit(Ht, 4, 0);
    bm_set_bit(Ht, 5, 1);
    bm_set_bit(Ht, 6, 2);
    bm_set_bit(Ht, 7, 3);
    int byte = 0;
    int status; //status of ham_decode
    uint8_t hold = 0; //holds last correct msg
    uint8_t msg1 = 0; //store decoded msg, lower nibble
    uint8_t msg2 = 0; //store decoded msg, upper nibble
    while ((byte = fgetc(infile)) != EOF) { //keep reading chars until end of file
        hold = msg1;
        status = ham_decode(Ht, (uint8_t) byte, &msg1); //decode byte and return status
        bproccessed += 1;
        if (status == -1) { //update stats based on status
            cerrors += 1;
        } else if (status == -2) {
            uerrors += 1;
            msg1 = hold; //if uncorected error, use last correct msg
        }
        hold = msg2;
        if ((byte = fgetc(infile)) != EOF) { //get next char
            status = ham_decode(Ht, (uint8_t) byte, &msg2);
            bproccessed += 1;
            if (status == -1) {
                cerrors += 1;
            } else if (status == -2) {
                uerrors += 1;
                msg2 = hold; //use last correct msg
            }
        }
        uint8_t dmsg = pack_byte(msg2, msg1); //combine lower and upper nibble
        fputc(dmsg, outfile); //put full msg in outfile
        //if(status==-2){
        //    fprintf(stderr,"%d %d\tmsg: %d\n",msg2,msg1,dmsg);
        //}
    }
    if (stats) { //print stats
        errate = (double) uerrors / (double) bproccessed;
        fprintf(stderr, "Total bytes processed: %d\n", bproccessed);
        fprintf(stderr, "Uncorrected errors: %d\n", uerrors);
        fprintf(stderr, "Corrected errors: %d\n", cerrors);
        fprintf(stderr, "Error rate: %1.6f\n", errate);
    }
    bm_delete(&Ht); //delete matrix and close files
    fclose(infile);
    fclose(outfile);
    return 0;
}
