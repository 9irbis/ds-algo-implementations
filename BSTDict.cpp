// Implementation of a Dictionary(multimap) based on an underlying binary search tree

#include <iostream>
#include <string>
using namespace std;

// Exception class for invalid node in the tree
class InvalidPositionException{
};

// Proper binary tree implemented with a linked structure of nodes

template <typename E>
class LinkedBinaryTree{
private:
    struct Node{         
        E elem;        
        Node* parent;
        Node* left;
        Node* right;
        Node():elem(), parent(NULL), left(NULL), right(NULL) {}
    };

public:
    class Position{     // public position class encapsulating an underlying
                        // private node
    private:
        Node* v;
    public:
        Position(Node* u = NULL): v(u) {}
        E& operator*() const { return v->elem; }
        bool operator==(const Position& p) const { return v == p.v; }
        bool isRoot() const { return v->parent == NULL; }
        bool isExternal() const { return v->left == NULL && v->right == NULL;}
        Position parent() const { return Position(v->parent); }
        Position left() const { return Position(v->left); } 
        Position right() const { return Position(v->right); }

        friend class LinkedBinaryTree;
    };

public:
    LinkedBinaryTree();
    ~LinkedBinaryTree();
    int size() const;
    bool empty() const;
    Position root() const;
    void addRoot();
    void expandExternal(const Position& p);  // make the external node referenced by
                                             // position p internal by adding
                                             // two empty children
    Position removeAboveExternal(const Position& p); // remove the external node
                                                     // referenced by p along
                                                     // with its parent replacing the
                                                     // parent with its sibling

protected:
    void destroyMemory(const Position& p);           // utility function called
                                                     // by destructor

private:
    Node* _root;   // pointer to root node
    int n;         // no of nodes in the tree
};

// binary tree method implementations

template <typename E>
LinkedBinaryTree<E>::LinkedBinaryTree()
{
    _root = NULL;
    n = 0;
}

template <typename E>
LinkedBinaryTree<E>::~LinkedBinaryTree()
{
    destroyMemory(root());
}

template <typename E>
void LinkedBinaryTree<E>::destroyMemory(const Position& p)
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
    return n == 0;
}

template <typename E>
typename LinkedBinaryTree<E>::Position LinkedBinaryTree<E>::root() const
{
    return Position(_root);
}

template <typename E>
void LinkedBinaryTree<E>::addRoot() 
{
    _root = new Node;
    n = 1;
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
typename LinkedBinaryTree<E>::Position
LinkedBinaryTree<E>::removeAboveExternal(const Position& p) 
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
    n-= 2;
    return Position(sib);
}

// Exception object to throw when trying to erase a non existent key
class NonExistentElementException{
};

// Dictionary class definition 

template <typename K, typename V>   // key type K and value type V
class BSTDictionary{
private:
    class Entry{
    private:
        K _key;
        V _value;
    public:
        Entry(const K& k = K(), const V& v = V())
        {
            _key = k;
            _value = v;
        }
        const K& key() const {return _key;}
        const V& value() const {return _value;}
        void setKey(const K& k) {_key = k;} 
        void setValue(const V& v) {_value = v;}
    };

public:
    class Iterator;
    class Range;      
    BSTDictionary();
    int size() const;
    bool empty() const;
    Iterator find(const K& k);
    Range findAll(const K& k);  // find all entries with key k and return a
                                // range object of starting and ending iterators
    Iterator insert(const K& k, const V& x);
    void erase(const K& k) throw(NonExistentElementException);
    void erase(const Iterator& p);
    Iterator begin();
    Iterator end();

protected:
    typedef LinkedBinaryTree<Entry> BinaryTree;
    typedef typename BinaryTree::Position TPos;
    // helper methods
    TPos root() const;
    TPos finder(const K& k, const TPos& v);
    TPos inserter(const K& k, const V& x);
    TPos eraser(const TPos& v);

private:
    BinaryTree T;   // underlying proper binary tree
    int n;          // no of entries in the dictionary

public:
    class Iterator{
    private:
        TPos v;
    public:
        Iterator(const TPos& vv): v(vv) {}
        Entry& operator*() {return *v;}
        const Entry& operator*() const {return *v;}
        bool operator==(const Iterator& p) const { return v == p.v; }
        bool operator!=(const Iterator& p) const { return !(*this == p); }
        Iterator& operator++();
        friend class BSTDictionary;
    };

    class Range{
    private:
        Iterator _begin;
        Iterator _end;
    public:
        Range(const Iterator& b, const Iterator& e) : _begin(b), _end(e) {}
        Iterator& begin() { return _begin; }
        Iterator& end() { return _end; }
    };

};

// Iterator method implementations

