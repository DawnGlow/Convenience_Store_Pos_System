#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>
#include "header.h"
#include <string.h>
#define BUFFSIZE 8192

int initUser(user **userdata, FILE *user_fp) {
    int rows = 0, cols = 6;
    char buf[BUFFSIZE];

    fseek(user_fp, 0, SEEK_SET);

    while (fgets(buf, BUFFSIZE, user_fp) != NULL) {
        rows++;
    }

    fseek(user_fp, 0, SEEK_SET);

    *userdata = (user*)malloc(rows * sizeof(user));
    if (*userdata == NULL) {
        fprintf(stderr, "memory allocation error\n");
        exit(1);
    }

    for (int i = 0; i < rows; i++) {
        fscanf(user_fp, "%d %d %d %d %d %d", &(*userdata)[i].grade, &(*userdata)[i].user_id, &(*userdata)[i].password, &(*userdata)[i].cph, &(*userdata)[i].start_time, &(*userdata)[i].end_time);
    }

    printf("initialize userDB successful!\n");
    sleep(1);
    return rows;
}


void printUser(user *userdata, int rows) {
    printf("userinfo 출력\n");
    sleep(1);
    for (int i = 0; i < rows; i++) {
        if (userdata[i].grade == 0) {
            printf("사장, ");
        } else {
            printf("근무자, ");
        }
        printf("id: %d\n", userdata[i].user_id);
        printf("시급: %d, 근무 시작 시각: %d, 근무 종료 시각: %d\n", userdata[i].cph, userdata[i].start_time, userdata[i].end_time);
    }
    fflush(stdin);
    printf("아무 키나 누르면 이전 메뉴로 돌아갑니다...\n");
    getchar();
    return;
}

void modUser(user *userdata, int rows) {
    // printf("미구현 상태입니다\n");
    printf("근무 시각 변경하기");
    printf("아무 키나 누르면 이전 메뉴로 돌아갑니다...\n");
    getchar();
}

void saveUser(user *userdata, int rows) {
    FILE* user_fp = fopen("data/userDB.txt", "w");
    if (user_fp == NULL) {
        fprintf(stderr, "Error opening file\n");
        exit(1);
    }
    int cols = 6;
    for (int i = 0; i < rows; i++) {
        fprintf(user_fp, "%d %d %d %d %d %d\n", userdata[i].grade, userdata[i].user_id, userdata[i].password, userdata[i].cph, userdata[i].start_time, userdata[i].end_time);
    }
    printf("save userDB successful!\n");
    fclose(user_fp);
}

int addUser(user **userdata, int rows) {
    int cols = 6;
    int user_size; // 추가할 인원 수
    system("clear");
    printf("추가할 사람의 수를 입력하세요: ");
    scanf("%d", &user_size);
    rows += user_size;

    // 기존 데이터를 새로운 배열에 복사
    user *new_userdata = (user*)malloc(rows * sizeof(user));
    for (int i = 0; i < rows - user_size; i++) {
        new_userdata[i].grade = (*userdata)[i].grade;
        new_userdata[i].user_id = (*userdata)[i].user_id;
        new_userdata[i].password = (*userdata)[i].password;
        new_userdata[i].cph = (*userdata)[i].cph;
        new_userdata[i].start_time = (*userdata)[i].start_time;
        new_userdata[i].end_time = (*userdata)[i].end_time;
    }

    // 새로운 데이터 추가
    for (int i = rows - user_size; i < rows; i++) {
        printf("%d번째 User 입니다.\n", i + 1);
        printf("직급(사장이면 0, 근무자면 1) 입력: ");
        scanf("%d", &new_userdata[i].grade);
        printf("사용자 id 입력: ");
        scanf("%d", &new_userdata[i].user_id);
        printf("사용자 password 입력: ");
        scanf("%d", &new_userdata[i].password);
        printf("시급 입력: ");
        scanf("%d", &new_userdata[i].cph);
        printf("근무 시작 시각 입력: ");
        scanf("%d", &new_userdata[i].start_time);
        printf("근무 종료 시각 입력: ");
        scanf("%d", &new_userdata[i].end_time);
    }

    free(*userdata);
    *userdata = new_userdata;
    saveUser(*userdata, rows);
    printf("add user to userDB successful!\n");
    return rows;
}

