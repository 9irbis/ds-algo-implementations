// C++ implementation of a doubly linked list

#include <iostream>
#include <string>

using namespace std;

class RuntimeException{    //  general base exception class
private:
    string errMsg;
public:
    RuntimeException(const string& err): errMsg(err) {}
    string getMessage() const {return errMsg;}
};

class ListEmpty: public RuntimeException{   // exception to throw when trying to
                                            // access or remove an item in an empty list
public:
    ListEmpty(const string& s): RuntimeException(s) {}
};

template <typename E>  // forward declaration of linked list class
class DLinkedList;

template <typename E>
class DNode{        // class representing an individual node of the list
private:        
    E elem;      // actual element stored at this node
    DNode<E>* prev;    // pointer to previous node
    DNode<E>* next;    // pointer to next node
    friend class DLinkedList<E>;  // declare list class as friend for private member
                               // access
};


template <typename E>
class DLinkedList{     // class representing a doubly linked list 
private:               
    DNode<E>* header;     // pointer to header dummy node used to simplify operations
    DNode<E>* trailer;    // pointer to trailer dummy node used to simplify operations
public:
    DLinkedList();
    ~DLinkedList();
    bool empty() const;           // return true if list is empty
    const E& front() const throw(ListEmpty);    // return value stored at front node
    const E& back() const throw(ListEmpty);     // return value stored at back node
    void addFront(const E& e);
    void addBack(const E& e);
    void removeFront() throw(ListEmpty);
    void removeBack() throw(ListEmpty);
protected:                       // protected methods for internal use
    void add(DNode<E>* v, const E& e);  // add e before node v
    void remove(DNode<E>* v);              // remove node v
};


template <typename E>
DLinkedList<E>::DLinkedList()
{
    header = new DNode<E>;
    trailer = new DNode<E>;
    header->next = trailer;
    trailer->prev = header;
}

template <typename E>
DLinkedList<E>::~DLinkedList()
{
    while(!empty())
        removeFront();
    delete header;
    delete trailer;
}

template <typename E>
bool DLinkedList<E>::empty() const
{
    return (header->next == trailer);    // If there are no nodes b/w header and
                                         // trailer, the list is empty
}

template <typename E>
const E& DLinkedList<E>::front() const throw(ListEmpty)
{
    if (empty())
        throw ListEmpty("List is empty!");
    return header->next->elem;
}

template <typename E>
const E& DLinkedList<E>::back() const throw(ListEmpty)
{  
    if (empty())
        throw ListEmpty("List is empty!");
    return trailer->prev->elem;
}

template <typename E>
void DLinkedList<E>::add(DNode<E>* v, const E& e)  // add e before node v
{
    DNode<E>* u = new DNode<E>;
    u->elem = e;
    u->next = v;
    u->prev = v->prev;
    v->prev->next = v->prev = u;
}

template <typename E>
void DLinkedList<E>::addFront(const E& e)
{
    add(header->next, e);
}

template <typename E>
void DLinkedList<E>::addBack(const E& e)
{
    add(trailer, e);
}

template <typename E>
void DLinkedList<E>::remove(DNode<E>* v)      // remove node pointed by v
{
    DNode<E>* u = v->prev;
    DNode<E>* w = v->next;
    u->next = w;
    w->prev = u;
    delete v;
}

template <typename E>
void DLinkedList<E>::removeFront() throw(ListEmpty)
{
    if (empty())
        throw ListEmpty("List is empty!");
    remove(header->next);
}

template <typename E>
void DLinkedList<E>::removeBack() throw(ListEmpty)
{
    if (empty())
        throw ListEmpty("List is empty!");
    remove(trailer->prev);
}

int main()
{
    DLinkedList<string> legends;
    legends.addFront("Pele");
    legends.addFront("Maradona");
    legends.addBack("Messi");
    while (!legends.empty())
    {
        cout << legends.back() << endl;
        legends.removeBack();
    }
    return 0;
}
