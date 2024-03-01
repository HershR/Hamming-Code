#include "hamming.h"

#include "bm.h"

#include <stdint.h>
#include <stdio.h>

/*typedef enum HAM_STATUS{
    HAM_OK=-3,
    HAM_ERR=-2,
    HAM_CORRECT=-1,
} HAM_STATUS;*/
HAM_STATUS table[] = { HAM_OK, 4, 5, HAM_ERR, 6, HAM_ERR, HAM_ERR, 3, 7, HAM_ERR, HAM_ERR, 2,
    HAM_ERR, 1, 0, HAM_ERR };

uint8_t ham_encode(BitMatrix *G, uint8_t msg) {
    BitMatrix *m = bm_from_data(msg, 4); //turn message into bitmatrix
    BitMatrix *c = bm_multiply(m, G); //multiply m with G to encode
    uint8_t code = bm_to_data(c); //convert matrix to data
    bm_delete(&m);
    bm_delete(&c);
    return code;
}

HAM_STATUS ham_decode(BitMatrix *Ht, uint8_t code, uint8_t *msg) {
    BitMatrix *c = bm_from_data(code, 8); //turn code into matrix
    //bm_print(c);
    BitMatrix *e = bm_multiply(c, Ht); //multiply c with ht
    //printf("e: ");
    //bm_print(e);
    uint8_t error = bm_to_data(e); //turn e into data
    bm_delete(&e);
    //printf("error: %d,table: %d\n",error,table[error]);
    if (table[error] == HAM_OK || table[error] == HAM_ERR) { //check e
        *msg = bm_to_data(c) & 0xF; //if no errors or uncorectable leave dont change byte
        bm_delete(&c);
        return table[error]; //return HAM_STATUS
    } else {
        uint8_t b = bm_get_bit(c, 0, table[error]);
        //printf("b: %d\n",b);
        if (b == 1) { //flip bit
            bm_clr_bit(c, 0, table[error]);
        } else {
            bm_set_bit(c, 0, table[error]);
        }
        *msg = bm_to_data(c) & 0xF; //get lower nibble
        bm_delete(&c);
        return HAM_CORRECT;
    }
}
