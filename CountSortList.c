
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CountSortList.h"

typedef struct SortLinkList {
    int index;
    struct SortLinkList *prev;
    struct SortLinkList *next;
}SortLinkList;

static int findItemInDataList(int data);
static void addDataToDataList(int data);
static void incListSizeIfNeed();
static void sortItemInLinkList(int idx, int data);
static SortLinkList * findItemInLinkList(int data);
static SortLinkList * findInsertSlot(int count, SortLinkList * curLink);
static void moveBeforeLink(SortLinkList * curLink, SortLinkList * dstLink);

CountSortData * g_dataList = NULL;
SortLinkList  * g_linkList = NULL;
SortLinkList  * g_linkHead = NULL;
SortLinkList  * g_linkTail = NULL;
int g_listSize = LIST_INIT_SIZE;
int g_dataSize = 0;
int g_dataSum = 0;

void CountSortList_init() {
    g_dataList = (CountSortData *)calloc(g_listSize, sizeof(CountSortData));
    g_linkList = (SortLinkList *)calloc(g_listSize, sizeof(SortLinkList));

    g_linkHead = g_linkTail = NULL;
}
void CountSortList_deinit() {
    free(g_dataList);
    free(g_linkList);
    g_dataList = NULL;
    g_linkList = NULL;
    g_linkHead = NULL;
    g_linkTail = NULL;
    g_dataSize = 0;
    g_dataSum = 0;
}
void CountSortList_reset() {
    memset(g_dataList, 0x00, g_dataSize * sizeof(CountSortData));
    memset(g_linkList, 0x00, g_dataSize * sizeof(SortLinkList));
    g_linkHead = NULL;
    g_linkTail = NULL;
    g_listSize = LIST_INIT_SIZE;
    g_dataSize = 0;
    g_dataSum = 0;
}

void CountSortList_setInitSize(int size) {
    if (NULL == g_dataList && NULL == g_linkList)
        g_listSize = size;
    else
        printf("Please invoke this function before CountSortList_init.\n");
}

void CountSortList_add(int data) {
    g_dataSum++;

    int idx = findItemInDataList(data);
    if (-1 == idx) { // not found this data in current list
        addDataToDataList(data);
        incListSizeIfNeed();
    }
    else {
        g_dataList[idx].count++;
        sortItemInLinkList(idx, data);
    }
}

void CountSortList_addArray(int * data, int size) {
    for (int i=0; i<size; i++)
        CountSortList_add(data[i]);
}

void CountSortList_raw(CountSortData * dataList, int size, int * sum) {
    int i = 0;
    while (i < size && i < g_dataSize) {
        dataList[i].data = g_dataList[i].data;
        dataList[i].count = g_dataList[i].count;
        i++;
    }
    *sum = g_dataSum;
}

void CountSortList_output(CountSortData * dataList, int size, int * sum) {
    SortLinkList  * link = g_linkHead;
    int i = 0;
    while (link && i < size) {
        dataList[i].data = g_dataList[link->index].data;
        dataList[i].count = g_dataList[link->index].count;
        link = link->next;
        i++;
    }
    *sum = g_dataSum;
}

void CountSortList_dump() {
    for (int i=0; i<g_dataSize; i++) {
        printf("[%d] data %d, count %d\n", i, g_dataList[i].data, g_dataList[i].count);
    }

    SortLinkList  * link = g_linkHead;
    char c = ' ';
    while(link) {
        if (link == g_linkHead)
            c = 'H';
        else if (link == g_linkTail)
            c = 'T';
        else
            c = 'M';
        printf("[%c] link index %d, data %d, count %d\n", c, link->index,
            g_dataList[link->index].data, g_dataList[link->index].count);

        link = link->next;
    }
}

int findItemInDataList(int data) {
    for (int i=0; i<g_dataSize; i++) {
        if (g_dataList[i].data == data)
            return i;
    }
    return -1;
}

void addDataToDataList(int data) {
    g_dataList[g_dataSize].data = data;
    g_dataList[g_dataSize].count = 1;

    // current link
    SortLinkList * curLink = &g_linkList[g_dataSize];
    curLink->index = g_dataSize;
    curLink->prev = g_linkTail;

    if (g_dataSize == 0) { // the first data
        g_linkHead = g_linkTail = curLink;
    }
    else {
        g_linkTail->next = curLink;
    }

    g_linkTail = curLink;
    g_dataSize++;
}

void incListSizeIfNeed() {
    if (g_dataSize > g_listSize*8/10) {
        g_listSize *= 2;

        CountSortData * dataList = (CountSortData *)realloc (g_dataList, g_listSize*sizeof(CountSortData));
        if (dataList == NULL) {
            printf("realloc %zd memory, but failed.", g_listSize * sizeof(CountSortData));
            exit(1);
        }
        else
            g_dataList = dataList;

        SortLinkList * linkList = (SortLinkList *)realloc (g_linkList, g_listSize*sizeof(SortLinkList));
        if (linkList == NULL) {
            printf("realloc %zd memory, but failed.", g_listSize * sizeof(SortLinkList));
            exit(1);
        }
        else
            g_linkList = linkList;
    }
}

void sortItemInLinkList(int idx, int data) {
    SortLinkList * curLink = findItemInLinkList(data);
    int curCount = g_dataList[curLink->index].count;
    SortLinkList * link = findInsertSlot(curCount, curLink);

    if (NULL == link) {
        // move after head
        moveBeforeLink(curLink, g_linkHead);
    }
    else {
        moveBeforeLink(curLink, link);
    }
}

SortLinkList * findItemInLinkList(int data) {
    SortLinkList  * link = g_linkTail;
    while (link) {
        if (g_dataList[link->index].data == data)
            return link;

        link = link->prev;
    }

    // shouldn't be here
    return NULL;
}

SortLinkList * findInsertSlot(int count, SortLinkList * curLink) {
    SortLinkList  * link = curLink->prev;
    while (link) {
        if (g_dataList[link->index].count >= count)
            return link->next;
        if (link->prev && g_dataList[link->index].count < count && g_dataList[link->prev->index].count >= count)
            return link;

        link = link->prev;
    }

    return NULL;
}

void moveBeforeLink(SortLinkList * curLink, SortLinkList * dstLink) {
    if (curLink == dstLink)
        return;

    // remove curLink from linkList
    if (curLink == g_linkTail) {
        g_linkTail = curLink->prev;
        g_linkTail->next = NULL;
    }
    else {
        curLink->prev->next = curLink->next;
        curLink->next->prev = curLink->prev;
    }

    // add link before dstLink
    if (dstLink == g_linkHead) {
        curLink->prev = NULL;
        g_linkHead = curLink;
        curLink->next = dstLink;
        dstLink->prev = curLink;
    }
    else {
        dstLink->prev->next = curLink;
        curLink->prev = dstLink->prev;

        curLink->next = dstLink;
        dstLink->prev = curLink;
    }
}
