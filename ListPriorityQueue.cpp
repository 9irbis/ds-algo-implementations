// Implementation of a priority queue using an underlying sorted STL list

#include <iostream>
#include <list>
using namespace std;

// Exception class to throw in case of trying to access an element of an empty
// priority queue
class QueueEmpty{
};

// priority queue class definition
template <typename E, typename C>  // E is element type and C is comparator type
class ListPriorityQueue{
private:
    list<E> L;   // underlying STL list
    C isLess;    // comparator object which defines a total order relation and
                 // implements a comparision function
public:
    int size() const;   // returns no of elements in Priority Queue
    bool isEmpty() const;  // returns true if queue is empty
    void insert(const E& e);   // insert e into queue while keeping it sorted
    const E& min() const throw(QueueEmpty);  // return a reference to the
                                             // element with the smallest key
    void removeMin() throw(QueueEmpty);   // remove the element returned by min
};

template <typename E, typename C>
int ListPriorityQueue<E,C>::size() const
{
    return L.size();
}

template <typename E, typename C>
bool ListPriorityQueue<E,C>::isEmpty() const
{
    return L.empty();
}

template <typename E, typename C>
void ListPriorityQueue<E,C>::insert(const E& e) 
{
    typename list<E>::iterator p;
    p = L.begin();
    while (p != L.end() && !isLess(e, *p))  
        p++;
    L.insert(p, e);
}

template <typename E, typename C>
const E& ListPriorityQueue<E,C>::min() const throw(QueueEmpty)
{
    if (isEmpty())
        throw QueueEmpty();
    return L.front();
}

template <typename E, typename C>
void ListPriorityQueue<E,C>::removeMin() throw(QueueEmpty)
{
    if (isEmpty())
        throw QueueEmpty();
    L.pop_front();
}

// a structure representing a point in a 2d plane
struct Point2D{
    double X;
    double Y;
    Point2D(double a, double b): X(a), Y(b) {}
    double getX() const {return X;}
    double getY() const {return Y;}
};

// Comparator class which provides a comparision method for sorting the point
// objects from left to right position in the 2d plane
class LeftRight{
public:
    bool operator()(const Point2D& p, const Point2D& q)
    {
        return (p.getX() < q.getX());
    }
};

int main()
{
    ListPriorityQueue<Point2D, LeftRight> points;
    points.insert(Point2D(3.5, 5.1));
    points.insert(Point2D(8.1, 6.7));
    points.insert(Point2D(1.5, 0.3));
    cout << points.size() << endl;
    while (!points.isEmpty())
    {
        Point2D p = points.min();
        cout << "X = " << p.getX() << ", Y = " << p.getY() << endl;
        points.removeMin();
    }
    return 0;
}
