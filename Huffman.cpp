#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <climits>
#include <algorithm>
#include <iterator>

using namespace std;

const int  UNIQUE_SYMBOLS = 1 << CHAR_BIT;   // size of the character set
typedef vector<bool> HuffCode;
typedef map<char, HuffCode> HuffCodeMap;

class INode{
public:
    const int f;
    virtual ~INode(){}

protected:
    INode(int f): f(f) {}
};

class InternalNode : public INode{
public:
    INode *const left;
    INode *const right;
    
    InternalNode(INode* c0, INode* c1): INode(c0->f+c1->f), left(c0), right(c1){}
    ~InternalNode()
    {
        delete left;
        delete right;
    }
};

class LeafNode : public INode{
public:
    const char c;
    LeafNode(int f, char c): INode(f), c(c) {}
};

class NodeCmp{
public:
    bool operator() (const INode* lhs, const INode* rhs) const
    {
        return lhs->f > rhs->f;
    }
};

INode* buildTree(const int (&frequencies)[UNIQUE_SYMBOLS])
{
    priority_queue<INode*, vector<INode*>, NodeCmp> Q;
    for(int i=0; i<UNIQUE_SYMBOLS; i++)
    {
        if (frequencies[i] != 0)
            Q.push(new LeafNode(frequencies[i], (char)i));
    }
    while (Q.size() > 1)
    {
        INode* childL = Q.top();
        Q.pop();
        INode* childR = Q.top();
        Q.pop();
        INode* parent = new InternalNode(childL, childR);
        Q.push(parent);
    }
    return Q.top();
}

void generateCodes(const INode* node, const HuffCode& prefix, HuffCodeMap&
outCodes)
{
    if (const LeafNode* lf = dynamic_cast<const LeafNode*>(node))
    {
        outCodes[lf->c] = prefix;
    }
    else if (const InternalNode* in = dynamic_cast<const InternalNode*>(node))
    {
        HuffCode leftPrefix = prefix;
        leftPrefix.push_back(false);
        generateCodes(in->left, leftPrefix, outCodes);
        
        HuffCode rightPrefix = prefix;
        rightPrefix.push_back(true);
        generateCodes(in->right, rightPrefix, outCodes);
    }
}

int main()
{
    string text;
    cout << "Enter your line of text below:\n";
    getline(cin, text);
    const char* ptr = text.c_str();
    int frequencies[UNIQUE_SYMBOLS] = {0};
    while (*ptr != '\0')
        ++frequencies[*ptr++];
    INode* root = buildTree(frequencies);
    HuffCodeMap codes;
    generateCodes(root, HuffCode(), codes);
    delete root;
    
    for(HuffCodeMap::const_iterator it = codes.begin(); it != codes.end(); ++it)
    {
        cout << it->first << " ";
        copy(it->second.begin(), it->second.end(),
        ostream_iterator<bool>(cout));
        cout << endl;
    }
    return 0;
}
