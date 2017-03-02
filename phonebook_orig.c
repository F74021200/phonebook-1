#include "phonebook_orig.h"

static Entry *findName_impl(char lastName[], Object **self)
{
    Entry * e;
    e = (*self)->pHead;

    while (e) {
        if (strcasecmp(lastName, e->lastName) == 0)
            return e;
        e = e->pNext;
    }
    return NULL;
}

static void readFile_impl(FILE *fp, Object **self)
{
    char line[MAX_LAST_NAME_SIZE];
    int i = 0;
    Entry *e;
    e = (*self)->pHead;

    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        e = append(line, e);
    }
}

Entry *append(char lastName[], Entry *e)
{
    /* allocate memory for the new entry and put lastName */
    e->pNext = (Entry *) malloc(sizeof(Entry));
    e = e->pNext;
    strcpy(e->lastName, lastName);
    e->pNext = NULL;

    return e;
}

int init_object (Object **self)
{
    if (NULL == (*self = malloc(sizeof(Object)))) return -1;
    if (NULL == ((*self)->pHead = (Entry *) malloc(sizeof(Entry)))) return -1;
    (*self)->pHead->pNext = NULL;
    (*self)->findName = findName_impl;
    (*self)->readFile = readFile_impl;
    return 0;
}

void free_object (Object **self)
{
    if ((*self)->pHead->pNext)free((*self)->pHead->pNext);
    free((*self)->pHead);
}

void clear_cache (Object **self)
{
    __builtin___clear_cache((char *) (*self)->pHead, (char *) (*self)->pHead + sizeof(Entry));
}
