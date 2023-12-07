#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <setjmp.h>
#include <time.h>
#include "header.h"
#include <string.h>
#define BUFFSIZE 8192

// User section
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
    int i;
    for (i = 0; i < rows; i++) {
        fscanf(user_fp, "%d %d %d %d %d %d", &(*userdata)[i].grade, &(*userdata)[i].user_id, &(*userdata)[i].password, &(*userdata)[i].cph, &(*userdata)[i].start_time, &(*userdata)[i].end_time);
    }

    printf("initialize userDB successful!\n");
    sleep(1);
    return rows;
}

void printUser(user *userdata, int rows) {
    printf("userinfo 출력\n");
    sleep(1);
    int i;
    for (i = 0; i < rows; i++) {
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
    int ID, idx, menu, temp;
    system("clear");
    printf("사용자 정보를 변경할 사용자의 ID를 입력해주세요: ");
    scanf("%d", &ID);
    int i;
    for (i = 0; i < rows; i++) {
        if (userdata[i].user_id == ID) {
            idx = i;
            break;
        }
    }
    system("clear");
    printf("1. 사용자 id\n");
    printf("2. 사용자 비밀번호\n");
    printf("3. 사용자 시급\n");
    printf("4. 사용자 근무 시간\n");
    printf("0. 돌아가기\n");
    printf("------------------------------\n");
    printf("어떤 정보를 수정하시겠습니까?: ");
    scanf("%d", &menu);
    switch(menu) {
        case 1:
            printf("현재 user ID: %d\n", userdata[idx].user_id);
            printf("변경할 user ID 입력: ");
            scanf("%d", &userdata[idx].user_id);
            printf("변경 완료\n");
            sleep(1);
            break;
        case 2:
            printf("현재 user password 입력: ");
            scanf("%d", &temp);
            if (temp == userdata[idx].password) {
                printf("변경할 user password 입력: ");
                scanf("%d", &userdata[idx].password);
                printf("변경 완료\n");
                sleep(1);
                break;
            }
            else {
                printf("비밀번호가 틀렸습니다\n");
            }
        case 3:
            printf("현재 user의 시급: %d\n", userdata[idx].cph);
            printf("변경할 user의 시급: ");
            scanf("%d", &userdata[idx].cph);
            printf("변경 완료\n");
            sleep(1);
            break;
        case 4:
            printf("현재 user의 근무 시작: %d시, 종료 시각: %d시\n", userdata[idx].start_time, userdata[idx].end_time);
            printf("변경할 user 근무 시작 / 종료 시각 입력: ");
            scanf("%d %d", &userdata[idx].start_time, &userdata[idx].end_time);
            printf("변경 완료\n");
            sleep(1);
            break;
    }
    saveUser(userdata, rows);
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
    int i;
    for (i = 0; i < rows; i++) {
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
    int i;
    for (i = 0; i < rows - user_size; i++) {
        new_userdata[i].grade = (*userdata)[i].grade;
        new_userdata[i].user_id = (*userdata)[i].user_id;
        new_userdata[i].password = (*userdata)[i].password;
        new_userdata[i].cph = (*userdata)[i].cph;
        new_userdata[i].start_time = (*userdata)[i].start_time;
        new_userdata[i].end_time = (*userdata)[i].end_time;
    }

    // 새로운 데이터 추가
    for (i = rows - user_size; i < rows; i++) {
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
    int i;
    for (i = 0; i < rows; i++) {
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
        for (i = 0; i < rows; i++) {
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
void check_request();
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
        printf("5. 근무자 요청 사항 조회\n");
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
            case 5:
                check_request();
                break;
        }
    }
    return;
}


// Item section
int initItem(item **itemdata, int item_fd) {
    int rows = 0, cols = 8;
    char buf[BUFFSIZE];
    FILE *item_fp = fdopen(item_fd, "r");
    if (item_fp == NULL) {
        fprintf(stderr, "fd open error");
    }
    fseek(item_fp, 0, SEEK_SET);

    while (fgets(buf, BUFFSIZE, item_fp) != NULL) {
        rows++;
    }

    fseek(item_fp, 0, SEEK_SET);

    *itemdata = (item*)malloc(rows * sizeof(item));
    if (*itemdata == NULL) {
        fprintf(stderr, "memory allocation error\n");
        exit(1);
    }
    int i;
    for (i = 0; i < rows; i++) {
        fscanf(item_fp, "%s %d %d %d %d %d %d %d", (*itemdata)[i].item_name, &(*itemdata)[i].item_id, &(*itemdata)[i].item_count, 
        &(*itemdata)[i].price, &(*itemdata)[i].dc_percent, &(*itemdata)[i].event_mode, &(*itemdata)[i].sale_count, &(*itemdata)[i].margin_percent);
        (*itemdata)[i].margin = (*itemdata)[i].price * (*itemdata)[i].margin_percent / 100;
    }

    printf("initialize itemDB successful!\n");
    sleep(1);
    return rows;
}

int addItem(item **itemdata, int rows) {
    int item_size; // 추가할 아이템 개수
    system("clear");
    printf("기존에 존재하지 않던 새로운 item을 추가합니다\n");
    printf("추가할 물건 종류 수를 입력하세요: ");
    scanf("%d", &item_size);
    rows += item_size;

    // 기존 데이터를 새로운 배열에 복사
    item *new_itemdata = (item*)malloc(rows * sizeof(item));
    int i;
    for (i = 0; i < rows - item_size; i++) {
        strcpy(new_itemdata[i].item_name, (*itemdata)[i].item_name);
        //new_itemdata[i].item_name = (*itemdata)[i].item_name;
        new_itemdata[i].item_id = (*itemdata)[i].item_id;
        new_itemdata[i].item_count = (*itemdata)[i].item_count;
        new_itemdata[i].price = (*itemdata)[i].price;
        new_itemdata[i].dc_percent = (*itemdata)[i].dc_percent;
        new_itemdata[i].event_mode = (*itemdata)[i].event_mode;
        new_itemdata[i].sale_count = (*itemdata)[i].sale_count;
        new_itemdata[i].margin_percent = (*itemdata)[i].margin_percent;
        new_itemdata[i].margin = (*itemdata)[i].margin;
    }

    // 새로운 데이터 추가
    for (i = rows - item_size; i < rows; i++) {
        printf("상품명 입력: ");
        scanf("%s", new_itemdata[i].item_name);
        printf("물건 id 입력: ");
        scanf("%d", &new_itemdata[i].item_id);
        printf("발주할 물건의 수량 입력: ");
        scanf("%d", &new_itemdata[i].item_count);
        printf("물건의 가격 입력: ");
        scanf("%d", &new_itemdata[i].price);
        printf("물건의 할인율 입력: ");
        scanf("%d", &new_itemdata[i].dc_percent);
        printf("증정 행사 설정 (0번 : 일반 , 1번 : 1 + 1, 2번 : 2 + 1): ");
        scanf("%d", &new_itemdata[i].event_mode);
        new_itemdata[i].sale_count = 0;
        printf("물건의 마진율 설정: ");
        scanf("%d", &new_itemdata[i].margin_percent);
        new_itemdata[i].margin = new_itemdata[i].price * new_itemdata[i].margin_percent / 100;
    }

    free(*itemdata);
    *itemdata = new_itemdata;
    update_item_DB2(*itemdata, rows);
    printf("add item to itemDB successful!\n");
    fflush(stdin);
    printf("아무 키나 누르면 이전 메뉴로 돌아갑니다...\n");
    getchar();
    return rows;
}

void search_item() { // customer function 사용
    FILE *fp = fopen("data/itemDB.txt", "r");
    char **item_name;
    int *item_id;
    int *item_count;
    int *price;
    int *dc_percent;
    int *event_mode;
    int *sale_count;
    int *margin_percent;

    int numRows = 20;
    int numCols = 8;
    item_name = (char**)malloc(numRows * sizeof(char*));
    int i;
    for (i = 0; i < numRows; i++) {
        item_name[i] = (char*)malloc(40 * sizeof(char));
    }
    item_id = (int*)malloc(numRows * sizeof(int));
    item_count = (int*)malloc(numRows * sizeof(int));
    price = (int*)malloc(numRows * sizeof(int));
    dc_percent = (int*)malloc(numRows * sizeof(int));
    event_mode = (int*)malloc(numRows * sizeof(int));
    sale_count = (int*)malloc(numRows * sizeof(int));
    margin_percent = (int*)malloc(numRows * sizeof(int));

    i = 0;
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
    fflush(stdin);
    printf("아무 키나 누르면 이전 메뉴로 돌아갑니다...\n");
    getchar();
}

int modItem(item *itemdata, int item_rows) {
    int new_itemid;
    int idx = -1;
    int addcount;
    system("clear");
    printf("발주할 물건의 ID를 입력해주세요: ");
    scanf("%d", &new_itemid);
    int i;
    for (i = 0; i < item_rows; i++) {
        if (itemdata[i].item_id == new_itemid) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("존재하지 않는 item입니다\n");
        return addItem(&itemdata, item_rows);
    }
    else {
        printf("idx: %d\n", idx);
        printf("현재 %s 의 수량은 %d개 입니다.\n", itemdata[idx].item_name, itemdata[idx].item_count);
        printf("발주하실 물품의 수량을 입력해주세요: ");
        scanf("%d", &addcount);
        itemdata[idx].item_count += addcount;
        update_item_DB2(itemdata, item_rows);
        printf("발주 후 %s 의 수량은 %d개 입니다.\n", itemdata[idx].item_name, itemdata[idx].item_count);
        fflush(stdin);
        printf("아무 키나 누르면 이전 메뉴로 돌아갑니다...\n");
        getchar();
        return item_rows;
    }
}

void update_item_DB2(item *item_list, int rows){
    FILE *item_fp = fopen("data/itemDB.txt", "w");
    int i;
    for (i = 0; i < rows; i++) {
        fprintf(item_fp, "%s %d %d %d %d %d %d %d\n", item_list[i].item_name, item_list[i].item_id, item_list[i].item_count, item_list[i].price, item_list[i].dc_percent, item_list[i].event_mode, item_list[i].sale_count, item_list[i].margin_percent);
    }
    fclose(item_fp);
}

void item_menu(item *itemdata, int item_rows) { // need to merge with customer.c
    while (1) {
        int menu; // 메뉴 번호를 입력받아 저장하는 변수
        system("clear");
        printf("경영주님, 환영합니다!\n");
        printf("---------------------------------------------\n");
        printf("1. 재고 확인\n");
        printf("2. 발주\n");
        printf("0. 이전 메뉴로 돌아가기\n");
        printf("원하는 기능을 선택하여 입력하세요: ");
        scanf("%d", &menu);
        switch (menu) {
            case 0:
                return;
            case 1:
                search_item();
                break;
            case 2:
                item_rows = modItem(itemdata, item_rows);
                break;
        }
    }
}

int init_purchase_list(purchase_list **purchase_data, int purchase_fd) {
    int rows = 0;
    char buf[BUFFSIZE];
    FILE *purchase_fp = fdopen(purchase_fd, "r");
    
    if (purchase_fp == NULL) {
        fprintf(stderr, "fd open error\n");
        exit(1);
    }

    while (fgets(buf, BUFFSIZE, purchase_fp) != NULL) {
        if (buf[0] == '\n') {
            rows++;
        }
    }
    fseek(purchase_fp, 0, SEEK_SET);

    *purchase_data = (purchase_list*)malloc(rows * sizeof(purchase_list));
    if (*purchase_data == NULL) {
        fprintf(stderr, "memory allocation error\n");
        exit(1);
    }
    int i;
    for (i = 0; i < rows; i++) {
        fgets(buf, BUFFSIZE, purchase_fp);
        sscanf(buf, "%d %d %d %d %d %d", &(*purchase_data)[i].year, &(*purchase_data)[i].month, &(*purchase_data)[i].day, &(*purchase_data)[i].hour, &(*purchase_data)[i].min, &(*purchase_data)[i].purchase_num);
        int j;
        for (j = 0; j < 100; j++) {
            fgets(buf, BUFFSIZE, purchase_fp);
            if (buf[0] == '\n' || buf[0] == '\0') {
                break;
            }
            else {
                sscanf(buf, "%d %d", &(*purchase_data)[i].id_list[j][0], &(*purchase_data)[i].id_list[j][1]);
            }
        }
    }

    printf("initialize purchase_list_DB successful!\n");
    sleep(1);
    return rows; // 여기서 rows --> 영수증 개수
}

void print_purchase_data(purchase_list *purchase_data, int num_purchases) {
    int year, month, day;

    // 입력 받은 년/월/일 정보
    printf("년을 입력하세요: ");
    scanf("%d", &year);
    printf("월을 입력하세요: ");
    scanf("%d", &month);
    printf("일을 입력하세요: ");
    scanf("%d", &day);

    // 입력 받은 날짜로 영수증 데이터 출력
    printf("%04d년 %02d월 %02d일의 영수증 데이터:\n", year, month, day);
    int i;
    for (i = 0; i < num_purchases; i++) {
        // 해당 날짜와 일치하는 영수증만 출력
        if (purchase_data[i].year == year && purchase_data[i].month == month && purchase_data[i].day == day) {
            printf("Purchase %d:\n", purchase_data[i].purchase_num);
            printf("Date: %04d-%02d-%02d %02d:%02d\n", purchase_data[i].year, purchase_data[i].month, purchase_data[i].day, purchase_data[i].hour, purchase_data[i].min);

            printf("Items:\n");
            int j;
            for (j = 0; j < 100; j++) {
                if (purchase_data[i].id_list[j][0] == 0 && purchase_data[i].id_list[j][1] == 0) {
                    // End of items
                    break;
                }
                printf("Item ID: %d, 수량: %d\n", purchase_data[i].id_list[j][0], purchase_data[i].id_list[j][1]);
            }

            printf("\n");
        }
    }

    fflush(stdin);
    printf("아무 키나 누르면 이전 메뉴로 돌아갑니다...\n");
    getchar();
}

void print_purchase_data_by_receipt_number(purchase_list *purchase_data, int num_purchases) {
    int receipt_number;

    // 입력 받은 영수증 번호 정보
    printf("영수증 번호를 입력하세요: ");
    scanf("%d", &receipt_number);

    // 입력 받은 영수증 번호로 영수증 데이터 출력
    printf("%d번 영수증 데이터:\n", receipt_number);
    int i;
    for (i = 0; i < num_purchases; i++) {
        // 해당 영수증 번호와 일치하는 영수증만 출력
        if (purchase_data[i].purchase_num == receipt_number) {
            printf("Purchase %d:\n", purchase_data[i].purchase_num);
            printf("Date: %04d-%02d-%02d %02d:%02d\n", purchase_data[i].year, purchase_data[i].month, purchase_data[i].day, purchase_data[i].hour, purchase_data[i].min);

            printf("Items:\n");
            int j;
            for (j = 0; j < 100; j++) {
                if (purchase_data[i].id_list[j][0] == 0 && purchase_data[i].id_list[j][1] == 0) {
                    // End of items
                    break;
                }
                printf("Item ID: %d, 수량: %d\n", purchase_data[i].id_list[j][0], purchase_data[i].id_list[j][1]);
            }

            printf("\n");
        }
    }

    fflush(stdin);
    printf("아무 키나 누르면 이전 메뉴로 돌아갑니다...\n");
    getchar();
}

int compare_desc(const void *a, const void *b) {
    return ((item_sold *)b)->total_quantity_sold - ((item_sold *)a)->total_quantity_sold;
}

void week_analyze(item *item_data, purchase_list *purchase_data, int num_purchases) {
    system("clear");
    time_t timer; // time_t type 변수
    struct tm *t; // time 구조체 인스턴스

    // Get current time
    time(&timer);
    t = localtime(&timer);

    int current_day = t->tm_mday; // Current day
    int current_month = t->tm_mon + 1; // Current month
    int current_year = t->tm_year + 1900; // Current year

    // Create an array to store the quantity sold and total sales amount for each item
    item_sold items_sold[MAX_ITEMS] = {0};
    item_sales_amount items_sales_amount[MAX_ITEMS] = {0};

    // Iterate over purchases
    int i;
    for (i = 0; i < num_purchases; i++) {
        int purchase_day = purchase_data[i].day;
        int purchase_month = purchase_data[i].month;
        int purchase_year = purchase_data[i].year;

        // Check if the purchase is within the last 7 days
        if (current_year == purchase_year) {
            if (current_month == purchase_month) {
                if (current_day - purchase_day <= 7 && current_day - purchase_day >= 0) {
                    // Iterate over items in the purchase
                    int j;
                    for (j = 0; j < MAX_ITEMS && purchase_data[i].id_list[j][0] != 0; j++) {
                        int item_id = purchase_data[i].id_list[j][0];
                        int quantity = purchase_data[i].id_list[j][1];

                        // Update the total quantity sold for the item
                        items_sold[item_id - 1].item_id = item_id;
                        items_sold[item_id - 1].total_quantity_sold += quantity;

                        // Update the total sales amount for the item
                        items_sales_amount[item_id - 1].item_id = item_id;
                        items_sales_amount[item_id - 1].total_sales_amount +=
                            quantity * item_data[item_id - 1].price * (100 - item_data[item_id - 1].dc_percent) / 100;
                    }
                }
            }
        }
    }

    // Sort the items based on quantity sold in descending order
    qsort(items_sold, MAX_ITEMS, sizeof(item_sold), compare_desc);

    // Print the total quantity sold for each item
    printf("Recent Sales for Each Item (판매량 내림차순):\n");
    printf("===========================================\n");
    for (i = 0; i < MAX_ITEMS; i++) {
        if (items_sold[i].item_id != 0) {
            printf("Item ID: %d, Total Quantity Sold: %d\n", items_sold[i].item_id, items_sold[i].total_quantity_sold);
        }
    }

    // Sort the items based on total sales amount in descending order
    qsort(items_sales_amount, MAX_ITEMS, sizeof(item_sold), compare_desc);

    // Print the total sales amount for each item
    printf("Recent Sales for Each Item (판매 금액 내림차순):\n");
    printf("===========================================\n");
    for (i = 0; i < MAX_ITEMS; i++) {
        if (items_sales_amount[i].item_id != 0) {
            printf("Item ID: %d, Total Sales Amount: %d\n", items_sales_amount[i].item_id,
                   items_sales_amount[i].total_sales_amount);
        }
    }
    printf("===========================================\n");
    fflush(stdin);
    printf("아무 키나 누르면 이전 메뉴로 돌아갑니다...\n");
    getchar();
}

void month_analyze(item *item_data, purchase_list *purchase_data, int num_purchases) {
    system("clear");
    time_t timer; // time_t type 변수
    struct tm *t; // time 구조체 인스턴스

    // Get current time
    time(&timer);
    t = localtime(&timer);

    int current_month = t->tm_mon + 1; // Current month
    int current_year = t->tm_year + 1900; // Current year

    // Create an array to store the quantity sold and total sales amount for each item
    item_sold items_sold[MAX_ITEMS] = {0};
    item_sales_amount items_sales_amount[MAX_ITEMS] = {0};

    // Iterate over purchases
    int i;
    for (i = 0; i < num_purchases; i++) {
        int purchase_month = purchase_data[i].month;
        int purchase_year = purchase_data[i].year;

        // Check if the purchase is within the last 30 days (previous month)
        if (current_year == purchase_year) {
            if ((current_month - purchase_month == 1 && current_month != 1) ||
                (current_month == 1 && purchase_month == 12)) {
                // Iterate over items in the purchase
                int j;
                for (j = 0; j < MAX_ITEMS && purchase_data[i].id_list[j][0] != 0; j++) {
                    int item_id = purchase_data[i].id_list[j][0];
                    int quantity = purchase_data[i].id_list[j][1];

                    // Update the total quantity sold for the item
                    items_sold[item_id - 1].item_id = item_id;
                    items_sold[item_id - 1].total_quantity_sold += quantity;

                    // Update the total sales amount for the item
                    items_sales_amount[item_id - 1].item_id = item_id;
                    items_sales_amount[item_id - 1].total_sales_amount +=
                        quantity * item_data[item_id - 1].price * (100 - item_data[item_id - 1].dc_percent) / 100;
                }
            }
        }
    }

    // Sort the items based on quantity sold in descending order
    qsort(items_sold, MAX_ITEMS, sizeof(item_sold), compare_desc);

    // Print the total quantity sold for each item
    printf("Recent Sales for Each Item (판매량 내림차순):\n");
    printf("===========================================\n");
    for (i = 0; i < MAX_ITEMS; i++) {
        if (items_sold[i].item_id != 0) {
            printf("Item ID: %d, Total Quantity Sold: %d\n", items_sold[i].item_id, items_sold[i].total_quantity_sold);
        }
    }

    // Sort the items based on total sales amount in descending order
    qsort(items_sales_amount, MAX_ITEMS, sizeof(item_sales_amount), compare_desc);

    // Print the total sales amount for each item
    printf("Recent Sales for Each Item (판매 금액 내림차순):\n");
    printf("===========================================\n");
    for (i = 0; i < MAX_ITEMS; i++) {
        if (items_sales_amount[i].item_id != 0) {
            printf("Item ID: %d, Total Sales Amount: %d\n", items_sales_amount[i].item_id,
                   items_sales_amount[i].total_sales_amount);
        }
    }
    printf("===========================================\n");
    fflush(stdin);
    printf("아무 키나 누르면 이전 메뉴로 돌아갑니다...\n");
    getchar();
}

void year_analyze(item *item_data, purchase_list *purchase_data, int num_purchases) {
    system("clear");
    time_t timer; // time_t type 변수
    struct tm *t; // time 구조체 인스턴스

    // Get current time
    time(&timer);
    t = localtime(&timer);

    int current_month = t->tm_mon + 1; // Current month
    int current_year = t->tm_year + 1900; // Current year

    // Create an array to store the quantity sold and total sales amount for each item
    item_sold items_sold[MAX_ITEMS] = {0};
    item_sales_amount items_sales_amount[MAX_ITEMS] = {0};

    // Iterate over purchases
    int i;
    for (i = 0; i < num_purchases; i++) {
        int purchase_month = purchase_data[i].month;
        int purchase_year = purchase_data[i].year;

        // Check if the purchase is within the current year
        if (current_year == purchase_year) {
            // Iterate over items in the purchase
            int j;
            for (j = 0; j < MAX_ITEMS && purchase_data[i].id_list[j][0] != 0; j++) {
                int item_id = purchase_data[i].id_list[j][0];
                int quantity = purchase_data[i].id_list[j][1];

                // Update the total quantity sold for the item
                items_sold[item_id - 1].item_id = item_id;
                items_sold[item_id - 1].total_quantity_sold += quantity;

                // Update the total sales amount for the item
                items_sales_amount[item_id - 1].item_id = item_id;
                items_sales_amount[item_id - 1].total_sales_amount +=
                    quantity * item_data[item_id - 1].price * (100 - item_data[item_id - 1].dc_percent) / 100;
            }
        }
    }

    // Sort the items based on quantity sold in descending order
    qsort(items_sold, MAX_ITEMS, sizeof(item_sold), compare_desc);

    // Print the total quantity sold for each item
    printf("Recent Sales for Each Item (판매량 내림차순):\n");
    printf("===========================================\n");
    for (i = 0; i < MAX_ITEMS; i++) {
        if (items_sold[i].item_id != 0) {
            printf("Item ID: %d, Total Quantity Sold: %d\n", items_sold[i].item_id, items_sold[i].total_quantity_sold);
        }
    }

    // Sort the items based on total sales amount in descending order
    qsort(items_sales_amount, MAX_ITEMS, sizeof(item_sales_amount), compare_desc);

    // Print the total sales amount for each item
    printf("Recent Sales for Each Item (판매 금액 내림차순):\n");
    printf("===========================================\n");
    for (i = 0; i < MAX_ITEMS; i++) {
        if (items_sales_amount[i].item_id != 0) {
            printf("Item ID: %d, Total Sales Amount: %d\n", items_sales_amount[i].item_id,
                   items_sales_amount[i].total_sales_amount);
        }
    }
    printf("===========================================\n");
    fflush(stdin);
    printf("아무 키나 누르면 이전 메뉴로 돌아갑니다...\n");
    getchar();
}

typedef struct {
    int hour;
    int purchases;
} HourData;

void time_analyze(purchase_list *purchase_data, int num_purchases) {
    // Create an array to store the number of purchases for each hour
    int purchases_per_hour[24] = {0};

    // Iterate over purchases
    for (int i = 0; i < num_purchases; i++) {
        int purchase_hour = purchase_data[i].hour;

        // Increment the count for the corresponding hour
        purchases_per_hour[purchase_hour]++;
    }

    // Create an array of HourData to store both hour and purchases
    HourData hours_data[24];
    for (int i = 0; i < 24; i++) {
        hours_data[i].hour = i;
        hours_data[i].purchases = purchases_per_hour[i];
    }

    // Sort the hours_data based on the number of purchases in descending order
    for (int i = 0; i < 24; i++) {
        for (int j = i + 1; j < 24; j++) {
            if (hours_data[i].purchases < hours_data[j].purchases) {
                // Swap values if needed
                HourData temp = hours_data[i];
                hours_data[i] = hours_data[j];
                hours_data[j] = temp;
            }
        }
    }

    // Print the results
    printf("Busiest Hours of the Day (내림차순):\n");
    printf("===========================================\n");
    for (int i = 0; i < 24; i++) {
        printf("Hour: %02d:00 - %02d:59, Number of Purchases: %d\n", hours_data[i].hour, 
               hours_data[i].hour, hours_data[i].purchases);
    }
    printf("===========================================\n");
    fflush(stdin);
    printf("아무 키나 누르면 이전 메뉴로 돌아갑니다...\n");
    getchar();
}

void analyze_menu(item* itemdata, int item_row, purchase_list *purchase_data, int purchase_row) {
    while (1) {
        int menu; // 메뉴 번호를 입력받아 저장하는 변수
        system("clear");
        printf("경영주님, 환영합니다!\n");
        printf("---------------------------------------------\n");
        printf("1. 최근 일주일 분석\n");
        printf("2. 월간 분석\n");
        printf("3. 연간 분석\n");
        printf("4. 시간대별 분석\n");
        printf("5. 구매내역 확인하기(일자 기준)\n");
        printf("6. 구매내역 확인하기(영수증 번호 기준)\n");
        printf("0. 이전 메뉴로 돌아가기\n");
        printf("원하는 기능을 선택하여 입력하세요: ");
        scanf("%d", &menu);
        switch (menu) {
            case 0:
                return;
            case 1:
                week_analyze(itemdata, purchase_data, purchase_row);
                break;
            case 2:
                month_analyze(itemdata, purchase_data, purchase_row);
                break;
            case 3:
                year_analyze(itemdata, purchase_data, purchase_row);
                break;
            case 4:
                time_analyze(purchase_data, purchase_row);
                break;
            case 5:
                print_purchase_data(purchase_data, purchase_row);
                break;
            case 6:
                print_purchase_data_by_receipt_number(purchase_data, purchase_row);
                break;
        }
    }
}

void discount_menu(item *itemdata, int rows) {
    int new_itemid;
    int idx = -1;
    int addcount;
    system("clear");
    printf("현재 할인 / 행사 중인 물품 목록\n");
    printf("-------------------------------------\n");
    int i;
    for (i = 0; i < rows; i++) {
        if (itemdata[i].event_mode != 0 || itemdata[i].dc_percent != 0) {
            printf("%s / id: %d / 정가: %d / 할인율: %d / 이벤트모드 : %d\n", itemdata[i].item_name, itemdata[i].item_id, itemdata[i].price, itemdata[i].dc_percent, itemdata[i].event_mode);
        }
    }
    while (1) {
        printf("할인 정책을 수정할 물건의 ID를 입력해주세요(-1 입력시 종료): ");
        scanf("%d", &new_itemid);
        if (new_itemid == -1) {
            break;
        }
        for (i = 0; i < rows; i++) {
            if (itemdata[i].item_id == new_itemid) {
                idx = i;
                break;
            }
        }
        if (idx == -1) {
            printf("잘못 입력하셨습니다\n");
        }
        else {
            printf("물건의 할인율 입력: ");
            scanf("%d", &itemdata[idx].dc_percent);
            printf("물건의 이벤트모드(일반:0 / 1+1-> 1 / 2 + 1-> 2) 입력: ");
            scanf("%d", &itemdata[idx].event_mode);
        }
    }
    update_item_DB2(itemdata, rows);
}
void part_timer(user user, int fd);


void admin_menu(user* userdata, int user_rows, item* itemdata, int item_rows, purchase_list *purchase_data, int purchase_row) {
    time_t timer; // time_t type 변수
    struct tm *t; // time 구조체 인스턴스
    while (1) {
        int menu; // 메뉴 번호를 입력받아 저장하는 변수

        timer = time(NULL); // 타임 변수 초기화
        t = localtime(&timer); // time 구조체로 변환
        //system("clear");
        printf("경영주님, 환영합니다!\n");
        printf("---------------------------------------------\n");
        printf("현재 시각: %d년 %d월 %d일 %d시 %d 분\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min);
        printf("---------------------------------------------\n");
        printf("1. 재고 확인 및 발주\n");
        printf("2. 근무자 정보 / 근무 시간 확인 및 수정\n");
        printf("3. 점포 경영 지표 분석\n");
        printf("4. 할인 정책 수정\n");
        printf("5. 아르바이트 모드 변경\n");
        printf("0. 경영 관리 시스템 나가기\n");
        printf("원하는 기능을 선택하여 입력하세요: ");
        user user2;
        scanf("%d", &menu);
        switch (menu) {
            case 0:
                return;
            case 1:
                item_menu(itemdata, item_rows);
                break;
            case 2:
                userdata_menu(userdata, user_rows);
                break;
            case 3:
                analyze_menu(itemdata, item_rows, purchase_data, purchase_row);
                break;
            case 4:
                discount_menu(itemdata, item_rows);
                break;
            case 5:
                
                user2.cph = userdata[0].cph;
                user2.end_time = userdata[0].end_time;
                user2.start_time = userdata[0].start_time;
                user2.grade = userdata[0].grade;
                user2.user_id = userdata[0].user_id;
                user2.password = userdata[0].password;
                int item_fd = open("data/itemDB.txt", O_RDWR);
                part_timer(user2, item_fd);
                break;

        }
    }
}

void buy_item(item * item_list, purchase_list *purchase_list);
void refund_item(item * item_list, FILE *purchase_list_fp);
void update_item_DB(item *item_list);
void search_item();
void check_purchase_list(FILE *purchase_list_fp);
void open_refrigerator(FILE *saved_item_fp);

void check_request() {
    FILE *fp = fopen("data/user_request.txt", "r");
    char buf[BUFFSIZE];
    while (fgets(buf, BUFFSIZE, fp) != NULL) {
        printf("%s", buf);
    }
    fflush(stdin);
    printf("요청 사항을 전부 출력했습니다.\n");
    printf("아무 키나 누르면 이전 메뉴로 돌아갑니다…\n");
    getchar();
}


void customer(int item_fd, int saved_item_fd, int purchase_list_fd){

    FILE *item_fp = fdopen(item_fd, "r");
    FILE *saved_item_fp = fdopen(saved_item_fd, "a+");
    FILE *purchase_list_fp = fdopen(purchase_list_fd, "a+");
    purchase_list purchase_list;
    int i;
    for (i = 0; i < 10; i++) {
        purchase_list.id_list[i][0] = -1;
        purchase_list.id_list[i][1] = -1;
    }
    system("clear");
    
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
    for (i = 0; i < numRows; i++) {
        itemDB_name_list[i] = (char*)malloc(10 * sizeof(char));
    }
    itemDB = (int**)malloc(numRows * sizeof(int*));
    for (i = 0; i < numRows; i++) {
        itemDB[i] = (int*)malloc(numCols * sizeof(int));
    }
    for (i = 0; i < numRows; i++) {
        fscanf(item_fp, "%s %d %d %d %d %d %d %d", itemDB_name_list[i], &itemDB[i][1], &itemDB[i][2], &itemDB[i][3], &itemDB[i][4], &itemDB[i][5], &itemDB[i][6], &itemDB[i][7]);
    }

    item item_list[10];
    for (i = 0; i < 10; i++) {
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
            system("clear");
            buy_item(item_list, &purchase_list);
            update_item_DB(item_list);
            longjmp(env, 1);
            break;
        case 2:
            system("clear");
            refund_item(item_list, purchase_list_fp);
            update_item_DB(item_list);
            longjmp(env, 1);
            break;
        case 3:
            system("clear");
            search_item();
            longjmp(env, 1);
            break;
        case 4:
            system("clear");
            check_purchase_list(purchase_list_fp);
            longjmp(env, 1);
            break;
        case 5:
            system("clear");
            open_refrigerator(saved_item_fp);
            longjmp(env, 1);
            break;
        case 0:
            write(STDOUT_FILENO, "프로그램을 종료합니다.\n", 34);
            if (purchase_list.id_list[0][0] != -1){
                fprintf(purchase_list_fp, "%d %d %d %d %d %d\n", purchase_list.year, purchase_list.month, purchase_list.hour, purchase_list.hour, purchase_list.min, purchase_list.purchase_num);
                for (i = 0; i < 10; i++) {
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
    int i;
    for (i = 0; i < 10; i++) {
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
            for(i = 0; i < 10; i++){
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
                        int i;
                        for (i = 0; i < 10; i++) {
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
    int i;
    for (i = 0; i < 10; i++) {
        if(item_list[i].item_count != 0){
        fprintf(item_fp, "%s %d %d %d %d %d %d %d\n", item_list[i].item_name, item_list[i].item_id, item_list[i].item_count, item_list[i].price, item_list[i].dc_percent, item_list[i].event_mode, item_list[i].sale_count, item_list[i].margin_percent);
    }
    }
    fclose(item_fp);
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
    fflush(stdin);
    printf("아무 키나 누르면 이전 메뉴로 돌아갑니다...\n");
    getchar();

}
void open_refrigerator(FILE *saved_item_fp){
    int item_id, item_count;
    while(fscanf(saved_item_fp, "%d %d", &item_id, &item_count) > 0){
        fprintf(stdout, "물건 ID : %d /물건 개수 : %d\n", item_id, item_count);
    }
    fflush(stdin);
    printf("아무 키나 누르면 이전 메뉴로 돌아갑니다...\n");
    getchar();
}
jmp_buf env;

user login( FILE *fp) {

    char cmd_buf[BUFFSIZE];
    int** userDB;
    int numRows = 20;
    int numCols = 6;

    userDB = (int**)malloc(numRows * sizeof(int*));
    int i;
    for (i = 0; i < numRows; i++) {
        userDB[i] = (int*)malloc(numCols * sizeof(int));
    }


    for (i = 0; i < numRows; i++) {
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
    

    for (i = 0; i < numRows; i++) {
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

void check_stock(int fp);
void change_work_time(user user);
void print_salary(user user);

void part_timer(user user, int item_fp) {
    jmp_buf env, env2;
    if (setjmp(env2) != 0) {
    }
    system("clear");
    write(STDOUT_FILENO, "알바생 모드입니다.\n", 28);

    if (setjmp(env) != 0) {
        write(STDOUT_FILENO, "잘못된 입력입니다. 다시 시도해주세요.\n", 55);
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
            return;
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
    fflush(stdin);
    printf("아무 키나 누르면 이전 메뉴로 돌아갑니다...\n");
    getchar();
    
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

    int fd = open("data/user_request.txt",  O_CREAT | O_RDWR , 0644);

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
    
    write(fd, message, 111);

    write(STDOUT_FILENO, "근무시간 조정 요청이 완료되었습니다.\n", 54);
    fflush(stdin);
    printf("아무 키나 누르면 이전 메뉴로 돌아갑니다...\n");
    getchar();
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

    fflush(stdin);
    printf("아무 키나 누르면 이전 메뉴로 돌아갑니다...\n");
    getchar();
}



int main(){
    FILE *user_fp = fopen("data/userDB.txt", "r");

    write(STDOUT_FILENO, "1. 관리자 로그인\n2. 고객 로그인\n", 44);
    char cmd_buf[BUFFSIZE];
    int choice;
    if ((read(STDIN_FILENO, cmd_buf, BUFFSIZE)) < 0)
        write(STDOUT_FILENO, "read error\n", 12);

    choice = atoi(cmd_buf);
    system("clear");

    if (choice == 1){
        user user1 = login(user_fp); 
        if (user1.grade == 0) {
            if (user_fp == NULL) {
                fprintf(stderr, "file open error\n");
            }
            user *userdata;
            item *itemdata;
            purchase_list *purchase_data;
            int item_fd = open("data/itemDB.txt", O_RDWR);
            int purchase_list_fd = open("data/purchase_list_DB.txt", O_RDWR);
            int saved_item_fd = open("data/saved_item_DB.txt", O_RDWR);
            int user_rows = initUser(&userdata, user_fp);
            int item_rows = initItem(&itemdata, item_fd);
            int purchase_row = init_purchase_list(&purchase_data, purchase_list_fd);
            admin_menu(userdata, user_rows, itemdata, item_rows, purchase_data, purchase_row);
        }
        else if (user1.grade == 1){
            int item_fd = open ("data/itemDB.txt", O_RDWR);
            part_timer(user1, item_fd);
        }
        else {
            
        }
        fclose(user_fp);
    }
    else{
        int item_fd = open("data/itemDB.txt", O_RDWR);
        int saved_item_fd = open("data/saved_item_DB.txt", O_RDWR);
        int purchase_list_fd = open("data/purchase_list_DB.txt", O_RDWR);
        customer(item_fd, saved_item_fd, purchase_list_fd);
    }
}