// C++ implementation of a circularly linked list
#include <iostream>
#include <string>
using namespace std;

template <typename E>  // forward declaration
class CLinkedList;

template <typename E>
class CNode{           // class representing a node of the list 
private:
    E elem;            // actual element stored at this node
    CNode<E>* next;    // pointer to the next node
    friend class CLinkedList<E>;   // declare as friend to provide access to
                                   // private members
};

template <typename E>
class CLinkedList{     // class representing a circularly linked list
private:            
    CNode<E>* cursor;  // pointer to a node of the list
public:
    CLinkedList();
    ~CLinkedList();
    bool empty() const;  // return true if list is empty
    const E& back() const;  // return element at node pointed by the cursor
    const E& front() const; // return element at node after the cursor
    void advance();         // move the cursor forward one node
    void add(const E& e);   // add a new node after cursor and store e at it. If
                            // the list was empty, then set this node as the
                            // cursor and point its next member to itself.
    void remove();          // remove a node after the cursor, if cursor is the
                            // only node left, then remove it and set cursor
                            // member variable to NULL.
};

template <typename E>
CLinkedList<E>::CLinkedList()
{
    cursor = NULL;
}

template <typename E>
CLinkedList<E>::~CLinkedList()
{
    while (!empty())
        remove();
}

template <typename E>
bool CLinkedList<E>::empty() const
{
    return (cursor == NULL);
}

template <typename E>
const E& CLinkedList<E>::back() const
{
    return cursor->elem;
}

template <typename E>
const E& CLinkedList<E>::front() const
{
    return cursor->next->elem;
}

template <typename E>
void CLinkedList<E>::advance() 
{
    cursor = cursor->next;
}

template <typename E>
void CLinkedList<E>::add(const E& e)
{
    CNode<E>* v = new CNode<E>;
    v->elem = e;
    if (empty())
    {
        v->next = v;
        cursor = v;
    }
    else
    {
        v->next = cursor->next;
        cursor->next = v;
    }
}

template <typename E>
void CLinkedList<E>::remove()
{
    CNode<E>* old = cursor->next;
    if (old == cursor)
        cursor = NULL;
    else
        cursor->next = old->next;
    delete old;
}

int main()
{
    CLinkedList<string> playList;    // empty list created []
    playList.add("Worst behaviour"); // [Worst behaviour*] (* indicates cursor
                                     // node)
    playList.add("Headlines");       // [Worst behaviour*, Headlines]
    playList.add("Grown simba");     // [Worst behaviour*, Grown simba,
                                     // Headlines]
    playList.advance();  // [Worst behaviour, Grown simba*, Headlines] 
    playList.advance();  // [Worst behaviour, Grown simba, Headlines*]
    playList.remove();   // [Grown simba, Headlines*]
    playList.add("Swimming pools"); // [Grown simba, Headlines*, Swimming pools]
    while(!playList.empty())        // should print Swimming pools, Grown simba
                                    // and Headlines in order
    {
        cout << playList.front() << endl;
        playList.remove();
    }
    return 0;
}   
