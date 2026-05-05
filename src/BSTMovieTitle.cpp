#include <cstring>

#include "BSTMovieTitle.h"

BSTMovieTitle::BSTMovieTitle()
{
    root = NULL;
    size = 0;
}

BSTMovieTitle::~BSTMovieTitle()
{
    clear();
}

void BSTMovieTitle::insert(int movieId, const std::string &title)
{
    Node *newNode = new Node;
    newNode->movieId = movieId;
    newNode->title = title;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;

    if (root == NULL)
    {
        root = newNode;
        size++;
        return;
    }

    Node *current = root;
    Node *parentNode = NULL;
    int compareResult = 0;

    while (current != NULL)
    {
        parentNode = current;
        if (title < current->title)
            compareResult = -1;
        else if (title > current->title)
            compareResult = 1;
        else
            compareResult = 0;

        if (compareResult < 0)
        {
            current = current->left;
        }
        else if (compareResult > 0)
        {
            current = current->right;
        }
        else
        {
            // Duplicate title: reject the insert to keep the BST consistent.
            delete newNode;
            return;
        }
    }

    newNode->parent = parentNode;
    if (compareResult < 0)
    {
        parentNode->left = newNode;
    }
    else
    {
        parentNode->right = newNode;
    }

    size++;
}

int BSTMovieTitle::searchExact(const std::string &title) const
{
    Node *current = root;
    while (current != NULL)
    {
        if (title == current->title)
        {
            return current->movieId;
        }
        if (title < current->title)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }

    return -1;
}

void BSTMovieTitle::inOrderList(int outIds[], int &count) const
{
    count = 0;
    inOrderFill(root, outIds, count);
}

void BSTMovieTitle::clear()
{
    clearNode(root);
    root = NULL;
    size = 0;
}

int BSTMovieTitle::getSize() const
{
    return size;
}

void BSTMovieTitle::clearNode(Node *node)
{
    if (node == NULL)
    {
        return;
    }

    clearNode(node->left);
    clearNode(node->right);
    delete node;
}

void BSTMovieTitle::inOrderFill(Node *node, int outIds[], int &count) const
{
    if (node == NULL)
    {
        return;
    }

    inOrderFill(node->left, outIds, count);
    outIds[count] = node->movieId;
    count++;
    inOrderFill(node->right, outIds, count);
}
