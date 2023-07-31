/*frantzy jerome
lab assignment 9*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// RecordType
struct RecordType{
    int id;
    char name;
    int order;
};

// Node for linked list
struct Node{
    struct RecordType data;
    struct Node* next;
};

// HashType: Hash table structure with separate chaining
struct HashType{
    int size;
    struct Node** table;
};

// Compute the hash function
int hash(int x, int size){
    // Simple hash function to distribute data evenly in the array
    return x % size;
}

//function prototypes----------------------------------------------------
struct HashType* createHashTable(int size);
void freeHashTable(struct HashType* hashTable);
void insertRecord(struct HashType* hashTable, struct RecordType record);
int parseData(char* inputFileName, struct RecordType** ppData);
void printRecords(struct RecordType pData[], int dataSz);
void displayRecordsInHash(struct HashType* pHashArray, int hashSz);
//-----------------------------------------------------------------------

// Initialize the hash table with separate chaining
// Function to create a new hash table
struct HashType* createHashTable(int size) {
    // Allocate memory for the hash table
    struct HashType* hashTable = (struct HashType*)malloc(sizeof(struct HashType));
    if (hashTable == NULL) {
        fprintf(stderr, "Memory allocation failed for the hash table.\n");
        exit(EXIT_FAILURE);
    }

    // Set the hash table's size
    hashTable->size = size;

    // Allocate memory for the hash table's array 
    hashTable->table = (struct Node**)malloc(sizeof(struct Node*) * size);
    if (hashTable->table == NULL) {
        fprintf(stderr, "Memory allocation failed for the hash array.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize each linked list in the hash table to NULL
    for (int i = 0; i < size; ++i) {
        hashTable->table[i] = NULL;
    }

    return hashTable;
}//end hashtype function call 

// Function to free the allocated memory for the hash table and its linked lists
void freeHashTable(struct HashType* hashTable){
    for (int i = 0; i < hashTable->size; ++i)
    {
        struct Node* current = hashTable->table[i];
        while (current != NULL)
        {
            struct Node* next = current->next;
            free(current);
            current = next;
        }
    }
    free(hashTable->table);
    free(hashTable);
}//end function call hash 

// Insert a record into the hash table
void insertRecord(struct HashType* hashTable, struct RecordType record){
    int index = hash(record.id, hashTable->size);

    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    newNode->data = record;
    newNode->next = hashTable->table[index];
    hashTable->table[index] = newNode;
}//end record function call

// Parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData){
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
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
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}//end parse function call

// Prints the records
void printRecords(struct RecordType pData[], int dataSz){
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}//end function call

// Function to display records in the hash structure
void displayRecordsInHash(struct HashType* pHashArray, int hashSz){
    for (int i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);
        struct Node* current = pHashArray->table[i];
        while (current != NULL)
        {
            printf("%d %c %d -> ", current->data.id, current->data.name, current->data.order);
            current = current->next;
        }
        printf("NULL\n");
    }
}//end function call

// Main function
int main(void){
    struct RecordType *pRecords;
    int recordSz = 0;
    int hashTableSize = 10; // Choose an appropriate hash table size

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    struct HashType* hashTable = createHashTable(hashTableSize);

    // Insert records into the hash table
    for (int i = 0; i < recordSz; ++i)
    {
        insertRecord(hashTable, pRecords[i]);
    }

    displayRecordsInHash(hashTable, hashTableSize);

    // Free allocated memory
    freeHashTable(hashTable);

    return 0;
}//end main function call