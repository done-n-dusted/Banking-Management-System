#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

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

normal get_normal(int acc_no);
joint get_joint(int acc_no);

bool confirm_normal(normal user);
bool confirm_joint(joint user);
bool confirm_admin(char pwd[10]);

bool deposit(int acc_type, int acc_no, float amount);
bool withdraw(int acc_type, int acc_no, float amount);
float balance_enquiry(int acc_type, int acc_no);
bool change_pwd(int acc_type, int acc_no, char new_pwd[10]);

bool add_normal(normal user);
bool add_joint(joint user);
bool del_normal(int acc_no);
bool del_joint(int acc_no);
bool mod_normal(normal user);
bool mod_joint(joint user);

void server_process(int sok);
void run_server();

int main(int argc, char const *argv[]) {
    printf("Welcome to the server\n");
    run_server();
    return 0;
}

normal get_normal(int acc_no){
    int i = acc_no;
    normal user;
    int fd = open("./data/NU.data", O_RDONLY, 0744);

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(normal);
    lock.l_len = sizeof(normal);
    lock.l_pid = getpid();

    fcntl(fd, F_SETLKW, &lock);

    lseek(fd, (i)*sizeof(normal), SEEK_SET);
    read(fd, &user, sizeof(normal));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return user;
}

joint get_joint(int acc_no){
    int i = acc_no;
    joint user;
    int fd = open("./data/JU.data", O_RDONLY, 0744);

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(joint);
    lock.l_len = sizeof(joint);
    lock.l_pid = getpid();

    fcntl(fd, F_SETLKW, &lock);

    lseek(fd, (i)*sizeof(joint), SEEK_SET);
    read(fd, &user, sizeof(joint));

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return user;
}

bool confirm_normal(normal user){
    int i = user.acc_no;
    int fd = open("./data/NU.data", O_RDONLY, 0744);
    bool result = false;
    normal u;

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(normal);
    lock.l_len = sizeof(normal);
    lock.l_pid = getpid();

    fcntl(fd, F_SETLKW, &lock);

    lseek(fd, (i)*sizeof(normal), SEEK_SET);
    read(fd, &u, sizeof(normal));

    if(!strcmp(u.pwd, user.pwd) && !strcmp(u.status, "ACTIVE")){
        result = true;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool confirm_joint(joint user){
    int i = user.acc_no;
    int fd = open("./data/JU.data", O_RDONLY, 0744);
    bool result = false;
    joint u;

    struct flock lock;
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(joint);
    lock.l_len = sizeof(joint);
    lock.l_pid = getpid();

    fcntl(fd, F_SETLKW, &lock);

    lseek(fd, (i)*sizeof(joint), SEEK_SET);
    read(fd, &u, sizeof(joint));

    if(!strcmp(u.pwd, user.pwd) && !strcmp(u.status, "ACTIVE")){
        result = true;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool confirm_admin(char pwd[10]){
    if(!strcmp(pwd, "1234567890")){
        return true;
    }
    else{
        return false;
    }
}

bool deposit(int acc_type, int acc_no, float amount){
    int i = acc_no;
    bool result = false;
    int fd;
    if(acc_type == 1){
        fd = open("./data/NU.data", O_RDWR, 0744);
        normal user_n;

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i)*sizeof(normal);
        lock.l_len = sizeof(normal);
        lock.l_pid = getpid();

        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (i)*sizeof(normal), SEEK_SET);
        read(fd, &user_n, sizeof(normal));

        if(!strcmp(user_n.status, "ACTIVE")){
            user_n.balance = user_n.balance + amount;
            lseek(fd, sizeof(normal)*(-1), SEEK_CUR);
            write(fd, &user_n, sizeof(normal));
            result = true;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }

    else if(acc_type == 2){
        fd = open("./data/JU.data", O_RDWR, 0744);
        joint user_j;

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i)*sizeof(joint);
        lock.l_len = sizeof(joint);
        lock.l_pid = getpid();

        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (i)*sizeof(joint), SEEK_SET);
        read(fd, &user_j, sizeof(joint));

        if(!strcmp(user_j.status, "ACTIVE")){
            user_j.balance = user_j.balance + amount;
            lseek(fd, sizeof(joint)*(-1), SEEK_CUR);
            write(fd, &user_j, sizeof(joint));
            result = true;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }
    return result;
}

bool withdraw(int acc_type, int acc_no, float amount){
    int i = acc_no;
    bool result = false;
    int fd;
    if(acc_type == 1){
        fd = open("./data/NU.data", O_RDWR, 0744);
        normal user_n;

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i)*sizeof(normal);
        lock.l_len = sizeof(normal);
        lock.l_pid = getpid();

        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (i)*sizeof(normal), SEEK_SET);
        read(fd, &user_n, sizeof(normal));

        if(!strcmp(user_n.status, "ACTIVE")){
            user_n.balance = user_n.balance - amount;
            lseek(fd, sizeof(normal)*(-1), SEEK_CUR);
            write(fd, &user_n, sizeof(normal));
            result = true;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }

    else if(acc_type == 2){
        fd = open("./data/JU.data", O_RDWR, 0744);
        joint user_j;

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i)*sizeof(joint);
        lock.l_len = sizeof(joint);
        lock.l_pid = getpid();

        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (i)*sizeof(joint), SEEK_SET);
        read(fd, &user_j, sizeof(joint));

        if(!strcmp(user_j.status, "ACTIVE")){
            user_j.balance = user_j.balance - amount;
            lseek(fd, sizeof(joint)*(-1), SEEK_CUR);
            write(fd, &user_j, sizeof(joint));
            result = true;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }
    return result;
}

