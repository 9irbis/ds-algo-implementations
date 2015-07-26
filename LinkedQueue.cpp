// Implementation of a queue with a circularly linked list as the underlying
// data member

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

// Exception class for an empty queue
class QueueEmpty{
private:
    string errMsg;
public:
    QueueEmpty(const string& err): errMsg(err) {}
    string getMessage() const {return errMsg;}
};

// Queue class begins

template <typename E>
class LinkedQueue{
private:
    CLinkedList<E> C;  // underlying linked list with cursor as rear and
                       // following element as front of the queue
    int n;             // size of queue
public:
    LinkedQueue();
    int size() const;
    bool empty() const;
    const E& front() const throw(QueueEmpty);
    void enqueue(const E& e);
    void dequeue() throw(QueueEmpty);
};

template <typename E>
LinkedQueue<E>::LinkedQueue()
: C(), n(0) {}

template <typename E>
int LinkedQueue<E>::size() const
{
    return n;
}

template <typename E>
bool LinkedQueue<E>::empty() const
{
    return (n == 0);
}

template <typename E>
const E& LinkedQueue<E>::front() const throw(QueueEmpty)
{
    if (empty())
        throw QueueEmpty("Queue is empty!");
    return C.front();
}

template <typename E>
void LinkedQueue<E>::enqueue(const E& e)
{
    C.add(e);
    C.advance();
    n++;
}

template <typename E>
void LinkedQueue<E>::dequeue() throw(QueueEmpty)
{
    if (empty())
        throw QueueEmpty("Queue is empty!");
    else
    {
        C.remove();
        n--;
    }
}

int main()
{
    LinkedQueue<string> frontmen;
    frontmen.enqueue("Dave Grohl");
    frontmen.enqueue("Bono");
    frontmen.enqueue("Chester Bennington");
    while (!frontmen.empty())
    {
        cout << frontmen.front() << endl;
        cout << frontmen.size() << endl;
        frontmen.dequeue();
    }
    return 0;
}
