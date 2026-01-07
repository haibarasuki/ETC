#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10007      // 哈希表大小
#define DATA_FILE "library_data.txt" // 定义文件名

// 定义用户角色
typedef enum {
    ROLE_ADMIN,  
    ROLE_READER  
} UserRole;

typedef struct Book {
    char id[20];        // 书号 (作为哈希的Key)
    char title[100];    
    char author[50];    
    int isBorrowed;     // 1: 已借出, 0: 在馆
	struct Book* next;  
} Book;

// 全局哈希表声明
extern Book* hashTable[TABLE_SIZE];     //  指向链表头

// 函数声明
void initSystem();
unsigned int hash(char* str);
void addBook(char* id, char* title, char* author);
void deleteBook(char* id);
Book* searchBook(char* id);
void updateBook(char* id); // 修改书名或作者
void borrowOrReturnBook(char* id, int action); // 1: 已借出, 0: 在馆
void displayAll();
void freeSystem();

void saveData();
void loadData();