// An ordered dictionary implementation based on an underlying AVL tree

#include <iostream>
#include <string>
#include <algorithm>
using namespace std;


class NonExistantKeyException{
};

class InvalidIteratorException{
};

class EmptyTreeException{
};

class InvalidPositionException{
};

// definition of a binary tree class with a linked structure of nodes
template <typename E>
class BinaryTree{
private:
    struct Node{
        E e;
        Node* parent;
        Node* left;
        Node* right;
        Node():e(), parent(NULL), left(NULL), right(NULL) {}
    };

public:
    class Position{
    private:
        Node* v;
    public:
        Position(Node* vv = NULL):v(vv) {}
        E& operator*() const {return v->e;}
        bool operator==(const Position& p) const {return v == p.v;}
        bool operator!=(const Position& p) const {return !(*this == p);}
        bool isRoot() const {return v->parent == NULL;}
        bool isExternal() const {return v->left == NULL && v->right == NULL;}
        Position parent() const {return Position(v->parent);}
        Position left() const {return Position(v->left);}
        Position right() const {return Position(v->right);}
        int height() const
        {
            if (isExternal())
                return 0;
            else
                return 1 + max(left().height(), right().height());
        }
        friend class BinaryTree<E>;
    };

public:
    BinaryTree():_root(NULL), n(0) {}
    ~BinaryTree();
    int size() const;
    bool empty() const;
    Position root() const;
    void addRoot();
    void expandExternal(const Position& p);
    Position removeAboveExternal(const Position& p);
    void restructure(const Position& p);

protected:
    void destroyMemory(Position p);

private:
    Node* _root;
    int n;
};

// binary tree class methods implementation
template <typename E>
void BinaryTree<E>::destroyMemory(Position p)
{
    if (!p.isExternal())
    {
        destroyMemory(p.left());
        destroyMemory(p.right());
    }
    delete p.v;
}

template <typename E>
BinaryTree<E>::~BinaryTree()
{
    if (!empty())
        destroyMemory(root());
}

template <typename E>
int BinaryTree<E>::size() const
{
    return n;
}

template <typename E>
bool BinaryTree<E>::empty() const
{
    return n == 0;
}

template <typename E>
typename BinaryTree<E>::Position BinaryTree<E>::root() const
{
    if (_root != NULL)
        return Position(_root);
    else
        throw EmptyTreeException();
}

template <typename E>
void BinaryTree<E>::addRoot()
{
    _root = new Node;
    n = 1;
}

template <typename E>
void BinaryTree<E>::expandExternal(const Position& p)
{
    if (!p.isExternal())
        throw InvalidPositionException();
    p.v->left = new Node;
    p.v->right = new Node;
    p.v->left->parent = p.v;
    p.v->right->parent = p.v;
    n += 2;
}

template <typename E>
typename BinaryTree<E>::Position BinaryTree<E>::removeAboveExternal(const Position& p)
{
    if (!p.isExternal())
        throw InvalidPositionException();
    Position par = p.parent();
    Position sib = (par.left() == p ? par.right() : par.left());
    if (par == root())
    {
        sib.v->parent = NULL;
    }
    else
    {
        Position gpar = par.parent();
        sib.v->parent = gpar.v;
        if (gpar.left() == par) gpar.v->left = sib.v;
        else gpar.v->right = sib.v;
    }
    delete par.v;
    delete p.v;
    n -= 2;
    return sib;
}

