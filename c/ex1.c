#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <limits.h>


int initialize_centroids(int k, int dim, char *input_file, double**); //Reads the first k lines in the input file and creates k centroids out of it
int get_dimension(char *input_file); // returns the dimention d of all vectors
double max_distance_between_centroids(int k, int dim, double **old_centroids, double **new_centroids);
int kmeans_iteration(int k, int dim, char *input_file, double **centroids, double **new_centroids);
int find_closest_centroid(int k, int dim, double *vector, double **centroids);
void initarray(int dim, int k, double **arr);
int write_result(int k, int dim, char *outname, double **data);


int main(int argc, char **argv)
{
    char *output_filename, *input_filename;
    int k, maxiter, dim, initialize_error, error;
    double **centroids, **new_centroids, **temp;

    k = atoi(argv[1]);
    input_filename = argv[argc-2];
    output_filename = argv[argc-1];
    maxiter = argc==5 ? atoi(argv[2]) : INT_MAX;
    if (maxiter <= 0 || k <= 0 || argc < 4 || argc > 5) {
        printf("Invalid Input!");
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
        new_centroids[i] = calloc(dim + 1, sizeof(double));
    }
    initialize_error = initialize_centroids(k, dim, input_filename, centroids);
    if (initialize_error == 1){
        printf("An Error Has Occurred");
        return 1;
    } else if (initialize_error == 2){
        printf("Invalid Input!");
        return 1;
    }

    for (int i=0; i < maxiter; i++) {
        error = kmeans_iteration(k, dim, input_filename, centroids, new_centroids);
        if (error){
            printf("An Error Has Occurred");
            return 1;
        }
        double maxd = max_distance_between_centroids(k, dim, centroids, new_centroids);
        if (maxd < 0.001) {
            break;
        }
        
        temp = centroids;
        centroids = new_centroids;
        new_centroids = temp;
        initarray(dim, k, new_centroids);
    }

    write_result(k, dim, output_filename, new_centroids);
    
    for (int i=0; i < k; i++) {
        free(centroids[i]);
        free(new_centroids[i]);
    }
    free(centroids);
    free(new_centroids);
    return 0;
}


double max_distance_between_centroids(int k, int dim, double **old_centroids, double **new_centroids) {
    double max_value = DBL_MIN;
    double current_value;

    for (int i=0; i < k; i++) {
        current_value = 0;
        for (int j=0; j < dim; j++) {
            current_value += pow((old_centroids[i][j] / old_centroids[i][dim]) - (new_centroids[i][j] / new_centroids[i][dim]), 2);
        }
        if (current_value > max_value) {
            max_value = current_value;
        }
    } 

    return sqrt(max_value);
}


int kmeans_iteration(int k, int dim, char *input_file, double **centroids, double **new_centroids) {
    FILE *ifp;
    int closet_centroid_index, end;
    double *vector;
    char c;
    vector = calloc(dim, sizeof(double));

    ifp = fopen(input_file, "r");
    if (ifp == NULL) {
        return 1;
    }

    while (!feof(ifp)) {
        for (int j=0; j < dim; j++) {
            end = fscanf(ifp, "%lf%c", &vector[j], &c);
        }
        if (end != 2){
            break;
        }

        closet_centroid_index = find_closest_centroid(k, dim, vector, centroids);

        for (int j = 0; j < dim; j++) {
            new_centroids[closet_centroid_index][j] += vector[j];
        }

        new_centroids[closet_centroid_index][dim] ++;
    }
    free(vector);
    fclose(ifp);
    return 0;
}

int find_closest_centroid(int k, int dim, double *vector, double **centroids) {
    double closest_value = DBL_MAX;
    double current_value = 0;
    int closest_index = -1;

    for (int i = 0; i < k; i++) {
        current_value = 0;
        for (int j=0; j < dim; j++) {
            current_value += pow((vector[j] - (centroids[i][j] / centroids[i][dim])), 2);
        }
        if (current_value < closest_value) {
            closest_value = current_value;
            closest_index = i;
        }
    }

    return closest_index;
}


int initialize_centroids(int k, int dim, char *input_file, double **datapoints) {
    FILE *ifp;
    char c;
    ifp = fopen(input_file, "r");
    if (ifp == NULL) {
        return 1;
    }

    for (int i = 0; i < k; i++) {
        
        for (int j = 0; j < dim; j++) {
            fscanf(ifp, "%lf%c", &datapoints[i][j],&c);
        }
        datapoints[i][dim] = 1;
    }

    rewind(ifp);
    float check;
    int fsc;
    for (int i = 0; i < INT_MAX; i++) {
        
        for (int j = 0; j < dim; j++) {
            fsc = fscanf(ifp, "%lf%c", &check,&c);
            if (fsc < 2){
                break;
            }
            if (j < dim-1 && c != ','){
                fclose(ifp);
                return 2;
            }
            else if (j == dim-1 && c != '\n'){
                fclose(ifp);
                return 2;
            }
        }
        if (fsc < 2){
            break;
        }
    }

    fclose(ifp);
    return 0;
}

void initarray(int dim, int k, double **arr){
    for (int i=0; i<k; i++){
        for (int j=0; j<dim+1; j++){
            arr[i][j]=0;
        }
    }
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


int write_result(int k, int dim, char *outname, double **data){
    FILE *ofp;
    ofp = fopen(outname, "w");
    if (ofp == NULL) {
        return -1;
    }
    for (int i = 0; i<k; i++){
        for (int j=0; j<dim; j++){
            fprintf(ofp, "%.4f",data[i][j]/data[i][dim]);
            if (j < dim-1){
                fprintf(ofp, ",");
            } else {
                fprintf(ofp, "\n");
            }
        }
    }
    fclose(ofp);
    return 0;
}