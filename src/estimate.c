#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

void free_MatrixArray_2D(double **matrix, int r);

double **multiply(double **firstMatrix, double **secondMatrix, int row1, int col1, int row2, int col2) {
    
    double **answer = malloc(row1 * sizeof(double *));

    for (int k = 0; k < row1; k++) {
        answer[k] = malloc(col2 * sizeof(double));
    }

    for (int r = 0; r < row1; r++) {
        for (int c = 0; c < col2; c++) {
            answer[r][c] = 0;
        }
    }

    for (int final_row = 0; final_row < row1; final_row++) {
        for (int final_col = 0; final_col < col2; final_col++) {
            double product = 0;

            for (int c = 0; c < col1; c++) {
                product += firstMatrix[final_row][c] * secondMatrix[c][final_col];
            }
            answer[final_row][final_col] = product;
        }
    }

    return answer;
}


double *multiply_1D_2D(double **firstMatrix, double *secondMatrix, int row1, int col1, int row2) {
    
    double *answer = malloc(row1 * sizeof(double));

    for (int r = 0; r < row1; r++) {
        for (int c = 0; c < 1; c++) {
            answer[r] = 0;
        }
    }

    for (int final_row = 0; final_row < row1; final_row++) {
        double product = 0;

        for (int c = 0; c < col1; c++) {
            product += firstMatrix[final_row][c] * secondMatrix[c];
        }
        answer[final_row] = product;
    }

    return answer;
}



double **matrix_inverse(double **firstMatrix, int size) {
    double **N = malloc(size * sizeof(double *));

    for (int k = 0; k < size; k++) {
        N[k] = malloc(size * sizeof(double *));
    }

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (row == col) {
                N[row][col] = 1;
            }
            else {
                N[row][col] = 0;
            }
        }
    }

    double **new_Matrix = malloc(size * sizeof(double *));

    for (int g = 0; g < size; g++) {
        new_Matrix[g] = malloc(size * sizeof(double));
    }

    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            double temp = firstMatrix[row][col];
            new_Matrix[row][col] = temp;
        }
    }

    for (int p = 0; p < size; p++) {
        double f = new_Matrix[p][p];

        for (int k = 0; k < size; k++) {
            double temp_newMatrix = new_Matrix[p][k] / f;
            new_Matrix[p][k] = temp_newMatrix;
            double temp_other = N[p][k] / f;
            N[p][k] = temp_other;
        }

        for (int i = p + 1; i < size; i++) {
            f = new_Matrix[i][p];

            for (int k = 0; k < size; k++) {
                double temp_newMatrix = new_Matrix[i][k] - (f * new_Matrix[p][k]);
                double temp_other = N[i][k] - (f * N[p][k]);
                new_Matrix[i][k] = temp_newMatrix;
                N[i][k] = temp_other;
            }
        }
    }

    for (int p = size - 1; p >= 0; p--) {
        for (int i = p - 1; i >= 0; i--) {
            double f = new_Matrix[i][p];

            for (int k = 0; k < size; k++) {
                double temp_newMatrix = new_Matrix[i][k] - (f * new_Matrix[p][k]);
                double temp_other = N[i][k] - (f * N[p][k]);
                new_Matrix[i][k] = temp_newMatrix;
                N[i][k] = temp_other;
            }
        }
    }

    free_MatrixArray_2D(new_Matrix, size);
    return N;
}

double **transpose(double **matrix, int row, int col) {
    double **transposed_matrix = malloc(col * sizeof(double *));

    for (int k = 0; k < col; k++) {
        transposed_matrix[k] = malloc(row * sizeof(double));
    }

    for (int r = 0; r < col; r++) {
        for (int c = 0; c < row; c++) {
            double t = matrix[c][r];
            transposed_matrix[r][c] = t;
        }
    }

    return transposed_matrix;
}

void free_MatrixArray_2D(double **matrix, int r) {

    for (int j = 0; j < r; ++j) {
        free(matrix[j]);
        matrix[j] = NULL;
    }

    free(matrix);
    matrix = NULL;
}

void Matrix_2D_print(double **matrix, int r, int c) {
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            printf("%lf	", matrix[i][j]);
        }

        printf("\n");
    }
}

void Matrix_1D_print(double *matrix, int span) {
    
    for (int i = 0; i < span; i++) {
        printf("%lf\n", matrix[i]);
    }
}


int main(int argc, char *argv[]) {
    
    if (argc < 3) {
        printf("error\n");
        exit(0);
    }

    int trait = 0;
    int data = 0;

    char string1[6];
    FILE *fp = fopen(argv[1], "r");         // scans training data file
    fscanf(fp, "%s\n", string1);
    fscanf(fp, "%d\n", &trait);
    fscanf(fp, "%d\n", &data);

    if (trait == 0 && data == 0) {
        printf("The data is not provided\n");
        exit(0);
    }

    double **X = malloc(data * sizeof(double *));
    double *Y = malloc(data * sizeof(double));

    for (int p = 0; p < data; p++) {
        X[p] = malloc((trait + 1) * sizeof(double));
    }


    int t = 0;
    while (t < data) {

        int y = 0;
        X[t][y] = 1;
        y = 1;

        while (y <= trait) {
            fscanf(fp, "%lf,", &X[t][y]);
            y++;
        }

        fscanf(fp, "%lf\n", &Y[t]);
        t++;
    }

    fclose(fp);
    fp = NULL;

    
    double **matrix_X_transpose = transpose(X, data, (trait + 1));
    
    double **X_into_X_transpose = multiply(matrix_X_transpose, X, (trait + 1), data, data, (trait + 1));

    double **inverse = matrix_inverse(X_into_X_transpose, (trait + 1));
   
    double **initial_multiply = multiply(inverse, matrix_X_transpose, (trait + 1), (trait + 1), (trait + 1), data);
    
    double *W = multiply_1D_2D(initial_multiply, Y, (trait + 1), data, data);
    
    fp = fopen(argv[2], "r");     //scans test file
    int test_data = 0;
    char string2[5];
    int temp;
    fscanf(fp, "%s\n", string2);
    fscanf(fp, "%d\n", &temp);
    fscanf(fp, "%d\n", &test_data);

    if (test_data == 0) {
        printf("The test data is not provided\n");
        exit(0);
    }

    for (int i = 0; i < test_data; i++) {
        double price = W[0];
        double attr = 0;
        int k = 1;

        while (k < trait) {
            fscanf(fp, "%lf,", &attr);
            price += (W[k] * attr);
            k++;
        }

        fscanf(fp, "%lf\n", &attr);
        price += (W[k] * attr);
        printf("%0.0lf\n", price);
    }

    fclose(fp);
    fp = NULL;

    free(Y);
    Y = NULL;

    free_MatrixArray_2D(X, data);
    X = NULL;

    free_MatrixArray_2D(matrix_X_transpose, (trait + 1));
    matrix_X_transpose = NULL;

    free_MatrixArray_2D(X_into_X_transpose, (trait + 1));
    X_into_X_transpose = NULL;

    free_MatrixArray_2D(inverse, (trait + 1));
    inverse = NULL;

    free_MatrixArray_2D(initial_multiply, (trait + 1));
    initial_multiply = NULL;

    free(W);
    W = NULL;

    return 0;
}



