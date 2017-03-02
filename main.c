#include <time.h>
#include <assert.h>

#include IMPL

#define HASH_TABLE_SIZE 137
#ifdef OPT /*for phonebook_opt*/
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
    struct timespec start, end;
    double cpu_time1, cpu_time2;

    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }

    /* create the object */
    Object *o = NULL;
    init_object(&o);

#if defined(__GNUC__)
    clear_cache(&o);
#endif
    clock_gettime(CLOCK_REALTIME, &start);

    /*read the input file*/
    o->readFile(fp, &o);

    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
    fclose(fp);

    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";

    assert(o->findName(input, &o) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(o->findName(input, &o)->lastName, "zyxel"));

#if defined(__GNUC__)
    clear_cache(&o);
#endif
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);

    o->findName(input, &o);

    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    FILE *output = fopen(OUT_FILE, "a");
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);

    free_object(&o);

    return 0;
}
