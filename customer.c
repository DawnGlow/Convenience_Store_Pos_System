#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>
#include "header.h"
#include <string.h>
#define BUFFSIZE 8192

void buy_item(item * item_list, purchase_list *purchase_list);
void refund_item(item * item_list, FILE *purchase_list_fp);
void update_item_DB(item *item_list);
void search_item();
void check_purchase_list(FILE *purchase_list_fp);
void open_refrigerator(FILE *saved_item_fp);


void customer(int item_fd, int saved_item_fd, int purchase_list_fd){

    FILE *item_fp = fdopen(item_fd, "r");
    FILE *saved_item_fp = fdopen(saved_item_fd, "a+");
    FILE *purchase_list_fp = fdopen(purchase_list_fd, "a+");
    purchase_list purchase_list;
    for (int i = 0; i < 10; i++) {
        purchase_list.id_list[i][0] = -1;
        purchase_list.id_list[i][1] = -1;
    }
    
    write(STDOUT_FILENO, "고객 모드입니다.\n", 25);

    jmp_buf env;
    if (setjmp(env) != 0) {
    }

    write(STDOUT_FILENO, "원하는 기능을 선택하세요\n", 37);
    write(STDOUT_FILENO, "1. 물건 구매\n2. 물건 환불\n3. 물건 목록 확인\n4. 구매 목록 확인\n5. 내 냉장고 열기\n0. 종료\n", 117);
    char cmd_buf[BUFFSIZE];
    int n;
    int choice;
    char **itemDB_name_list;
    int **itemDB;
    int numRows = 10;
    int numCols = 8;
    itemDB_name_list = (char**)malloc(numRows * sizeof(char*));
    for (int i = 0; i < numRows; i++) {
        itemDB_name_list[i] = (char*)malloc(10 * sizeof(char));
    }
    itemDB = (int**)malloc(numRows * sizeof(int*));
    for (int i = 0; i < numRows; i++) {
        itemDB[i] = (int*)malloc(numCols * sizeof(int));
    }
    for (int i = 0; i < numRows; i++) {
        fscanf(item_fp, "%s %d %d %d %d %d %d %d", itemDB_name_list[i], &itemDB[i][1], &itemDB[i][2], &itemDB[i][3], &itemDB[i][4], &itemDB[i][5], &itemDB[i][6], &itemDB[i][7]);
    }

    item item_list[10];
    for (int i = 0; i < 10; i++) {
        strcpy(item_list[i].item_name, itemDB_name_list[i]);
        item_list[i].item_id = itemDB[i][1];
        item_list[i].item_count = itemDB[i][2];
        item_list[i].price = itemDB[i][3];
        item_list[i].dc_percent = itemDB[i][4];
        item_list[i].event_mode = itemDB[i][5];
        item_list[i].sale_count = itemDB[i][6];
        item_list[i].margin_percent = itemDB[i][7];
        item_list[i].margin = (double)item_list[i].price * item_list[i].margin_percent / 100.0;
    }

    if ((n = read(STDIN_FILENO, cmd_buf, BUFFSIZE)) < 0)
        write(STDOUT_FILENO, "read error\n", 12);

    choice = atoi(cmd_buf);
    switch (choice) {
        case 1:
            buy_item(item_list, &purchase_list);
            update_item_DB(item_list);
            longjmp(env, 1);
            break;
        case 2:
            refund_item(item_list, purchase_list_fp);
            update_item_DB(item_list);
            longjmp(env, 1);
            break;
        case 3:
            search_item();
            longjmp(env, 1);
            break;
        case 4:
            check_purchase_list(purchase_list_fp);
            longjmp(env, 1);
            break;
        case 5:
            open_refrigerator(saved_item_fp);
            longjmp(env, 1);
            break;
        case 0:
            write(STDOUT_FILENO, "프로그램을 종료합니다.\n", 34);
            for (int i = 0 ; i < 10; i++){
                printf("%d %d\n", purchase_list.id_list[i][0], purchase_list.id_list[i][1]);
            }
            if (purchase_list.id_list[0][0] != -1){
                fprintf(purchase_list_fp, "%d %d %d %d %d %d\n", purchase_list.year, purchase_list.month, purchase_list.hour, purchase_list.hour, purchase_list.min, purchase_list.purchase_num);
                for (int i = 0; i < 10; i++) {
                    if(purchase_list.id_list[i][0] != -1){
                        fprintf(purchase_list_fp, "%d %d\n", purchase_list.id_list[i][0], purchase_list.id_list[i][1]);
                    }
                }
            }
            return;
        default:
            write(STDOUT_FILENO, "잘못된 입력입니다. 다시 시도해주세요.\n", 55);
            longjmp(env, 1);
    }


}

