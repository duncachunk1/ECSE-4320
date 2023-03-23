
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

#define CAPACITY 50000 // Size of the HashTable.

unsigned long hash_function(std::string str)
{
    unsigned long i = 0;

    for (int j = 0; j < str.size(); j++) {
        i += str[j];
    }

    return i % CAPACITY;
}


// Defines the HashTable item.
typedef struct Ht_item
{
    std::string key;
    std::vector<int> value;
    Ht_item* nextInIndex = NULL;
} Ht_item;


// Defines the LinkedList.
typedef struct LinkedList
{
    Ht_item *item;
    LinkedList *next;
} LinkedList;


// Defines the HashTable.
typedef struct HashTable
{
    // Contains an array of pointers to items.
    Ht_item **items;
    LinkedList **overflow_buckets;
    int size;
    int count;
} HashTable;


LinkedList *allocate_list()
{
    // Allocates memory for a LinkedList pointer.
    LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
    return list;
}

LinkedList *linkedlist_insert(LinkedList *list, Ht_item *item)
{
    // Inserts the item onto the LinkedList.
    if (!list)
    {
        LinkedList *head = allocate_list();
        head->item = item;
        head->next = NULL;
        list = head;
        return list;
    }
    else if (list->next == NULL)
    {
        LinkedList *node = allocate_list();
        node->item = item;
        node->next = NULL;
        list->next = node;
        return list;
    }

    LinkedList *temp = list;

    while (temp->next->next)
    {
        temp = temp->next;
    }

    LinkedList *node = allocate_list();
    node->item = item;
    node->next = NULL;
    temp->next = node;
    return list;
}

Ht_item *linkedlist_remove(LinkedList *list)
{
    // Removes the head from the LinkedList.
    // Returns the item of the popped element.
    if (!list)
        return NULL;

    if (!list->next)
        return NULL;

    LinkedList *node = list->next;
    LinkedList *temp = list;
    temp->next = NULL;
    list = node;
    Ht_item *it = NULL;
    memcpy(temp->item, it, sizeof(Ht_item));
    //free(temp->item->key);
    //free(temp->item->value);
    free(temp->item);
    free(temp);
    return it;
}

void free_linkedlist(LinkedList *list)
{
    LinkedList *temp = list;

    while (list)
    {
        temp = list;
        list = list->next;
        //free(temp->item->key);
        //free(temp->item->value);
        free(temp->item);
        free(temp);
    }
}

LinkedList **create_overflow_buckets(HashTable *table)
{
    // Create the overflow buckets; an array of LinkedLists.
    LinkedList **buckets = (LinkedList **)calloc(table->size, sizeof(LinkedList *));

    for (int i = 0; i < table->size; i++)
        buckets[i] = NULL;

    return buckets;
}

void free_overflow_buckets(HashTable *table)
{
    // Free all the overflow bucket lists.
    LinkedList **buckets = table->overflow_buckets;

    for (int i = 0; i < table->size; i++)
        free_linkedlist(buckets[i]);

    free(buckets);
}


Ht_item *create_item1(std::string key, int value)
{
    // Creates a pointer to a new HashTable item.
    Ht_item *item = new Ht_item;//(Ht_item *)malloc(sizeof(Ht_item));
    item->key = key;
    item->value = {value};
    return item;
}

Ht_item *create_item(std::string key, std::vector<int> value) {
    Ht_item *item = (Ht_item *)malloc(sizeof(Ht_item));
    //item->key = (char *)malloc(key.size() + 1);
    //item->value = (char *)malloc(value.size() + 1);
    item->key = key;
    item->value = value;
    return item;
}

HashTable *create_table(int size)
{
    // Creates a new HashTable.
    HashTable *table = (HashTable *)malloc(sizeof(HashTable));
    table->size = size;
    table->count = 0;
    table->items = (Ht_item **)calloc(table->size, sizeof(Ht_item *));

    for (int i = 0; i < table->size; i++)
        table->items[i] = NULL;

    table->overflow_buckets = create_overflow_buckets(table);

    return table;
}

void free_item(Ht_item *item)
{
    // Frees an item.
    //free(item->key);
    //free(item->value);
    free(item);
}

void free_table(HashTable *table)
{
    // Frees the table.
    for (int i = 0; i < table->size; i++)
    {
        Ht_item *item = table->items[i];
        if (item != NULL)
            free_item(item);
    }

    // Free the overflow bucket lists and its items.
    free_overflow_buckets(table);
    free(table->items);
    free(table);
}

void handle_collision(HashTable *table, Ht_item *indexItem, Ht_item *currentItem)
{

    // Collision handling will be dealt with this way:
    // We have a finite number of bins with which an item can be placed
    // But I have added a ht_item pointer object to each ht_item instance
    // So, if 2 items share the same index,
    // They can both reside in that index, and each new addition is daisy chained to the next one
    // Visual example provided in the print_table function. Easily visible if u significantly reduce MAXIMUM_CAPACITY

    if (indexItem->key == currentItem->key)
    {
        for (int i = 0; i < currentItem->value.size(); i++) {
            indexItem->value.push_back(currentItem->value[i]);
        }
        return;
    }
    else {
        if (indexItem->nextInIndex == NULL) {
            indexItem->nextInIndex = currentItem;
        }
        else {
            handle_collision(table, indexItem->nextInIndex, currentItem);
        }
    }
}

