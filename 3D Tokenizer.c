#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#define PROMPT "Please enter some sentences:"                                  // Setting the user prompt
#define PROMPTSIZE sizeof(PROMPT)
#define SENTENCES_PER_TEXT 30                                                  // Setting constant parameters
#define WORDS_PER_SENTENCE 40
#define CHARACTERS_PER_WORD 10

int
main(int argc, char** argv) {

    // Memory allocation
    char ***myarray = malloc(sizeof(char**)*SENTENCES_PER_TEXT);
    *myarray = malloc(sizeof(char*)*WORDS_PER_SENTENCE);
    **myarray = malloc(sizeof(char)*CHARACTERS_PER_WORD);
    char *prompt_buffer = malloc(sizeof(char)*PROMPTSIZE);
    char *character = malloc(sizeof(char));
    int bytes_formatted, bytes_written, characters_copied, words_copied, sentences_copied;

    //User prompt
    bytes_formatted = sprintf(prompt_buffer, "%s", PROMPT);                    // Loading the prompt into the buffer
    if (bytes_formatted < 0) {
        perror("Error in prompt call");
        return 1;
    }
    bytes_written = write(1, prompt_buffer, bytes_formatted);                  // Printing prompt buffer to the screen
    if (bytes_written < 0) {
        perror("Error in write system call");
        return 1;
    }

    // Tokenizer
    characters_copied = 0;                                                     // Initializing the variables
    words_copied = 0;
    sentences_copied = 0;
    while(1)
    {
        read(0, character, 1);                                                 // Reading user input
        if (*character == ' ' || *character == '\n' || *character == '\0' || *character == ',' || *character == '.' || *character == '!' || *character == '?')
        {
            ***myarray  = '\0';                                                // Words are null terminated
            **myarray +=- characters_copied*(sizeof(char));                    // Setting the pointer to the first character of the current word
            write(1, **myarray, characters_copied);                            // Writing word to screen
            write(1, "\n", 1);
            *myarray += sizeof(char*);                                         // Shifting the pointer to the next word
            characters_copied = 0;                                             // Resetting the number of characters copied
            words_copied++;                                                    // Increment number of words copied
            if (*character == '\n' || *character == '.' || *character == '!' || *character == '?') {
                write(1, "\n", 1);
                *myarray +=- words_copied*sizeof(char*);                       // Setting the pointer to the first word of the current sentence
                words_copied = 0;                                              // Resetting the number of words copied
                if(*character == '.' || *character == '!' || *character == '?'){
                    sentences_copied++;                                        // Increment number of sentences copied
                    myarray += sizeof(char**);                                 // Shifting the pointer to the next sentence
                    *myarray = malloc(sizeof(char*)*WORDS_PER_SENTENCE);       // Dynamic allocation of space for the next sentence
                    **myarray = malloc(sizeof(char)*CHARACTERS_PER_WORD);
                    continue;
                }
                break;
            }
            **myarray = malloc(sizeof(char)*CHARACTERS_PER_WORD);              // Dynamic allocation of space for the next word
            continue;
        }
        ***myarray = *character;                                               // Placing individual characters into an array
        **myarray += sizeof(char);                                             // Shifting the pointer to the next character
        characters_copied++;                                                   // Increment number of characters copied
    }
    myarray +=- sizeof(char**)*sentences_copied;                               // Setting a pointer to the first word of the first sentence.
    // myarray is the array of given sentences(arrays of words)
    return 0;
}