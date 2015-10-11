#include <stdlib.h>
#include <stdint.h>

#include "phonebook_opt_hash.h"

hashTable *createHashTable(int tableSize)
{
    hashTable *ht;
    ht = (hashTable *) malloc(sizeof(hashTable));
    ht->buckets = (entry **) calloc(tableSize, sizeof(entry *));
    ht->tableSize = tableSize;
    return ht;
}

static inline uint32_t hash(char *key, hashTable *ht)
{
    uint32_t hashVal = 0;
    while (*key != '\0') {
        hashVal = (hashVal << 5) + hashVal + *key++;
    }

    return hashVal % ht->tableSize;
}

entry *findName(char lastName[], hashTable *ht)
{
    uint32_t hashIndex = hash(lastName, ht);
    entry *pHead = ht->buckets[hashIndex];
	count = 1;

    while (pHead != NULL) {
        if (strcasecmp(lastName, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
		++count;
    }

    return NULL;
}

void append(char lastName[], hashTable *ht)
{
    uint32_t hashIndex = hash(lastName, ht);
    entry *e;
    e = (entry *) malloc(sizeof(entry));

    strcpy(e->lastName, lastName);
    e->pNext = ht->buckets[hashIndex];
    ht->buckets[hashIndex] = e;
}
