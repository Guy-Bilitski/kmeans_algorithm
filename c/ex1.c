#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <limits.h>

void initialize_centroids(int k, int dim, char *input_file, double**); //Reads the first k lines in the input file and creates k centroids out of it
int get_dimension(char *input_file); // returns the dimention d of all vectors
void add_two_vectors(float *vec1, float *vec2); // adding vec2 to vec1
double max_distance_between_centroids(int k, int dim, double **old_centroids, double **new_centroids);
void kmeans_iteration(int k, int dim, char *input_file, double **centroids, double **new_centroids);
int find_closest_centroid(int k, int dim, double *vector, double **centroids);


int main(int argc, char **argv)
{
    char *output_filename, *input_filename;
    int k, maxiter = -1, dim;
    double **centroids;
    double **new_centroids;

    if (argc == 4 || argc == 5) {
        k = atoi(argv[1]);
        input_filename = argv[argc-2];
        output_filename = argv[argc-1];
        maxiter = argc==5 ? atoi(argv[2]) : INT_MAX;
        if (maxiter <= 0 || k <= 0) {
            printf("Invalid Input! 0");
            return 1;
        }
    }

    else {
        printf("Invalid Input! 1");
        return 1;
    }

    dim = get_dimension(input_filename);
    if (dim == -1){
        printf("An Error Has Occurred");
        return 1;
    }

    centroids = calloc(k, sizeof(double *));
    new_centroids = calloc(k, sizeof(double *));
    for (int i=0; i < k; i++) {
        centroids[i] = calloc(dim + 1, sizeof(double));
        new_centroids = calloc(k, sizeof(double *));
    }

    initialize_centroids(k, dim, input_filename, centroids);

    for (int i=0; i < maxiter; i++) {
        kmeans_iteration(k, dim, input_filename, centroids, new_centroids);
        if (max_distance_between_centroids(k, dim, centroids, new_centroids) < 0.001) {
            break;
        }
        double ** temp = centroids;
        centroids = new_centroids;
        new_centroids = temp;
    }

    for (int i = 0; i < k; i++) {
        
        for (int j = 0; j < dim; j++) {
            printf("%lf,", new_centroids[i][j]/new_centroids[i][dim]);
        }
        printf("\n");
    }
    free(centroids);
    free(new_centroids);

    return 0;
}


double max_distance_between_centroids(int k, int dim, double **old_centroids, double **new_centroids) {
    double max_value = DBL_MAX;
    double current_value;

    for (int i=0; i < k; i++) {
        current_value = 0;
        for (int j=0; k < dim; j++) {
            current_value = pow((old_centroids[i][j] / old_centroids[i][dim]) - (new_centroids[i][j] / new_centroids[i][dim]), 2);
            if (current_value > max_value) {
                max_value = current_value;
            }
        }
    } 

    return max_value;
}


void kmeans_iteration(int k, int dim, char *input_file, double **centroids, double **new_centroids) {
    FILE *ifp;
    // double **new_centroids;
    int closet_centroid_index;
    // new_centroids = calloc(k, sizeof(double *));
    // for (int i=0; i < k; i++) {
    //     new_centroids[i] = calloc(dim + 1, sizeof(double));
    // }
    double *vector;
    vector = calloc(dim, sizeof(double));

    for (int i=0; i<k; i++){
        new_centroids[i][dim] = 0;
    }

    ifp = fopen(input_file, "r");
    if (ifp == NULL) {
        printf("An Error Has Occurred");
        return;
    }

    while (!feof(ifp)) {
        for (int j = 0; j < dim; j++) {
            fscanf(ifp, "%lf,", &vector[j]);
        }

        closet_centroid_index = find_closest_centroid(k, dim, vector, centroids);

        for (int j = 0; j < dim; j++) {
            new_centroids[closet_centroid_index][j] += vector[j];
        }

        new_centroids[closet_centroid_index][dim] ++;
    }
}

int find_closest_centroid(int k, int dim, double *vector, double **centroids) {
    double closest_value = DBL_MAX;
    double current_value = 0;
    int closest_index = -1;

    for (int i = 0; i < k; i++) {
        current_value = 0;
        for (int j=0; j < dim; j++) {
            current_value += pow((vector[j] - (centroids[i][j] / centroids[i][k])), 2);
        }
        if (current_value < closest_value) {
            closest_value = current_value;
            closest_index = i;
        }
    }

    return closest_index;
}


void initialize_centroids(int k, int dim, char *input_file, double **datapoints) {
    FILE *ifp;
    // double **datapoints;
    // datapoints = calloc(k, sizeof(double *));
    // for (int i=0; i < k; i++) {
    //     datapoints[i] = calloc(dim + 1, sizeof(double));
    // }
    ifp = fopen(input_file, "r");
    if (ifp == NULL) {
        printf("An Error Has Occurred");
        return;
    }

    for (int i = 0; i < k; i++) {
        
        for (int j = 0; j < dim; j++) {
            fscanf(ifp, "%lf,", &datapoints[i][j]);
        }
        datapoints[0][dim] = 1;

    }

    fclose(ifp);
    return;
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