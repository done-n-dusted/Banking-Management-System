#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8000

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

void run_client();

void choose_account(int sok);

void normal_login(int sok);
void joint_login(int sok);
void admin_login(int sok);

void list_options(int sok);

void deposit(int sok);
void withdraw(int sok);
void balance_enquiry(int sok);
void change_pwd(int sok);
void view_deets(int sok);

void add_acc(int sok);
void rem_acc(int sok);
void mod_acc(int sok);
void view_deets_admin(int sok);

int acc, curr_accno;


int main(int argc, char const *argv[]) {

    run_client();

    return 0;
}

void run_client(){
    struct sockaddr_in server;
    int sok;

    sok = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(PORT);

    connect(sok, (struct sockaddr *) &server, sizeof(server));
    choose_account(sok);
    list_options(sok);
    close(sok);
}

void choose_account(int sok){
    printf("Choose one of the following:\n");
    printf("1 Normal User\n2 Joint User\n3 Admin\n");
    scanf("%d", &acc);

    switch (acc) {
        case 1: normal_login(sok); break;
        case 2: joint_login(sok); break;
        case 3: admin_login(sok); break;
        default: printf("Invalid. Try again\n"); choose_account(sok); break;
    }
}

void normal_login(int sok){
    bool result;
    normal current;
    printf("Account Number: ");
    scanf("%d", &current.acc_no);
    curr_accno = current.acc_no;
    printf("Password: ");
    scanf("%s", current.pwd);

    write(sok, &acc, sizeof(int));
    write(sok, &current, sizeof(normal));

    read(sok, &result, sizeof(result));

    if(result){
        printf("successful login\n\n");
    }

    else{
        printf("Invalid\n\n");
        choose_account(sok);
    }
}

void joint_login(int sok){
    bool result;
    joint current;
    printf("Account Number: ");
    scanf("%d", &current.acc_no);
    curr_accno = current.acc_no;
    printf("Password: ");
    scanf("%s", current.pwd);

    write(sok, &acc, sizeof(int));
    write(sok, &current, sizeof(normal));

    read(sok, &result, sizeof(result));

    if(result){
        printf("successful login\n\n");
    }

    else{
        printf("Invalid\n\n");
        choose_account(sok);
    }
}

void admin_login(int sok){
    bool result;
    char temp_pwd[10];
    printf("Password : ");
    scanf("%s", temp_pwd);

    write(sok, &acc, sizeof(int));
    write(sok, &temp_pwd, sizeof(temp_pwd));

    read(sok, &result, sizeof(result));

    if(result){
        printf("Admin successful login\n\n");
    }

    else{
        printf("Invalid\n\n");
        choose_account(sok);
    }
}

void list_options(int sok){
    int opt;

    if(acc == 1 || acc == 2){
        printf("Choose one of the following:\n");
        printf("1 Deposit\n2 Withdraw\n3 Balance Enquiry\n4 Password Change\n5 Details\n6 Quit\n");
        scanf("%d", &opt);

        switch (opt) {
            case 1: deposit(sok); break;
            case 2: withdraw(sok); break;
            case 3: balance_enquiry(sok); break;
            case 4: change_pwd(sok); break;
            case 5: view_deets(sok); break;
            case 6: write(sok, &opt, sizeof(int)); exit(0);
            default: printf("Invalid Option. Try Again.\n"); list_options(sok); break;
        }
    }

    if(acc == 3){
        printf("Admin chosen\n Choose one of the following:\n");
        printf("1 Add Account\n2 Delete Account\n3 Modify Account\n4 Search Account\n5 Quit\n");
        scanf("%d", &opt);

        switch (opt) {
            case 1: add_acc(sok); break;
            case 2: rem_acc(sok); break;
            case 3: mod_acc(sok); break;
            case 4: view_deets_admin(sok); break;
            case 5: write(sok, &opt, sizeof(int)); exit(0);
            default: printf("Invalid Option. Try Again.\n"); list_options(sok); break;

        }
    }
}

void deposit(int sok){
    float amount;
    int opt = 1;
    bool result;

    printf("Amount to deposit :");
    scanf("%f", &amount);

    write(sok, &opt, sizeof(int));
    write(sok, &amount, sizeof(float));

    read(sok, &result, sizeof(result));

    if(result){
        printf("Deposited\n\n");
    }

    else{
        printf("Error. Try Again.\n\n");
    }
    list_options(sok);
}

void withdraw(int sok){
    float amount;
    int opt = 2;
    bool result;

    printf("Amount to Withdraw :");
    scanf("%f", &amount);

    write(sok, &opt, sizeof(int));
    write(sok, &amount, sizeof(float));

    read(sok, &result, sizeof(result));

    if(result){
        printf("Withdrawn\n");
    }

    else{
        printf("Error. Try Again.\n");
    }
    list_options(sok);
}

void balance_enquiry(int sok){
    float amount;
    int opt = 3;

    write(sok, &opt, sizeof(int));

    read(sok, &amount, sizeof(amount));

    printf("Balance : %f\n", amount);

    list_options(sok);
}

void change_pwd(int sok){
    int opt = 4;
    char temp_pwd[10];
    bool result;

    printf("Enter new password:");
    scanf("%s", temp_pwd);

    write(sok, &opt, sizeof(int));
    write(sok, temp_pwd, sizeof(temp_pwd));

    read(sok, &result, sizeof(result));

    if(result){
        printf("Password Changed.\n\n");
    }

    else{
        printf("Error. Try Again.\n\n");
    }
    list_options(sok);
}

