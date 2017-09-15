/*
 * Set ADT
 * Project5.c
 *
 * Rajat Ahuja
 * Wednesday 3:00-4:00
 * Spring 2017
 * Vallath Nandakumar
 *
 */

/*TO DO
 * isMemberSet - needs a search algorithm [to not be linear]    done
 * insertSet                                                    done
 * removeSet                                                    done
 * isEqualto                                                    done
 * isSubsetOf                                                   done
 * intersectFromSet                                             done
 * subtractFromSet                                              done
 * UnionInSet
 *
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Set.h"

/*
 * Several of the required functions have been written already
 * You may use these functions as-is or you may modify them in any way.
 * Please NOTE, you are responsible for ensuring that the functions work correctly in
 * your project solution. Just because the function works correctly in my solution does
 * NOT mean that the function will work correctly in yours.
 */

/*
 * Design NOTES:
 *
 * The design provided in this starter kit assumes
 * (1) empty sets will be represented with length == 0 and elements == nullptr (i.e., address 0)
 * (2) amortized doubling is not used, and capacity is ignored/unused. Functions should assume that
 * the amount of storage available in the elements[] array is equal to length
 */



/* done for you already */
void destroySet(Set* self) {
	free(self->elements);
}

/* done for you already */
void createEmptySet(Set* self) {
	self->len = 0;
	self->elements = 0;
}

/* done for you already */
void createSingletonSet(Set* self, int x) {
	self->elements = (int*) malloc(sizeof(int));
	self->elements[0] = x;
	self->len = 1;
}

/* done for you already */
void createCopySet(Set* self, const Set* other) {
	self->elements = (int*) malloc(other->len * sizeof(int));
	for (int k = 0; k < other->len; k += 1) {
		self->elements[k] = other->elements[k];
	}
	self->len = other->len;
}

/* done for you already */
void assignSet(Set* self, const Set* other) {
	if (self == other) { return; }

	destroySet(self);
	createCopySet(self, other);
}

int searchAlg(const Set* self, int x, int start, int end) { //Implemented so that isMemberSet does not have linear scaling
    if (start > end) {
        return -1;
    }
    else {
        int midpoint = start + ((end - start) / 2);
        if (x > self->elements[midpoint]) {
            return searchAlg(self, x, midpoint + 1, end);
        }
        else if (self->elements[midpoint] > x) {
            return searchAlg(self, x, start, midpoint - 1);
        }
        else {
            return midpoint;
        }
    }
}


/* return true if x is an element of self */
bool isMemberSet(const Set* self, int x) {      //Should be sub-linear
    int index = searchAlg(self, x, 0, self->len - 1);
    if (index == -1) {
        return false;
    }
    else {
        return true;
    }

}

/*
 * add x as a new member to this set.
 * If x is already a member, then self should not be changed
 * Be sure to restore the design invariant property that elemnts[] remains sorted
 * (yes, you can assume it is sorted when the function is called, that's what an invariant is all about)
 */
void insertSet(Set* self, int x) {
    int* elements = (int*)malloc(sizeof(int)*self->len + 4);
    int token = 0;
    bool check = false;
    if (self->len == 0) {
        elements[0] = x;
        free(self->elements);
        self->elements = elements;
        self->len++;
        return;
    }
    bool exists = false;
    for (int index = 0; index < self->len; index++) {
        if (x == self->elements[index]) {
            exists = true;
        }
        if (x < self->elements[index] && check == false && exists == false) {
            elements[token] = x;
            token++;
            check = true;
        }
        else if (!check) {
            elements[token] = self->elements[index];
        }
        if (check) {
            elements[token] = self->elements[index];
        }
        token++;
        if (check == false && index == (self->len - 1) && exists == false) {
            elements[token] = x;
        }
    }

    free(self->elements);
    self->elements = elements;
    if (!exists) {
        self->len++;
    }
    return;
}

/*
 * don't forget: it is OK to try to remove an element
 * that is NOT in the set.
 * If 'x' is not in the set 'self', then
 * removeSet should do nothing (it's not an error)
 * Otherwise, ('x' IS in the set), remove x. Be sure to update self->length
 * It is not necessary (nor recommended) to call malloc -- if removing an element means the
 * array on the heap is "too big", that's almost certainly OK, and reallocating a smaller array
 * is almost definitely NOT worth the trouble
 */
