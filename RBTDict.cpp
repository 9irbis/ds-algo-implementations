// An ordered dictionary implementation based on an underlying red black tree

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
    Position restructure(const Position& p);

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
typename BinaryTree<E>::Position BinaryTree<E>::restructure(const Position& p)  // trinode restructuring
{
    Position x, y, z, a, b, c, t0, t1, t2, t3;
    x = p;
    y = x.parent();
    z = y.parent();
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
    return b;
}

// Dictionary class definition

enum Color {RED, BLACK};

template <typename K, typename V>
class RBTDictionary{
public:
    class Entry{
    private:
        K _key;
        V _value;
        Color col;
    public:
        Entry(const K& k = K(), const V& v = V())
        :_key(k), _value(v), col(BLACK) {}
        const K& key() const {return _key;}
        const V& value() const {return _value;}
    protected:
        void setKey(const K& k) {_key = k;}
        void setValue(const V& v) {_value = v;}
        Color color() const {return col;}
        void setColor(Color c) {col = c;}
        bool isRed() const {return col == RED;}
        bool isBlack() const {return col == BLACK;}
        friend class RBTDictionary<K,V>;
    };

protected:
    typedef BinaryTree<Entry> BT;
    typedef typename BT::Position TPos;
    TPos finder(const K& k, const TPos& v);
    TPos inserter(const K& k, const V& x);
    TPos eraser(TPos& v);
    TPos root() const;
    TPos sibling(const TPos& p)
    {
        if (p.isRoot())
            throw InvalidPositionException();
        TPos v = p.parent();
        return (v.left() == p ? v.right() : v.left());
    }
    void setBlack(const TPos& p) {(*p).setColor(BLACK);}
    void setRed(const TPos& p) {(*p).setColor(RED);}
    void remedyDoubleRed(const TPos& z);
    void remedyDoubleBlack(const TPos& r);

public:
    class Iterator;
    RBTDictionary();
    int size() const;
    bool empty() const;
    Iterator find(const K& k);
    Iterator insert(const K& k, const V& x);
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
        friend class RBTDictionary<K,V>;
    };

private:
    BT T;
    int n;
};

// Iterator class methods implementation
template <typename K, typename V>
typename RBTDictionary<K,V>::Iterator&
RBTDictionary<K,V>::Iterator::operator++()
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

// RBTDictionary class methods implementation
template <typename K, typename V>
RBTDictionary<K,V>::RBTDictionary():T(), n(0)
{
    T.addRoot();
    T.expandExternal(T.root());
}

template <typename K, typename V>
typename RBTDictionary<K,V>::TPos RBTDictionary<K,V>::root() const
{
    return T.root().left();
}

template <typename K, typename V>
int RBTDictionary<K,V>::size() const
{
    return n;
}

template <typename K, typename V>
bool RBTDictionary<K,V>::empty() const
{
    return n==0;
}

template <typename K, typename V>
typename RBTDictionary<K,V>::Iterator RBTDictionary<K,V>::begin()
{
    TPos w = root();
    while (!w.isExternal())
    {
        w = w.left();
    }
    return Iterator(w.parent());
}

template <typename K, typename V>
typename RBTDictionary<K,V>::Iterator RBTDictionary<K,V>::end()
{
    return Iterator(T.root());
}

