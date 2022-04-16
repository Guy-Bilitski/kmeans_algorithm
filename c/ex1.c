#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

double **initialize_centroids(int k, int dim, char *input_file); //Reads the first k lines in the input file and creates k centroids out of it
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

    // centroids = malloc(k * sizeof(struct Centriod));

    initialize_centroids(k, dim, input_filename);
    // printf("hi!");

    

    return 0;
}


double **initialize_centroids(int k, int dim, char *input_file) {
    FILE *ifp;
    float num;
    double **datapoints;
    datapoints = calloc(k*(dim + 1), sizeof(double));
    for (int i=0; i < k; i++) {
        datapoints[i] = calloc(dim, sizeof(double));
    }
    ifp = fopen(input_file, "r");
    if (ifp == NULL) {
        printf("An Error Has Occurred");
        return NULL;
    }

    for (int i = 0; i < k; i++) {
        datapoints[0][i] = 1;
        for (int j = 1; j < dim+1; j++) {
            fscanf(ifp, "%lf,", &datapoints[i][j]);
            printf("%lf", datapoints[i][j]);
            printf("\t");
        }

        
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

    if (c == EOF){  //ERROR in read char 
        return -1;
    }
    return d;
}