#include <iostream>
#include <string>
using namespace std;

class RuntimeException{
private:
    string errMsg;
public:
    RuntimeException(const string& err) : errMsg(err) {}
    string getMessage() const {return errMsg;}
};

class StackEmpty : public RuntimeException{
public:
    StackEmpty(const string& err) : RuntimeException(err) {}
};

class StackFull : public RuntimeException{
public:
    StackFull(const string& err) : RuntimeException(err) {}
};

template <typename E>
class ArrayStack{
private:
    E* S;          // underlying array representing the stack
    int capacity;  //  total capacity of stack
    int t;  // index of top element
public:
    ArrayStack(int cap = 100);
    ~ArrayStack();
    int size() const;
    bool empty() const;
    const E& top() const throw(StackEmpty);
    void push(const E& e) throw(StackFull);
    void pop() throw(StackEmpty);
};

template <typename E>
ArrayStack<E>::ArrayStack(int cap)
{
    capacity = cap;
    t = -1;
    S = new E[cap];
}

template <typename E>
ArrayStack<E>::~ArrayStack()
{
    delete [] S;
}

template <typename E>
int ArrayStack<E>::size() const
{
    return t+1;
}

template <typename E>
bool ArrayStack<E>::empty() const
{
    return (t == -1);
}

template <typename E>
const E& ArrayStack<E>::top() const throw(StackEmpty)
{
    if (empty())
        throw StackEmpty("Stack is empty!");
    return S[t];
}

template <typename E>
void ArrayStack<E>::push(const E& e) throw(StackFull)
{
    if (size() == capacity)
        throw StackFull("Stack is full!");
    S[++t] = e;
}

template <typename E>
void ArrayStack<E>::pop() throw(StackEmpty)
{
    if (empty())
        throw StackEmpty("Stack is empty!");
    t--;
}

int main()
{
    ArrayStack<int> stats;
    stats.push(34);
    stats.push(56);
    stats.push(90);
    while (!stats.empty())
    {
        cout << stats.size() << endl << stats.top() << endl;
        stats.pop();
    }
}