float balance_enquiry(int acc_type, int acc_no){
    int i = acc_no;
    float result = 0;
    int fd;
    if(acc_type == 1){
        fd = open("./data/NU.data", O_RDWR, 0744);
        normal user_n;

        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i)*sizeof(normal);
        lock.l_len = sizeof(normal);
        lock.l_pid = getpid();

        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (i)*sizeof(normal), SEEK_SET);
        read(fd, &user_n, sizeof(normal));

        if(!strcmp(user_n.status, "ACTIVE")){
            result = user_n.balance;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }

    else if(acc_type == 2){
        fd = open("./data/JU.data", O_RDWR, 0744);
        joint user_j;

        struct flock lock;
        lock.l_type = F_RDLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i)*sizeof(joint);
        lock.l_len = sizeof(joint);
        lock.l_pid = getpid();

        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (i)*sizeof(joint), SEEK_SET);
        read(fd, &user_j, sizeof(joint));

        if(!strcmp(user_j.status, "ACTIVE")){
            result = user_j.balance;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }
    return result;
}

bool change_pwd(int acc_type, int acc_no, char new_pwd[10]){
    int i = acc_no;
    bool result = false;
    int fd;
    if(acc_type == 1){
        fd = open("./data/NU.data", O_RDWR, 0744);
        normal user_n;

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i)*sizeof(normal);
        lock.l_len = sizeof(normal);
        lock.l_pid = getpid();

        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (i)*sizeof(normal), SEEK_SET);
        read(fd, &user_n, sizeof(normal));

        if(!strcmp(user_n.status, "ACTIVE")){
            strcpy(user_n.pwd, new_pwd);
            lseek(fd, sizeof(normal)*(-1), SEEK_CUR);
            write(fd, &user_n, sizeof(normal));
            result = true;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }

    else if(acc_type == 2){
        fd = open("./data/JU.data", O_RDWR, 0744);
        joint user_j;

        struct flock lock;
        lock.l_type = F_WRLCK;
        lock.l_whence = SEEK_SET;
        lock.l_start = (i)*sizeof(joint);
        lock.l_len = sizeof(joint);
        lock.l_pid = getpid();

        fcntl(fd, F_SETLKW, &lock);

        lseek(fd, (i)*sizeof(joint), SEEK_SET);
        read(fd, &user_j, sizeof(joint));

        if(!strcmp(user_j.status, "ACTIVE")){
            strcpy(user_j.pwd, new_pwd);
            lseek(fd, sizeof(joint)*(-1), SEEK_CUR);
            write(fd, &user_j, sizeof(joint));
            result = true;
        }

        lock.l_type = F_UNLCK;
        fcntl(fd, F_SETLK, &lock);

        close(fd);
        return result;
    }
    return result;
}

bool add_normal(normal user){
    int fd = open("./data/NU.data", O_RDWR, 0744);
    bool result = false;

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start - (-1)*sizeof(normal);
    lock.l_len = sizeof(normal);
    lock.l_pid = getpid();

    fcntl(fd, F_SETLKW, &lock);

    normal last;
    lseek(fd, (-1)*sizeof(normal), SEEK_END);
    read(fd, &last, sizeof(normal));

    user.acc_no = last.acc_no + 1;
    strcpy(user.status, "ACTIVE");

    int j = write(fd, &user, sizeof(normal));
    if(j)   result = true;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

    return result;
}

