#include <iostream>
using namespace std;

template <typename E>
class NodeList{

private:
    struct Node{
        E elem;
        Node* prev;
        Node* next;
    };

public:
    class Iterator{
        private:
            Node* v;
            Iterator(Node* u);
        public:
            E& operator*();
            bool operator==(const Iterator& p) const;
            bool operator!=(const Iterator& p) const;
            Iterator& operator++();
            Iterator& operator--();
            friend class NodeList<E>;
    };

private:
    int n;
    Node* header;
    Node* trailer;

public:
    NodeList();
    ~NodeList();
    int size() const;
    bool empty() const;
    Iterator begin() const;
    Iterator end() const;
    void insertFront(const E& e);
    void insertBack(const E& e);
    void insert(const Iterator& p, const E& e);
    void eraseFront();
    void eraseBack();
    void erase(const Iterator& p);
};

template <typename E>
NodeList<E>::Iterator::Iterator(Node* u)
{
    v = u;
}

template<typename E>
E& NodeList<E>::Iterator::operator*()
{
    return v->elem;
}

template <typename E>
bool NodeList<E>::Iterator::operator==(const Iterator& p) const
{
    return (v == p.v);
}

template <typename E>
bool NodeList<E>::Iterator::operator!=(const Iterator& p) const
{
    return (v != p.v);
}

template <typename E>
typename NodeList<E>::Iterator& NodeList<E>::Iterator::operator++()
{
    v = v->next;
    return *this;
}

template <typename E>
typename NodeList<E>::Iterator& NodeList<E>::Iterator::operator--()
{
    v = v->prev;
    return *this;
}

template <typename E>
NodeList<E>::NodeList()
{
    n = 0;
    header = new Node;
    trailer = new Node;
    header->next = trailer;
    trailer->prev = header;
}

template <typename E>
NodeList<E>::~NodeList()
{
    while (!empty())
        eraseFront();
    delete header;
    delete trailer;
}

template <typename E>
int NodeList<E>::size() const
{
    return n;
}

template <typename E>
bool NodeList<E>::empty() const
{
    return (n == 0);
}

template <typename E>
typename NodeList<E>::Iterator NodeList<E>::begin() const
{
    return (Iterator(header->next));
}

template <typename E>
typename NodeList<E>::Iterator NodeList<E>::end() const
{
    return (Iterator(trailer));
}

template <typename E>
void NodeList<E>::insert(const Iterator& p, const E& e)
{
    Node* v = p.v;
    Node* u = v->prev;
    Node* x = new Node;
    x->elem = e;
    x->next = v;
    x->prev = u;
    u->next = v->prev = x;
    n++;
}

template <typename E>
void NodeList<E>::insertFront(const E& e)
{
    insert(Iterator(header->next), e);
}

template <typename E>
void NodeList<E>::insertBack(const E& e)
{
    insert(Iterator(trailer), e);
}

template <typename E>
void NodeList<E>::erase(const Iterator& p)
{
    Node* v = p.v;
    Node* u = v->prev;
    Node* w = v->next;
    u->next = w;
    w->prev = u;
    delete v;
    n--;
}

template <typename E>
void NodeList<E>::eraseFront()
{
    erase(Iterator(header->next));
}

template <typename E>
void NodeList<E>::eraseBack()
{
    erase(Iterator(trailer->prev));
}

int main()
{
    NodeList<int> scores;
    scores.insertBack(45);
    scores.insertBack(62);
    scores.insertFront(0);
    cout << scores.size() << endl;
    for(NodeList<int>::Iterator it = scores.begin(); it!=scores.end(); ++it)
        cout << *it << ", ";
    cout << endl;
    while (!scores.empty())
    {
        cout << *(scores.begin()) << endl;
        scores.eraseFront();
    }
    return 0;
}
