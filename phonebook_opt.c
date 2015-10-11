#include <stdlib.h>

#include "phonebook_opt.h"

entry *findName(char lastname[], entry *pHead)
{
    while (pHead != NULL) {
        if (strcasecmp(lastname, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
    }
    return NULL;
}

entry *append(char lastName[], entry *e)
{
    strcpy(e->lastName, lastName);
    e->pNext = (entry *) malloc(sizeof(entry));
    e = e->pNext;
    e->pNext = NULL;

    return e;
}
