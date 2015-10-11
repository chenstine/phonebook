#ifndef _PHONEBOOK_OPT_HASH_H
#define _PHONEBOOK_OPT_HASH_H

#define MAX_LAST_NAME_SIZE 16

/* original version */
typedef struct __PHONE_BOOK_ENTRY {
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
} entryDetail;

typedef struct __LASTNAME {
    char lastName[MAX_LAST_NAME_SIZE];
    struct __LASTNAME *pNext;
    entryDetail *detail;
} entry;

typedef struct __HASH_TABLE {
    int tableSize;
    entry **buckets;
} hashTable;

hashTable *createHashTable(int tableSize);
entry *findName(char lastname[], hashTable *ht);
void append(char lastName[], hashTable *ht);
int count;

#endif
