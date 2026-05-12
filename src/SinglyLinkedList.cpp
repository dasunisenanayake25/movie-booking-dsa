#include <cstddef>
#include "SinglyLinkedListInt.h"

SinglyLinkedListInt::SinglyLinkedListInt()
{
    head = NULL;
    tail = NULL;
    size = 0;
}

SinglyLinkedListInt::~SinglyLinkedListInt()
{
    clear();
}

void SinglyLinkedListInt::insertLast(int value)
{
    Node *newNode = new Node;
    newNode->value = value;
    newNode->next = NULL;

    if (head == NULL)
    {
        head = newNode;
        tail = newNode;
    }
    else
    {
        tail->next = newNode;
        tail = newNode;
    }

    size++;
}

int SinglyLinkedListInt::removeValue(int value)
{
    Node *current = head;
    Node *previous = NULL;

    while (current != NULL)
    {
        if (current->value == value)
        {
            if (previous == NULL)
            {
                head = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            if (current == tail)
            {
                tail = previous;
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

int SinglyLinkedListInt::contains(int value) const
{
    Node *current = head;
    while (current != NULL)
    {
        if (current->value == value)
        {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void SinglyLinkedListInt::print() const
{
    Node *current = head;
    if (current == NULL)
    {
        cout << "(empty)";
    }

    while (current != NULL)
    {
        cout << current->value;
        if (current->next != NULL)
        {
            cout << " -> ";
        }
        current = current->next;
    }
    cout << endl;
}

void SinglyLinkedListInt::clear()
{
    Node *current = head;
    Node *nextNode;

    while (current != NULL)
    {
        nextNode = current->next;
        delete current;
        current = nextNode;
    }

    head = NULL;
    tail = NULL;
    size = 0;
}

SinglyLinkedListInt::Node *SinglyLinkedListInt::getHead() const
{
    return head;
}

int SinglyLinkedListInt::getSize() const
{
    return size;
}