void view_deets(int sok){
    int opt = 5;

    write(sok, &opt, sizeof(int));

    if(acc == 1){
        normal current_n;
        read(sok, &current_n, sizeof(normal));

        printf("Acc No: %d\n", current_n.acc_no);
        printf("Name: %s\n", current_n.name);
        printf("Balance: %f\n", current_n.balance);

    }

    else if(acc == 2){
        joint current_j;

        read(sok, &current_j, sizeof(joint));

        printf("Acc No: %d\n", current_j.acc_no);
        printf("Name1: %s\n", current_j.name1);
        printf("Name2: %s\n", current_j.name2);
        printf("Balance: %f\n", current_j.balance);

    }

    list_options(sok);
}

void add_acc(int sok){
    int opt = 1;
    int acc_type;
    bool result;

    write(sok, &opt, sizeof(int));

    printf("Enter Account Type\n 1 Normal Account\n 2 Joint Account\n");
    scanf("%d", &acc_type);

    write(sok, &acc_type, sizeof(int));

    if(acc_type == 1){
        normal new_n;
        printf("Name : ");
        scanf("%s", new_n.name);
        printf("Password : ");
        scanf("%s", new_n.pwd);
        new_n.balance = 0.0;
        write(sok, &new_n, sizeof(normal));
    }

    else if(acc_type == 2){
        joint new_j;
        printf("Name1 : ");
        scanf("%s", new_j.name1);
        printf("Name2 : ");
        scanf("%s", new_j.name2);
        printf("Password : ");
        scanf("%s", new_j.pwd);
        new_j.balance = 0.0;
        write(sok, &new_j, sizeof(joint));
    }

    read(sok, &result, sizeof(result));

    if(result){
        printf("Account Created.\n\n");
    }
    else{
        printf("Error. Try Again\n\n");
    }
    list_options(sok);
}

void rem_acc(int sok){
    int opt = 2;
    int acc_type, acc_no;
    bool result;

    write(sok, &opt, sizeof(int));

    printf("Enter Account Type\n 1 Normal Account\n 2 Joint Account\n");
    scanf("%d", &acc_type);

    write(sok, &acc_type, sizeof(int));

    printf("Account Number : ");
    scanf("%d", &acc_no);
    write(sok, &acc_no, sizeof(int));

    read(sok, &result, sizeof(result));

    if(result){
        printf("Account added Succesfully\n\n");
    }

    else{
        printf("Error. Try Again\n\n");
    }
    list_options(sok);
}

void mod_acc(int sok){
    int opt = 3;
    int acc_type;
    bool result;

    write(sok, &opt, sizeof(int));

    printf("Enter Account Type\n 1 Normal Account\n 2 Joint Account\n");
    scanf("%d", &acc_type);

    write(sok, &acc_type, sizeof(int));

    if(acc_type == 1){
        normal mod_n;
        printf("Acc No: ");scanf("%d", &mod_n.acc_no);
        printf("Name: "); scanf("%s", mod_n.name);
        printf("Password: "); scanf("%s", mod_n.pwd);
        printf("Balance: "); scanf("%f", &mod_n.balance);

        write(sok, &mod_n, sizeof(normal));
    }


    else if(acc_type == 2){
        joint mod_j;
        printf("Acc No: ");scanf("%d", &mod_j.acc_no);
        printf("Name1: "); scanf("%s", mod_j.name1);
        printf("Name2: "); scanf("%s", mod_j.name2);
        printf("Password: "); scanf("%s", mod_j.pwd);
        printf("Balance: "); scanf("%f", &mod_j.balance);

        write(sok, &mod_j, sizeof(joint));
    }

    read(sok, &result, sizeof(result));

    if(result){
        printf("Account Modified.\n\n");
    }

    else{
        printf("Error. Try Again\n\n");
    }

    list_options(sok);
}

void view_deets_admin(int sok){
    int opt = 4;
    int acc_type, flag;
    bool result;

    write(sok, &opt, sizeof(int));

    printf("Enter Account Type\n 1 Normal Account\n 2 Joint Account\n");
    scanf("%d", &acc_type);

    write(sok, &acc_type, sizeof(int));

    if(acc_type == 1){
        normal temp_n;
        int acc_search;
        printf("Account Number: "); scanf("%d", &acc_search);
        write(sok, &acc_search, sizeof(int));

        flag = read(sok, &temp_n, sizeof(normal));

        if(!flag){
            printf("No account found.\n");
        }

        else{
            printf("Acc No: %d\n", temp_n.acc_no);
            printf("Name: %s\n", temp_n.name);
            printf("Balance: %f\n", temp_n.balance);
        }
    }

    else if(acc_type == 2){
        joint temp_j;
        int acc_search;
        printf("Account Number: "); scanf("%d", &acc_search);
        write(sok, &acc_search, sizeof(int));

        flag = read(sok, &temp_j, sizeof(normal));

        if(!flag){
            printf("No account found.\n");
        }

        else{
            printf("Acc No: %d\n", temp_j.acc_no);
            printf("Name1: %s\n", temp_j.name1);
            printf("Name2: %s\n", temp_j.name2);
            printf("Balance: %f\n", temp_j.balance);
        }
    }

    list_options(sok);
}
