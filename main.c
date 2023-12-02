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
    int numRows = 10;
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
    FILE *user_fp = fopen("data/userDB.txt", "r");
    user user1 = login(user_fp);
    
    if (user1.grade == 0) {
        item *itemdata;
        purchase_list *purchase_data;
        int item_fd = open("data/itemDB.txt", O_RDWR);
        int purchase_list_fd = open("data/random_purchase_data.txt", O_RDWR);
        int saved_item_fd = open("data/saved_item_DB.txt", O_RDWR);
        int user_rows = initUser(&user1, user_fp);
        int item_rows = initItem(&itemdata, item_fd);
        int purchase_row = init_purchase_list(&purchase_data, purchase_list_fd);
        admin_menu(user_rows, user_rows, itemdata, item_rows, purchase_data, purchase_row);
    }
    else {
        int item_fd = open("data/itemDB.txt", O_RDWR);
        int saved_item_fd = open("data/saved_item_DB.txt", O_RDWR);
        int purchase_list_fd = open("data/purchase_list_DB.txt", O_RDWR);
        customer(item_fd, saved_item_fd, purchase_list_fd);
    }
    fclose(user_fp);
}
