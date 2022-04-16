#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

struct Centroid {
    float *vec_sum;
    int count;
};

void initialize_centroids(int k, int dim, char *input_file, struct Centroid *centroids); //Reads the first k lines in the input file and creates k centroids out of it
int get_dimension(char *input_file); // returns the dimention d of all vectors
// void add_vec_to_centroid(struct Centroid centroid, int *vector); // Adding a vector to a centroid
void add_two_vectors(float *vec1, float *vec2); // adding vec2 to vec1



// void add_vec_to_centroid(struct Centroid centroid, int *vector) {
//     add_two_vectors(centroid.vec_sum, vector);
//     centroid.count ++; 
// };

void add_two_vectors(float *vec1, float *vec2) {
    for (int i=0; i < sizeof(vec1); i++) {
        vec1[i] += vec2[i];
    }
}


int main(int argc, char **argv)
{
    char *output_filename, *input_filename;
    int k, maxiter, dim;
    struct Centriod *centroids;

    if (argc == 4){
        k = atoi(argv[1]);
        input_filename = argv[2];
        output_filename = argv[3];
    }

    else if (argc == 5) {
        k = atoi(argv[1]);
        maxiter = atoi(argv[2]);
        input_filename = argv[3];
        output_filename = argv[4];
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

    dim = get_dimension(input_filename);
    if (dim == -1){
        printf("An Error Has Occurred");
        return 1;
    }

    centroids = malloc(k);

    initialize_centroids(k, dim, input_filename, centroids);
    printf("hi!");

    






    return 0;
}


void initialize_centroids(int k, int dim, char *input_file, struct Centroid *centroids) {
    FILE *ifp = NULL;
    char filechar, *filestring;
    float *vector, filenumber;
    struct Centroid centroid;
    int i=0, j=0;
    vector = malloc(dim);
    ifp = fopen(input_file, "r");
    if (ifp == NULL) {
        printf("Invalid Input! 3");
    }

    // Now we need to read k first rows and insert all values as centroids to the given array



    fclose(ifp);
}



int get_dimension(char *input_file) {
    FILE *ifp = NULL;
    char c;
    int d=1;
    ifp = fopen(input_file, "r");
    if (ifp == NULL) {
        return -1;
    }
    while ((c = fgetc(ifp)) != EOF){
        if (c == ','){
            d += 1;
        } else if (c == '\n')
        {
            break;
        }
    }
    fclose(ifp);

    if (c == EOF){  //ERROR in read char TODO: why is needed? if the is only one vector?
        return -1;
    }
    return d;
}