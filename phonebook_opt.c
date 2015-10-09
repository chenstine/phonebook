#include <stdlib.h>

#include "phonebook_opt.h"

/* FILL YOUR OWN IMPLEMENTATION HERE! */
lastNameEntry *findName(char lastname[], lastNameEntry *pHead)
{
    /* TODO: implement */
    while (pHead != NULL) {
        if (strcasecmp(lastname, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
    }
    return NULL;
}

lastNameEntry *append(char lastName[], lastNameEntry *e)
{
    e->pNext = (lastNameEntry *) malloc(sizeof(lastNameEntry));
    e = e->pNext;
    strcpy(e->lastName, lastName);
    e->pNext = NULL;

    return e;
}
