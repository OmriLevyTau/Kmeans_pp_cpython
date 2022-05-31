
#define PY_SSIZE_T_CLEAN
#include <Python.h>

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <math.h>
# include <setjmp.h>
# include <limits.h>


int countLines(char* filePath);
int countCols(char* filePath);
double** buildMatrix(int rows, int cols);
double** createMatrix(int rows, int cols, char* filePath);
double* sub_vectors(const double *A, const double *B, int n);
double* add_vectors(const double *A, const double *B, int n);
double squared_dot_product(const double *A, const double *B, int n);
double** K_means(int K, int max_iter, double epsilon, char* tmp_combined_inputs, char* tmp_initial_centroids);
FILE* write_output(char* output_filename, int rows, int cols,double** centroids);
void print_vector(double* pointer, int cols);
void printMatrix(double** mat, int rows, int cols);
void print_error_and_exit();

void print_error_and_exit(){
    printf("An Error Has Occurred");
    exit(1);
}

int countLines(char* filePath){
    /*
     * input: file Name
     * output: number of lines in file
     */
    char c;
    int counter=0;
    FILE *fp =  fopen(filePath,"r");

    if (fp==NULL){
        print_error_and_exit();
    }
    for (c= getc(fp); c!=EOF; c= getc(fp)){
        if (c=='\n'){
            counter+=1;
        }
    }
    if(fclose(fp)!=0){
        print_error_and_exit();
    }
    return counter;
}

int countCols(char* filePath){
    /*
     * input: file Name
     * output: number of columns
     * details: open files, reads first line of file (loops until first '\n').
     *          counts number of ",", return counter+1 if not 0, otherwise 0.
     */

    char c;
    int counter=0;
    FILE *fp =  fopen(filePath,"r");

    if (fp==NULL){
        print_error_and_exit();
    }

    for (c= getc(fp); c!='\n'; c= getc(fp)){
        if (c==','){
            counter+=1;
        }
    }

    if(fclose(fp)!=0){
        print_error_and_exit();
    }

    if (counter==0){
        return 1;
    } else{
        return ++counter;
    }
}

void printMatrix(double** mat, int rows, int cols){
    int i,j;
    for (i=0; i<rows;i++){
        for (j=0;j<cols;j++){
            printf("  %.4f",mat[i][j]);
        }
        printf("\n");
    }
}

void print_vector(double* pointer, int cols){
    int i;
    for (i=0; i<cols;i++){
        printf("  %.4f",pointer[i]);
    }
}

double** buildMatrix(int rows, int cols){
    /*
     * Creates empty matrix of size rows x cols
     */
    int i;
    double **a = calloc(rows, sizeof(int*));
    if (a==NULL){
        print_error_and_exit();
    }
    for (i=0;i<rows;i++){
        a[i] = calloc(cols, sizeof(double));
        if (a[i]==NULL){
            print_error_and_exit();
        }
    }
    return a;
}

double** createMatrix(int rows, int cols, char* filePath){
    /*
     * Creates empty matrix and fills it with read values from file
     */
    double** matrix;
    int lineSize = cols*18; /* 17 + 1 */
    char *token; /* String pointer*/
    int i=0,j=0;
    char* line;
    FILE *fp;

    line = calloc(lineSize, sizeof(char ));
    if(line == NULL){
        print_error_and_exit();
    }

    matrix = buildMatrix(rows,cols);

    fp = fopen(filePath,"r");

    if (fp==NULL){
        print_error_and_exit();
    }

    /* Reads each line as a string*/
    while (fgets(line,lineSize,fp)!=NULL){
        token = strtok(line,","); /* token is a string between 2 commas*/
        while (token!=NULL){               /* in end of line token is NULL*/
            matrix[i][j] = atof(token); /* converts the string token to double */
            token = strtok(NULL,","); /* move forward to the next comma. Pointer=NULL so it will continue from the last place */
            j++;
        }
        /* finished line*/
        i++;
        j=0;
    }
    if(fclose(fp)!=0){
        print_error_and_exit();
    }
    free(line);

    return matrix;
}