void buy_item(item * item_list, purchase_list *purchase_list) {
    write(STDOUT_FILENO, "물건 구매 기능입니다.\n", 31);
    int n;
    char buf[BUFFSIZE];

    int item_id;
    int item_count;
    int purchased_price;

    jmp_buf env, env2;
    if (setjmp(env) != 0) {
        write(STDOUT_FILENO, "잘못된 입력입니다. 다시 시도해주세요.\n", 55);
    }

    write(STDOUT_FILENO, "구매할 물건의 ID를 입력하세요 :", 44);
    if ((n = read(STDIN_FILENO, buf, BUFFSIZE)) < 0)
    sscanf(buf, "%d", &item_id);
    int temp_item_id = item_id;

    int wanted_item_index;

    for (int i = 0; i < 10; i++) {
        if (item_id == item_list[i].item_id) {
            wanted_item_index = i -1;
            break;
        }
    }

    if (item_list[wanted_item_index].item_count == 0) {
                write(STDOUT_FILENO, "재고가 없습니다.\n", 31);
                longjmp(env, 1);
            }
    else{
        write(STDOUT_FILENO, "구매할 물건의 개수를 입력하세요 :", 48);
        if ((n = read(STDIN_FILENO, buf, BUFFSIZE)) < 0) 
            perror("read error");
        sscanf(buf, "%d", &item_count);
        if (item_count > item_list[wanted_item_index].item_count) {
            write(STDOUT_FILENO, "재고가 부족합니다.\n", 31);
            longjmp(env, 1);
        }
        else {

            int real_price = item_list[wanted_item_index].price * (100 - item_list[wanted_item_index].dc_percent) / 100.0;

            if (item_list[wanted_item_index].event_mode == 1) {
                write(STDOUT_FILENO, "1 + 1 이벤트가 적용되었습니다.\n", 43);

                if (item_count % 2 == 0) {
                    item_list[wanted_item_index].item_count -= item_count;
                    purchased_price = (item_count / 2) * real_price;
                    item_list[wanted_item_index].sale_count += item_count;
                }
                else {
                    item_count += 1;
                    item_list[wanted_item_index].item_count -= item_count;
                    purchased_price = (item_count / 2) * item_list[wanted_item_index].price;
                    item_list[wanted_item_index].sale_count += item_count;
                }
            }
            else if (item_list[wanted_item_index].event_mode == 2) {
                write(STDOUT_FILENO, "2 + 1 이벤트가 적용되었습니다.\n", 43);

                item_list[wanted_item_index].item_count -= item_count;
                purchased_price = (item_count / 3) * (2 * real_price) + (item_count % 3) * real_price;
                item_list[wanted_item_index].sale_count += item_count;
            }
            else {
                item_list[wanted_item_index].item_count -= item_count;
                purchased_price = item_count * real_price;
                item_list[wanted_item_index].sale_count += item_count;
            
        }
            }
    time_t t = time(NULL);
            struct tm tm = *localtime(&t);           
            purchase_list->year = tm.tm_year + 1900;
            purchase_list->month = tm.tm_mon + 1;
            purchase_list->day =  tm.tm_mday;
            purchase_list->hour = tm.tm_hour;
            purchase_list->min = tm.tm_min;
            purchase_list->purchase_num = rand() % 90000 + 10000;
            for(int i = 0; i < 10; i++){
                if (purchase_list->id_list[i][0] == -1){
                    purchase_list->id_list[i][0] = item_list[wanted_item_index].item_id;
                    purchase_list->id_list[i][1] = item_count;
                    break;
                }
            }

    write(STDOUT_FILENO, "구매가 완료되었습니다.\n", 34);
    write(STDOUT_FILENO, "개인 냉장고에 보관하실 제품이 있으신가요?\n", 61);
    write(STDOUT_FILENO, "1. 있음\n2. 없음\n", 21);
    int choice;
    if ((n = read(STDIN_FILENO, buf, BUFFSIZE)) < 0) 
        perror("read error");
    sscanf(buf, "%d", &choice);
    if(choice == 1){
        
        FILE *FP;
        FP = fopen("data/saved_item_DB.txt", "a+");

        write(STDOUT_FILENO, "개인 냉장고에 보관하실 제품의 개수를 입력하세요 :", 71);
        int wanted_store_count;
        if (setjmp(env2) != 0) {
        }

        if ((n = read(STDIN_FILENO, buf, BUFFSIZE)) < 0) {
            perror("read error");}

        sscanf(buf, "%d", &item_count);

        if (item_count < wanted_store_count) {
            write(STDOUT_FILENO, "구매하신 수량보다 많습니다.\n", 31);
            longjmp(env2, 1);
        }
        else {
            item_list[wanted_item_index].item_count += item_count;
            fprintf(FP, "%d %d\n", item_id, item_count);
        }
    }
    else if(choice == 2){
    }
    else{
        write(STDOUT_FILENO, "잘못된 입력입니다. 다시 시도해주세요.\n", 55);
        longjmp(env, 1);
    }
    
   
    }   
}
void refund_item(item * item_list, FILE *purchase_list_fp) {
    write(STDOUT_FILENO, "물건 환불 기능입니다.\n", 32);
    int n;
    char buf[BUFFSIZE];

    int bill_num;
    int item_id;
    int item_count;
    int refund_price;

    jmp_buf env;
    if (setjmp(env) != 0) {
        write(STDOUT_FILENO, "잘못된 입력입니다. 다시 시도해주세요.\n", 55);
    }

    write(STDOUT_FILENO, "구매한 영수증 번호를 알려주세요", 46);
    if ((n = read(STDIN_FILENO, buf, BUFFSIZE)) < 0){ 
        perror("read error");}
    sscanf(buf, "%d", &bill_num);

    int year, month, day, hour, minute, read_bill_num;

    while (fscanf( purchase_list_fp , "%d %d %d %d %d %d", &year, &month, &day, &hour, &minute, &read_bill_num) > 0) {
        memset(buf, 0, sizeof(buf)); // 버퍼 초기화
        while (fscanf(purchase_list_fp, "%d %d", &item_id, &item_count) >0){

            if (bill_num == read_bill_num) {
                write(STDOUT_FILENO, "환불할 물건의 ID를 입력하세요 :", 44);
                if ((n = read(STDIN_FILENO, buf, BUFFSIZE)) < 0) 
                    perror("read error");
                int want_item_id;
                sscanf(buf, "%d", &want_item_id);

                if(want_item_id == item_id){
                    write(STDOUT_FILENO, "환불할 물건의 개수를 입력하세요 :", 48);
                    if ((n = read(STDIN_FILENO, buf, BUFFSIZE)) < 0) 
                        perror("read error");
                    int want_item_count;
                    sscanf(buf, "%d", &want_item_count);
                    if ( want_item_count > item_count){
                        write(STDOUT_FILENO, "환불할 물건의 개수가 너무 많습니다.\n", 48);
                        longjmp(env, 1);
                    }
                    else{
                        int wanted_item_index;
                        for (int i = 0; i < 10; i++) {
                            if (item_list[i].item_id == want_item_id) {
                                wanted_item_index = i;
                            }
                        }
                        item_list[wanted_item_index].item_count += want_item_count;
                        item_list[wanted_item_index].sale_count -= want_item_count;


                        write(STDOUT_FILENO, "환불이 완료되었습니다.\n", 34);
                        break;
                    }

                }
            }
            memset(buf, 0, sizeof(buf)); // 버퍼 초기화
        }
        
    }
}
void update_item_DB(item *item_list){
    FILE *item_fp = fopen("data/itemDB.txt", "w");
    for (int i = 0; i < 10; i++) {
        if(item_list[i].item_count != 0){
        fprintf(item_fp, "%s %d %d %d %d %d %d %d\n", item_list[i].item_name, item_list[i].item_id, item_list[i].item_count, item_list[i].price, item_list[i].dc_percent, item_list[i].event_mode, item_list[i].sale_count, item_list[i].margin_percent);
    }
    }
    fclose(item_fp);
}
void search_item(){
    FILE *fp = fopen("data/itemDB.txt", "r");
    char **item_name;
    int *item_id;
    int *item_count;
    int *price;
    int *dc_percent;
    int *event_mode;
    int *sale_count;
    int *margin_percent;

    int numRows = 10;
    int numCols = 8;
    item_name = (char**)malloc(numRows * sizeof(char*));
    for (int i = 0; i < numRows; i++) {
        item_name[i] = (char*)malloc(40 * sizeof(char));
    }
    item_id = (int*)malloc(numRows * sizeof(int));
    item_count = (int*)malloc(numRows * sizeof(int));
    price = (int*)malloc(numRows * sizeof(int));
    dc_percent = (int*)malloc(numRows * sizeof(int));
    event_mode = (int*)malloc(numRows * sizeof(int));
    sale_count = (int*)malloc(numRows * sizeof(int));
    margin_percent = (int*)malloc(numRows * sizeof(int));

    int i = 0;
    while(fscanf(fp, "%s %d %d %d %d %d %d %d", item_name[i], &item_id[i], &item_count[i], &price[i], &dc_percent[i], &event_mode[i], &sale_count[i], &margin_percent[i]) > 0){
        char event_message[7];

        if (event_mode[i] == 0)
            sprintf(event_message, "일반");
        else if (event_mode[i] == 1)
            sprintf(event_message, "1 + 1");
        else if (event_mode[i] == 2)
            sprintf(event_message, "2 + 1");
        if(item_count[i] != 0){
        fprintf(stdout, "물건 이름 : %s /물건 ID : %d /물건 개수 : %d /물건 가격 : %d /할인율 : %d /이벤트 모드 : %s \n", item_name[i], item_id[i], item_count[i], price[i], dc_percent[i], event_message);}
        i++;
    }
        
}

