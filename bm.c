#include "bm.h"

#include "bv.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
struct BitMatrix {
    uint32_t rows;
    uint32_t cols;
    BitVector *vector;
};
BitMatrix *bm_create(uint32_t rows, uint32_t cols) {
    BitMatrix *m = (BitMatrix *) malloc(sizeof(BitMatrix)); //create bitmatrix
    if (m) {
        m->rows = rows;
        m->cols = cols;
        m->vector = bv_create(rows * cols); //create bitvector
        if (!(m->vector)) {
            free(m);
            m = NULL; //free and return null pointer if bitvector not create
        }
    }
    return m;
}
void bm_delete(BitMatrix **m) {
    if (*m && (*m)->vector) {
        bv_delete(&(*m)->vector); //delte bitvector
        free(*m); //free bitmatrix
        *m = NULL; //pointer me to null
    }
    return;
}
uint32_t bm_rows(BitMatrix *m) {
    return m->rows; //return num rows
}
uint32_t bm_cols(BitMatrix *m) {
    return m->cols; //return num columns
}
void bm_set_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint32_t l = r * (m->cols) + c; //get the position of bit
    bv_set_bit(m->vector, l); //set bit at position l
}
void bm_clr_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint32_t l = r * (m->cols) + c;
    //printf("clr_bit l:%d\n",l);
    bv_clr_bit(m->vector, l); //clear bit
}
uint8_t bm_get_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    uint32_t l = r * (m->cols) + c;
    //printf("get_bit:%d\n",l);
    return bv_get_bit(m->vector, l); //get bit value 1 or 0
}
BitMatrix *bm_from_data(uint8_t byte, uint32_t length) {
    assert(length < 9);
    BitMatrix *m = bm_create(1, length); //create 1xlength matrix
    for (uint32_t i = 0; i < length; i++) { //iterate matrix
        uint8_t b = 1 << i; //store bits from byte
        b &= byte; // AND byte and b
        b = b >> i; //shift back into orignal position
        if (b == 1) {
            bv_set_bit(m->vector, i);
        }
    }
    return m;
}
uint8_t bm_to_data(BitMatrix *m) {
    uint8_t x = 0; //store the data
    for (uint32_t i = 0; i < m->rows; i++) {
        for (uint32_t j = 0; (i * m->cols + j) < 8 && j < m->cols; j++) {
            //printf("i:%d, j:%d\n",i,j);
            uint8_t b = bm_get_bit(m, i, j); //get every bit
            x = x + (b << (i * m->cols + j)); //move them into position and add to x
        }
    }
    return x;
}
BitMatrix *bm_multiply(BitMatrix *A, BitMatrix *B) {
    assert(A->cols == B->rows);
    BitMatrix *C = bm_create(A->rows, B->cols);
    //printf("C: %s",C?"true":"false");
    if (C) {
        for (uint32_t i = 0; i < C->rows; i++) {
            for (uint32_t j = 0; j < C->cols; j++) {
                uint8_t x = 0; //stores the cross product of 2 vectors
                //printf("k<: %d\n",A->cols);
                for (uint32_t k = 0; k < A->cols; k++) {
                    uint8_t a = bm_get_bit(A, i, k);
                    uint8_t b = bm_get_bit(B, k, j);
                    x += a && b; //add a AND b to x
                    //printf("a:%d b:%d a&&b:%d x:%d\n",a,b,a&&b,x);
                }
                x = x % 2;
                //printf("x mod 2: %d\n",x);
                if (x == 1) {
                    bm_set_bit(C, i, j);
                }
            }
        }
    }
    return C;
}
void bm_print(BitMatrix *m) {
    for (uint32_t i = 0; i < m->rows; i++) {
        for (uint32_t j = 0; j < m->cols; j++) {
            printf("%d ", bm_get_bit(m, i, j));
        }
        printf("\n");
    }
}
