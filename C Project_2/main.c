#include "library.h"
#include <windows.h>
#include <conio.h>

// ANSI 颜色控制宏
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define CYAN        "\033[36m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define RED         "\033[31m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"

UserRole currentUserRole;

// 隐藏密码输入
void getPassword(char* password, int maxLength) {
    int i = 0;
    char ch;
    while (1) {
        ch = _getch();

        if (ch == 13) { // 回车键 ASCII
            password[i] = '\0';
            printf("\n");
            break;
        }
        else if (ch == 8) { // 退格键 ASCII
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        }
        else if (i < maxLength - 1) {
            password[i] = ch;
            i++;
            printf("*"); 
        }
    }
}

// 登录系统
int loginSystem() {
    char username[50];
    char password[50];
    int attempts = 3;

    while (attempts > 0) {
        system("cls");
        printf(CYAN BOLD);
        printf("\n   ========================================\n");
        printf("        欢迎进入图书管理系统安全网关\n");
        printf("   ========================================\n" RESET);

        printf(YELLOW "\n[剩余尝试次数: %d]\n" RESET, attempts);
        printf("请输入用户名 : ");
        scanf("%s", username);

        printf("请输入密码 (隐藏显示): ");
        getPassword(password, 50);

        // 管理员账户: admin / 123456
        if (strcmp(username, "admin") == 0 && strcmp(password, "123456") == 0) {
            currentUserRole = ROLE_ADMIN;
            printf(GREEN "\n[认证成功] 欢迎回来，超级管理员。\n" RESET);
            Sleep(1000);
            return 1;
        }

        // 读者账户: reader / 123456
        if (strcmp(username, "reader") == 0 && strcmp(password, "123456") == 0) {
            currentUserRole = ROLE_READER;
            printf(GREEN "\n[认证成功] 欢迎回来，读者。\n" RESET);
            Sleep(1000);
            return 1;
        }

        printf(RED "\n[错误] 用户名或密码无效！\n" RESET);
        Sleep(1000);
        attempts--;
    }

    printf(RED "\n[安全警报] 尝试次数过多，系统锁定。\n" RESET);
    return 0;
}

void printLogo() {
    printf(CYAN BOLD);
    printf("  ██████╗  ██████╗  ██████╗  ██╗  ██╗    \n");
    printf("  ██╔══██╗ ██╔══██╗ ██╔══██╗ ██║ ██╔╝  \n");
    printf("  ██████╔╝ ██║  ██║ ██║  ██║ █████╔╝     \n");
    printf("  ██╔══██╗ ██║  ██║ ██║  ██║ ██╔═██╗    \n");
    printf("  ██████╔╝ ██████╔╝ ██████╔╝ ██║  ██╗   \n");
    printf("  ╚═════╝  ╚═════╝  ╚═════╝  ╚═╝  ╚═╝   \n");
    printf(RESET);
    printf(MAGENTA BOLD "   [ B O O K   M A N A G E M E N T   S Y S T E M ]" RESET "\n");
    printf(YELLOW "      >>> 基于Hash-Table的高性能架构 <<<      \n" RESET);
    printf(CYAN "--------------------------------------------------------------\n" RESET);
}

void loadingEffect() {
    printf(GREEN "加载中...");
    for (int i = 0; i < 20; i++) {
        printf("█");
        fflush(stdout); // 强制刷新缓冲区
        Sleep(50);      // 毫秒
    }
    printf(" [DONE]\n" RESET);
}

// 菜单：根据权限显示不同选项
void printMenu() {
    printf("\n" BOLD MAGENTA "╔════════════════════ %s菜单 ════════════════════╗" RESET "\n",
        currentUserRole == ROLE_ADMIN ? "管理员" : "读者");

    // 管理员独有功能
    if (currentUserRole == ROLE_ADMIN) {
        printf(BOLD "  (1) " RESET CYAN "INSERT" RESET " - 向表添加新书\n");
        printf(BOLD "  (2) " RESET RED "DELETE" RESET " - 从文件中删除书籍\n");
    }
    else {
        printf(BOLD "  (1) " RESET "%-20s\n", "------ (权限不足)");
        printf(BOLD "  (2) " RESET "%-20s\n", "------ (权限不足)");
    }

    // 公共功能
    printf(BOLD "  (3) " RESET YELLOW "SEARCH" RESET " - 通过DJB2算法进行查询\n");

    // 管理员独有功能
    if (currentUserRole == ROLE_ADMIN) {
        printf(BOLD "  (4) " RESET GREEN "UPDATE" RESET " - 修改现有图书属性\n");
    }
    else {
        printf(BOLD "  (4) " RESET "%-20s\n", "------ (权限不足)");
    }

    // 公共功能
    printf(BOLD "  (5) " RESET BLUE "BORROW" RESET " - 借书操作\n");
    printf(BOLD "  (6) " RESET BLUE "RETURN" RESET " - 归还书籍\n");
    printf(BOLD "  (7) " RESET "LIST  " RESET " - 获取所有图书状态\n");
    printf(BOLD "  (0) " RESET RED "EXIT  " RESET " - 同步数据并终止\n");

    printf(BOLD MAGENTA "╚══════════════════════════════════════════════════╝" RESET "\n");
    printf(BOLD YELLOW "[%s] ~# " RESET, currentUserRole == ROLE_ADMIN ? "Admin" : "User");
}

int main() {
    // 登录验证
    if (!loginSystem()) {
        return 0;
    }

    // 展示启动动画
    printLogo();
    loadingEffect();

    initSystem();
    loadData();

    int choice;
    char id[20], title[100], author[50];

    while (1) {
        printMenu();
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf(RED "输入非法，请输入数字选项！" RESET "\n");
            continue;
        }

        switch (choice) {
        case 1: // Insert
            if (currentUserRole != ROLE_ADMIN) {
                printf(RED "[权限拒绝] 只有管理员可以录入图书。\n" RESET);
                break;
            }
            system("cls");
            printMenu();
            printf(CYAN "请输入书号(ID): " RESET); scanf("%s", id);
            printf(CYAN "请输入书名: " RESET); scanf("%s", title);
            printf(CYAN "请输入作者: " RESET); scanf("%s", author);
            addBook(id, title, author);
            break;

        case 2: // Delete
            if (currentUserRole != ROLE_ADMIN) {
                printf(RED "[权限拒绝] 只有管理员可以删除图书。\n" RESET);
                break;
            }
            system("cls");
            printMenu();
            printf(RED "请输入要删除的书号: " RESET); scanf("%s", id);
            deleteBook(id);
            break;

        case 3: // Search
            system("cls");
            printMenu();
            printf(YELLOW "请输入要查找的书号: " RESET); scanf("%s", id);
            Book* book = searchBook(id);
            if (book) {
                printf(GREEN "[找到] " RESET "《%s》 " YELLOW "作者: " RESET "%s " YELLOW "状态: " RESET "%s\n",
                    book->title, book->author, book->isBorrowed ? RED "已借出" RESET : GREEN "在馆" RESET);
            }
            else {
                printf(RED "[提示] 未找到该书。" RESET "\n");
            }
            break;

        case 4: // Update
            if (currentUserRole != ROLE_ADMIN) {
                printf(RED "[权限拒绝] 只有管理员可以修改元数据。\n" RESET);
                break;
            }
            system("cls");
            printMenu();
            printf(CYAN "请输入要修改的书号: " RESET); scanf("%s", id);
            updateBook(id);
            break;

        case 5: // Borrow 
            system("cls");
            printMenu();
            printf(BLUE "请输入借阅书号: " RESET); scanf("%s", id);
            borrowOrReturnBook(id, 1);
            break;

        case 6: // Return
            system("cls");
            printMenu();
            printf(BLUE "请输入归还书号: " RESET); scanf("%s", id);
            borrowOrReturnBook(id, 0);
            break;

        case 7: // List 
            system("cls");
            printMenu();
            displayAll();
            break;

        case 0: // Exit
            system("cls");
            printMenu();
            printf(YELLOW "正在保存数据并退出..." RESET "\n");
            saveData();
            freeSystem();
            return 0;

        default:
            system("cls");
            printMenu();
            printf(RED "无效选项，请重试。" RESET "\n");
        }
    }
    return 0;
}