template <typename K, typename V>
typename BSTDictionary<K,V>::Iterator&
BSTDictionary<K,V>::Iterator::operator++()  // advance iterator to the next node
                                            // in inorder traversal of the tree
{
    TPos w = v.right();
    if (!w.isExternal())    // If right child of current node is internal, then
                            // next visited node will be leftmost internal node in v's right subtree
    {
        do
        {
            v = w;
            w = w.left();
        } while(!w.isExternal());
    }
    else                    // otherwise next visited node depends on whether v
                            // was its parent's right or left child.
    {
        w = v.parent();
        while (v == w.right())  // as long as v is parent's right child, move
                                // upwards
        {
            v = w;
            w = w.parent();
        }
        v = w;
    }
    return *this;
}

// Dictionary method implementations
template <typename K, typename V>
BSTDictionary<K,V>::BSTDictionary() : T(), n(0)   
{
    T.addRoot();   // sentinel node called super root referenced by the end iterator.
    T.expandExternal(T.root());  // expand it so that left child can be
                                 // considered as actual root of our BST
}

template <typename K, typename V>
typename BSTDictionary<K,V>::TPos BSTDictionary<K,V>::root() const
{
    return T.root().left();
}

template <typename K, typename V>
typename BSTDictionary<K,V>::Iterator BSTDictionary<K,V>::begin() // first node
                                                                  // is leftmost node in the tree
{
    TPos v = root();
    while (!v.isExternal())
        v = v.left();
    return Iterator(v.parent());
}

template <typename K, typename V>
typename BSTDictionary<K,V>::Iterator BSTDictionary<K,V>::end()  // end iterator
                                                                 // means super root
{
    return Iterator(T.root());
}

template <typename K, typename V>
int BSTDictionary<K,V>::size() const
{
    return n;
}

template <typename K, typename V>
bool BSTDictionary<K,V>::empty() const
{
    return n == 0;
}

template <typename K, typename V>
typename BSTDictionary<K,V>::TPos BSTDictionary<K,V>::finder(const K& k, const TPos& v)
{
    if (v.isExternal()) return v;
    if (k < (*v).key()) return finder(k, v.left());
    else if ((*v).key() < k) return finder(k, v.right());
    else return v;
}

template <typename K, typename V>
typename BSTDictionary<K,V>::Iterator BSTDictionary<K,V>::find(const K& k)
{
    TPos w = finder(k, root());
    if (!w.isExternal()) return Iterator(w);
    else return end();
}

template <typename K, typename V>
typename BSTDictionary<K,V>::Range BSTDictionary<K,V>::findAll(const K& k)
{
    Iterator b = find(k);
    Iterator e = b;
    do
    {
        ++e;
    }while(e != end() && (*e).key() == k);
    return Range(b, e);
}

template <typename K, typename V>
typename BSTDictionary<K,V>::TPos BSTDictionary<K,V>::inserter(const K& k, const
V& x)
{
    TPos v = finder(k, root());
    while (!v.isExternal())
        v = finder(k, v.right());
    T.expandExternal(v);
    (*v).setKey(k);
    (*v).setValue(x);
    n++;
    return v;
}

template <typename K, typename V>
typename BSTDictionary<K,V>::Iterator BSTDictionary<K,V>::insert(const K& k, const V& x)
{
    TPos w = inserter(k, x);
    return Iterator(w);
}

template <typename K, typename V>
typename BSTDictionary<K,V>::TPos BSTDictionary<K,V>::eraser(const TPos& v)
{
    TPos w;
    if (v.left().isExternal()) w = v.left();
    else if (v.right().isExternal()) w = v.right();
    else
    {
        w = v.right();
        do
        {
            w = w.left();
        }while(!w.isExternal());
        TPos u = w.parent();
        (*v).setKey((*u).key()) ;
        (*v).setValue((*u).value());
    }
    n--;
    return T.removeAboveExternal(w);
}

template <typename K, typename V>
void BSTDictionary<K,V>::erase(const K& k) throw(NonExistentElementException)
{
    TPos w = finder(k, root());
    if (w.isExternal())
        throw NonExistentElementException();
    eraser(w);
}

template <typename K, typename V>
void BSTDictionary<K,V>::erase(const typename BSTDictionary<K,V>::Iterator& p)
{
    eraser(p.v);
}

int main()
{
    BSTDictionary<int, string> d;
    d.insert(1, "Nakul");
    d.insert(2, "Roman");
    d.insert(6, "Sean");
    d.insert(1, "Jessica");
    cout << "size of d is " << d.size() << endl;
    cout << d.empty() << endl;
    auto p = d.find(6);
    cout << "key = " << (*p).key() << ", value =  " << (*p).value() << endl;
    d.erase(p);
    d.erase(2);
    cout << d.size() << endl;
    d.insert(3, "Naveen");
    for(auto q = d.begin(); q != d.end(); ++q)
        cout << "Key = " << (*q).key() << ", Value = " << (*q).value() << endl;
    auto r = d.findAll(1);
    cout << "entries with key equal to 1 are: \n";
    for(auto rng = r.begin(); rng != r.end(); ++rng)
        cout << (*rng).value() << endl;
    return 0;
}
