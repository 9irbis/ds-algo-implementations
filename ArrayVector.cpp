// Implementation of a vector with an underlying extendable array

#include <iostream>
using namespace std;

// Exception class to throw in case of out of bounds indexing
class IndexOutOfBoundsException{
private:
    string errMsg;
public:
    IndexOutOfBoundsException(const string& err) : errMsg(err){}
};

template <typename E>
class ArrayVector{
private:
    E* A;  // array that holds vector elements
    int capacity;  // capacity of the array
    int n;   // no. of elements in the vector ie size
public:
    ArrayVector();
    ArrayVector(const ArrayVector<E>& v);
    ~ArrayVector();
    int size() const;
    bool empty() const;
    E& operator[](int i);  // overloaded index operator
    E& at(int i) throw(IndexOutOfBoundsException); // returns reference to
                                                   // element at index i
    void remove(int i) throw(IndexOutOfBoundsException); // remove element at i
    void reserve(int N);  // reserve capacity for atleast N elements
    void insert(int i, const E& e) throw(IndexOutOfBoundsException); // insert element with value e at index i
};

template <typename E>
ArrayVector<E>::ArrayVector()
: capacity(0), n(0), A(NULL) {}

template <typename E>
ArrayVector<E>::ArrayVector(const ArrayVector<E>& v)  // copy constructor
{
    capacity = v.capacity;
    n = v.n;
    for(int j=0; j<v.n; j++)
    {
        A[j] = v.A[j];
    }
}

template <typename E>
ArrayVector<E>::~ArrayVector()
{
    if (A != NULL)
        delete [] A;
}

template <typename E>
int ArrayVector<E>::size() const
{
    return n;
}

template <typename E>
bool ArrayVector<E>::empty() const
{
    return (n == 0);
}

template <typename E>
E& ArrayVector<E>::operator[](int i) 
{
    return A[i];
}

template <typename E>
E& ArrayVector<E>::at(int i) throw(IndexOutOfBoundsException)
{
    if (i < 0 || i > n-1)
        throw IndexOutOfBoundsException("Index is out of bounds.");
    return A[i];
}

template <typename E>
void ArrayVector<E>::remove(int i) throw(IndexOutOfBoundsException)
{
    if (i < 0 || i > n-1)
        throw IndexOutOfBoundsException("Index is out of bounds.");
    for(int j=i+1; j<n; j++)
        A[j-1] = A[j];
    n--;
}

template <typename E>
void ArrayVector<E>::reserve(int N)
{
    if (capacity >= N)
        return;
    E* B = new E[N];
    for(int j=0; j<n; j++)
        B[j] = A[j];
    if (A != NULL)
        delete [] A;
    A = B;
    capacity = N;
}

template <typename E>
void ArrayVector<E>::insert(int i, const E& e) throw(IndexOutOfBoundsException)
{
    if (i < 0 || i > n)
        throw IndexOutOfBoundsException("Index is out of bounds.");
    if(n == capacity)
        reserve(max(1, 2*capacity));
    for(int j=n-1; j>=i; j--)
        A[j+1] = A[j];
    A[i] = e;
    n++;
}

int main()
{
    ArrayVector<int> scores;
    scores.insert(0, 34);
    scores.insert(1, 8);
    scores.insert(2, 56);
    scores.insert(3, 104);
    scores.insert(4, 87);
    cout << scores.size() << endl;
    cout << scores.at(3)<< endl;
    cout << scores[0] << endl;
    while (!scores.empty())
    {
        cout << scores[0] << endl;
        scores.remove(0);
    }
    return 0;
}