template <typename K, typename V>
typename RBTDictionary<K,V>::TPos RBTDictionary<K,V>::finder(const K& k, const
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
typename RBTDictionary<K,V>::Iterator RBTDictionary<K,V>::find(const K& k)
{
    TPos w = finder(k, root());
    if (!w.isExternal())
        return Iterator(w);
    else
        return end();
}

template <typename K, typename V>
typename RBTDictionary<K,V>::TPos RBTDictionary<K,V>::inserter(const K& k, const
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
void RBTDictionary<K,V>::remedyDoubleRed(const TPos& z)
{
    TPos v = z.parent();
    if ((*v).isBlack())
        return;
    if ((*(sibling(v))).isBlack())
    {
        v = T.restructure(z);
        setBlack(v);
        setRed(v.left());
        setRed(v.right());
    }
    else
    {
        setBlack(v);
        setBlack(sibling(v));
        TPos u = v.parent();
        if (u == root())
            return;
        setRed(u);
        remedyDoubleRed(u);
    }
}

template <typename K, typename V>
typename RBTDictionary<K,V>::Iterator RBTDictionary<K,V>::insert(const K& k,
const V& x)
{
    TPos v = inserter(k, x);
    if (v == root())
        setBlack(v);
    else
    {
        setRed(v);
        remedyDoubleRed(v);
    }
    return Iterator(v);
}

template <typename K, typename V>
typename RBTDictionary<K,V>::TPos RBTDictionary<K,V>::eraser(TPos& v)
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
void RBTDictionary<K,V>::remedyDoubleBlack(const TPos& r)
{
    TPos x = r.parent();
    TPos y = sibling(r);
    if ((*y).isBlack())
    {
        if ((*(y.left())).isRed() || (*(y.right())).isRed())
        {
            TPos z = ((*(y.left())).isRed() ? y.left() : y.right());
            Color topColor = (*x).color();
            z = T.restructure(z);
            (*z).setColor(topColor);
            setBlack(z.left());
            setBlack(z.right());
            setBlack(r);
        }
        else
        {
            setBlack(r);
            setRed(y);
            if (x == root() || (*x).isRed())
                setBlack(x);
            else
                remedyDoubleBlack(x);
        }
    }
    else
    {
        TPos z = (y == x.left() ? y.left() : y.right());
        T.restructure(z);
        setRed(x);
        setBlack(y);
        remedyDoubleBlack(r);
     }
}

template <typename K, typename V>
void RBTDictionary<K,V>::erase(const K& k) throw(NonExistantKeyException)
{
    TPos v = finder(k, root());
    if (v.isExternal())
        throw NonExistantKeyException();
    Color oldCol;
    if (v.left().isExternal() || v.right().isExternal())
        oldCol = (*v).color();
    else
    {
        TPos w = v.right();
        do{
            w = w.left();
        }while(!w.isExternal());
        TPos u = w.parent();
        oldCol = (*u).color();
    }
    TPos r = eraser(v);
    if (r == root() || (*r).isRed() || oldCol == RED)
        setBlack(r);   
    else
        remedyDoubleBlack(r);
}

template <typename K, typename V>
void RBTDictionary<K,V>::erase(const Iterator& p) throw(InvalidIteratorException)
{
    if (p == end())
        throw InvalidIteratorException();
    TPos v = p.v;
    Color oldCol;
    if (v.left().isExternal() || v.right().isExternal())
        oldCol = (*v).color();
    else
    {
        TPos w = v.right();
        do{
            w = w.left();
        }while(!w.isExternal());
        TPos u = w.parent();
        oldCol = (*u).color();
    }
    TPos r = eraser(v);
    if (r == root() || (*r).isRed() || oldCol == RED)
        setBlack(r);   
    else
        remedyDoubleBlack(r);
}

int main()
{
    RBTDictionary<int, string> d;
    cout << d.size() << endl;
    if (d.empty())
        cout << "d is empty!\n";
    d.insert(1, "Nakul");
    d.insert(5, "Mesut");
    d.insert(3, "Mathieu");
    cout << "now size is " << d.size() << endl;
    auto p = d.find(3);
    cout << (*p).key() << ", " << (*p).value() << endl;
    d.erase(p);
    cout << "and now size is " << d.size() << endl;
    d.insert(7, "Roman");
    d.insert(2, "Adnan");
    d.insert(3, "Jorge");
    d.insert(1, "Carlito");
    d.insert(1, "Aleksander");
    cout << "new size is " << d.size() << endl;
    d.erase(7);
    cout << "Roman's gone, now size is " << d.size() << endl;
    cout << "All entries are:\n";
    for(auto q = d.begin(); q != d.end(); ++q)
        cout << (*q).key() << ", " << (*q).value() << endl;
    return 0;
}
