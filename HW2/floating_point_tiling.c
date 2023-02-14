#include <x86intrin.h>
#include <xmmintrin.h>
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

//*
void SIMD_mult_float(float *a, float *b, float *c, int mSize, int tileSize)
{
    //remainder variable will add an extra 'chunk' if it's not equal to zero
    clock_t t = clock();
    //loop through the matrix by tile size
    for (int i = 0; i < mSize; i += tileSize) {
        for (int j = 0; j < mSize; j += tileSize) {
            for (int k = 0; k < mSize; k += tileSize) {
                // Loop over the tile of matrix a
                //switched from += 8
                for (int ii = i; ii < i + tileSize && ii < mSize; ii += 8) {
                    for (int jj = j; jj < j + tileSize && jj < mSize; jj += 8) {
                        //switched to += 8
                        for (int kk = k; kk < k + tileSize && kk < mSize; kk ++) {
                            //memory location a or b and row + col offset
                            __m256 va = _mm256_loadu_ps(a + (ii * mSize) + kk);
                            __m256 vb = _mm256_loadu_ps(b + (kk * mSize) + jj);
                            //__m256 vb1 = _mm256_loadu_ps(b + kk * mSize + jj + 8);
                            for (int l = 0; l < 8; l++) {
                                __m256 vc = _mm256_loadu_ps(c + ((ii + l) * mSize) + jj);
                                vc = _mm256_fmadd_ps(va, vb, vc);
                                _mm256_storeu_ps(c + ((ii + l) * mSize) + jj, vc);
                                //vc = _mm256_loadu_ps(c + (ii + l) * mSize + jj + 8);
                                //vc = _mm256_fmadd_ps(va, vb1, vc);
                                //_mm256_storeu_ps(c + (ii + l) * mSize + jj + 8, vc);
                            }
                        }
                    }
                }
            }
        }
    }
    t = clock()-t;
    printf("SIMD time of execution: (%f seconds).\n", ((float)t) / CLOCKS_PER_SEC);
}
//*/
//*
void SIMD_mult_short(const unsigned short *A, const unsigned short *B, unsigned short *C, int n)
{

    //with a matrix of short data types, the register can fit 16 values to operate on in parallel.
    //This implementation divides each row and column length by 16 and performs the necessary
    //operations to multiply the two randomly generated matrices. 

    __m256i a, b, c, d, sum;
    int chunks = n / SHORT_SIZE;
    clock_t t = clock();
    // Matrix multiplication with AVX2 instructions
        for (int i = 0; i < n; i++) {   //'i' represents matrix row
            for (int chunk = 0; chunk < cchunkunks; chunk++) { //'h' represents each size-16 chunk that can fit in 1 row/column of the matrix
                for (int j = 0; j < n; j++) {  //'j' represents matrix column
                    sum = _mm256_setzero_si256();
                    //a relates to the ith row of Matrix A
                    //i*n is the first element in each chunk
                    //chunk * SHORT_SIZE is the offset value
                    //15-0 grabs each proceeding value in the matrix as it is part of the 'chunk'
                    a = _mm256_set_epi16(A[i*n+chunk*SHORT_SIZE+15], A[i*n+chunk*SHORT_SIZE+14],
                                          A[i*n+chunk*SHORT_SIZE+13], A[i*n+chunk*SHORT_SIZE+12],
                                          A[i*n+chunk*SHORT_SIZE+11], A[i*n+chunk*SHORT_SIZE+10],
                                          A[i*n+chunk*SHORT_SIZE+9], A[i*n+chunk*SHORT_SIZE+8],
                                          A[i*n+chunk*SHORT_SIZE+7], A[i*n+chunk*SHORT_SIZE+6],
                                          A[i*n+chunk*SHORT_SIZE+5], A[i*n+chunk*SHORT_SIZE+4],
                                          A[i*n+chunk*SHORT_SIZE+3], A[i*n+chunk*SHORT_SIZE+2],
                                          A[i*n+chunk*SHORT_SIZE+1], A[i*n+chunk*SHORT_SIZE+0]);
                    //b relates to the jth row of matrix B
                    //15*n = the first element in the 15th column of the matrix
                    //n*SHORT_SIZE*chunk offsets the chunk downward by 16 in the matrix
                    //j shifts the right as we increment
                    b = _mm256_set_epi16(B[15*n+(n*SHORT_SIZE*chunk)+j], B[14*n+(n*SHORT_SIZE*chunk)+j],
                                          B[13*n+(n*SHORT_SIZE*chunk)+j], B[12*n+(n*SHORT_SIZE*chunk)+j],
                                          B[11*n+(n*SHORT_SIZE*chunk)+j], B[10*n+(n*SHORT_SIZE*chunk)+j],
                                          B[9*n+(n*SHORT_SIZE*chunk)+j], B[8*n+(n*SHORT_SIZE*chunk)+j],
                                          B[7*n+(n*SHORT_SIZE*chunk)+j], B[6*n+(n*SHORT_SIZE*chunk)+j],
                                          B[5*n+(n*SHORT_SIZE*chunk)+j], B[4*n+(n*SHORT_SIZE*chunk)+j],
                                          B[3*n+(n*SHORT_SIZE*chunk)+j], B[2*n+(n*SHORT_SIZE*chunk)+j],
                                          B[1*n+(n*SHORT_SIZE*chunk)+j], B[0*n+(n*SHORT_SIZE*chunk)+j]);
                    c = _mm256_add_epi32(_mm256_mulhi_epi16(a, b), _mm256_mullo_epi16(a, b)); //multiply the two matrices
                    sum += _mm256_add_epi32(sum, c);
                    
                    //allocate the sum in its corresponding index in the resulting matrix
                    C[i*n+j] += _mm256_extract_epi16(sum, 15)+ _mm256_extract_epi16(sum, 14)+ _mm256_extract_epi16(sum, 13)+ _mm256_extract_epi16(sum, 12)+ _mm256_extract_epi16(sum, 11)+ _mm256_extract_epi16(sum, 10)+ _mm256_extract_epi16(sum, 9)+ _mm256_extract_epi16(sum, 8)+ _mm256_extract_epi16(sum, 7)+ _mm256_extract_epi16(sum, 6)+ _mm256_extract_epi16(sum, 5)+ _mm256_extract_epi16(sum, 4)+ _mm256_extract_epi16(sum, 3)+ _mm256_extract_epi16(sum, 2)+ _mm256_extract_epi16(sum, 1)+ _mm256_extract_epi16(sum, 0);
                }
            }
        }
    t = clock()-t;
    printf("SIMD time of execution: (%f seconds).\n", ((float)t) / CLOCKS_PER_SEC);
}
//*/

