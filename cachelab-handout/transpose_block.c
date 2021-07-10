#include <stdio.h>

void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{
    int bi, bj, i, j;//bi,bj子矩阵的行列序数,bn,bm子矩阵的行列数
    int t0, t1, t2, t3, t4, t5, t6, t7;
    for (bi = 0; bi < N; bi += 8) {
        for (bj = 0; bj < M; bj += 8) {
            for (i = bi ; i < bi + 4 && i < N; i++) {
                t0 = A[i][bj + 0];
                t1 = A[i][bj + 1];
                t2 = A[i][bj + 2];
                t3 = A[i][bj + 3];
                t4 = A[i][bj + 4];
                t5 = A[i][bj + 5];
                t6 = A[i][bj + 6];
                t7 = A[i][bj + 7];
                
                B[bj + 0][i] = t0;
                B[bj + 1][i] = t1;
                B[bj + 2][i] = t2;
                B[bj + 3][i] = t3;

                B[bj + 0][i + 4] = t4;
                B[bj + 1][i + 4] = t5;
                B[bj + 2][i + 4] = t6;
                B[bj + 3][i + 4] = t7;
            }

            for (j = bj; j < bj + 4 && j < M; j++) {
                t0 = A[i + 0][j];
                t1 = A[i + 1][j];
                t2 = A[i + 2][j];
                t3 = A[i + 3][j];
                
                t4 = B[j][i + 0];
                t5 = B[j][i + 1];
                t6 = B[j][i + 2];
                t7 = B[j][i + 3];
            
                B[j][i + 0] = t0;
                B[j][i + 1] = t1;
                B[j][i + 2] = t2;
                B[j][i + 3] = t3;

                B[j + 4][i + 0 - 4] = t4;
                B[j + 4][i + 1 - 4] = t5;
                B[j + 4][i + 2 - 4] = t6;
                B[j + 4][i + 3 - 4] = t7;
                
            }

            for (; i < bi + 8 && i < N; i++) {
                t0 = A[i][j + 0];
                t1 = A[i][j + 1];
                t2 = A[i][j + 2];
                t3 = A[i][j + 3];

                B[j + 0][i] = t0; 
                B[j + 1][i] = t1;
                B[j + 2][i] = t2;
                B[j + 3][i] = t3;
            }
        }
    }
}

int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

int main() {
    int M = 64;
    int N = 64;
    int A[N][M];
    int B[M][N];

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            A[i][j] = i * N + j;
            B[j][i] = 0;
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 16; j++) {
            printf("%4d ", A[i][j]);
        }
        printf("\n");
    }

    transpose_submit(M, N, A, B);
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 8; j++) {
            printf("%4d ", B[i][j]);
        }
        printf("\n");
    }

    int transposed = is_transpose(M, N, A, B);
    printf("is transposed = %d\n", transposed);
}