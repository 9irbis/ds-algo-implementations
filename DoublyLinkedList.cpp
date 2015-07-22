// C++ implementation of a doubly linked list

#include <iostream>
#include <string>

using namespace std;

/* change the following type definition to get a doubly linked list holding 
   some type other than string. I Could have used templated class to implement 
   a truly generic list but I wanted to avoid the notational overhead.
*/
typedef string Elem;

class DNode{        // class representing an individual node of the list
private:        
    Elem elem;      // actual element stored at this node
    DNode* prev;    // pointer to previous node
    DNode* next;    // pointer to next node
    friend class DLinkedList;  // declare list class as friend for private member
                               // access
};

class DLinkedList{     // class representing a doubly linked list 
private:               
    DNode* header;     // pointer to header dummy node used to simplify operations
    DNode* trailer;    // pointer to trailer dummy node used to simplify operations
public:
    DLinkedList();
    ~DLinkedList();
    bool empty() const;           // return true if list is empty
    const Elem& front() const;    // return value stored at front node
    const Elem& back() const;     // return value stored at back node
    void addFront(const Elem& e);
    void addBack(const Elem& e);
    void removeFront();
    void removeBack();
protected:                       // protected methods for internal use
    void add(DNode* v, const Elem& e);  // add e before node v
    void remove(DNode* v);              // remove node v
};

DLinkedList::DLinkedList()
{
    header = new DNode;
    trailer = new DNode;
    header->next = trailer;
    trailer->prev = header;
}

DLinkedList::~DLinkedList()
{
    while(!empty())
        removeFront();
    delete header;
    delete trailer;
}

bool DLinkedList::empty() const
{
    return (header->next == trailer);    // If there are no nodes b/w header and
                                         // trailer, the list is empty
}

const Elem& DLinkedList::front() const
{
    return header->next->elem;
}

const Elem& DLinkedList::back() const
{
    return trailer->prev->elem;
}

void DLinkedList::add(DNode* v, const Elem& e)  // add e before node v
{
    DNode* u = new DNode;
    u->elem = e;
    u->next = v;
    u->prev = v->prev;
    v->prev->next = v->prev = u;
}

void DLinkedList::addFront(const Elem& e)
{
    add(header->next, e);
}

void DLinkedList::addBack(const Elem& e)
{
    add(trailer, e);
}

void DLinkedList::remove(DNode* v)      // remove node pointed by v
{
    DNode* u = v->prev;
    DNode* w = v->next;
    u->next = w;
    w->prev = u;
    delete v;
}

void DLinkedList::removeFront()
{
    remove(header->next);
}

void DLinkedList::removeBack()
{
    remove(trailer->prev);
}

int main()
{
    DLinkedList legends;
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
