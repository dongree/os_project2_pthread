/*
 * Copyright 2021. Heekuck Oh, all rights reserved
 * 이 프로그램은 한양대학교 ERICA 소프트웨어학부 재학생을 위한 교육용으로 제작되었습니다.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

/*
 * 기본 스도쿠 퍼즐
 */
int sudoku[9][9] = {{6,3,9,8,4,1,2,7,5},{7,2,4,9,5,3,1,6,8},{1,8,5,7,2,6,3,9,4},{2,5,6,1,3,7,4,8,9},
        {4,9,1,5,8,2,6,3,7},{8,7,3,4,6,9,5,2,1},{5,4,2,3,9,8,7,1,6},{3,1,8,6,7,5,9,4,2},{9,6,7,2,1,4,8,5,3}};

/*
 * valid[0][0], valid[0][1], ..., valid[0][8]: 각 행이 올바르면 1, 아니면 0
 * valid[1][0], valid[1][1], ..., valid[1][8]: 각 열이 올바르면 1, 아니면 0
 * valid[2][0], valid[2][1], ..., valid[2][8]: 각 3x3 그리드가 올바르면 1, 아니면 0
 */
int valid[3][9];

// 스도쿠의 각 행, 열, 서브그리드에 들어가야 하는 숫자
int numArr[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

//qsort함수를 사용하기 위해서 만든 비교함수
int compare(const void *a, const void *b)
{
        int num1 = *(int *)a;
        int num2 = *(int *)b;

        if (num1 < num2)
                return -1;
        if (num1 > num2)
                return 1;
        return 0;
}

/*
 * 스도쿠 퍼즐의 각 행이 올바른지 검사한다.
 * 행 번호는 0부터 시작하며, i번 행이 올바르면 valid[0][i]에 1을 기록한다.
 */
void *check_rows(void *arg)
{
        int flag = 1;
        int num[9];
        for(int i=0; i<9; i++){
                for(int j=0; j<9; j++){
                        num[j] = sudoku[i][j];
                }
                qsort(num, 9, sizeof(int), compare);
                for(int a=0; a<9; a++){
                        if(num[a] != numArr[a]){
                                flag = 0;
                                break;
                        }
                }
                if(flag == 1){
                        valid[0][i] = 1;
                }
                flag = 1;
        }
}

/*
 * 스도쿠 퍼즐의 각 열이 올바른지 검사한다.
 * 열 번호는 0부터 시작하며, j번 열이 올바르면 valid[1][j]에 1을 기록한다.
 */
void *check_columns(void *arg)
{
        int flag = 1;
        int num[9];
        for(int j=0; j<9; j++){
                for(int i=0; i<9; i++){
                        num[i] = sudoku[i][j];
                }
                qsort(num, 9, sizeof(int), compare);
                for(int a=0; a<9; a++){
                        if(num[a] != numArr[a]){
                                flag = 0;
                                break;
                        }
                }
                if(flag == 1){
                        valid[1][j] = 1;
                }
                flag = 1;
        }
}

/*
 * 스도쿠 퍼즐의 각 3x3 서브그리드가 올바른지 검사한다.
 * 3x3 서브그리드 번호는 0부터 시작하며, 왼쪽에서 오른쪽으로, 위에서 아래로 증가한다.
 * k번 서브그리드가 올바르면 valid[2][k]에 1을 기록한다.
 */
void *check_subgrid(void *arg)
{
        long k = *((long *)arg);
        int flag = 1;
        int num[9];
        int index = 0;
        for(int i=(k/3)*3; i<(k/3+1)*3; i++){
                for(int j=(k%3)*3; j<(k%3+1)*3; j++){
                        num[index] = sudoku[i][j];
                        index++;
                }
        }
        qsort(num, 9, sizeof(int), compare);
        for(int a=0; a<9; a++){
                if(num[a] != numArr[a]){
                        flag = 0;
                        break;
                }
        }
        if(flag == 1){
                valid[2][k] = 1;
        }
}

/*
 * 스도쿠 퍼즐이 올바르게 구성되어 있는지 11개의 스레드를 생성하여 검증한다.
 * 한 스레드는 각 행이 올바른지 검사하고, 다른 한 스레드는 각 열이 올바른지 검사한다.
 * 9개의 3x3 서브그리드에 대한 검증은 9개의 스레드를 생성하여 동시에 검사한다.
 */
void check_sudoku(void)
{
        int i, j;

    /*
     * 검증하기 전에 먼저 스도쿠 퍼즐의 값을 출력한다.
     */
        for (i = 0; i < 9; ++i) {
                for (j = 0; j < 9; ++j)
                        printf("%2d", sudoku[i][j]);
                        printf("\n");
        }
        printf("---\n");
    /*
     * 스레드를 생성하여 각 행을 검사하는 check_rows() 함수를 실행한다.
     */
        pthread_t thread[11]; // thread식별자 11개 선언

        // valid[0][i] 초기화 
        for(int i=0; i<9; i++){
                valid[0][i] = 0;
        }

        pthread_create(&thread[0], NULL, check_rows, NULL);
    /*
     * 스레드를 생성하여 각 열을 검사하는 check_columns() 함수를 실행한다.
     */
        // valid[1][i] 초기화
        for(int i=0; i<9; i++){
                valid[1][i] = 0;
        }

        pthread_create(&thread[1], NULL, check_columns, NULL);
    /*
     * 9개의 스레드를 생성하여 각 3x3 서브그리드를 검사하는 check_subgrid() 함수를 실행한다.
     * 3x3 서브그리드의 위치를 식별할 수 있는 값을 함수의 인자로 넘긴다.
     */

        // valid[2][i] 초기화 
        for(int i=0; i<9; i++){
                valid[2][i] = 0;
        }

        long num[9] = {0,1,2,3,4,5,6,7,8};
        for(int i=0; i<9; i++){
                pthread_create(&thread[i+2], NULL, check_subgrid, (void*)&num[i]);
        }

    /*
     * 11개의 스레드가 종료할 때까지 기다린다.
     */

        for(int i=0; i<11; i++){
                pthread_join(thread[i], NULL);
        }

    /*
     * 각 행에 대한 검증 결과를 출력한다.
     */
        printf("ROWS: ");
        for (i = 0; i < 9; ++i)
                printf(valid[0][i] == 1 ? "(%d,YES)" : "(%d,NO)", i);
        printf("\n");
    /*
     * 각 열에 대한 검증 결과를 출력한다.
     */
        printf("COLS: ");
        for (i = 0; i < 9; ++i)
                printf(valid[1][i] == 1 ? "(%d,YES)" : "(%d,NO)", i);
        printf("\n");
    /*
     * 각 3x3 서브그리드에 대한 검증 결과를 출력한다.
     */
        printf("GRID: ");
        for (i = 0; i < 9; ++i)
                printf(valid[2][i] == 1 ? "(%d,YES)" : "(%d,NO)", i);
        printf("\n---\n");
}

/*
 * 스도쿠 퍼즐의 값을 3x3 서브그리드 내에서 무작위로 섞는 함수이다.
 */
void *shuffle_sudoku(void *arg)
{
        int i, tmp;
        int grid;
        int row1, row2;
        int col1, col2;

        srand(time(NULL));
        for (i = 0; i < 100; ++i) {
        /*
         * 0부터 8번 사이의 서브그리드 하나를 무작위로 선택한다.
         */
                grid = rand() % 9;
        /*
         * 해당 서브그리드의 좌측 상단 행열 좌표를 계산한다.
         */
                row1 = row2 = (grid/3)*3;
                col1 = col2 = (grid%3)*3;
        /*
         * 해당 서브그리드 내에 있는 임의의 두 위치를 무작위로 선택한다.
         */
                row1 += rand() % 3; col1 += rand() % 3;
                row2 += rand() % 3; col2 += rand() % 3;
        /*
         * 홀수 서브그리드이면 두 위치에 무작위 수로 채우고,
         */
                if (grid & 1) {
                        sudoku[row1][col1] = rand() % 8 + 1;
                        sudoku[row2][col2] = rand() % 8 + 1;
                }
        /*
         * 짝수 서브그리드이면 두 위치에 있는 값을 맞바꾼다.
         */
                else {
                        tmp = sudoku[row1][col1];
                        sudoku[row1][col1] = sudoku[row2][col2];
                        sudoku[row2][col2] = tmp;
                }
        }
        pthread_exit(NULL);
}

/*
 * 메인 함수는 위에서 작성한 함수가 올바르게 동작하는지 검사하기 위한 것으로 수정하면 안 된다.
 */
int main(void)
{
        int tmp;
        pthread_t tid;

    /*
     * 기본 스도쿠 퍼즐을 출력하고 검증한다.
     */
        check_sudoku();
    /*
     * 기본 퍼즐에서 값 두개를 맞바꾸고 검증해본다.
     */
        tmp = sudoku[5][3]; sudoku[5][3] = sudoku[6][2]; sudoku[6][2] = tmp;
        check_sudoku();
    /*
     * 기본 스도쿠 퍼즐로 다시 바꾼 다음, shuffle_sudoku 스레드를 생성하여 퍼즐을 섞는다.
     */
        tmp = sudoku[5][3]; sudoku[5][3] = sudoku[6][2]; sudoku[6][2] = tmp;
        if (pthread_create(&tid, NULL, shuffle_sudoku, NULL) != 0) {
                fprintf(stderr, "pthread_create error: shuffle_sudoku\n");
                exit(-1);
        }
    /*
     * 무작위로 섞는 중인 스도쿠 퍼즐을 검증해본다.
     */
        check_sudoku();
    /*
     * shuffle_sudoku 스레드가 종료될 때까지 기다란다.
     */
        pthread_join(tid, NULL);
    /*
     * shuffle_sudoku 스레드 종료 후 다시 한 번 스도쿠 퍼즐을 검증해본다.
     */
        check_sudoku();
        exit(0);
}
