//Hersh  Rudrawal
#include "bv.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//inspired from Eugene's section
struct BitVector {
    uint32_t length; //length of vector
    uint8_t *vector; //array of bytes
};
BitVector *bv_create(uint32_t length) {
    BitVector *v = (BitVector *) malloc(sizeof(BitVector));
    if (v) {
        v->length = length;
        //length is the number of bits, so we dive length by 9 and add 1 to get the correct num of bytes
        v->vector = (uint8_t *) calloc(length / 9 + 1, sizeof(uint8_t));
        if (!v->vector) {
            free(v);
            v = NULL;
        }
    }
    return v;
}
void bv_delete(BitVector **v) {
    if (*v && (*v)->vector) {
        free((*v)->vector);
        free(*v);
        *v = NULL;
    }
    return;
}
uint32_t bv_length(BitVector *v) {
    return (v->length);
}
void bv_set_bit(BitVector *v, uint32_t i) {
    assert(i < v->length);
    uint32_t l = i / 8; //which byte the bit is in
    uint32_t p = i % 8; //position of the bit
    uint8_t b = 1 << p; //shift bit into position
    v->vector[l] |= b; //OR operation with b
}
void bv_clr_bit(BitVector *v, uint32_t i) {
    assert(i < v->length);
    uint32_t l = i / 8; //which byte the bit is in
    uint32_t p = i % 8; //position of the bit
    uint8_t b = 1 << p;
    v->vector[l] &= ~b; //AND operation with flipped b
}
void bv_xor_bit(BitVector *v, uint32_t i, uint8_t bit) {
    assert(i < v->length);
    uint32_t l = i / 8; //which byte the bit is in
    uint32_t p = i % 8; //position of the bit
    bit = bit << p;
    v->vector[l] ^= bit; //XOR operation with b
}
uint8_t bv_get_bit(BitVector *v, uint32_t i) {
    assert(i < v->length);
    uint32_t l = i / 8; //which byte the bit is in
    uint32_t p = i % 8; //position of the bit
    uint8_t b = 1 << p;
    b &= ((v)->vector[l]); //b AND bitvector
    b = b >> p; //shift b back to original position
    return b;
}
void bv_print(BitVector *v) {
    for (uint32_t i = 0; i < v->length; i++) {
        printf("%d ", bv_get_bit(v, i));
    }
    printf("\n");
}
