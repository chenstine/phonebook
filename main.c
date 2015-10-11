#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL

#define DICT_FILE "./dictionary/words.txt"
#define HASH_TABLE_SIZE 17389
#define FIND_NUM 8

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
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
    e->pNext = NULL;

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif

#if defined(ORIG) || defined(OPT)
    clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        e = append(line, e);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);
#elif defined(OPTHASH)
    /* build the hashTable */
    hashTable *ht = createHashTable(HASH_TABLE_SIZE);

    clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        append(line, ht);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);
#endif

    /* close file as soon as possible */
    fclose(fp);

    /* the givn last name to find */
    char input[FIND_NUM][MAX_LAST_NAME_SIZE] = {
		"uninvolved", "zyxel", "whiteshank", "odontomous",
		"pungoteague", "reweighted", "xiphisternal", "yakattalo"};

    e = pHead;

#if defined(ORIG) || defined(OPT)
	for (i = 0; i < FIND_NUM; ++i) {
    	assert(findName(input[i], e) &&
        	"Did you implement findName() in " IMPL "?");
   		assert(0 == strcmp(findName(input[i], e)->lastName, input[i]));
		printf("finding times:%3d\t%s is found!\n", count, input[i]);
	}
#elif defined(OPTHASH)
	for (i = 0; i < FIND_NUM; ++i) {
    	assert(findName(input[i], ht) &&
        	"Did you implement findName() in " IMPL "?");
   		assert(0 == strcmp(findName(input[i], ht)->lastName, input[i]));
		printf("finding times:%3d\t%s is found!\n", count, input[i]);
	}
#endif

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif

    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);

#if defined(ORIG) || defined(OPT)
    for (i = 0; i < FIND_NUM; ++i) findName(input[i], e);
#elif defined(OPTHASH)
    for (i = 0; i < FIND_NUM; ++i) findName(input[i], ht);
#endif

    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);

    /* FIXME: release all allocated entries */
    free(pHead);

    return 0;
}
