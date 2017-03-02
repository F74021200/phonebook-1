#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define MAX_LAST_NAME_SIZE 16

typedef struct object Object;
typedef struct __PHONE_BOOK_ENTRY Entry;
typedef Entry * (*func_t)(char *, Object **);
typedef void (*read_t)(FILE *, Object **);

/* original version */
struct __PHONE_BOOK_ENTRY {
    char lastName[MAX_LAST_NAME_SIZE];
    char firstName[16];
    char email[16];
    char phone[10];
    char cell[10];
    char addr1[16];
    char addr2[16];
    char city[16];
    char state[2];
    char zip[5];
    struct __PHONE_BOOK_ENTRY *pNext;
};

struct object {
    Entry *pHead;
    func_t findName;
    read_t readFile;
};

Entry *append(char *, Entry *);
int init_object(Object **);
void free_object(Object **);
void clear_cache(Object **);

#endif
