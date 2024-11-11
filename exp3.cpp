#include <iostream>
#include <queue> 
#include <vector>
#include <string>
#include <unordered_map>

using Rank = unsigned int;

template <typename T> struct BinNode;
template <typename T> using BinNodePosi = BinNode<T>*;

template <typename T> struct BinNode
{
    T data;
    BinNodePosi<T> parent, lc, rc;
    Rank height;
    Rank npl;

    BinNode(T e, BinNodePosi<T> p = NULL, BinNodePosi<T> lc = NULL,
        BinNodePosi<T> rc = NULL, int h = 0, int l = 1)
        : data(e), parent(p), lc(lc), rc(rc), height(h), npl(l)
    {
        if (lc) lc->parent = this;
        if (rc) rc->parent = this;
    }

    Rank size();
    Rank updateHeight();
    void updateHeightAbove();
    BinNodePosi<T> insertLc(T const&);
    BinNodePosi<T> insertRc(T const&);
    void attachLc(BinNodePosi<T>);
    void attachRc(BinNodePosi<T>);
    BinNodePosi<T> succ();

    template <typename VST> void travLevel(VST& visit);
    template <typename VST> void travPre(VST& visit);
    template <typename VST> void travIn(VST& visit);
    template <typename VST> void travPost(VST& visit);
};

template <typename T>
Rank BinNode<T>::size()
{
    Rank s = 1;
    if (lc) s += lc->size();
    if (rc) s += rc->size();
    return s;
}

template <typename T>
Rank BinNode<T>::updateHeight()
{
    Rank hl = lc ? lc->height : -1;
    Rank hr = rc ? rc->height : -1;
    height = (hl > hr ? hl : hr) + 1;
    return height;
}

template <typename T>
void BinNode<T>::updateHeightAbove()
{
    BinNode<T>* p = this;
    while (p) {
        p->updateHeight();
        p = p->parent;
    }
}

template <typename T>
BinNode<T>* BinNode<T>::insertLc(T const& e)
{
    lc = new BinNode<T>(e, this);
    updateHeightAbove();
    return lc;
}

template <typename T>
BinNode<T>* BinNode<T>::insertRc(T const& e)
{
    rc = new BinNode<T>(e, this);
    updateHeightAbove();
    return rc;
}

template <typename T>
void BinNode<T>::attachLc(BinNodePosi<T> t)
{
    lc = t;
    if (t) t->parent = this;
    updateHeightAbove();
}

template <typename T>
void BinNode<T>::attachRc(BinNodePosi<T> t)
{
    rc = t;
    if (t) t->parent = this;
    updateHeightAbove();
}

template <typename T>
BinNode<T>* BinNode<T>::succ()
{
    BinNode<T>* p = this;
    if (rc) {
        p = rc;
        while (p->lc) p = p->lc;
    }
    else {
        while (p->parent && p == p->parent->rc)
            p = p->parent;
        p = p->parent;
    }
    return p;
}

template <typename T>
template <typename VST>
void BinNode<T>::travLevel(VST& visit)
{
    std::queue<BinNode<T>*> q;
    q.push(this);
    while (!q.empty()) {
        BinNode<T>* node = q.front();
        q.pop();
        visit(node->data);
        if (node->lc) q.push(node->lc);
        if (node->rc) q.push(node->rc);
    }
}

template <typename T>
template <typename VST>
void BinNode<T>::travPre(VST& visit)
{
    visit(data);
    if (lc) lc->travPre(visit);
    if (rc) rc->travPre(visit);
}

template <typename T>
template <typename VST>
void BinNode<T>::travIn(VST& visit) {
    if (lc) lc->travIn(visit);
    visit(data);
    if (rc) rc->travIn(visit);
}

template <typename T>
template <typename VST>
void BinNode<T>::travPost(VST& visit) {
    if (lc) lc->travPost(visit);
    if (rc) rc->travPost(visit);
    visit(data);
}

