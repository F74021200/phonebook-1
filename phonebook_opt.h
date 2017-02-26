#ifndef _PHONEBOOK_H
#define _PHONEBOOK_H

#define MAX_LAST_NAME_SIZE 16

#define OPT 1
typedef struct __PHONE_BOOK_ENTRY {
    char lastName[MAX_LAST_NAME_SIZE];
    struct __PHONE_BOOK_ENTRY *pNext;
} entry;

entry *findName(char lastName[], entry *pHead);
entry *append(char lastName[], entry *e);

#endif
