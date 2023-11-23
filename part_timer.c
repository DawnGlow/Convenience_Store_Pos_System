#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>
#include "header.h"

#define BUFFSIZE 8192

void check_stock(int fp);
void change_work_time(user user);
void print_salary(user user);

void part_timer(user user, int item_fp) {
    jmp_buf env, env2;

    write(STDOUT_FILENO, "알바생 모드입니다.\n", 28);

    if (setjmp(env) != 0) {
        write(STDOUT_FILENO, "잘못된 입력입니다. 다시 시도해주세요.\n", 55);
    }
    if (setjmp(env2) != 0) {
    }

    write(STDOUT_FILENO, "\n원하는 기능을 선택하세요\n", 38);
    write(STDOUT_FILENO, "1. 재고 확인\n2. 근무시간 조정 요청\n3. 예상 급여 명세서\n0. 종료\n", 85);

    char cmd_buf[BUFFSIZE];
    int n;
    int choice;

    if ((n = read(STDIN_FILENO, cmd_buf, BUFFSIZE)) < 0)
        write(STDOUT_FILENO, "read error\n", 11);
    choice = atoi(cmd_buf);

    switch (choice) {
        case 1:
            check_stock(item_fp);
            longjmp(env2, 1);
            break;
        case 2:
            change_work_time(user);
            longjmp(env2, 1);
            break;
        case 3:
            print_salary(user);
            longjmp(env2, 1);
            break;
        case 0:
            write(STDOUT_FILENO, "프로그램을 종료합니다.\n", 31);
            exit(0);
            break;
        default:
            longjmp(env, 1);
    }
}

void check_stock(int fp) {
    write(STDOUT_FILENO, "재고 확인 기능입니다.\n", 31);
    int n;
    char buf[BUFFSIZE];

    while ((n = read(fp, buf, BUFFSIZE)) > 0) {
        write(STDOUT_FILENO, buf, n);
    }
}

void change_work_time(user user) {
    write(STDOUT_FILENO, "근무시간 조정 요청 기능입니다.\n", 45);
    int n;
    char buf[BUFFSIZE];

    int wanted_start_time;
    int wanted_end_time;

    jmp_buf env;
    if (setjmp(env) != 0) {
        write(STDOUT_FILENO, "잘못된 입력입니다. 다시 시도해주세요.\n", 55);
    }

    write(STDOUT_FILENO, "원하는 근무 시작 시간을 입력하세요 :", 52);
    if ((n = read(STDIN_FILENO, buf, BUFFSIZE)) < 0) 
       perror("read error");
    sscanf(buf, "%d", &wanted_start_time);

    write(STDOUT_FILENO, "원하는 근무 종료 시간을 입력하세요 :", 52);

    if((n = read(STDIN_FILENO, buf, BUFFSIZE)) < 0) 
        perror("read error");
    sscanf(buf, "%d", &wanted_end_time);

    if (wanted_start_time > wanted_end_time) {
        longjmp(env, 1);
    }

    int fd = open("user_request.txt",  O_CREAT | O_RDWR , 0644);

    if (fd < 0) {
        write(STDOUT_FILENO, "open error\n", 12);
    }
    lseek(fd, 0, SEEK_END);

    char message[120];
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    
    sprintf(message, "%d가 근무시간 조정을 요청했습니다.\n시작 시간: %d, 종료 시간: %d\n요청 날짜: %d-%d-%d\n",
            user.user_id, wanted_start_time, wanted_end_time,tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday
            );
    
    write(fd, message, 120);

    write(STDOUT_FILENO, "근무시간 조정 요청이 완료되었습니다.\n", 54);
}

void print_salary(user user) {
    write(STDOUT_FILENO, "예상 급여 명세서 기능입니다.\n", 42);
    int n;
    char buf[BUFFSIZE];
    time_t now = time(NULL);
    struct tm *tm = localtime(&now);
    int current_time = tm->tm_hour;
    int current_min = tm->tm_min;
    int current_day = tm->tm_mday;

    int expected_salary;
    if (user.end_time > current_time) {
        expected_salary = (current_day - 1) * user.cph * (user.end_time - user.start_time) + (current_time - user.start_time) * user.cph + (current_min / 60) * user.cph;
    } else {
        expected_salary = (current_day) * user.cph * (user.end_time - user.start_time);
    }

    write(STDOUT_FILENO, "예상 급여 명세서입니다.\n", 35);
    sprintf(buf, "예상 급여: %d\n", expected_salary);
    write(STDOUT_FILENO, buf, 19);
}

int main() {
    int item_fp = open("data/itemDB.txt", O_RDONLY);
    user user;
    user.grade = 1;
    user.user_id = 1;
    user.password = 1;
    user.cph = 10000;
    user.start_time = 9;
    user.end_time = 18;
    part_timer(user, item_fp);
    return 0;
}