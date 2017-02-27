/*
 * Rajat Ahuja
 * Project 3
 * EE312 Spring 2017
 * Dr. Nandakumar TTH 2-3:30
 */

#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "String.h"
#include <stdio.h>

/* use these two macros and the function functions if you want -- not required */
#define SIGNATURE (~0xdeadbeef)

/** STRING(s)
 * Helper macro for accessing ut String object (if applicable)
 * [INPUT] pointer to cstring
 * [OUTPUT] pointer to UT String object (if applicable)
 */
#define STRING(s) ((String*)(s - 3*sizeof(uint32_t)))

/* this simple function can be useful when you implement stage 4
 * you are not required to use the function, and you can implement stage 4 without it */
int32_t isOurs(const char* s) {             //not sure what to do here - if anything (apparently stage 4 isn't happening - ask TA
    if (STRING(s)->check == SIGNATURE) { return 1; }
    else { return 0; }
}

/* allocate a utstring on the heap, initialize the string correctly by copying
 * the characters from 'src' and return a pointer to the first character of actual string data */
char* utstrdup(const char* src) {

    uint32_t size = strlen(src);

    String* betterString = (String*)malloc(sizeof(String) + (size + 1)); //allocates the string


    betterString->length = size;
    betterString->check = SIGNATURE;        // checks where data is coming from
    betterString->capacity = size;          //set both the length and capacity of the new array to the same #

    strcpy(betterString->data, src);

    return (betterString->data);    //returns a pointer to the "betterString" new data
}


/* the parameter 'utstr' must be a utstring. Find the length of this string by accessing the meta-data
 * and return that length */
uint32_t utstrlen(const char* utstr) {

    assert(utstr != 0);

    assert(STRING(utstr)->check == SIGNATURE);      //checks where data comes from

    return (STRING(utstr)->length);    //returns the length of our new string
}

/* s must be a utstring. suffix can be an ordinary string (or a utstring)
 * append the characters from suffix to the string s. Do not allocate any additional storage, and
 * only append as many characters as will actually fit in s. Update the length meta-data for utstring s
 * and then return s */
char* utstrcat(char* s, const char* suffix) {

    assert(s != 0);
    assert(STRING(s)->check == SIGNATURE);  //checks where data comes from

    int space = ((STRING(s)->capacity) - (STRING(s)->length));  //determines how much space is available post-string
    uint32_t k;

    for (k = 0; (suffix[k] != 0) && (k < space) ; k++) {
        int free = (STRING(s)->length + k);
        STRING(s)->data[free] = suffix[k];
    }

    STRING(s)->length += k;       //increases the length of the string based on how many were added
    STRING(s)->data[STRING(s)->length] = NULL;     //updates new string

    return (STRING(s)->data);
}

/* 'dst' must be a utstring. 'src' can be an ordinary string (or a utstring)
 * overwrite the characters in dst with the characters from src. Do not overflow the capacity of dst
 * For example, if src has five characters and dst has capacity for 10, then copy all five characters
 * However, if src has ten characters and dst only has capacity for 8, then copy only the first 8
 * characters. Do not allocate any additional storage, do not change capacity. Update the length
 * meta-data for dst and then return dst */
char* utstrcpy(char* dst, const char* src) {

    assert(dst != 0);       //overwrite dst with src up until src's capacity.
    assert(STRING(dst)->check == SIGNATURE);

    uint32_t counter;
    for (counter = 0; ((src[counter] != NULL && counter < STRING(dst)->capacity)); counter++) {
        STRING(dst)->data[counter] = src[counter];
    }
    STRING(dst)->data[counter] = NULL;

    STRING(dst)->length = counter;

    return (STRING(dst)->data);
}

/* self must be a utstring. deallocate the storage for this string
 * (i.e., locate the start of the chunk and call free to dispose of the chunk, note that the start of
 * the chunk will be 12 bytes before *self) */
void utstrfree(char* self) {

    assert(self != 0);
    assert(STRING(self)->check == SIGNATURE);

    free(STRING(self)); //free up the memory that was originally used for the new string
}

/* s must be a utstring.
 * ensure that s has capacity at least as large as 'new_capacity'
 * if s already has capacity equal to or larger than new_capacity, then return s
 * if s has less capacity than new_capacity, then allocate new storage with sufficient space to store
 * new_capacity characters (plus a terminating zero), copy the current characters from s into this
 * new storage. Update the meta-data to correctly describe new new utstring you've created, deallocate s
 * and then return a pointer to the first character in the newly allocated storage */

char* utstrrealloc(char* s, uint32_t new_capacity) {

    assert(s != 0);
    assert(STRING(s)->check == SIGNATURE);

    if (STRING(s)->capacity >= new_capacity) {  //from second line of comment if the capacity>= new then we are done
        return (STRING(s)->data);
    }
    else {
        uint32_t string_length = STRING(s)->length;
        char temp[new_capacity];

        strcpy(temp, STRING(s)->data);
        String* newString = (String*)realloc(STRING(s), ((sizeof(String) + new_capacity))); //reallocates if there is not enough storage
        strcpy(newString->data, temp);

        newString->capacity = new_capacity;
        newString->check = SIGNATURE;
        newString->length = string_length;


        return (newString->data);     //returns the pointer to data for the newly allocated storage
    }
}