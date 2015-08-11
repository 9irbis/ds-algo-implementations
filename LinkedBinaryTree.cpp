// A complete binary tree implementation using a linked structure

#include <iostream>
#include <list>
#include <string>
using namespace std;

class TreeEmptyException{
};

class InvalidPositionException{ 
};

template <typename E>
class LinkedBinaryTree{
private:              // private struct for internal use
    struct Node{      // nested node struct representing a single node of tree
        E elem;       // value stored at this node 
        Node* parent; // pointer to parent node
        Node* left;   // pointer to left child node
        Node* right;  // pointer to right child node
        Node(): elem(), parent(NULL), left(NULL), right(NULL) {}
    };

public:                
    class Position{   // public position class representing an underlying node
    private:
        Node* v;      // pointer to underlying node
        Position(Node* _v = NULL): v(_v) {}
    public:
        E& operator*();  // overloaded dereferencing operator to get/set value stored at
                         // this position
        Position left() const;   // returns position obj representing left child node
        Position right() const;  // returns position obj representing right child node
        Position parent() const; // returns position obj representing parent node
        bool isRoot() const;     // returns true if current position is root
        bool isExternal() const; // returns true if current position is an
                                 // external node
        friend class LinkedBinaryTree<E>; // declare as friend to provide access
                                          // to private members
    };

private:
    Node* _root;   // pointer to root node
    int n;         // total no of nodes in the tree
public:
    LinkedBinaryTree(): _root(NULL), n(0) {}
    ~LinkedBinaryTree();
    int size() const;
    bool empty() const;
    Position root() const;
    void addRoot();
    list<Position> positions() const; // returns a list of all the positions of
                                      // the tree
    void expandExternal(const Position& p); // make external node into internal
                                            // by adding two children nodes
    Position removeAboveExternal(const Position& p); // remove external node and
                         // its parent and place its sibling at its parent's position
protected:
    void preorder(Node* v, list<Position>& pl) const; // utility function for
                                     // generating positions in preorder traversal fashion
    void destroyMemory(const Position& p); // utility function used by
                                           // destructor to free memory
};

// Position class method definitions 

template <typename E>
E& LinkedBinaryTree<E>::Position::operator*()
{
    return v->elem;
}

template <typename E>
typename LinkedBinaryTree<E>::Position LinkedBinaryTree<E>::Position::left() const
{
    return Position(v->left);
}

template <typename E>
typename LinkedBinaryTree<E>::Position LinkedBinaryTree<E>::Position::right() const
{
    return Position(v->right);
}

template <typename E>
typename LinkedBinaryTree<E>::Position LinkedBinaryTree<E>::Position::parent() const
{
    return Position(v->parent);
}

template <typename E>
bool LinkedBinaryTree<E>::Position::isRoot() const
{
    return (v->parent == NULL);
}

template <typename E>
bool LinkedBinaryTree<E>::Position::isExternal() const
{
    return (v->left == NULL && v->right == NULL);
}

// LinkedBinaryTree class method definitions

template <typename E>
LinkedBinaryTree<E>::~LinkedBinaryTree()
{
    destroyMemory(root());
}

template <typename E>
void LinkedBinaryTree<E>::destroyMemory(const LinkedBinaryTree<E>::Position& p)
{
    if (!p.isExternal())
    {
        destroyMemory(p.left());
        destroyMemory(p.right());
    }
    delete p.v;
}

template <typename E>
int LinkedBinaryTree<E>::size() const
{
    return n;
}

template <typename E>
bool LinkedBinaryTree<E>::empty() const
{
    return (n == 0);
}

template <typename E>
typename LinkedBinaryTree<E>::Position LinkedBinaryTree<E>::root() const
{
    if (empty())
        throw TreeEmptyException();
    return Position(_root);
}

template <typename E>
void LinkedBinaryTree<E>::addRoot() 
{
    if (empty())
    {
        _root = new Node;
        n = 1;
    }
    else
        cout << "Root already exists!\n";
}

template <typename E>
void LinkedBinaryTree<E>::expandExternal(const Position& p)
{
    if (!p.isExternal())
        throw InvalidPositionException();
    Node* v = p.v;
    v->left = new Node;
    v->right = new Node;
    v->left->parent = v;
    v->right->parent = v;
    n += 2;
}

template <typename E>
typename LinkedBinaryTree<E>::Position LinkedBinaryTree<E>::removeAboveExternal(const Position& p)
{
    if (!p.isExternal() || p.isRoot())
        throw InvalidPositionException();
    Node* v = p.v;
    Node* par = v->parent;
    Node* sib = (par->left == v ? par->right : par->left);
    if (par == _root)
    {
        _root = sib;
        sib->parent = NULL;
    }
    else
    {
        Node* gpar = par->parent;
        sib->parent = gpar;
        if (gpar->left == par) gpar->left = sib;
        else gpar->right = sib;
    }
    delete v;
    delete par;
    n -= 2;
    return Position(sib);
}

template <typename E>
void LinkedBinaryTree<E>::preorder(Node* v, list<LinkedBinaryTree<E>::Position>& pl) const
{
    pl.push_back(Position(v));
    if (v->left != NULL)
        preorder(v->left, pl);
    if (v->right != NULL)
        preorder(v->right, pl);
}

template <typename E>
list<typename LinkedBinaryTree<E>::Position> LinkedBinaryTree<E>::positions() const
{
    if (empty()) 
        throw TreeEmptyException();
    list<Position> pl;
    preorder(_root, pl);
    return pl;
}

int main()
{
    LinkedBinaryTree<string> org;
    typedef LinkedBinaryTree<string>::Position pos;
    org.addRoot();
    pos r = org.root();
    *(r) = "Sales";
    org.expandExternal(r);
    *(r.left()) = "N America";
    *(r.right()) = "Europe";
    org.expandExternal(r.right());
    *(r.right().left()) = "Western";
    *(r.right().right()) = "Eastern";
    cout << org.size() << endl;
    for (auto p: org.positions())
        cout <<*p << " -- ";
    cout << endl;
    if (r.isRoot())
        cout << "r is root\n";
    if (r.left().isExternal())
        cout << "left child of root is external\n";
    cout << "Parent of node with value eastern is " <<
    *(r.right().right().parent()) << endl;
    org.removeAboveExternal(r.left());
    r = org.root();
    cout << "new root is " << *r << endl;
    cout << "Now size is " << org.size() << endl;
    return 0;
}
