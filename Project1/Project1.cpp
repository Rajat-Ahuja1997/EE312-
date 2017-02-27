/*
 * Project1.cpp
 * 
 * Name: Rajat Ahuja
 * EE312 Spring 2017
 * Nandakumar
 * Tuesday/Thursday 2-3:30
 * SpellCheck
 */

#include <stdio.h> // provides declarations for printf and putchar
#include <stdint.h> // provides declarations for int32_t uint32_t and the other (new) standard C types

/* All of your code must be in this file. Please no #includes other than standard system headers (ie.., stdio.h, stdint.h)
 *
 * Many students find it helpful to declare global variables (often arrays). You are welcome to use
 * globals if you find them helpful. Global variables are by no means necessary for this project.
 */

/* You must write this function (spellCheck). Do not change the way the function is declared (i.e., it has
 * exactly two parameters, each parameter is a standard (mundane) C string (see SpellCheck.pdf).
 * You are expected to use reasonable programming style. I *insist* that you indent
 * reasonably and consistently in your code. I strongly encourage you to avoid big functions
 * So, plan on implementing spellCheck by writing two or three other "support functions" that
 * help make the actual spell checking easier for you.
 * There are no explicit restrictions on using functions from the C standard library. However,
 * for this project you should avoid using functionality from the C++ standard libary. You will
 * almost certainly find it easiest to just write everything you need from scratch!
 */



//Converts the string from any case to lowercase - roughly from Nandakumar lecture
void convertToLower(char token[]){
    uint32_t i = 0; //parameter for word
    while (token[i] != 0){
        if (token[i] >='A' && token[i] <= 'Z'){   //If uppercase, make lowercase
            char c = token[i];          //if lowercase, nothing happens
            c = c + ('z' - 'Z');
            token[i] = c;
        }
        i++;
    }
}


// Creates null-terminated strings
void split(int beginning, int end, char article[], char word[])
{   int position = 0;
    int size = (end - beginning);

    while (position < size){
        word[position] = article[beginning];
        position++;
        beginning++;
    }
    word[position] = 0;  // Null-terminates each word
}
//Checks if a word is in the dictionary; returning 0 = the word should be printed
int parseDictionary(char theWord[], char dictionary[], int wordlength){
    int DictionaryWordLength;
    char dictionaryWord[50];
    int i = 0;
    int start = 0;
    int match;
    while (dictionary[i] != 0)  //while not at the end of the dictionary
    {
        start = i;
        while (dictionary[i] != '\n')
        {
            i++;
        }


        split(start, i, dictionary, dictionaryWord);
        DictionaryWordLength = i - start;   //last letter - first letter
        match = 0;
        convertToLower(theWord);
        convertToLower(dictionaryWord); // converts the article word and a dictionary word to lowercase.




        if (wordlength == DictionaryWordLength){
            int a = 0;
            while (dictionaryWord[a] != 0)
            {
                if (dictionaryWord[a] == theWord[a]){
                    match += 1;
                }
                a++;
            }
            if (match == DictionaryWordLength){
                return 1;
            }
        }
        i++; // Moves to the next dictionary word
    }
    return 0;
}


void spellCheck(char article[], char dictionary[]) {   //"Main" subroutine to be run by the main function in main.cpp
    int i= 0, first = 0;
    char theWord[50];//
    while (article[i] != 0){   //While looping through the article
        first = i;
        while((article[i] >= 'A' && article[i] <= 'Z') || (article[i] >= 'a' && article[i] <= 'z')) //If the selected character is a letter
        {
            if (article[i] == 0)    //if the end of a string is reached
            { break; }              //break loop
            i++;
        }
        if (i > first + 1){      //if the "word" is a word, according to the convention of the instructions (more than two letters)
            split(first, i, article, theWord);
            if (parseDictionary(theWord, dictionary, i - first) != 1 ){
                printf("%s \n", theWord);
            }
        }
        if (article[i] == 0)
        { break; }
        i++;
    }
}