bool add_joint(joint user){
    int fd = open("./data/JU.data", O_RDWR, 0744);
    bool result = false;

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_END;
    lock.l_start - (-1)*sizeof(joint);
    lock.l_len = sizeof(joint);
    lock.l_pid = getpid();

    fcntl(fd, F_SETLKW, &lock);

    joint last;
    lseek(fd, (-1)*sizeof(joint), SEEK_END);
    read(fd, &last, sizeof(joint));

    user.acc_no = last.acc_no + 1;
    strcpy(user.status, "ACTIVE");

    int j = write(fd, &user, sizeof(joint));
    if(j)   result = true;

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);
    close(fd);

    return result;
}

bool del_normal(int acc_no){
    int i = acc_no;
    int fd;
    fd = open("./data/NU.data", O_RDWR, 0744);
    bool result = false;

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(normal);
    lock.l_len = sizeof(normal);
    lock.l_pid = getpid();

    fcntl(fd, F_SETLKW, &lock);

    normal user;
    lseek(fd, (i)*sizeof(normal), SEEK_SET);
    read(fd, &user, sizeof(normal));

    if(!strcmp(user.status, "ACTIVE")){
        strcpy(user.status, "CLOSED");
        user.balance = 0;

        lseek(fd, (-1)*sizeof(normal), SEEK_CUR);
        int w = write(fd, &user, sizeof(normal));
        if(w)   result = true;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool del_joint(int acc_no){
    int i = acc_no;
    int fd;
    fd = open("./data/JU.data", O_RDWR, 0744);
    bool result = false;

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(joint);
    lock.l_len = sizeof(joint);
    lock.l_pid = getpid();

    fcntl(fd, F_SETLKW, &lock);

    joint user;
    lseek(fd, (i)*sizeof(joint), SEEK_SET);
    read(fd, &user, sizeof(joint));

    if(!strcmp(user.status, "ACTIVE")){
        strcpy(user.status, "CLOSED");
        user.balance = 0;

        lseek(fd, (-1)*sizeof(joint), SEEK_CUR);
        int w = write(fd, &user, sizeof(joint));
        if(w)   result = true;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool mod_normal(normal user){
    int i = user.acc_no;
    int fd = open("./data/NU.data", O_RDWR, 0744);
    bool result = false;

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(normal);
    lock.l_len = sizeof(normal);
    lock.l_pid = getpid();

    fcntl(fd, F_SETLKW, &lock);

    normal old_user;
    lseek(fd, i*sizeof(normal), SEEK_SET);
    read(fd, &old_user, sizeof(normal));

    if(!strcmp(old_user.status, "ACTIVE") && (user.acc_no == old_user.acc_no)){
        strcpy(user.status, "ACTIVE");
        lseek(fd, (-1)*sizeof(normal), SEEK_CUR);
        int w = write(fd, &user, sizeof(normal));
        if(w)   result = true;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

bool mod_joint(joint user){
    int i = user.acc_no;
    int fd = open("./data/JU.data", O_RDWR, 0744);
    bool result = false;

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = (i)*sizeof(joint);
    lock.l_len = sizeof(joint);
    lock.l_pid = getpid();

    fcntl(fd, F_SETLKW, &lock);

    joint old_user;
    lseek(fd, i*sizeof(joint), SEEK_SET);
    read(fd, &old_user, sizeof(joint));

    if(!strcmp(old_user.status, "ACTIVE") && (user.acc_no == old_user.acc_no)){
        strcpy(user.status, "ACTIVE");
        lseek(fd, (-1)*sizeof(joint), SEEK_CUR);
        int w = write(fd, &user, sizeof(joint));
        if(w)   result = true;
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLK, &lock);

    close(fd);
    return result;
}

void server_process(int sok){
    int opt, acc_type;
    int acc_lol, type_admin;
    bool result;

    for(;;){
        read(sok, &acc_type, sizeof(acc_type));
        printf("acc_type : %d\n", acc_type);

        if(acc_type == 1){
            normal user_n;
            read(sok, &user_n, sizeof(user_n));
            printf("Normal account no: %d\n", user_n.acc_no);
            acc_lol = user_n.acc_no;
            result = confirm_normal(user_n);
            write(sok, &result, sizeof(result));
        }

        else if(acc_type == 2){
            joint user_j;
            read(sok, &user_j, sizeof(user_j));
            printf("Joint account no: %d\n", user_j.acc_no);
            acc_lol = user_j.acc_no;
            result = confirm_joint(user_j);
            write(sok, &result, sizeof(result));
        }

        else if(acc_type == 3){
            char sent_pwd[10];
            read(sok, sent_pwd, 10);
            printf("Admin Login\n");
            result = confirm_admin(sent_pwd);
            write(sok, &result, sizeof(result));
        }

        else{
            result = false;
            write(sok, &result, sizeof(result));
        }
        if(result) break;
    }

    for(;;){
        read(sok, &opt, sizeof(int));
        if(acc_type == 1 || acc_type == 2){
            if(opt == 1){
                float amount;
                read(sok, &amount, sizeof(float));
                result = deposit(acc_type, acc_lol, amount);
                write(sok, &result, sizeof(result));
            }

            else if(opt == 2){
                float amount;
                read(sok, &amount, sizeof(float));
                result = withdraw(acc_type, acc_lol, amount);
                write(sok, &result, sizeof(result));
            }

            else if(opt == 3){
                float amount;
                amount = balance_enquiry(acc_type, acc_lol);
                write(sok, &amount, sizeof(float));
            }

            else if(opt == 4){
                char new_pwd[10];
                read(sok, new_pwd, 10);
                result = change_pwd(acc_type, acc_lol, new_pwd);
                write(sok, &result, sizeof(result));
            }

            else if(opt == 5){
                if(acc_type == 1){
                    normal user = get_normal(acc_lol);
                    write(sok, &user, sizeof(normal));
                }

                else if(acc_type == 2){
                    joint user = get_joint(acc_lol);
                    write(sok, &user, sizeof(joint));
                }
            }

            else break;
        }

        else if(acc_type == 3){
            read(sok, &type_admin, sizeof(int));

            if(opt == 1){
                if(type_admin == 1){
                    normal new_n;
                    read(sok, &new_n, sizeof(normal));
                    result = add_normal(new_n);
                    write(sok, &result, sizeof(result));
                }

                else if(type_admin == 2){
                    joint new_j;
                    read(sok, &new_j, sizeof(joint));
                    result = add_joint(new_j);
                    write(sok, &result, sizeof(result));
                }
            }

            else if(opt == 2){
                if(type_admin == 1){
                    int del_n;
                    read(sok, &del_n, sizeof(del_n));
                    result = del_normal(del_n);
                    write(sok, &result, sizeof(result));
                }

                else if(type_admin == 2){
                    int del_j;
                    read(sok, &del_j, sizeof(int));
                    result = del_joint(del_j);
                    write(sok, &result, sizeof(result));
                }
            }

            else if(opt == 3){
                if(type_admin == 1){
                    normal mod_n;
                    read(sok, &mod_n, sizeof(normal));
                    result = mod_normal(mod_n);
                    write(sok, &result, sizeof(result));
                }

                else if(type_admin == 2){
                    joint mod_j;
                    read(sok, &mod_j, sizeof(joint));
                    result = mod_joint(mod_j);
                    write(sok, &result, sizeof(result));
                }
            }

            else if(opt == 4){
                if(type_admin == 1){
                    normal deet_n;
                    int acc_n;
                    read(sok, &acc_n, sizeof(int));
                    deet_n = get_normal(acc_n);
                    write(sok, &deet_n, sizeof(normal));
                }

                else if(type_admin == 2){
                    joint deet_j;
                    int acc_j;
                    read(sok, &acc_j, sizeof(int));
                    deet_j = get_joint(acc_j);
                    write(sok, &deet_j, sizeof(joint));
                }
            }

            else if(opt == 5) break;
        }
    }

    close(sok);
    printf("Ended session\n\n");
}

void run_server(){
    struct sockaddr_in server, client;
    int fd, sok, len_client;
    bool result;

    fd = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    bind(fd, (struct sockaddr *)&server, sizeof(server));

    listen(fd, 10);

    printf("Waiting for clients\n");
    // getchar();
    for(;;){
        len_client = sizeof(client);
        sok = accept(fd, (struct sockaddr *)&client, &len_client);

        printf("Connected to a client.\n");
        if(!fork()){
            close(fd);
            server_process(sok);
            exit(0);
        }

        else{
            close(sok);
        }
    }
}