void check_purchase_list(FILE *purchase_list_fp){
    int year, month, day, hour, minute, read_bill_num;
    int item_id, item_count;
    char line[100]; // 줄을 저장할 버퍼

    while (fgets(line, sizeof(line), purchase_list_fp) != NULL) {
        sscanf(line, "%d %d %d %d %d %d", &year, &month, &day, &hour, &minute, &read_bill_num);
        fprintf(stdout, "구매 날짜 : %d-%d-%d %d:%d /영수증 번호 : %d\n", year, month, day, hour, minute, read_bill_num);
        memset(line, 0, sizeof(line)); // 버퍼 초기화
        while (fgets(line, sizeof(line), purchase_list_fp) != NULL) {
            sscanf(line, "%d %d", &item_id, &item_count);
            if (item_id == 2023){
                break;}
            fprintf(stdout, "물건 ID : %d /물건 개수 : %d\n", item_id, item_count);
            memset(line, 0, sizeof(line)); // 버퍼 초기화
        }
    }

}
void open_refrigerator(FILE *saved_item_fp){
    int item_id, item_count;
    while(fscanf(saved_item_fp, "%d %d", &item_id, &item_count) > 0){
        fprintf(stdout, "물건 ID : %d /물건 개수 : %d\n", item_id, item_count);
    }
}

int main(){
    int item_fd = open("data/itemDB.txt", O_RDWR);
    int saved_item_fd = open("data/saved_item_DB.txt", O_RDWR);
    int purchase_list_fd = open("data/purchase_list_DB.txt", O_RDWR);

    search_item();
    customer(item_fd, saved_item_fd, purchase_list_fd);


}