void removeSet(Set* self, int x) {
    bool exists = false;
    if (self->len == 0) {   //x is clearly not in this set
        return;
    }
    if (self->len == 1) {           //if the only element in the set equals x, get rid of this element
        if (self->elements[0] == x) {
            self->len--;
        }
        return;
    }
    bool check = false;
    for (int token = 0; token < self->len; token++) {
        if (check) {
            self->elements[token] = self->elements[token + 1];  //if x exists every token gets shifted to ultimately
        }// get rid of one

        if (!check) {
            if (x == self->elements[token]) {
                check = true;
                exists = true;
                token--;
            }
        }
    }
    if (exists) {
        self->len--;
    }
    return;
}

/* done for you already */
void displaySet(const Set* self) {
	int k;

	printf("{");

	if (self->len == 0) {
		printf("}");
	}
	else {
		for (k = 0; k < self->len; k += 1) {
			if (k < self->len - 1) {
				printf("%d,", self->elements[k]);
			} else {
				printf("%d}", self->elements[k]);
			}
		}
	}
}

/* return true if self and other have exactly the same elements */
bool isEqualToSet(const Set* self, const Set* other) {
    if (self->len != other->len) {  //if the lengths are not equal then obviously the elements are not all equal
        return false;
    }
    for (int index = 0; index < other->len; index++) {
        if (self->elements[index] != other->elements[index]) {
            return false;
        }
    }
    return true;
}

/* return true if every element of self is also an element of other */
bool isSubsetOf(const Set* self, const Set* other) {
    if (self->len > other->len) {       // if there are more self elements than other, then this function is false ofc
        return false;
    }
    if (isEmptySet(self)) {
        return true;
    }
    int otherToken = 0;
    for (int token = 0; token < self->len; token++) {
        if (otherToken == other->len) {
            return false;
        }
        else if (self->elements[token] == other->elements[otherToken]) {
            otherToken++;
        }
        else if (other->elements[otherToken] > self->elements[token]) {
            return false;
        }
        else if (self->elements[token] > other->elements[otherToken]) {
            otherToken++;
            token--; }
    }
    return true;
}
/* done for you */
bool isEmptySet(const Set* self) {
	return self->len == 0;
}

/* remove all elements from self that are not also elements of other */
void intersectFromSet(Set* self, const Set* other) {
    if (isEmptySet(self)) {
        return;
    }
    if (isEmptySet(other)) {    //if there are no elements in other, then 'delete' all of the elements in self
        self->len = 0;
        return;
    }
    int* elements = (int*)malloc(4 * (self->len));
    int index;
    int index2 = 0;
    int token = 0;

    for (index = 0; index < self->len; index++) {
        if (token == other->len) {
        }
        else if (self->elements[index] == other->elements[token]) {
            elements[index2] = self->elements[index];
            token++;
            index2++;
        }
        else if (self->elements[index] < other->elements[token]) {
        }
        else if (self->elements[index] > other->elements[token]) {
            index--;
            token++;
        }
    }
    free(self->elements);
    self->elements = elements;
    self->len = index2;
    return;
}

/* remove all elements from self that are also elements of other */
void subtractFromSet(Set* self, const Set* other) {
    if (self->len == 0 || other->len == 0) {    //if either set is empty then we're already done
        return;
    }
    int token;
    int index = 0;
    int token2 = 0;
    int* elements = (int*)malloc(self->len * 4);

    for (token = 0; token < self->len; token++) {
        if (index == other->len) {
            elements[token2] = self->elements[token];
            token2++;
        }
        else if (self->elements[token] == other->elements[index]) {
            index++;
        }
        else if (self->elements[token] < other->elements[index]) {
            elements[token2] = self->elements[token];
            token2++;
        }
        else if (self->elements[token] > other->elements[index]) {
            token--;
            index++;
        }
    }
    free(self->elements);
    self->elements = elements;
    self->len = token2;
}

/* add all elements of other to self (obviously, without creating duplicate elements) */
void unionInSet(Set* self, const Set* other) {
    int index;
    int index1 = 0;
    int token = 0;
    if (other->len == 0) {
        return;
    }
    int*elements = (int*)malloc((self->len + other->len) * 4);

    for (index = 0; index < other->len; index++) {
        if (index1 == self->len) {
            elements[token] = other->elements[index];
            token++;
        }
        else if (other->elements[index] < self->elements[index1]) {
            elements[token] = other->elements[index];
            token++;
        }
        else if (other->elements[index] == self->elements[index1]) {
            elements[token] = other->elements[index];
            index1++;
            token++;
        }
        else if (other->elements[index] > self->elements[index1]) {
            elements[token] = self->elements[index1];
            index1++;
            index--;
            token++;
        }
    }
    if (index1 < self->len) {
        for (;index1 < self->len; index1++) {
            elements[token] = self->elements[index1];
            token++;
        }
    }
    self->len = token;
    free(self->elements);
    self->elements = elements;
    return;
}