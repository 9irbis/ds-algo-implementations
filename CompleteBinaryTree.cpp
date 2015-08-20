// An implementation of a complete binary tree with an underlying vector.
// A tree with height h is a complete binary tree when levels 0, 1, 2, ... h-1
// have maximum possible nodes i.e. 2^i nodes where i is level number. The last
// level h has atleast one node and its nodes are arranged from left to right.
// our scheme for storing nodes in a vector works as follows:
// index 0 is not used i.e. dummy position.
// root is stored at index 1.
// every node v other than the root, if it is a left child of its parent u, then
// it is stored at index 2*ind(u), if it is a right child of its parent u, then
// it is stored at index 2*ind(u) + 1.
#include <iostream>
#include <string>
#include <vector>
using namespace std;

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

int main()
{
    CompleteBinaryTree<string> t;
    cout << t.size() << endl;
    t.addLast("ab");
    t.addLast("cd");
    t.addLast("ef");
    t.addLast("gh");
    t.addLast("ij");
    cout << t.size() << endl;
    cout << "root node = " << *(t.root()) << ", last node = " << *(t.last()) << endl;
    cout << *(t.left(t.root())) << endl;
    cout << *(t.right(t.root())) << endl;
    if (t.hasRight(t.root()))
        cout << "Root has a right child\n";
    if (!t.hasLeft(t.last()))
        cout << "Last node has no child\n";
    t.removeLast();
    cout << "now last node is = " << *(t.last()) << endl;
    return 0;
}
