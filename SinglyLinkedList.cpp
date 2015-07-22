// An implementation of a generic singly linked list

#include <iostream>
#include <string>
using namespace std;

template <typename E>     // Forward declaration of linkedlist class
class SLinkedList;

template <typename E>
class SNode{              // Class representing an individual node in the list
private:                   
    E elem;               // Actual element stored at this node 
    SNode<E>* next;       // Pointer to the next node
    friend class SLinkedList<E>;  // Declare the actual list as friend class so it 
};                            // can access private members elem and next

template <typename E>
class SLinkedList{        // Class representing a singly linked list
private:
    SNode<E>* head;       // Pointer to the first element of the list
public:
    SLinkedList();        // Empty list constructor
    ~SLinkedList();       // Destructor
    bool empty() const;   // Return true if list is empty
    const E& front() const;  // Return the front element
    void addFront(const E& e);  // Add to front of the list
    void removeFront();         // Remove the front element
};

template <typename E>
SLinkedList<E>::SLinkedList()
: head(NULL) {}

template <typename E>
SLinkedList<E>::~SLinkedList()
{
    while (!empty()) 
        removeFront();
}

template <typename E>
bool SLinkedList<E>::empty() const
{
    return (head == NULL);
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

int main()
{
    SLinkedList<string> actors;
    actors.addFront("Al Pacino");
    actors.addFront("Robert De Niro");
    while(!actors.empty())
    {
        cout << actors.front() << endl;
        actors.removeFront();
    }
    return 0;
}
