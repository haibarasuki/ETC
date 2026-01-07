#include "library.h"

#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define CYAN        "\033[36m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define RED         "\033[31m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"

// 哈希表 (每个元素是一个链表的头指针)
Book* hashTable[TABLE_SIZE];

// 初始化系统
void initSystem() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }
}

// DJB2 哈希算法
unsigned int hash(char* str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; 
    return hash % TABLE_SIZE;
}

// 增加图书 
void addBook(char* id, char* title, char* author) {
    if (searchBook(id) != NULL) {
        printf(RED "[错误] 书号 %s 已存在！" RESET "\n", id);
        return;
    }

    Book* newBook = (Book*)malloc(sizeof(Book));
    if (!newBook) {
        printf(RED "[错误] 内存分配失败！" RESET "\n");
        return;
    }
    strcpy(newBook->id, id);
    strcpy(newBook->title, title);
    strcpy(newBook->author, author);
    newBook->isBorrowed = 0;

    unsigned int index = hash(id);

    newBook->next = hashTable[index];
    hashTable[index] = newBook;

    printf(GREEN "[成功] 图书《%s》已添加" RESET "\n", title);
}

// 查找图书
Book* searchBook(char* id) {
    unsigned int index = hash(id);
    Book* current = hashTable[index];
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// 删除图书
void deleteBook(char* id) {
    unsigned int index = hash(id);
    Book* current = hashTable[index];
    Book* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            if (prev == NULL) {
                hashTable[index] = current->next;
            }
            else {
                prev->next = current->next;
            }
            free(current);
            printf(GREEN "[成功] 书号 %s 已从数据库抹除" RESET "\n", id);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf(RED "[失败] 未找到书号 %s" RESET "\n", id);
}

// 修改图书信息
void updateBook(char* id) {
    Book* book = searchBook(id);
    if (book == NULL) {
        printf(RED "[失败] 未找到书号 %s" RESET "\n", id);
        return;
    }

    printf(YELLOW "当前信息:" RESET " 《%s》 " YELLOW "作者:" RESET " %s\n", book->title, book->author);
    printf("请输入新书名: ");
    scanf("%s", book->title);
    printf("请输入新作者: ");
    scanf("%s", book->author);
    printf(GREEN "[成功] 属性已重写" RESET "\n");
}

// 借阅/归还图书
void borrowOrReturnBook(char* id, int action) {
    Book* book = searchBook(id);
    if (book == NULL) {
        printf(RED "[失败] 未找到书号 %s" RESET "\n", id);
        return;
    }

    if (action == 1) { // 借书
        if (book->isBorrowed) {
            printf(YELLOW "[提示] 《%s》 已处于借出状态" RESET "\n", book->title);
        }
        else {
            book->isBorrowed = 1;
            printf(BLUE "[成功] 资源借出锁定: 《%s》" RESET "\n", book->title);
        }
    }
    else { // 还书
        if (!book->isBorrowed) {
            printf(YELLOW "[提示] 《%s》 已在库内" RESET "\n", book->title);
        }
        else {
            book->isBorrowed = 0;
            printf(BLUE "[成功] 资源归还同步: 《%s》" RESET "\n", book->title);
        }
    }
}

// 显示所有图书
void displayAll() {
    int count = 0;
    printf("\n" CYAN "--- 图书馆馆藏清单 ---" RESET "\n");
    printf(BOLD "%-10s %-20s %-15s %-10s" RESET "\n", "ID", "书名", "作者", "状态");
    printf("----------------------------------------------------------\n");

    for (int i = 0; i < TABLE_SIZE; i++) {
        Book* current = hashTable[i];
        while (current != NULL) {
            printf("%-10s %-20s %-15s %-10s\n",
                current->id,
                current->title,
                current->author,
                current->isBorrowed ? RED "已借出" RESET : GREEN "可借阅" RESET);
            current = current->next;
            count++;
        }
    }

    if (count == 0) printf(YELLOW "(暂无记录)" RESET "\n");
    printf("----------------------------------------------------------\n");
}

// 释放内存
void freeSystem() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Book* current = hashTable[i];
        while (current != NULL) {
            Book* temp = current;
            current = current->next;
            free(temp);
        }
        hashTable[i] = NULL;
    }
    printf(RED "[系统] 内存已完全释放，核心停止运行" RESET "\n");
}

// 保存数据
void saveData() {
    FILE* fp = fopen(DATA_FILE, "w");
    if (fp == NULL) {
        printf(RED "[错误] 核心无法访问 I/O 设备，保存失败！" RESET "\n");
        return;
    }

    int count = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        Book* current = hashTable[i];
        while (current != NULL) {
            fprintf(fp, "%s %s %s %d\n",
                current->id, current->title, current->author, current->isBorrowed);
            current = current->next;
            count++;
        }
    }

    fclose(fp);
    printf(GREEN "[系统] 同步成功：已将 %d 条记录存入磁盘" RESET "\n", count);
}

// 加载数据
void loadData() {
    FILE* fp = fopen(DATA_FILE, "r");
    if (fp == NULL) {
        printf(YELLOW "[系统] 未检测到存档，初始化空数据库" RESET "\n");
        return;
    }

    char id[20], title[100], author[50];
    int isBorrowed;
    int count = 0;

    while (fscanf(fp, "%s %s %s %d", id, title, author, &isBorrowed) != EOF) {
        Book* newBook = (Book*)malloc(sizeof(Book));
        if (!newBook) continue;

        strcpy(newBook->id, id);
        strcpy(newBook->title, title);
        strcpy(newBook->author, author);
        newBook->isBorrowed = isBorrowed;

        unsigned int index = hash(id);
        newBook->next = hashTable[index];
        hashTable[index] = newBook;

        count++;
    }

    fclose(fp);
    printf(GREEN "[系统] 数据热加载完成：已恢复 %d 条记录" RESET "\n", count);
}