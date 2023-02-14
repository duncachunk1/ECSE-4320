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
void SIMD_mult_short(const short *a, const short *b, short *c, int mSize, int tileSize)
{
    clock_t t = clock();

    for (int i = 0; i < mSize; i += tileSize) {
        for (int j = 0; j < mSize; j += tileSize) {
            for (int k = 0; k < mSize; k += tileSize) {
                for (int ii = i; ii < i + tileSize && ii < mSize; ii += 8) {
                    for (int jj = j; jj < j + tileSize && jj < mSize; jj += 8) {
                        for (int kk = k; kk < k + tileSize && kk < mSize; kk++) {
                            __m256i va = _mm256_loadu_si256((const __m256i *)&a[ii * mSize + kk]);
                            __m256i vb0 = _mm256_loadu_si256((const __m256i *)&b[kk * mSize + jj]);
                            //__m256i vb1 = _mm256_loadu_si256((const __m256i *)&b[kk * mSize + jj + 8]);
                            for (int l = 0; l < 8; l++) {
                                __m256i vc = _mm256_loadu_si256((const __m256i *)&c[(ii + l) * mSize + jj]);
                                vc = _mm256_add_epi16(vc, _mm256_mulhrs_epi16(va, vb0));
                                _mm256_storeu_si256((__m256i *)&c[(ii + l) * mSize + jj], vc);
                                //vc = _mm256_loadu_si256((const __m256i *)&c[(ii + l) * mSize + jj + 8]);
                                // vc = _mm256_add_epi16(vc, _mm256_mulhrs_epi16(va, vb1));
                                //_mm256_storeu_si256((__m256i *)&c[(ii + l) * mSize + jj + 8], vc);
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
        //implementing th float should be the same as implementing the short
        //just reevaluating the size of register contents

        //2D array represented as a 1D array
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
        //prints the contents of the arrays, easy since I'm only inputting arrays of size 16/32 at the moment
        //__________________________________________________
        printf("A\n");
        ///*
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%.2f ", A[i*n+j]);
            }
            printf("\n");
        }
        //*/
        printf("\nB\n");
        ///*
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%.2f ", B[i*n+j]);
            }
            printf("\n");
        }
        //*/
        //__________________________________________________


        SIMD_mult_float(A, B, C, n, 8);
        ///*
        printf("\nC\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%.2f ", C[i*n+j]);
            }
            printf("\n");
        }
        //*/

        //__________________________________________________

        trivial_mult_float(A, B, D, n);
        ///*
        printf("\nD\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%.2f ", D[i*n+j]);
            }
            printf("\n");
        }
        //*/

        /*
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%.2f ", A[i*n+j]);
            }
            printf("\n");
        }
        */

    }
    else if (argv[2][0] == 's') {
        //2D array represented as a 1D array
        short *A = (short *)malloc(n * n * sizeof(short));
        short *B = (short *)malloc(n * n * sizeof(short));
        short *C = (short *)malloc(n * n * sizeof(short));
        short *D = (short *)malloc(n * n * sizeof(short));

        //assigning values to the matrices to be multiplied
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                A[i*n+j] = 1; //i*n+j;//rand();
                B[i*n+j] = 2; //i*n+j;//rand();
            }
        }
        //prints the contents of the arrays, easy since I'm only inputting arrays of size 16/32 at the moment
        //__________________________________________________
        printf("A\n");
        /*
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%d ", A[i*n+j]);
            }
            printf("\n");
        }
        //*/
        printf("\nB\n");
        /*
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%d ", B[i*n+j]);
            }
            printf("\n");
        }
        //*/
        //__________________________________________________


        SIMD_mult_short(A, B, C, n,8);
        //*
        printf("\nC\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%d ", C[i*n+j]);
            }
            printf("\n");
        }
        //*/

        //__________________________________________________

        trivial_mult_short(A, B, D, n);
        //*
        printf("\nD\n");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%d ", D[i*n+j]);
            }
            printf("\n");
        }
        //*/
    }
    return 0;
}