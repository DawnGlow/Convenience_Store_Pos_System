#include <stdio.h>
#include <time.h>

typedef struct _user {
    int grade; // 0번이면 사장, 1번이면 알바
    int user_id; // 사용자 id
    int password; // 사용자 password
    int cph; // cost per hour
    int start_time; // 근무 시작 시각
    int end_time; // 근무 종료 시각
} user;

typedef struct _item {
    char *item_name; // 물건의 이름
    int item_id; // 물건 id(바코드)
    int item_count; // 물건의 양
    int price; // 물건의 가격(정가)
    int dc_percent; // 물건의 할인율
    int event_mode; // 0번 : 일반 , 1번 : 1 + 1, 2번 : 2 + 1
    int sale_count; // 물건의 판매량
    int margin_percent; // 물건의 마진율
    int margin; // 물건의 마진
} item;

typedef struct _purchase_list {
    int year;
    int month;
    int day;
    int hour;
    int min;
    int purchase_num; // 구매 일련번호(= 영수증 번호)
    int **item_list; // 구매 물건 리스트(item ID, item Count) (동적할당)
} purchase_list;

typedef struct _saved_item {
    int **item_list; // 저장된 물건 리스트(item id, item count) (동적할당)
} saved_item;