template <typename E>
void BinaryTree<E>::restructure(const Position& p)  // trinode restructuring
{
    Position x, y, z, a, b, c, t0, t1, t2, t3;
    z = p;
    y = (z.left().height() > z.right().height() ? z.left() : z.right());
    if (y.left().height() > y.right().height())
        x = y.left();
    else if (y.right().height() > y.left().height())
        x = y.right();
    else
        x = (z.left() == y ? y.left() : y.right());
    if (y == z.right())
    {
        a = z;
        if (x == y.right())
        {
            b = y;
            c = x;
        }
        else
        {
            b = x;
            c = y;
        }
    }
    else
    {
        c = z;
        if (x == y.right())
        {
            a = y;
            b = x;
        }
        else
        {
            a = x;
            b = y;
        }
    }
    t0 = a.left();
    t3 = c.right();
    if (a.right() != b && a.right() != c)
        t1 = a.right();
    else
        t1 = b.left();
    if (c.left() != b && c.left() != a)
        t2 = c.left();
    else
        t2 = b.right();
    b.v->parent = z.v->parent;
    if (!z.isRoot())
    {
        Position par = z.parent();
        if (par.left() == z) par.v->left = b.v;
        else par.v->right = b.v;
    }
    a.v->parent = b.v;
    c.v->parent = b.v;
    b.v->left = a.v;
    b.v->right = c.v;
    a.v->left = t0.v;
    a.v->right = t1.v;
    t0.v->parent = t1.v->parent = a.v;
    c.v->left = t2.v;
    c.v->right = t3.v;
    t2.v->parent = t3.v->parent = c.v;
}

// AVL Dictionary class
template <typename K, typename V>
class AVLDictionary{
public:
    class Entry{
    private:
        K _key;
        V _value;
    public:
        Entry(const K& k = K(), const V& v = V()): _key(k), _value(v) {}
        const K& key() const {return _key;}
        const V& value() const {return _value;}
        void setKey(const K& k) {_key = k;}
        void setValue(const V& v) {_value = v;}
    };

protected:
    typedef BinaryTree<Entry> BT;
    typedef typename BT::Position TPos;
    TPos finder(const K& k, const TPos& v);
    TPos inserter(const K& k, const V& x);
    TPos eraser(TPos& v);
    bool isBalanced(const TPos& v) const;
    void rebalance(const TPos& v);
    TPos root() const;

public:
    class Iterator;
    class Range;
    AVLDictionary();
    int size() const;
    bool empty() const;
    Iterator find(const K& k);
    Range findAll(const K& k);
    Iterator insert(const K& k, const V& v);
    void erase(const K& k) throw(NonExistantKeyException);
    void erase(const Iterator& p) throw(InvalidIteratorException);
    Iterator begin();
    Iterator end();

public:
    class Iterator{
    private:
        TPos v;
    public:
        Iterator(const TPos& vv) : v(vv) {}
        Entry& operator*() {return *v;}
        bool operator==(const Iterator& p) const {return v == p.v;}
        bool operator!=(const Iterator& p) const {return !(*this == p);}
        Iterator& operator++();
        friend class AVLDictionary<K,V>;
    };

    class Range{
    private:
        Iterator _begin;
        Iterator _end;
    public:
        Range(const Iterator& b, const Iterator& e): _begin(b), _end(e) {}
        Iterator& begin() {return _begin;}
        Iterator& end() {return _end;}
    };

private:
    BT T;
    int n;
};

// Iterator class methods implementation
template <typename K, typename V>
typename AVLDictionary<K,V>::Iterator&
AVLDictionary<K,V>::Iterator::operator++()
{
    TPos w = v.right();
    if (!w.isExternal())
    {
        do{
            v = w;
            w = w.left();
        }while(!w.isExternal());
    }
    else
    {
        w = v.parent();
        while (v == w.right())
        {
            v = w;
            w = w.parent();
        }
        v = w;
    }
    return *this;
}

// AVLDictionary class methods implementation
template <typename K, typename V>
AVLDictionary<K,V>::AVLDictionary():T(), n(0)
{
    T.addRoot();
    T.expandExternal(T.root());
}

template <typename K, typename V>
typename AVLDictionary<K,V>::TPos AVLDictionary<K,V>::root() const
{
    return T.root().left();
}

template <typename K, typename V>
int AVLDictionary<K,V>::size() const
{
    return n;
}

template <typename K, typename V>
bool AVLDictionary<K,V>::empty() const
{
    return n==0;
}

template <typename K, typename V>
typename AVLDictionary<K,V>::Iterator AVLDictionary<K,V>::begin()
{
    TPos w = root();
    while (!w.isExternal())
    {
        w = w.left();
    }
    return Iterator(w.parent());
}

template <typename K, typename V>
typename AVLDictionary<K,V>::Iterator AVLDictionary<K,V>::end()
{
    return Iterator(T.root());
}