template <typename T> class BinTree
{
protected:
    Rank _size;
    BinNodePosi<T> _root;

public:
    BinTree() : _size(0), _root(NULL) {}
    ~BinTree() { if (_size > 0) remove(_root); }
    BinTree(BinTree<T> const&);
    Rank size() const { return _size; }
    bool empty() const { return !_root; }
    BinNodePosi<T> root() const { return _root; }

    BinNodePosi<T> insert(T const&);
    BinNodePosi<T> insert(T const&, BinNodePosi<T>);
    BinNodePosi<T> insert(BinNodePosi<T>, T const&);
    Rank remove(BinNodePosi<T>);
    BinTree<T>* secede(BinNodePosi<T>);

    template <typename VST>
    void travLevel(VST& visit) { if (_root) _root->travLevel(visit); }

    template <typename VST>
    void travPre(VST& visit) { if (_root) _root->travPre(visit); }

    template <typename VST>
    void travIn(VST& visit) { if (_root) _root->travIn(visit); }

    template <typename VST>
    void travPost(VST& visit) { if (_root) _root->travPost(visit); }
};

template <typename T>
BinNode<T>* BinTree<T>::insert(T const& e) {
    if (!_root) {
        _root = new BinNode<T>(e);
        _size = 1;
    }
    else
    {
        _root->insertLc(e);
        _size++;
    }
    return _root;
}

template <typename T>
BinNode<T>* BinTree<T>::insert(T const& e, BinNodePosi<T> node)
{
    node->insertLc(e);
    _size++;
    return node->lc;
}

template <typename T>
BinNode<T>* BinTree<T>::insert(BinNodePosi<T> node, T const& e) {
    node->insertRc(e);
    _size++;
    return node->rc;
}

template <typename T>
Rank BinTree<T>::remove(BinNodePosi<T> node) {
    if (node->lc && node->rc) {
        BinNode<T>* succNode = node->succ();
        node->data = succNode->data;
        return remove(succNode);
    }
    else {
        BinNode<T>* child = node->lc ? node->lc : node->rc;
        if (child) child->parent = node->parent;
        if (node == _root) _root = child;
        delete node;
        _size--;
        return 1;
    }
}

template <typename T>
BinTree<T>* BinTree<T>::secede(BinNodePosi<T> node) {
    BinTree<T>* newTree = new BinTree<T>();
    newTree->_root = node;
    node->parent = NULL;
    return newTree;
}

void visit(int data) {
    std::cout << data << " ";
}

struct HuffNode {
    char data;
    int freq;
    HuffNode* left;
    HuffNode* right;

    HuffNode(char d, int f) : data(d), freq(f), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(HuffNode* left, HuffNode* right) {
        return left->freq > right->freq;
    }
};

class HuffTree {
private:
    HuffNode* root;

    void generateCodes(HuffNode* node, const std::string& code, std::unordered_map<char, std::string>& huffmanCodes) {
        if (!node) return;
        if (!node->left && !node->right) {
            huffmanCodes[node->data] = code;
        }
        generateCodes(node->left, code + "0", huffmanCodes);
        generateCodes(node->right, code + "1", huffmanCodes);
    }

public:
    HuffTree() : root(nullptr) {}

    void build(const std::vector<std::pair<char, int>>& charFreqs) {
        std::priority_queue<HuffNode*, std::vector<HuffNode*>, Compare> pq;
        for (const auto& cf : charFreqs) {
            pq.push(new HuffNode(cf.first, cf.second));
        }
        while (pq.size() > 1) {
            HuffNode* left = pq.top(); pq.pop();
            HuffNode* right = pq.top(); pq.pop();
            HuffNode* newNode = new HuffNode('\0', left->freq + right->freq);
            newNode->left = left;
            newNode->right = right;
            pq.push(newNode);
        }
        root = pq.top();
    }

    void displayHuffmanCodes() {
        std::unordered_map<char, std::string> huffmanCodes;
        generateCodes(root, "", huffmanCodes);
        for (const auto& code : huffmanCodes) {
            std::cout << code.first << ": " << code.second << "\n";
        }
    }
};

int main() {
    HuffTree huffTree;
    std::vector<std::pair<char, int>> charFreqs = {
        {'a', 6}, {'b', 9}, {'c', 16}, {'d', 19}, {'e', 26}, {'f', 65}
    };
    huffTree.build(charFreqs);
    huffTree.displayHuffmanCodes();
    return 0;
}