void trivial_mult_short(const short *A, const short *B, short *C, int n) {
    clock_t t = clock();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i*n + j] = 0;
            for (int k = 0; k < n; k++) {
                C[i*n + j] += A[i*n + k] * B[k*n + j];
            }
        }
    }
    t = clock()-t;
    printf("SIMD time of execution: (%f seconds).\n", ((float)t) / CLOCKS_PER_SEC);
}
void trivial_mult_float(const float *A, const float *B, float *C, int n) {
    clock_t t = clock();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i*n + j] = 0;
            for (int k = 0; k < n; k++) {
                //C[i*n + j] += A[i*n + k] * B[k*n + j];
                C[i*n + j] += A[i*n + k] * B[k*n + j];
            }
        }
    }
    t = clock()-t;
    printf("Naive time of execution: (%f seconds).\n", ((float)t) / CLOCKS_PER_SEC);
}

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);
    int chunks, remainder;
    if (argv[2][0] == 'f') {

        //2D matrix represented as a 1D array
        float *A = (float *)malloc(n * n * sizeof(float));
        float *B = (float *)malloc(n * n * sizeof(float));
        float *C = (float *)malloc(n * n * sizeof(float));
        float *D = (float *)malloc(n * n * sizeof(float));

        //assigning values to the matrices to be multiplied
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                A[i*n+j] = rand()%10;
                B[i*n+j] = rand()%10;
            }
        }
        //Samples how'd you'd print the contents of the matrix
        /*
        printf("A\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%.2f ", A[i*n+j]);
            }
            printf("\n");
        }
        //*/
        
        SIMD_mult_float(A, B, C, n, 8);
        
        trivial_mult_float(A, B, D, n);

        //Error Checking
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (C[i*n+j] != D[i*n+j]) {
                    printf("An error occurred: the two matrices do not match");
                    exit(0);
                }
            }
        }

    }
    else if (argv[2][0] == 's') {
        //2D matrix represented as a 1D array
        unsigned short *A = (unsigned short *)malloc(n * n * sizeof(short));
        unsigned short *B = (unsigned short *)malloc(n * n * sizeof(short));
        unsigned short *C = (unsigned short *)malloc(n * n * sizeof(short));
        unsigned short *D = (unsigned short *)malloc(n * n * sizeof(short));

        //assigning random values to matrices
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                A[i*n+j] = rand();
                B[i*n+j] = rand();
            }
        }
        
        SIMD_mult_short(A, B, C, n);
        trivial_mult_short(A, B, D, n);
        
        //Error Checking
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (C[i*n+j] != D[i*n+j]) {
                    printf("An error occurred: the two matrices do not match");
                    exit(0);
                }
            }
        }
    }
    return 0;
}