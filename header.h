#include <stdio.h>
#include <time.h>

typedef struct _user {
    int grade; // 0번이면 사장, 1번이면 알바
    int user_id; // 사용자 id --> 문자열로 바꾸는게 더 나을 듯???
    int password; // 사용자 password --> 문자열로 바꾸는게 더 나을 듯???
    int cph; // cost per hour
    int start_time; // 근무 시작 시각
    int end_time; // 근무 종료 시각
} user;

typedef struct _item {
    char item_name[40]; // 물건의 이름
    int item_id; // 물건 id(바코드)
    int item_count; // 물건의 양
    int price; // 물건의 가격(정가)
    int dc_percent; // 물건의 할인율
    int event_mode; // 0번 : 일반 , 1번 : 1 + 1, 2번 : 2 + 1
    int sale_count; // 물건의 판매량
    int margin_percent; // 물건의 마진율(int -> float)
    int margin; // 물건의 마진
} item;

typedef struct _purchase_list {
    int year;
    int month;
    int day;
    int hour;
    int min;
    int purchase_num; // 구매 일련번호(= 영수증 번호)
    int id_list[100][2]; // 구매 물건 리스트(item ID, item Count) (동적할당)
} purchase_list;

typedef struct _saved_item {
    int **item_list; // 저장된 물건 리스트(item id, item count) (동적할당)
} saved_item;


// admin function
void admin_menu(int item_fd, int purchase_list_fd, user* userdata, int user_rows, item* itemdata, int item_rows);
void item_menu(item *itemdata, int rows);
void userdata_menu(user* userdata, int user_rows);

// userDB function
int initUser(user **userdata,  FILE *user_fp);
void printUser(user *userdat, int rows);
void modUser(user *userdata, int rows);
void saveUser(user *userdata, int rows);
int addUser(user **userdata, int rows);
int delUser(user **userdata, int rows);

int initItem(item **itemdata, int item_fd);
int addItem(item **itemdata, int rows);
int modItem(item *itemdata, int rows);
void update_item_DB2(item *item_list, int rows);

void analyze_menu(int item_fd, int purchase_list_fd);
void discount_menu(item *itemdata, int rows);


// customer function
void customer(int item_fd, int saved_item_fd, int purchase_list_fd);
void buy_item(item * item_list);
void refund_item(item * item_list, FILE *purchase_list_fp);
void update_item_DB(item *item_list);
void search_item(); // common function
void check_purchase_list(FILE *purchase_list_fp);
void open_refrigerator();

// login function
user login( FILE *fp);