// Implementation of a stack using a singly linked list
#include <iostream>
#include <string>
using namespace std;

// Singly linked list
template <typename E>  // forward declaration of a linked list class
class SLinkedList;

template <typename E>
class SNode{
private:
    E elem;
    SNode<E>* next;
    friend class SLinkedList<E>;
};

template <typename E>
class SLinkedList{
private:
    SNode<E>* head;
public:
    SLinkedList();
    ~SLinkedList();
    bool empty() const;
    const E& front() const;
    void addFront(const E& e);
    void removeFront();
};

template <typename E>
SLinkedList<E>::SLinkedList()
: head(nullptr) {}

template <typename E>
SLinkedList<E>::~SLinkedList()
{
    while (!empty()) 
        removeFront();
}

template <typename E>
bool SLinkedList<E>::empty() const
{
    return (head == nullptr);
}

template <typename E>
const E& SLinkedList<E>::front() const
{
    return head->elem;
}

template <typename E>
void SLinkedList<E>::addFront(const E& e)
{
    SNode<E>* v = new SNode<E>;
    v->elem = e;
    v->next = head;
    head = v;
}

template <typename E>
void SLinkedList<E>::removeFront()
{
    SNode<E>* old = head;
    head = head->next;
    delete old;
}

class StackEmpty{    // Exception to raise in case of accessing an empty stack
private:
    string errMsg;
public:
    StackEmpty(const string& err): errMsg(err) {}
    string getMessage() const {return errMsg;}
};

// following definition is of a class representing a stack
template <typename E>
class LinkedStack{
private:
    SLinkedList<E> S; // underlying linked list whose front is top of the stack
    int sz;           // number of items in the stack
public:
    LinkedStack();
    int size() const;
    bool empty() const;
    const E& top() const throw(StackEmpty);
    void push(const E& e);
    void pop() throw(StackEmpty);
};

template <typename E>
LinkedStack<E>::LinkedStack(): S(), sz(0) {}

template <typename E>
int LinkedStack<E>::size() const
{
    return sz;
}

template <typename E>
bool LinkedStack<E>::empty() const
{
    return (sz == 0);
}

template <typename E>
const E& LinkedStack<E>::top() const throw(StackEmpty)
{
    if (empty())
        throw StackEmpty("Stack is empty!");
    return S.front();
}

template <typename E>
void LinkedStack<E>::push(const E& e)
{
    S.addFront(e);
    sz++;
}

template <typename E>
void LinkedStack<E>::pop() throw(StackEmpty)
{
    if (empty())
        throw StackEmpty("Stack is empty!");
    sz--;
    S.removeFront();
}

int main()
{
    LinkedStack<string> bands;
    bands.push("Led Zeppelin");
    bands.push("Metallica");
    bands.push("Pink Floyd");
    while (!bands.empty())
    {
        cout << bands.size() << endl << bands.top() << endl;
        bands.pop();
    }
    return 0;
}
