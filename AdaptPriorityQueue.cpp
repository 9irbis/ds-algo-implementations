// Implementation of an adaptable priority queue which provides facilities for
// removing or modifying an arbitrary entry
#include <iostream>
#include <list>
using namespace std;

class QueueEmpty{
};

template <typename E, typename C>
class AdaptPriorityQueue{
private:
    list<E> L;
    C isLess;
public:
    class Position{
    private:
        typename list<E>::iterator q;
    public:
        const E& operator*() {return *q;}
        friend class AdaptPriorityQueue;
    };
public:
    int size() const;
    bool empty() const;
    const E& min() const throw(QueueEmpty);
    void removeMin() throw(QueueEmpty);
    Position insert(const E& e);
    void remove(const Position& p) throw(QueueEmpty);
    Position replace(const Position& p, const E& e) throw(QueueEmpty);
};

template <typename E, typename C>
int AdaptPriorityQueue<E,C>::size() const
{
    return L.size();
}

template <typename E, typename C>
bool AdaptPriorityQueue<E,C>::empty() const
{
    return L.empty();
}

template <typename E, typename C>
const E& AdaptPriorityQueue<E,C>::min() const throw(QueueEmpty)
{
    if (empty())
        throw QueueEmpty();
    return L.front();
}

template <typename E, typename C>
void AdaptPriorityQueue<E,C>::removeMin() throw(QueueEmpty)
{
    if (empty())
        throw QueueEmpty();
    L.pop_front();
}

template <typename E, typename C>
typename AdaptPriorityQueue<E,C>::Position AdaptPriorityQueue<E,C>::insert(const
E& e)
{
    typename list<E>::iterator p = L.begin();
    while (p!=L.end() && !isLess(e, *p)) ++p;
    L.insert(p, e);
    Position pos;
    pos.q = --p;
    return pos;
}

template <typename E, typename C>
void AdaptPriorityQueue<E,C>::remove(const Position& p) throw(QueueEmpty)
{
    if (empty())
        throw QueueEmpty();
    L.erase(p.q);
}

template <typename E, typename C>
typename AdaptPriorityQueue<E,C>::Position
AdaptPriorityQueue<E,C>::replace(const Position& p, const E& e) throw(QueueEmpty)
{
    if (empty())
        throw QueueEmpty();
    remove(p);
    return insert(e);
}

struct Point2D{
    double x;
    double y;
    Point2D(double a, double b): x(a), y(b) {}
};

class LeftRight{
public:
    bool operator()(const Point2D& p, const Point2D& q)
    {
        return p.x < q.x;
    }
};

int main()
{
    AdaptPriorityQueue<Point2D, LeftRight> Q;
    Q.insert(Point2D(1.5, 2.3));
    AdaptPriorityQueue<Point2D, LeftRight>::Position pos = Q.insert(Point2D(2.1, 4.0));
    cout << Q.size() << "\n";
    if (!Q.empty())
        cout << "Q is not empty!\n";
    cout << "x = " << Q.min().x << ", y = " << Q.min().y << "\n";
    AdaptPriorityQueue<Point2D, LeftRight>::Position pos2 = Q.replace(pos, Point2D(0.6, -4.0));
    cout << "x = " << Q.min().x << ", y = " << Q.min().y << "\n";
    Q.remove(pos2);
    cout << Q.size() << "\n";
    cout << "x = " << Q.min().x << ", y = " << Q.min().y << "\n";
    Q.removeMin();
    Q.min(); 
    return 0;
}
