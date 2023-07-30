#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct RecordType
{
    int id;
    char* name;
    int order;
    struct RecordType* next;
};

struct HashType
{
    struct RecordType* head;
};

int hash(int x, int hashSz)
{
    return x % hashSz;
}

int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char name[100];
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
    fscanf(inFile, "%d\n", &dataSz);
    *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);

    if (*ppData == NULL)
        {
    printf("Cannot allocate memory\n");
    exit(-1);
        }

    for (i = 0; i < dataSz; ++i)
        {
    pRecord = *ppData + i;
    fscanf(inFile, "%d ", &n);
    pRecord->id = n;

    fscanf(inFile, "%s ", name);
    pRecord->name = strdup(name);

    fscanf(inFile, "%d ", &n);
    pRecord->order = n;
        }

    fclose(inFile);
    }

    return dataSz;
}

void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
    printf("\t%d %s %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

void displayRecordsInHash(struct HashType* pHashArray, int hashSz)
{
    for (int i = 0; i < hashSz; ++i)
    {
    printf("index %d ->", i);
    struct RecordType* record = pHashArray[i].head;
    while (record != NULL)
    {
    printf(" %d %s %d ->", record->id, record->name, record->order);
    record = record->next;
        }
    printf(" NULL\n");
    }
}

void freeRecords(struct RecordType pData[], int dataSz)
{
    for (int i = 0; i < dataSz; ++i)
    {
    free(pData[i].name);
    }
    free(pData);
}

int main(void)
{
    struct RecordType* pRecords;
    struct HashType* pHashArray;
    int recordSz = 0;
    int hashSz = 15;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    pHashArray = (struct HashType*)malloc(sizeof(struct HashType) * hashSz);
    if (pHashArray == NULL)
    {
    printf("Cannot allocate memory\n");
    exit(-1);
    }

    for (int i = 0; i < hashSz; ++i)
    {
    pHashArray[i].head = NULL;
    }

    for (int i = 0; i < recordSz; ++i)
    {
    int index = hash(pRecords[i].id, hashSz);

    struct RecordType* newRecord = (struct RecordType*)malloc(sizeof(struct RecordType));
    if (newRecord == NULL)
        {
    printf("Cannot allocate memory\n");
    exit(-1);
        }

    *newRecord = pRecords[i];
    newRecord->next = pHashArray[index].head;
    pHashArray[index].head = newRecord;
    }

    displayRecordsInHash(pHashArray, hashSz);

    for (int i = 0; i < hashSz; ++i)
    {
    struct RecordType* record = pHashArray[i].head;
    while (record != NULL)
        {
    struct RecordType* nextRecord = record->next;
    free(record);
    record = nextRecord;
        }
    }
    free(pHashArray);
    freeRecords(pRecords, recordSz);

    return 0;
}