int delUser(user **userdata, int rows) {
    int user_id;
    printf("삭제할 사용자의 ID를 입력하세요: ");
    scanf("%d", &user_id);

    // 삭제할 사용자를 찾아서 그 위치를 저장
    int delete_index = -1;
    for (int i = 0; i < rows; i++) {
        if ((*userdata)[i].user_id == user_id) {
            delete_index = i;
            break;
        }
    }
    // 사용자를 찾았는지 확인 후 삭제 작업 수행
    if (delete_index != -1) {
        // 삭제할 사용자를 제외한 데이터를 새로운 배열에 복사
        user *new_userdata = (user*)malloc((rows - 1) * sizeof(user));
        int new_index = 0;
        for (int i = 0; i < rows; i++) {
            if (i != delete_index) {
                new_userdata[new_index++] = (*userdata)[i];
            }
        }
        free(*userdata);
        *userdata = new_userdata;
        (rows)--;
        saveUser(*userdata, rows);
        printf("사용자 삭제 성공!\n");
    } else {
        printf("해당 사용자를 찾을 수 없습니다.\n");
    }
    fflush(stdin);
    printf("아무 키나 누르면 이전 메뉴로 돌아갑니다...\n");
    getchar();
    return rows;
}

void userdata_menu(user* userdata, int user_rows) {
    while (1) {
        int menu; // 메뉴 번호를 입력받아 저장하는 변수
        system("clear");
        printf("경영주님, 환영합니다!\n");
        printf("---------------------------------------------\n");
        printf("1. 근무자 정보 조회\n");
        printf("2. 근무자 정보 수정\n");
        printf("3. 근무자 정보 추가\n");
        printf("4. 근무자 정보 삭제\n");
        printf("0. 이전 메뉴로 돌아가기\n");
        printf("원하는 기능을 선택하여 입력하세요: ");
        scanf("%d", &menu);
        switch (menu) {
            case 0:
                return;
            case 1:
                printUser(userdata, user_rows);
                break;
            case 2:
                modUser(userdata, user_rows);
                break;
            case 3:
                user_rows = addUser(&userdata, user_rows);
                break;
            case 4:
                user_rows = delUser(&userdata, user_rows);
                break;
        }
    }
    return;
}

void item_menu(int item_fd) {
    /*while (1) {
        int menu; // 메뉴 번호를 입력받아 저장하는 변수
        system("clear");
        printf("경영주님, 환영합니다!\n");
        printf("---------------------------------------------\n");
        printf("1. 재고 확인\n");
        printf("2. 발주\n");
        printf("3. 근무자 정보 추가\n");
        printf("4. 근무자 정보 삭제\n");
        printf("0. 이전 메뉴로 돌아가기\n");
        printf("원하는 기능을 선택하여 입력하세요: ");
        scanf("%d", &menu);
        switch (menu) {
            case 0:
                return;
            case 1:
                printUser(userdata, user_rows);
                break;
            case 2:
                modUser(userdata, user_rows);
                break;
            case 3:
                user_rows = addUser(&userdata, user_rows);
                break;
            case 4:
                user_rows = delUser(&userdata, user_rows);
                break;
        }
    }*/
}

void analyze_menu(int item_fd, int purchase_list_fd) {
    system("clear");
    printf("analyze_menu\n");
    sleep(5);
    return;
}
void discount_menu(int item_fd) {
    system("clear");
    printf("discount_menu\n");
    sleep(5);
    return;
}

void admin_menu(int item_fd, int purchase_list_fd, user* userdata, int user_rows) {
    time_t timer; // time_t type 변수
    struct tm *t; // time 구조체 인스턴스
    FILE *item_fp = fdopen(item_fd, "r");
    while (1) {
        int menu; // 메뉴 번호를 입력받아 저장하는 변수

        timer = time(NULL); // 타임 변수 초기화
        t = localtime(&timer); // time 구조체로 변환
        system("clear");
        printf("경영주님, 환영합니다!\n");
        printf("---------------------------------------------\n");
        printf("현재 시각: %d년 %d월 %d일 %d시 %d 분\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
        printf("---------------------------------------------\n");
        printf("1. 재고 확인 및 발주\n");
        printf("2. 근무자 정보 / 근무 시간 확인 및 수정\n");
        printf("3. 점포 경영 지표 분석\n");
        printf("4. 할인 정책 수정\n");
        printf("0. 경영 관리 시스템 나가기\n");
        printf("원하는 기능을 선택하여 입력하세요: ");
        scanf("%d", &menu);
        switch (menu) {
            case 0:
                return;
            case 1:
                item_menu(item_fd);
                break;
            case 2:
                userdata_menu(userdata, user_rows);
                break;
            case 3:
                analyze_menu(item_fd, purchase_list_fd);
                break;
            case 4:
                discount_menu(item_fd);
                break;
        }
    }
}

/* int main(){
    FILE *user_fp = fopen("data/userDB.txt", "r");
    if (user_fp == NULL) {
        fprintf(stderr, "file open error\n");
    }
    user *userdata;
    int user_rows = initUser(&userdata, user_fp);
    int item_fd = open("data/itemDB.txt", O_RDWR);
    int saved_item_fd = open("data/saved_item_DB.txt", O_RDWR);
    int purchase_list_fd = open("data/purchase_list_DB.txt", O_RDWR);
    admin_menu(item_fd, purchase_list_fd, userdata, user_rows);
} */