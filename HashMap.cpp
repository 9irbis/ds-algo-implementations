// A c++ implementation of a map using a hash function and a bucket array
// with separate chaining for efficiency

#include <iostream>
#include <vector>
#include <list>
#include <string>

using namespace std;

class InvalidKeyException{
};

template <typename K, typename V, typename H>
class HashMap{
public:
    class Entry{
    private:
        K _key;
        V _value;
    public:
        Entry(const K& k, const V& v) : _key(k), _value(v) {}
        K key() const {return _key;}
        V value() const {return _value;}
        void setValue(const V& v1) {_value = v1;}
    };

    class Iterator;

protected:
    typedef list<Entry> Bucket;
    typedef vector<Bucket> BktArray;
    typedef typename BktArray::iterator BItor;
    typedef typename Bucket::iterator EItor;
    Iterator finder(const K& k);
    Iterator inserter(const Iterator& p, const Entry& e);
    void eraser(const Iterator& p);
    static void nextEntry(Iterator& p)
    {++(p.ent);}
    static bool endOfBkt(const Iterator& p)
    {return p.ent == p.bkt->end();}


public:
    class Iterator{
    private:
        const BktArray* ba;
        BItor bkt;
        EItor ent;
    public:
        Iterator(const BktArray& a, const BItor& b, const EItor& q = EItor())
            : ba(&a), bkt(b), ent(q) {}
        Entry& operator*() const;
        bool operator==(const Iterator& p) const;
        bool operator!=(const Iterator& p) const;
        Iterator& operator++();
        friend class HashMap;
    };

public:
    HashMap(int capacity = 101);
    int size() const;
    bool empty() const;
    Iterator find(const K& k);
    Iterator put(const K& k, const V& v);
    void erase(const K& k) throw(InvalidKeyException);
    void erase(const Iterator& p);
    Iterator begin();
    Iterator end();

private:
    int n;
    H hash;
    BktArray B;
};

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Entry& HashMap<K,V,H>::Iterator::operator*() const
{
    return *ent;
}

template <typename K, typename V, typename H>
bool HashMap<K,V,H>::Iterator::operator==(const Iterator& p) const
{
    if (ba != p.ba || bkt != p.bkt) return false;
    else if (bkt == ba->end()) return true;
    else return (ent == p.ent);
}

template <typename K, typename V, typename H>
bool HashMap<K,V,H>::Iterator::operator!=(const Iterator& p) const
{
    return !(*this == p);
}

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator& HashMap<K,V,H>::Iterator::operator++()
{
    ++ent;
    if (endOfBkt(*this))
    {
        ++bkt;
        while (bkt != ba->end() && bkt->empty()) 
            ++bkt;
        if (bkt == ba->end()) return *this;
        ent = bkt->begin();
    }
    return *this;
}

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator HashMap<K,V,H>::end()
{
    return Iterator(B, B.end());
}

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator HashMap<K,V,H>::begin()
{
    if (empty()) return end();
    BItor bkt = B.begin();
    while (bkt->empty()) 
        ++bkt;
    return Iterator(B, bkt, bkt->begin());
}

template <typename K, typename V, typename H>
HashMap<K,V,H>::HashMap(int capacity) : n(0), B(capacity) {}

template <typename K, typename V, typename H>
int HashMap<K,V,H>::size() const
{
    return n;
}

template <typename K, typename V, typename H>
bool HashMap<K,V,H>::empty() const
{
    return size() == 0;
}

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator HashMap<K,V,H>::finder(const K& k)
{
    int i = hash(k) % B.size();
    BItor bkt = B.begin() + i;
    Iterator p(B, bkt, bkt->begin());
    while (!endOfBkt(p) && (*p).key() != k)
        nextEntry(p);
    return p;
}

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator HashMap<K,V,H>::find(const K& k)
{
    Iterator p = finder(k);
    if (endOfBkt(p))
        return end();
    else
        return p;
}

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator HashMap<K,V,H>::inserter(const Iterator& p,
const Entry& e)
{
    EItor ins = p.bkt->insert(p.ent, e);
    n++;
    return Iterator(B, p.bkt, ins);
}

template <typename K, typename V, typename H>
typename HashMap<K,V,H>::Iterator HashMap<K,V,H>::put(const K& k, const V& v)
{
    Iterator p = finder(k);
    if (endOfBkt(p))
        return inserter(p, Entry(k, v));
    else
        p.ent->setValue(v);
        return p;
}

template <typename K, typename V, typename H>
void HashMap<K,V,H>::eraser(const Iterator& p)
{
    p.bkt->erase(p.ent);
    n--;
}

template <typename K, typename V, typename H>
void HashMap<K,V,H>::erase(const Iterator& p)
{
    eraser(p);
}

template <typename K, typename V, typename H>
void HashMap<K,V,H>::erase(const K& k) throw(InvalidKeyException)
{
    Iterator p = finder(k);
    if (endOfBkt(p))
        throw InvalidKeyException();
    eraser(p);
}

template <typename K, typename V, typename H>
class HashDict : public HashMap<K,V,H>{
public:
    typedef typename HashMap<K,V,H>::Iterator Iterator;
    typedef typename HashMap<K,V,H>::Entry Entry;
    class Range{
    private:
        Iterator _begin;
        Iterator _end;
    public:
        Range(const Iterator& b, const Iterator& e) : _begin(b), _end(e) {}
        Iterator& begin() {return _begin;}
        Iterator& end() {return _end;}
    };

public:
    HashDict(int capacity = 101);
    Iterator insert(const K& k, const V& v);
    Range findAll(const K& k);
};

template <typename K, typename V, typename H>
HashDict<K,V,H>::HashDict(int capacity) : HashMap<K,V,H>(capacity) {}  

template <typename K, typename V, typename H>
typename HashDict<K,V,H>::Iterator HashDict<K,V,H>::insert(const K& k, const V& v)
{ 
    Iterator p = this->finder(k);
    Iterator q = this->inserter(p, Entry(k, v));
    return q;
}

template <typename K, typename V, typename H>
typename HashDict<K,V,H>::Range HashDict<K,V,H>::findAll(const K& k)
{
    Iterator b = this->finder(k);
    Iterator p = b;
    while (!this->endOfBkt(p) && (*p).key() == k)
        ++p;
    return Range(b, p);
}
class HashFunc{
public:
    int operator()(int val)
    {
        return val;
    }
};

int main()
{
    HashMap<int, string, HashFunc> m;
    cout << m.size() << endl;
    if (m.empty())
        cout << "Map is empty!" << endl;
    m.put(0, "Nakul");
    m.put(5, "Keith");
    m.put(2, "Andrew");
    cout << "now size is " << m.size() << endl;
    for (auto p = m.begin(); p != m.end(); ++p)
        cout << (*p).key() << " " << (*p).value() << endl;
    auto q = m.find(0);
    cout << (*q).key() << " " << (*q).value() << endl;
    auto r = m.find(13);
    if (r == m.end())
        cout << "find works correctly" << endl;
    m.erase(q);
    m.erase(2);
    cout << m.size() << endl;
    auto p = m.begin();
    cout << (*p).key() << " " << (*p).value() << endl;
    cout << "--------------------------Dictionary Testing----------------------------\n";
    HashDict<int, string, HashFunc> d;
    cout << d.size() << endl;
    if (d.empty())
        cout << "Dictionary is empty!\n";
    d.insert(3, "Roman");
    d.insert(3, "Mike");
    auto range = d.findAll(3);
    for (auto p = d.begin(); p != d.end(); ++p)
        cout << (*p).key() << " : " << (*p) .value() << endl;
    return 0;
}
