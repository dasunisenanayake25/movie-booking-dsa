#ifndef BSTMOVIETITLE_H
#define BSTMOVIETITLE_H

#include <algorithm>
#include <cctype>
#include <string>

class BSTMovieTitle
{
public:
    struct Node
    {
        int movieId;
        std::string title;
        std::string titleLower;
        Node *left;
        Node *right;
        Node *parent;
    };

private:
    Node *root;
    int size;

    void clearNode(Node *node);
    void inOrderFill(Node *node, int outIds[], int &count) const;

    std::string toLower(std::string s) const{
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c){
            return static_cast<char>(std::tolower(c));
        });
        return s;
    }
public:
    BSTMovieTitle();
    ~BSTMovieTitle();

    void insert(int movieId, const std::string &title);
    int searchExact(const std::string &title) const;
    void inOrderList(int outIds[], int &count) const;
    void clear();

    int getSize() const;
};

#endif