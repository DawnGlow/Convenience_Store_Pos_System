#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <fcntl.h>
#include <stdlib.h>
#include "header.h"
#include <setjmp.h>
#define BUFFSIZE 8192

jmp_buf env;

user login( FILE *fp) {

    char cmd_buf[BUFFSIZE];
    int** userDB;
    int numRows = 20;
    int numCols = 6;

    userDB = (int**)malloc(numRows * sizeof(int*));
    for (int i = 0; i < numRows; i++) {
        userDB[i] = (int*)malloc(numCols * sizeof(int));
    }


    for (int i = 0; i < numRows; i++) {
        fscanf(fp, "%d %d %d %d %d %d", &userDB[i][0], &userDB[i][1], &userDB[i][2], &userDB[i][3], &userDB[i][4], &userDB[i][5]);
    }

     if (setjmp(env) != 0) {
        write(STDOUT_FILENO, "아이디나 비밀번호가 틀렸습니다. 다시 시도해주세요.\n", 76);
    }

    user user;
    int n;

    write(STDOUT_FILENO, "아이디를 입력하세요 :", 31);



    while(n = read(STDIN_FILENO, cmd_buf, BUFFSIZE) > 0) {
        sscanf(cmd_buf, "%d", &user.user_id);
        break;
    }

    write(STDOUT_FILENO, "비밀번호를 입력하세요 :", 34);

    while( n = read(STDIN_FILENO, cmd_buf, BUFFSIZE) > 0) {
        sscanf(cmd_buf, "%d", &user.password);
        break;
    }
    

    for (int i = 0; i < numRows; i++) {
        if (user.user_id == userDB[i][1] ) {
            if (user.password == userDB[i][2]) {
                user.grade = userDB[i][0];
                user.cph = userDB[i][3];
                user.start_time = userDB[i][4];
                user.end_time = userDB[i][5];
                printf("로그인 성공\n");
                break;
            } else {
                printf("비밀번호가 틀렸습니다.\n");
                longjmp(env, 1);
            } 
        }
        if (i == numRows - 1) {
            printf("아이디가 존재하지 않습니다.\n");
            longjmp(env, 1);
        }
    }

    return user;

}

int main() {
    FILE *fp = fopen("data/userDB.txt", "r");
    user user = login(fp);
    printf("user.grade : %d\n", user.grade);
    printf("user.user_id : %d\n", user.user_id);
    printf("user.password : %d\n", user.password);
    printf("user.cph : %d\n", user.cph);
    printf("user.start_time : %d\n", user.start_time);
    printf("user.end_time : %d\n", user.end_time);
    fclose(fp);
}
