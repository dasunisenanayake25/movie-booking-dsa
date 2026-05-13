#include <cstddef>
#include "HashTableIntToIndex.h"

HashTableIntToIndex::HashTableIntToIndex()
{
    buckets = NULL;
    bucketCount = 0;
    size = 0;
}

HashTableIntToIndex::~HashTableIntToIndex()
{
    clear();
}

void HashTableIntToIndex::init(int bucketCountValue)
{
    clear();

    bucketCount = bucketCountValue;
    if (bucketCount > 0)
    {
        buckets = new Node *[bucketCount];
        for (int i = 0; i < bucketCount; i++)
        {
            buckets[i] = NULL;
        }
    }

    size = 0;
}

void HashTableIntToIndex::put(int key, int index)
{
    if (buckets == NULL || bucketCount == 0)
    {
        return;
    }

    int bucketIndex = hash(key);
    Node *current = buckets[bucketIndex];

    while (current != NULL)
    {
        if (current->key == key)
        {
            current->index = index;
            return;
        }
        current = current->next;
    }

    Node *newNode = new Node;
    newNode->key = key;
    newNode->index = index;
    newNode->next = buckets[bucketIndex];
    buckets[bucketIndex] = newNode;
    size++;
}

int HashTableIntToIndex::get(int key, int &outIndex) const
{
    if (buckets == NULL || bucketCount == 0)
    {
        return 0;
    }

    int bucketIndex = hash(key);
    Node *current = buckets[bucketIndex];

    while (current != NULL)
    {
        if (current->key == key)
        {
            outIndex = current->index;
            return 1;
        }
        current = current->next;
    }

    return 0;
}

int HashTableIntToIndex::remove(int key)
{
    if (buckets == NULL || bucketCount == 0)
    {
        return 0;
    }

    int bucketIndex = hash(key);
    Node *current = buckets[bucketIndex];
    Node *previous = NULL;

    while (current != NULL)
    {
        if (current->key == key)
        {
            if (previous == NULL)
            {
                buckets[bucketIndex] = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            delete current;
            size--;
            return 1;
        }

        previous = current;
        current = current->next;
    }

    return 0;
}

void HashTableIntToIndex::clear()
{
    if (buckets == NULL)
    {
        bucketCount = 0;
        size = 0;
        return;
    }

    for (int i = 0; i < bucketCount; i++)
    {
        Node *current = buckets[i];
        Node *nextNode;

        while (current != NULL)
        {
            nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    delete[] buckets;
    buckets = NULL;
    bucketCount = 0;
    size = 0;
}

int HashTableIntToIndex::getSize() const
{
    return size;
}

int HashTableIntToIndex::hash(int key) const
{
    if (key < 0)
    {
        key = -key;
    }
    return key % bucketCount;
}