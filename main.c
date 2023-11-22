#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"
#include "function.c"

int main(void) {
    user *user_list;
    user_load(user_list);
}