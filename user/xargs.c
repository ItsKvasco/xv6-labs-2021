#include "kernel/types.h"
#include "kernel/fs.h"
#include "kernel/param.h"
#include "user/user.h"

//Tokenazing arguments to words
char **
line_to_words(int fd)
{
    char **words = (char**)malloc(sizeof(char*) * MAXARG);
    char buff[64];
    int char_count = 0, word_count = 0;
    while(read(fd, &buff[char_count], 1)){
        switch(buff[char_count]){
            case '\n':
            case ' ': ;
                char *word = (char*)malloc(sizeof(char*) * char_count);
                memmove(word, buff, char_count);
                words[word_count] = word;
                char_count = 0;
                word_count++;
                break;
            default:
                char_count++;
        }
    
    }
    return words;
}

int 
main(int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(2, "error: not enough arguments\n");
        exit(0);
    }
    //fd[0] reading file descriptor
    int fd = 0;

    //copying arguments to buffer
    char *argv_buff[MAXARG];    
    for(int i = 1; i < argc; i++){
        argv_buff[i-1] = argv[i];
    }

    //Getting tokens
    char **words = line_to_words(fd);
    //Saving pointer position for later
    char **words_tmp = words;

    //Joining all arguments
    int argc_tmp = argc-1;
    while((argv_buff[argc_tmp++] = *words++));

    //Executing arguments on 1st argument
    if(fork() == 0){
        exec(argv_buff[0], argv_buff);
    }
    wait(0);

    //Freeing memory
    while(*words_tmp){
        free(*words_tmp);
        words_tmp++;
    }
    free(words_tmp);
    
    exit(0);
}
