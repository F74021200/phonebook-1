#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define MAX_LAST_NAME_SIZE 16
#define HASH_TABLE_SIZE 137

#define OPT 1

typedef struct object Object;
typedef struct __PHONE_BOOK_ENTRY Entry;
typedef Entry * (*func_t)(char *, Object **);
typedef void (*read_t)(FILE *, Object **);

struct __PHONE_BOOK_ENTRY {
    char lastName[MAX_LAST_NAME_SIZE];
    struct __PHONE_BOOK_ENTRY *pNext;
};

struct object {
    Entry *HashTable[HASH_TABLE_SIZE];
    func_t findName;
    read_t readFile;
};

Entry *append(char *, Entry *);
int init_object (Object **);
void free_object(Object **);
void clear_cache(Object **);
unsigned int BKDRHash(char *str);

#endif
