// Implementation of a priority queue using an underlying heap

#include <iostream>
#include <vector>
using namespace std;

// complete binary tree class used as a heap
template <typename E>
class CompleteBinaryTree{
private:
    vector<E> v;
public:
    typedef typename vector<E>::iterator Position;
protected:
    Position pos(int i) 
    {
        return (v.begin() + i);
    }

    int idx(const Position& p) const
    {
        return (p - v.begin());
    }
public:
    CompleteBinaryTree(): v(1) {}
    int size() const {return v.size() - 1;}
    Position left(const Position& p) {return pos(2*idx(p));}
    Position right(const Position& p) {return pos(2*idx(p)+1);}
    Position parent(const Position& p) {return pos(idx(p)/2);}
    bool hasLeft(const Position& p) const {return 2*idx(p) <= size();}
    bool hasRight(const Position& p) const {return 2*idx(p)+1 <= size();}
    bool isRoot(const Position& p) const {return idx(p) == 1;}
    Position root() {return pos(1);}
    Position last() {return pos(size());}
    void addLast(const E& e) {v.push_back(e);}
    void removeLast() {v.pop_back();}
    void swap(const Position& p, const Position& q) {E e = *p; *p = *q; *q = e;}
};

// exception object to throw in case of accessing an empty priority queue
class QueueEmpty{
};

// heap priority queue class
template <typename E, typename C>
class HeapPriorityQueue{
private:
    CompleteBinaryTree<E> T;
    C isLess;
public:
    int size() const;
    bool empty() const;
    void insert(const E& e);
    const E& min() throw(QueueEmpty);
    void removeMin() throw(QueueEmpty);

    typedef typename CompleteBinaryTree<E>::Position Position;
};

template <typename E, typename C>
int HeapPriorityQueue<E,C>::size() const
{
    return T.size();
}

template <typename E, typename C>
bool HeapPriorityQueue<E,C>::empty() const
{
    return (size() == 0);
}

template <typename E, typename C>
const E& HeapPriorityQueue<E,C>::min() throw(QueueEmpty)
{
    if (empty())
        throw QueueEmpty();
    return *(T.root());
}

// insert the new element at last node and keep swapping upwards until heap
// order property is satisfied
template <typename E, typename C>
void HeapPriorityQueue<E,C>::insert(const E& e)
{
    T.addLast(e);
    Position v = T.last();
    while (!T.isRoot(v))
    {
        Position u = T.parent(v);
        if (!isLess(*v, *u))
            break;
        T.swap(v, u);
        v = u;
    }
}

//swap the root and last element, remove the last element and keep swapping the
//new root downwards until heap order property is satisfied.
template <typename E, typename C>
void HeapPriorityQueue<E,C>::removeMin() throw(QueueEmpty)
{
    if (empty())
        throw QueueEmpty();
    if (size() == 1)
        T.removeLast();
    else
    {
        Position u = T.root();
        T.swap(u, T.last());
        T.removeLast();
        while (T.hasLeft(u))
        {
            Position v = T.left(u);
            if (T.hasRight(u) && isLess(*(T.right(u)), *v))
                v = T.right(u);
            if (isLess(*v, *u))
            {
                T.swap(v, u);
                u = v;
            }
            else
                break;
        }
    }
}

// class representing a point in a 2d plane
struct Point2D{
    double x;
    double y;
    Point2D() {}
    Point2D(double a, double b): x(a), y(b) {}
    double getX() const {return x;}
    double getY() const {return y;}
};

// comparator class to arrange points vertically upwards
class BottomUp{
public:
    bool operator()(const Point2D& a, const Point2D& b)
    {
        return (a.getY() < b.getY());
    }
};

int main()
{
    HeapPriorityQueue<Point2D, BottomUp> points;
    points.insert(Point2D(1.0, 2.0));
    points.insert(Point2D(1.0, 0.5));
    points.insert(Point2D(2.3, 3.7));
    cout << "Size is " << points.size() << endl;
    while (!points.empty())
    {
        cout << "x = " << points.min().getX() << ", y = " << points.min().getY() << endl;
        points.removeMin();
    }
    return 0;
}
