#include "phonebook_opt.h"

static Entry *findName_impl(char lastName[], Object **self)
{
    Entry *e;

    e = ((*self)->HashTable)[BKDRHash(lastName) % HASH_TABLE_SIZE];

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
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        e = (*self)->HashTable[BKDRHash(line) % HASH_TABLE_SIZE];
        while(e->pNext)
            e = e->pNext;
        e = append(line, e);
    }
}

Entry *append(char lastName[], Entry *e)
{
    e->pNext = (Entry *) malloc(sizeof(Entry));
    e = e->pNext;
    strcpy(e->lastName, lastName);
    e->pNext = NULL;

    return e;
}

int init_object (Object **self)
{
    int i;

    if (NULL == ((*self) = malloc(sizeof(Object)))) return -1;
    for (i = 0; i < HASH_TABLE_SIZE; ++i) {
        if (NULL == (((*self)->HashTable)[i] = (Entry *) malloc(sizeof(Entry)))) return -1;
        ((*self)->HashTable)[i]->pNext = NULL;
    }
    (*self)->findName = findName_impl;
    (*self)->readFile = readFile_impl;
    return 0;
}

void free_object (Object **self)
{
    int i;

    for (i = 0; i < HASH_TABLE_SIZE; ++i) {
        if ((*self)->HashTable[i]->pNext)free((*self)->HashTable[i]->pNext);
        free((*self)->HashTable[i]);
    }
}

void clear_cache(Object **self)
{
    __builtin___clear_cache((char *) (*self)->HashTable, (char *) (*self)->HashTable + (sizeof(Entry*)+ sizeof(Entry)) * HASH_TABLE_SIZE);
}

unsigned int BKDRHash(char *str)
{
    unsigned int seed = 131;
    unsigned int hash = 0;

    while (*str) {
        hash = hash * seed + (*str++);
    }

    return hash;
}