template <typename K, typename V>
typename AVLDictionary<K,V>::TPos AVLDictionary<K,V>::finder(const K& k, const
TPos& v)
{
    if (v.isExternal())
        return v;
    if (k < (*v).key())
        return finder(k, v.left());
    else if ((*v).key() < k)
        return finder(k, v.right());
    else
        return v;
}

template <typename K, typename V>
typename AVLDictionary<K,V>::Iterator AVLDictionary<K,V>::find(const K& k)
{
    TPos w = finder(k, root());
    if (!w.isExternal())
        return Iterator(w);
    else
        return end();
}

template <typename K, typename V>
typename AVLDictionary<K,V>::Range AVLDictionary<K,V>::findAll(const K& k)
{
    Iterator b = find(k);
    Iterator e = b;
    while (e != end() && (*e).key() == k)
        ++e;
    return Range(b, e);
}

template <typename K, typename V>
bool AVLDictionary<K,V>::isBalanced(const TPos& p) const
{
    int bal = p.left().height() - p.right().height();
    return -1 <= bal && bal <= 1;
}

template <typename K, typename V>
void AVLDictionary<K,V>::rebalance(const TPos& v)
{
    TPos z = v;
    while (!(z == root()))
    {
        z = z.parent();
        if (!isBalanced(z))
            T.restructure(z);
    }
}

template <typename K, typename V>
typename AVLDictionary<K,V>::TPos AVLDictionary<K,V>::inserter(const K& k, const
V& x)
{
    TPos w = finder(k, root());
    while (!w.isExternal())
        w = finder(k, w.right());
    T.expandExternal(w);
    (*w).setKey(k);
    (*w).setValue(x);
    n++;
    return w;
}

template <typename K, typename V>
typename AVLDictionary<K,V>::Iterator AVLDictionary<K,V>::insert(const K& k,
const V& x)
{
    TPos w = inserter(k, x);
    rebalance(w);
    return Iterator(w);
}

template <typename K, typename V>
typename AVLDictionary<K,V>::TPos AVLDictionary<K,V>::eraser(TPos& v)
{
    TPos w;
    if (v.left().isExternal()) w = v.left();
    else if (v.right().isExternal()) w = v.right();
    else
    {
        w = v.right();
        do{
            w = w.left();
        }while(!w.isExternal());
        TPos u = w.parent();
        (*v).setKey((*u).key());
        (*v).setValue((*u).value());
    }
    n--;
    return T.removeAboveExternal(w);
}

template <typename K, typename V>
void AVLDictionary<K,V>::erase(const K& k) throw(NonExistantKeyException)
{
    TPos w = finder(k, root());
    if (w.isExternal())
        throw NonExistantKeyException();
    w = eraser(w);
    rebalance(w);
}

template <typename K, typename V>
void AVLDictionary<K,V>::erase(const Iterator& p) throw(InvalidIteratorException)
{
    if (p == end())
        throw InvalidIteratorException();
    TPos v = p.v;
    TPos w = eraser(v);
    rebalance(w);
}

// testing our data structure
int main()
{
    AVLDictionary<int, string> d;
    d.insert(1, "Nakul");
    d.insert(4, "Mark");
    d.insert(2, "Edward");
    cout << d.size() << endl;
    cout << d.empty() << endl;
    auto p = d.find(1);
    cout << (*p).key() << ", " << (*p).value() << endl;
    d.erase(p);
    cout << d.size() << endl;
    d.erase(2);
    cout << d.size() << endl;
    d.insert(1, "Naveen");
    d.insert(6, "Jitu");
    d.insert(3, "Yogi");
    d.insert(4, "Mandal");
    d.insert(3, "Rastogi");
    d.insert(5, "Shailendra");
    d.insert(3, "Bhatia");
    for (auto i = d.begin(); i != d.end(); ++i)
        cout << (*i).key() << ", " << (*i).value() << endl;
    auto r = d.findAll(3);
    cout << "Entries with key 3 are:\n";
    for (auto q = r.begin(); q != r.end(); ++q)
        cout << (*q).key() << ", " << (*q).value() << endl;
    return 0;
}