double* sub_vectors(const double *A, const double *B, int n){
    int i;
    double* res;

    res = (double*)malloc(n*sizeof(double));

    if (res==NULL){
        print_error_and_exit();
    }
    for(i=0; i<n; i++){
        res[i] = A[i] - B[i];
    }
    return res;
}

double* add_vectors(const double *A, const double *B, int n){
    int i;
    double* res;

    res = (double*)malloc(n*sizeof(double));

    if (res==NULL){
        print_error_and_exit();
    }
    for(i=0; i<n; i++){
        res[i] = A[i] + B[i];
    }
    return res;
}

double squared_dot_product(const double *A, const double *B, int n){
    int i;
    double res;
    res = 0;
    for(i=0; i<n; i++){
        res = res + (A[i] * B[i]);
    }
    return res;
}

double** copy(double** data, int K, int cols){
    int i,j;
    double** new_mat;
    new_mat = buildMatrix(K, cols);
    for(i=0; i<K; i++){
        for(j=0; j<cols; j++){
            new_mat[i][j] = data[i][j];
        }
    }
    return new_mat;
}

int free_helper(double** pointer, int rows){
    int i;
    for (i = 0; i<rows; i++){
        if (pointer[i]!=NULL){
            free(pointer[i]);
        } else {
            return 1;
        }
    }
    free(pointer);
    return 0;
}


FILE* write_output(char* output_filename, int rows, int cols,double** centroids){
    int r,c;
    char tmp_str[100];
    FILE* fp;
    fp = fopen(output_filename, "w");

    if (fp==NULL){
        print_error_and_exit();
    }
    for (r=0;r<rows;r++){
        c = 0;
        for (;c<cols-1;c++){
            sprintf(tmp_str,"%.4f",centroids[r][c]) ; /* saves centroids[r][c] in tmp_str */
            fputs(tmp_str,fp);
            fputs(",",fp);
        }
        sprintf(tmp_str,"%.4f",centroids[r][c]) ;
        fputs(tmp_str,fp);
        fputs("\n", fp);
    }
    if(fclose(fp)!=0){
        print_error_and_exit();
    }
    return fp;
}

