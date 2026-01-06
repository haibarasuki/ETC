#include "library.h"
#include <windows.h>

// ANSI 颜色控制宏
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define CYAN        "\033[36m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define RED         "\033[31m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"

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

void printMenu() {
    printf("\n" BOLD MAGENTA "╔════════════════════ 操作菜单 ════════════════════╗" RESET "\n");
    printf(BOLD "  (1) " RESET CYAN "INSERT" RESET " - 向表添加新书\n");
    printf(BOLD "  (2) " RESET RED "DELETE" RESET " - 从文件中删除书籍\n");
    printf(BOLD "  (3) " RESET YELLOW "SEARCH" RESET " - 通过DJB2算法进行查询\n");
    printf(BOLD "  (4) " RESET GREEN "UPDATE" RESET " - 修改现有图书属性\n");
    printf(BOLD "  (5) " RESET BLUE "BORROW" RESET " - 借书操作\n");
    printf(BOLD "  (6) " RESET BLUE "RETURN" RESET " - 归还书籍\n");
    printf(BOLD "  (7) " RESET "LIST  " RESET " - 获取所有图书状态\n");
    printf(BOLD "  (0) " RESET RED "EXIT  " RESET " - 同步数据并终止\n");
    printf(BOLD MAGENTA "╚══════════════════════════════════════════════════╝" RESET "\n");
    printf(BOLD YELLOW "~# " RESET);
}

int main() {
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
        case 1:
            printf(CYAN "请输入书号(ID): " RESET); scanf("%s", id);
            printf(CYAN "请输入书名: " RESET); scanf("%s", title);
            printf(CYAN "请输入作者: " RESET); scanf("%s", author);
            addBook(id, title, author);
            break;
        case 2:
            printf(RED "请输入要删除的书号: " RESET); scanf("%s", id);
            deleteBook(id);
            break;
        case 3:
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
        case 4:
            printf(CYAN "请输入要修改的书号: " RESET); scanf("%s", id);
            updateBook(id);
            break;
        case 5:
            printf(BLUE "请输入借阅书号: " RESET); scanf("%s", id);
            borrowOrReturnBook(id, 1);
            break;
        case 6:
            printf(BLUE "请输入归还书号: " RESET); scanf("%s", id);
            borrowOrReturnBook(id, 0);
            break;
        case 7:
            displayAll();
            break;
        case 0:
            printf(YELLOW "正在保存数据并退出..." RESET "\n");
            saveData();
            freeSystem();
            return 0;
        default:
            printf(RED "无效选项，请重试。" RESET "\n");
        }
    }
    return 0;
}