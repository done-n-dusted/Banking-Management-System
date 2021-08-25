#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

typedef struct normal{
    int acc_no;
    char name[10];
    char pwd[10];
    float balance;
    char status[6];
} normal;

typedef struct joint{
    int acc_no;
    char name1[10];
    char name2[10];
    char pwd[10];
    float balance;
    char status[6];
} joint;

int main(int argc, char const *argv[]) {
    printf("Creating Normal user\n");
    int fdn = open("./data/NU.data", O_RDWR | O_CREAT, 0744);
    normal user_n;
    user_n.acc_no = 0;
    printf("Name : "); scanf("%s", user_n.name);
    printf("Password : "); scanf("%s", user_n.pwd);
    user_n.balance = 0;
    strcpy(user_n.status, "ACTIVE");
    write(fdn, &user_n, sizeof(user_n));

    printf("Creating Joint user\n");
    int fdj = open("./data/JU.data", O_RDWR | O_CREAT, 0744);
    joint user_j;
    user_j.acc_no = 0;
    printf("Name1 : "); scanf("%s", user_j.name1);
    printf("Name2 : "); scanf("%s", user_j.name2);
    printf("Password : "); scanf("%s", user_j.pwd);
    user_j.balance = 0;
    strcpy(user_j.status, "ACTIVE");
    write(fdj, &user_j, sizeof(user_j));
    return 0;
}