#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv)
{
    char *output, *input;
    int k, maxiter;

    if (argc == 4){
        k = atoi(argv[1]);
        input = argv[2];
        output = argv[3];
    }

    else if (argc == 5) {
        k = atoi(argv[1]);
        maxiter = atoi(argv[2]);
        input = argv[3];
        output = argv[4];
    }
    else {
        printf("Invalid Input! 1");
        return 1;
    }

    int outputlength = strlen(output);

    /* Args validations:
        1. output file length and .txt suffix
        2. k is integer bigger than 0
        3. max itter is bigger than 0, if exists
    */
    if (outputlength < 4 || k <= 0 || (argc==5 && maxiter<=0) ){
        printf("Invalid Input! 2");
        return 1;
    }

    int checkOutput = strcmp(output+outputlength-4, ".txt");
    printf("%d", checkOutput);

    return 0;
}


