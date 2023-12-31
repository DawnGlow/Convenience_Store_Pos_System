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
    int ID, idx, menu, temp;
    system("clear");
    printf("사용자 정보를 변경할 사용자의 ID를 입력해주세요: ");
    scanf("%d", &ID);
    for (int i = 0; i < rows; i++) {
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

    for (int i = 0; i < rows; i++) {
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
    for (int i = 0; i < rows - item_size; i++) {
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
    for (int i = rows - item_size; i < rows; i++) {
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
    for (int i = 0; i < item_rows; i++) {
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
    for (int i = 0; i < rows; i++) {
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

    for (int i = 0; i < rows; i++) {
        fgets(buf, BUFFSIZE, purchase_fp);
        sscanf(buf, "%d %d %d %d %d %d", &(*purchase_data)[i].year, &(*purchase_data)[i].month, &(*purchase_data)[i].day, &(*purchase_data)[i].hour, &(*purchase_data)[i].min, &(*purchase_data)[i].purchase_num);
        for (int j = 0; j < 100; j++) {
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

    for (int i = 0; i < num_purchases; i++) {
        // 해당 날짜와 일치하는 영수증만 출력
        if (purchase_data[i].year == year && purchase_data[i].month == month && purchase_data[i].day == day) {
            printf("Purchase %d:\n", purchase_data[i].purchase_num);
            printf("Date: %04d-%02d-%02d %02d:%02d\n", purchase_data[i].year, purchase_data[i].month, purchase_data[i].day, purchase_data[i].hour, purchase_data[i].min);

            printf("Items:\n");
            for (int j = 0; j < 100; j++) {
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

    for (int i = 0; i < num_purchases; i++) {
        // 해당 영수증 번호와 일치하는 영수증만 출력
        if (purchase_data[i].purchase_num == receipt_number) {
            printf("Purchase %d:\n", purchase_data[i].purchase_num);
            printf("Date: %04d-%02d-%02d %02d:%02d\n", purchase_data[i].year, purchase_data[i].month, purchase_data[i].day, purchase_data[i].hour, purchase_data[i].min);

            printf("Items:\n");
            for (int j = 0; j < 100; j++) {
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
    for (int i = 0; i < num_purchases; i++) {
        int purchase_day = purchase_data[i].day;
        int purchase_month = purchase_data[i].month;
        int purchase_year = purchase_data[i].year;

        // Check if the purchase is within the last 7 days
        if (current_year == purchase_year) {
            if (current_month == purchase_month) {
                if (current_day - purchase_day <= 7 && current_day - purchase_day >= 0) {
                    // Iterate over items in the purchase
                    for (int j = 0; j < MAX_ITEMS && purchase_data[i].id_list[j][0] != 0; j++) {
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
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items_sold[i].item_id != 0) {
            printf("Item ID: %d, Total Quantity Sold: %d\n", items_sold[i].item_id, items_sold[i].total_quantity_sold);
        }
    }

    // Sort the items based on total sales amount in descending order
    qsort(items_sales_amount, MAX_ITEMS, sizeof(item_sold), compare_desc);

    // Print the total sales amount for each item
    printf("Recent Sales for Each Item (판매 금액 내림차순):\n");
    printf("===========================================\n");
    for (int i = 0; i < MAX_ITEMS; i++) {
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
    for (int i = 0; i < num_purchases; i++) {
        int purchase_month = purchase_data[i].month;
        int purchase_year = purchase_data[i].year;

        // Check if the purchase is within the last 30 days (previous month)
        if (current_year == purchase_year) {
            if ((current_month - purchase_month == 1 && current_month != 1) ||
                (current_month == 1 && purchase_month == 12)) {
                // Iterate over items in the purchase
                for (int j = 0; j < MAX_ITEMS && purchase_data[i].id_list[j][0] != 0; j++) {
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
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items_sold[i].item_id != 0) {
            printf("Item ID: %d, Total Quantity Sold: %d\n", items_sold[i].item_id, items_sold[i].total_quantity_sold);
        }
    }

    // Sort the items based on total sales amount in descending order
    qsort(items_sales_amount, MAX_ITEMS, sizeof(item_sales_amount), compare_desc);

    // Print the total sales amount for each item
    printf("Recent Sales for Each Item (판매 금액 내림차순):\n");
    printf("===========================================\n");
    for (int i = 0; i < MAX_ITEMS; i++) {
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
    for (int i = 0; i < num_purchases; i++) {
        int purchase_month = purchase_data[i].month;
        int purchase_year = purchase_data[i].year;

        // Check if the purchase is within the current year
        if (current_year == purchase_year) {
            // Iterate over items in the purchase
            for (int j = 0; j < MAX_ITEMS && purchase_data[i].id_list[j][0] != 0; j++) {
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
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (items_sold[i].item_id != 0) {
            printf("Item ID: %d, Total Quantity Sold: %d\n", items_sold[i].item_id, items_sold[i].total_quantity_sold);
        }
    }

    // Sort the items based on total sales amount in descending order
    qsort(items_sales_amount, MAX_ITEMS, sizeof(item_sales_amount), compare_desc);

    // Print the total sales amount for each item
    printf("Recent Sales for Each Item (판매 금액 내림차순):\n");
    printf("===========================================\n");
    for (int i = 0; i < MAX_ITEMS; i++) {
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

void time_analyze(purchase_list *purchase_data, int num_purchases) {
    // Create an array to store the number of purchases for each hour
    int purchases_per_hour[24] = {0};

    // Iterate over purchases
    for (int i = 0; i < num_purchases; i++) {
        int purchase_hour = purchase_data[i].hour;

        // Increment the count for the corresponding hour
        purchases_per_hour[purchase_hour]++;
    }

    // Sort the hours based on the number of purchases in descending order
    for (int i = 0; i < 24; i++) {
        for (int j = i + 1; j < 24; j++) {
            if (purchases_per_hour[i] < purchases_per_hour[j]) {
                // Swap values if needed
                int temp_count = purchases_per_hour[i];
                purchases_per_hour[i] = purchases_per_hour[j];
                purchases_per_hour[j] = temp_count;
            }
        }
    }

    // Print the results
    printf("Busiest Hours of the Day (내림차순):\n");
    printf("===========================================\n");
    for (int i = 0; i < 24; i++) {
        printf("Hour: %02d:00 - %02d:59, Number of Purchases: %d\n", i, i, purchases_per_hour[i]);
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
    for (int i = 0; i < rows; i++) {
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
        for (int i = 0; i < rows; i++) {
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
        printf("0. 경영 관리 시스템 나가기\n");
        printf("원하는 기능을 선택하여 입력하세요: ");
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
        }
    }
}

int main(){
    FILE *user_fp = fopen("data/userDB.txt", "r");
    if (user_fp == NULL) {
        fprintf(stderr, "file open error\n");
    }
    user *userdata;
    item *itemdata;
    purchase_list *purchase_data;
    int item_fd = open("data/itemDB.txt", O_RDWR);
    int purchase_list_fd = open("data/random_purchase_data.txt", O_RDWR);
    int saved_item_fd = open("data/saved_item_DB.txt", O_RDWR);
    int user_rows = initUser(&userdata, user_fp);
    int item_rows = initItem(&itemdata, item_fd);
    int purchase_row = init_purchase_list(&purchase_data, purchase_list_fd);
    admin_menu(userdata, user_rows, itemdata, item_rows, purchase_data, purchase_row);
}