double** K_means(int K, int max_iter, double epsilon, char* tmp_combined_inputs, char* tmp_initial_centroids){
    /*
     * recieves input file, K = number of clusters, max_iter = max number of iterations
     * connects every point to the closest cluster
     * returns vector of centroids
     */
    double ** data;
    double** centroids;
    int idx, arg_min, counter,iter,point,cluster_ind, r, k, c, rows, cols, rows_pp,cols_pp, f2, f3, f4;
    double min_dist, dist_point_cluster;
    double** cluster_sum;
    double** old_centroids;
    double* cluster;
    double* tmp_arr;
    int* points_clusters;
    double* cluster_change;
    int* cluster_counter;
    double* tmp_vec;

    /* read data points */
    rows = countLines(tmp_combined_inputs);
    cols = countCols(tmp_combined_inputs);

    points_clusters = calloc(rows, sizeof(int));
    if(points_clusters==NULL){
        print_error_and_exit();
    }
    data = createMatrix(rows,cols,tmp_combined_inputs);

    /* Read initial centroids from kmeans_pp */
    rows_pp = countLines(tmp_initial_centroids);
    cols_pp = countCols(tmp_initial_centroids);
    centroids = calloc(rows_pp, sizeof(double));
    if(centroids==NULL){
        print_error_and_exit();
    }
    centroids = createMatrix(rows_pp,cols_pp,tmp_initial_centroids);

    /* Train Model */

    for(iter=0; iter<max_iter; iter++){
        /* iterate through points and assign to the closest cluster */
        for (point=0; point<rows; point++){
            min_dist = INT_MAX;
            arg_min = -1;
            for(cluster_ind=0; cluster_ind<K; cluster_ind++){
                cluster = centroids[cluster_ind];
                tmp_arr = sub_vectors(cluster,data[point], cols);
                dist_point_cluster = squared_dot_product(tmp_arr,tmp_arr,cols);
                if(dist_point_cluster<min_dist){
                    min_dist = dist_point_cluster;
                    arg_min = cluster_ind;
                }
            }
            points_clusters[point] = arg_min;
        }
        /* calculate new centroids */
        old_centroids = copy(centroids,K, cols); /* for changes checking */
        cluster_sum = buildMatrix(K, cols); /* zero matrix */
        cluster_change = calloc(K, sizeof(double));
        if(cluster_change==NULL){
            print_error_and_exit();
        }
        cluster_counter = calloc(K, sizeof(int));
        if(cluster_counter==NULL){
            print_error_and_exit();
        }

        memset(cluster_counter, 0, K*sizeof(int)); /* zero array */
        memset(cluster_change, 0, K*sizeof(double));

        /* sum and count */
        for(r=0; r<rows; r++){
            idx = points_clusters[r];
            cluster_counter[idx] += 1;
            cluster_sum[idx] = add_vectors(cluster_sum[idx], data[r], cols);
        }

        /* update centroids */
        counter = 0;
        for(k=0; k<K; k++){
            for(c=0; c<cols; c++){
                if (cluster_counter[k]==0){
                    print_error_and_exit();
                }
                centroids[k][c] = cluster_sum[k][c] / cluster_counter[k];
            }
            /* check change vector */
            tmp_vec = sub_vectors(centroids[k],old_centroids[k], cols);
            cluster_change[k] = sqrt(squared_dot_product(tmp_vec, tmp_vec, cols));
            if(cluster_change[k]<epsilon){
                counter += 1;
            }
            free(tmp_vec);
        }
        /* check if all coordinates changes are less than epsilon*/
        if(counter == K){
            break;
        }
    }

    /*
    write_output(output_filename, K, cols, centroids);
    */

    /* free matrices */


    f2 = free_helper(data,rows);
    if (max_iter>0){
        f3 = free_helper(old_centroids, K);
        f4 = free_helper(cluster_sum, K);
        /* free arrays */
        free(points_clusters);
        free(cluster_change);
        free(cluster_counter);

    } else {
        f3 = 0;
        f4 = 0;
    }
    if ((f2+f3+f4)>0){
        print_error_and_exit();
    }
    return centroids;
}


static PyObject* fit_capi(PyObject *self, PyObject *args){
    int k;
    int max_iter;
    double epsilon;
    char* tmp_combined_inputs;
    char* tmp_initial_centroids;

    if (!PyArg_ParseTuple(args,"iidss",&k, &max_iter, &epsilon, &tmp_combined_inputs, &tmp_initial_centroids)){
        return NULL;
    }
    return Py_BuildValue("O", K_means(k, max_iter, epsilon, tmp_combined_inputs, tmp_initial_centroids));
}

static PyMethodDef capiMethods[] = {
        {"fit",
         (PyCFunction) fit_capi,
         METH_VARARGS,
         PyDoc_STR("Trains kmeans model")},
         {NULL,NULL,0,NULL}
};

static struct PyModuleDef moduledef= {
        PyModuleDef_HEAD_INIT,
        "mykmeanssp",
        NULL,
        -1,
        capiMethods
};

PyMODINIT_FUNC PyInit_mykmeanssp(void)
{
    PyObject *m;
    m = PyModule_Create(&moduledef);
    if (!m){
        return NULL;
    }
    return m;
}


double** fit(int K, int max_iter, double epsilon, char* tmp_combined_inputs, char* tmp_initial_centroids) {
    double** result = K_means(K, max_iter,epsilon,tmp_combined_inputs,tmp_initial_centroids);
    return result;

}

/*
int main(int argc, char * argv[]) {

    double** c = buildMatrix(1,2) ;
    write_output("fit.txt",1,2,c);

    double** result;
    char* combined =  "C:\\Users\\Omri\\Desktop\\CS_Omri\\Second_Year\\SW_Project\\EX_2\\Kmeans_pp_cpython\\tmp_combined_inputs.txt";
    char* initial = "C:\\Users\\Omri\\Desktop\\CS_Omri\\Second_Year\\SW_Project\\EX_2\\Kmeans_pp_cpython\\tmp_initial_centroids.txt";

    result = K_means(3, 100,0.01,combined,initial);
    printMatrix(result,3,4);
    return 0;
}
*/
