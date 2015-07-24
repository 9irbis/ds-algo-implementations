// Implementing a queue by using an underlying dynamic array in a circular
// manner

#include <iostream>
#include <string>

using namespace std;

class RuntimeException{
private:
    string errMsg;
public:
    RuntimeException(const string& err): errMsg(err) {}
    string getMessage() const {return errMsg;}
};

class QueueEmpty: public RuntimeException{
public:
    QueueEmpty(const string& err): RuntimeException(err) {}
};

class QueueFull: public RuntimeException{
public:
    QueueFull(const string& err): RuntimeException(err) {}
};

template <typename E>
class ArrayQueue{
private:
    E* Q;          // underlying dynamic array
    int capacity;  // total capacity of the queue
    int f;         // index of front
    int r;         // index of position one after rear
    int n;         // number of items currently in the queue
public:
    ArrayQueue(int cap = 100);
    ~ArrayQueue();
    int size() const;
    bool empty() const;
    const E& front() const throw(QueueEmpty);
    void enqueue(const E& e) throw(QueueFull);
    void dequeue() throw(QueueEmpty);
};

template <typename E>
ArrayQueue<E>::ArrayQueue(int cap)
{
    Q = new E[cap];
    capacity = cap;
    f = 0;
    r = 0;
    n = 0;
}

template <typename E>
ArrayQueue<E>::~ArrayQueue()
{
    delete [] Q;
}

template <typename E>
int ArrayQueue<E>::size() const
{
    return n;
}

template <typename E>
bool ArrayQueue<E>::empty() const
{
    return (n == 0);
}

template <typename E>
const E& ArrayQueue<E>::front() const throw(QueueEmpty)
{
    if (empty())
        throw QueueEmpty("Queue is empty!");
    return Q[f];
}

template <typename E>
void ArrayQueue<E>::enqueue(const E& e) throw(QueueFull)
{
    if (size() == capacity)
        throw QueueFull("Queue is full!");
    Q[r] = e;
    r = (r+1)%capacity;    // advance r, wrap if goes past the end
    n++;
}

template <typename E>
void ArrayQueue<E>::dequeue() throw(QueueEmpty)
{
    if (empty())
        throw QueueEmpty("Queue is empty!");
    f = (f+1)%capacity;
    n--;
}
int main()
{
    ArrayQueue<string> names;
    names.enqueue("Novak Djokovic");
    names.enqueue("Roger Federer");
    names.enqueue("Andy Murray");
    while (!names.empty())
    {
        cout << names.size() << endl;
        cout << names.front() << endl;
        names.dequeue();
    }
    return 0;
}
