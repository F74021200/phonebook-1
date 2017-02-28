#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL

#define HASH_TABLE_SIZE 137
#if OPT == 1 /*for phonebook_opt*/
#define OUT_FILE "opt.txt"
#else
#define OUT_FILE "orig.txt"
#endif

#define DICT_FILE "./dictionary/words.txt"

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int i = 0;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;

    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }

    /* build the entry */
    entry *pHead, *e, *Hash_Table[HASH_TABLE_SIZE];
    if (OPT == 1)
        for (i = 0; i < HASH_TABLE_SIZE; ++i) {
            Hash_Table[i] = (entry *) malloc(sizeof(entry));
            e = Hash_Table[i];
            e->pNext = NULL;
        }
    else {
        pHead = (entry *) malloc(sizeof(entry));
        e = pHead;
        e->pNext = NULL;
    }
#if defined(__GNUC__)
    if (OPT == 1)
        __builtin___clear_cache((char *) Hash_Table, (char *) Hash_Table + (sizeof(entry*)+ sizeof(entry)) * HASH_TABLE_SIZE);
    else
        __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    clock_gettime(CLOCK_REALTIME, &start);
    if (!OPT) e = pHead;
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        if (OPT == 1) {
            e = Hash_Table[BKDRHash(line) % HASH_TABLE_SIZE];
            while (e->pNext)
                e = e->pNext;
        }
        e = append(line, e);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
    fclose(fp);

    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";

    if (OPT == 1) {
        e = Hash_Table[BKDRHash(input) % HASH_TABLE_SIZE];
    } else
        e = pHead;

    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));

#if defined(__GNUC__)
    if (OPT == 1)
        __builtin___clear_cache((char *) Hash_Table, (char *) Hash_Table + (sizeof(entry*)+ sizeof(entry)) * HASH_TABLE_SIZE);
    else
        __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);
    if (OPT == 1) {
        e = Hash_Table[BKDRHash(input) % HASH_TABLE_SIZE];
    } else
        e = pHead;

    findName(input, e);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    FILE *output = fopen(OUT_FILE, "a");
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);

    if (OPT == 1)
        for (i = 0; i < HASH_TABLE_SIZE; ++i) {
            if (Hash_Table[i]->pNext) free(Hash_Table[i]->pNext);
            free(Hash_Table[i]);
        }
    else {
        if (pHead->pNext) free(pHead->pNext);
        free(pHead);
    }

    return 0;
}