void ht_insert(HashTable *table, std::string key, int value)
{
    // Creates the item.
    Ht_item *item = create_item1(key, value);

    // Computes the index.
    int index = hash_function(key);

    Ht_item *current_item = table->items[index];

    if (current_item == NULL)
    {
        // Key does not exist.
        if (table->count == table->size)
        {
            // HashTable is full.
            std::cout << "Insert Error: Hash Table is full\n";
            free_item(item);
            return;
        }

        // Insert directly.
        table->items[index] = item;
        table->count++;
    }
    else
    {
        // Scenario 1: Update the value.
        if (current_item->key == key)
        {
            //table->items[index]->value = value;
            table->items[index]->value.push_back(value);
            return;
        }
        else
        {
            // Scenario 2: Handle the collision.
            handle_collision(table, table->items[index], item);
            return;
        }
    }
}

std::vector<int> ht_search(HashTable *table, std::string key)
{
    // Searches for the key in the HashTable.
    // Returns NULL if it doesn't exist.
    int index = hash_function(key);
    Ht_item *item = table->items[index];
    Ht_item *temp = item;
    LinkedList *head = table->overflow_buckets[index];

    // Provide only non-NULL values.
    if (item != NULL)
    {
        if (item->key == key) {
            return item->value;
        } 
        else {
            while(item->nextInIndex != NULL) {
                temp = temp->nextInIndex;
                if (temp->key == key) {
                    return item->value;
                }
            }
            return std::vector<int>(); //if we reach this point, they are looking for an item that is not present
        }

        if (head == NULL)
            return std::vector<int>();

        item = head->item;
        head = head->next;
    }

    return std::vector<int>();
}

std::vector<int> ht_search_AVX(HashTable *table, std::string key)
{
    unsigned long index = hash_function(key);

    // Search for the key using AVX-512 instructions.
    const int chunk_size = 64;  // Chunk size for AVX-512 operations.
    int key_size = key.size();
    int i = 0;
    while (i + chunk_size <= key_size)
    {
        __m512i key_chunk = _mm512_loadu_si512((__m512i*)&key[i]);
        for (int j = 0; j < table->size; j++)
        {
            item = table->items[j];
            while (item != NULL)
            {
                __m512i item_chunk = _mm512_loadu_si512((__m512i*)&item->key[i]);
                __mmask64 cmp_mask = _mm512_cmpeq_epi8_mask(key_chunk, item_chunk);
                if (cmp_mask != 0)
                    return item;
                item = item->nextInIndex;
            }
        }
        i += chunk_size;
    }

    // Key not found.
    return NULL;
}

void ht_delete(HashTable *table, std::string key)
{
    // Deletes an item from the table.
    int index = hash_function(key);
    Ht_item *item = table->items[index];
    LinkedList *head = table->overflow_buckets[index];

    if (item == NULL)
    {
        // Does not exist.
        return;
    }
    else
    {
        if (head == NULL && item->key == key)
        {
            // Collision chain does not exist.
            // Remove the item.
            // Set table index to NULL.
            table->items[index] = NULL;
            free_item(item);
            table->count--;
            return;
        }
        else if (head != NULL)
        {
            // Collision chain exists.
            if (item->key == key)
            {
                // Remove this item.
                // Set the head of the list as the new item.
                free_item(item);
                LinkedList *node = head;
                head = head->next;
                node->next = NULL;
                table->items[index] = create_item(node->item->key, node->item->value);
                free_linkedlist(node);
                table->overflow_buckets[index] = head;
                return;
            }

            LinkedList *curr = head;
            LinkedList *prev = NULL;

            while (curr)
            {
                if (curr->item->key == key)
                {
                    if (prev == NULL)
                    {
                        // First element of the chain.
                        // Remove the chain.
                        free_linkedlist(head);
                        table->overflow_buckets[index] = NULL;
                        return;
                    }
                    else
                    {
                        // This is somewhere in the chain.
                        prev->next = curr->next;
                        curr->next = NULL;
                        free_linkedlist(curr);
                        table->overflow_buckets[index] = head;
                        return;
                    }
                }

                curr = curr->next;
                prev = curr;
            }
        }
    }
}

void print_search(HashTable *table, std::string key)
{
    std::vector<int> val = ht_search(table, key);

    if (val.empty())
    {
        std::cout << "Key: '" << key << "' does not exist\n", key;
        return;
    }
    else
    {
        std::cout << "\nKey: " << key << std::endl;
        std::cout << "Values: ";
        for (int i = 0; i < val.size(); i++) {
            std::cout << val[i] << ", ";
        }
        std::cout << std::endl;
    }
}



void print_item(Ht_item *it) {
    std::cout << "Key: " << it->key << "-->";
}

void print_table(HashTable *table)
{
    std::cout << "\nHash Table\n-------------------\n";

    for (int i = 0; i < table -> size; i++)
    {
        if (table -> items[i])
        {
            std::cout << "Index: " << i << "\n\t";
            auto item = table->items[i];
            while (item != NULL) {
                print_item(item);
                item = item->nextInIndex;
            }
            std::cout << std::endl;
        }
    }

    std::cout << "-------------------\n\n";
}
std::string getKey(HashTable *table, int n) {
    return table->items[n]->key;
}


/*
int main()
{
    HashTable *ht = create_table(CAPACITY);
    ht_insert(ht, (char *)"1", (char *)"First address");
    ht_insert(ht, (char *)"2", (char *)"Second address");
    ht_insert(ht, (char *)"Hel", (char *)"Third address");
    ht_insert(ht, (char *)"Cau", (char *)"Fourth address");
    print_search(ht, (char *)"1");
    print_search(ht, (char *)"2");
    print_search(ht, (char *)"3");
    print_search(ht, (char *)"Hel");
    print_search(ht, (char *)"Cau"); // Collision!
    print_table(ht);
    ht_delete(ht, (char *)"1");
    ht_delete(ht, (char *)"Cau");
    print_table(ht);
    free_table(ht);
    return 0;
}
*/
