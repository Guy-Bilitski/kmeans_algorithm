#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void initialize_centroids(int k, char *input_file); //Reads the first k lines in the input file and creates k centroids out of it
int get_dimension(char *input_file);


int main(int argc, char **argv)
{
    char *output, *input;
    int k, maxiter, dim;

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

    /* Args validations:
        1. k is integer bigger than 0
        2. max itter is bigger than 0, if exists
    */
    if ( k <= 0 || (argc==5 && maxiter<=0) ){
        printf("Invalid Input! 2");
        return 1;
    }

    dim = get_dimension(input);
    if (dim == -1){
        printf("An Error Has Occurred");
    }

    






    return 0;
}


void initialize_centroids(int k, char *input_file) {
    return;
}



int get_dimension(char *input_file){
    char c;
    FILE *ifp;
    int d=1;
    ifp = fopen(*input_file, "r");
    while ((c = fgetc(ifp)) != EOF){
        if (c == ','){
            d += 1;
        } else if (c == '\n')
        {
            break;
        }
    }
    fclose(ifp);

    if (c == EOF){  //ERROR in read char
        return -1;
    }
    return d;
}