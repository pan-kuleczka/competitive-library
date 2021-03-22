#include <bits/stdc++.h>

// Conditional compilation to reduce memory and time requirements (select only the features you need)
#define TREAP_KEY_SUM true // adds information about sum of the keys in a subtree
#define TREAP_REVERSE_OPERATION true // adds operation reversing a subtree

std::random_device rd;
std::mt19937 randomGenerator(rd());
std::uniform_int_distribution<unsigned int> keyDistribution(0, UINT_MAX);

template <class _keyType = int>
class TreapNode
{
public:
    enum Child
    {
        LeftChild,
        RightChild
    };

private:

    #if TREAP_KEY_SUM
    _keyType subtreeSum;
    #endif

    #if TREAP_REVERSE_OPERATION
    bool reversed = false;
    #endif

    _keyType key;
    unsigned int priority;
    unsigned int subtreeSize = 1;

    TreapNode<_keyType> *leftChild = nullptr;
    TreapNode<_keyType> *rightChild = nullptr;

    constexpr unsigned int randomPriority() const { return keyDistribution(randomGenerator); }
    void updateNode() {
        subtreeSize = 1 + (leftChild ? leftChild->subtreeSize : 0) + (rightChild ? rightChild->subtreeSize : 0);

        #if TREAP_KEY_SUM
        subtreeSum = key + (leftChild ? leftChild->key : 0) + (rightChild ? rightChild->key : 0);
        #endif
    }

    constexpr TreapNode<_keyType> *&getChildFromEnum(enum TreapNode::Child childType) { return (childType == LeftChild ? leftChild : rightChild); }

public:
    TreapNode(_keyType _key = _keyType()) : 
    #if TREAP_KEY_SUM
        subtreeSum(_key),
    #endif
    key(_key), priority(randomPriority()) {}

    TreapNode(_keyType _key, unsigned int _priority) : 
    #if TREAP_KEY_SUM
        subtreeSum(_key),
    #endif
    key(_key), priority(_priority) {}

    constexpr _keyType getKey() const { return key; }
    constexpr TreapNode<_keyType> *getChild(enum TreapNode::Child childType) { return getChildFromEnum(childType); }

    #if TREAP_KEY_SUM
    constexpr _keyType getSum() const { return subtreeSum; }
    #endif

    void attachChild(TreapNode<_keyType> *newChild, enum TreapNode::Child childType)
    {
        TreapNode<_keyType> *&child = getChildFromEnum(childType);
        if (child)
            throw std::runtime_error("TreapNode error: Potential memory leak caused by an unsafe replacement of a child");
        child = newChild;
        updateNode();
    }

    TreapNode<_keyType> *detachChild(enum TreapNode::Child childType)
    {
        TreapNode<_keyType> *&child = getChildFromEnum(childType);
        TreapNode<_keyType> *detached = child;
        child = nullptr;
        updateNode();
        return detached;
    }

    ~TreapNode()
    {
        if (leftChild)
            delete leftChild;
        if (rightChild)
            delete rightChild;
    }
};

template <class _keyType = int>
struct Treap
{
    TreapNode<_keyType> *root;

    Treap(TreapNode<_keyType> *_root = nullptr) : root(_root) {}
    Treap(const std::vector<_keyType> &keys)
    {
        // TO DO : create bst from a vector
    }

    ~Treap()
    {
        if (root)
            delete root;
    }
};

TreapNode<> tr;

int main()
{
    tr.attachChild(new TreapNode<>(), TreapNode<>::LeftChild);
    std::cout << tr.getChild(TreapNode<>::LeftChild)->getKey() << "\n";
    return 0;
}