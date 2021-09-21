#include <bits/stdc++.h>

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
    _keyType key;
    unsigned int priority;
    unsigned int subtreeSize = 1;

    TreapNode<_keyType> *leftChild = nullptr;
    TreapNode<_keyType> *rightChild = nullptr;

    constexpr unsigned int randomPriority() const { return keyDistribution(randomGenerator); }
    void updateNode()
    {
        subtreeSize = 1 + (leftChild ? leftChild->subtreeSize : 0) + (rightChild ? rightChild->subtreeSize : 0);
    }

    constexpr TreapNode<_keyType> *&getChildFromEnum(enum TreapNode::Child childType) { return (childType == LeftChild ? leftChild : rightChild); }

public:
    TreapNode(_keyType _key = _keyType()) : key(_key), priority(randomPriority())
    {
    }

    TreapNode(_keyType _key, unsigned int _priority) : key(_key), priority(_priority)
    {
    }

    constexpr unsigned int getSize() const { return subtreeSize; }
    constexpr unsigned int getPriority() const { return priority; }
    constexpr _keyType getKey() const { return key; }
    constexpr _keyType &getKeyReference() { return key; }
    constexpr TreapNode<_keyType> *getChild(enum TreapNode::Child childType) { return getChildFromEnum(childType); }

    void attachChild(enum TreapNode::Child childType, TreapNode<_keyType> *newChild)
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
class Treap
{
    std::pair<TreapNode<_keyType> *, TreapNode<_keyType> *> splitNode(TreapNode<_keyType> *node, _keyType splitKey)
    {
        if (!node)
            return {nullptr, nullptr};

        if (splitKey < node->getKey())
        {
            TreapNode<_keyType> *left = node->detachChild(TreapNode<>::LeftChild);
            std::pair<TreapNode<_keyType> *, TreapNode<_keyType> *> leftSplit = splitNode(left, splitKey);

            node->attachChild(TreapNode<>::LeftChild, leftSplit.second);

            return {leftSplit.first, node};
        }
        else
        {
            TreapNode<_keyType> *right = node->detachChild(TreapNode<>::RightChild);
            std::pair<TreapNode<_keyType> *, TreapNode<_keyType> *> rightSplit = splitNode(right, splitKey);

            node->attachChild(TreapNode<>::RightChild, rightSplit.first);

            return {node, rightSplit.second};
        }
    }

    TreapNode<_keyType> *mergeNodes(TreapNode<_keyType> *first, TreapNode<_keyType> *second)
    {

        if (!first)
            return second;

        if (!second)
            return first;

        TreapNode<_keyType> *lesserNode = first;
        TreapNode<_keyType> *greaterNode = second;

        if (lesserNode->getKey() > greaterNode->getKey())
            std::swap(lesserNode, greaterNode);

        if (lesserNode->getPriority() < greaterNode->getPriority())
        {
            greaterNode->attachChild(
                TreapNode<>::LeftChild,
                mergeNodes(
                    lesserNode,
                    greaterNode->detachChild(TreapNode<>::LeftChild)));

            return greaterNode;
        }
        else
        {
            lesserNode->attachChild(
                TreapNode<>::RightChild,
                mergeNodes(
                    greaterNode,
                    lesserNode->detachChild(TreapNode<>::RightChild)));

            return lesserNode;
        }
    }

    TreapNode<_keyType> *insertNode(TreapNode<_keyType> *node)
    {
        if (!node)
            return root;

        if (node->getSize() > 1)
            throw std::runtime_error("TreapNode error: Trying to insert a node with children");

        auto rootSplit = splitNode(root, node->getKey());

        TreapNode<_keyType> *leftMerge = mergeNodes(node, rootSplit.first);

        return mergeNodes(leftMerge, rootSplit.second);
    }

    std::pair<bool, TreapNode<_keyType> *> eraseNode(_keyType key)
    {
        auto rootSplit = splitNode(root, key);
        auto leftSplit = splitNode(rootSplit.first, key - 1);

        TreapNode<_keyType> *newRoot = mergeNodes(leftSplit.first, rootSplit.second);

        if (leftSplit.second != nullptr)
        {
            delete leftSplit.second;
            return {true, newRoot};
        }
        return {false, newRoot};
    }

    TreapNode<_keyType> *findNodeByKey(TreapNode<_keyType> *node, _keyType key)
    {
        if (!node)
            return nullptr;

        _keyType nodeKey = node->getKey();

        if (key < nodeKey)
            return findNodeByKey(node->getChild(TreapNode<>::LeftChild), key);
        else if (key == nodeKey)
            return node;
        else
            return findNodeByKey(node->getChild(TreapNode<>::RightChild), key);
    }

    TreapNode<_keyType> *findNodeByOrder(TreapNode<_keyType> *node, unsigned int order)
    {
        if (!node)
            return nullptr;

        if (order >= node->getSize())
            return nullptr;

        TreapNode<_keyType> *leftChild = node->getChild(TreapNode<>::LeftChild);
        TreapNode<_keyType> *rightChild = node->getChild(TreapNode<>::RightChild);

        unsigned int leftSize = (leftChild ? leftChild->getSize() : 0);

        if (order < leftSize)
            return findNodeByOrder(leftChild, order);
        else if (order == leftSize)
            return node;
        else
            return findNodeByOrder(rightChild, order - leftSize - 1);
    }

public:
    TreapNode<_keyType> *root;

    Treap(TreapNode<_keyType> *_root = nullptr) : root(_root) {}
    Treap(const std::vector<_keyType> &keys)
    {
        // TO DO : create bst from a vector
    }

    constexpr unsigned int size() const { return (root ? root->getSize() : 0); }

    void insert(TreapNode<_keyType> *node)
    {
        root = insertNode(node);
    }

    void insert(_keyType key)
    {
        insert(new TreapNode<_keyType>(key));
    }

    bool erase(_keyType key)
    {
        std::pair<bool, TreapNode<_keyType> *> result = eraseNode(key);
        root = result.second;

        return result.first;
    }

    TreapNode<_keyType> *findByKey(_keyType key)
    {
        // Returns pointer to the node with a key equal to given
        // If the node does not exist returns nullptr instead

        return findNodeByKey(root, key);
    }

    TreapNode<_keyType> *findByOrder(unsigned int order)
    {
        // Returns pointer to the node with given order in a treap
        // If the node does not exist (order >= size()) returns nullptr instead

        return findNodeByOrder(root, order);
    }

    std::pair<Treap<_keyType>, Treap<_keyType>> split(_keyType key)
    {
        // Splits treap into two in a std::pair:
        // - first with keys less than splitKey
        // - second with keys greater or equal to splitKey

        std::pair<Treap<_keyType *>, Treap<_keyType *>> splitResult = splitNode(root, key);

        return {Treap(splitResult.first), Treap(splitResult.second)};
    }

    void merge(const Treap<_keyType> &other)
    {
        // Merges current treap with other, all keys from current tree must be:
        // - either less than any key from the other
        // - or greater or equal to any key from the other

        root = mergeNodes(root, other.root);
    }

    ~Treap()
    {
        if (root)
            delete root;
    }
};

template <class _keyType>
void printStructure(std::ostream &out, TreapNode<_keyType> *node, unsigned int indent = 0)
{
    for (unsigned int i = 0; i < indent; ++i)
        out << '\t';

    if (!node)
    {
        out << "-\n";
        return;
    }

    out << node->getKey() << " (P = " << node->getPriority() << ")\n";
    printStructure(out, node->getChild(TreapNode<>::LeftChild), indent + 1);
    printStructure(out, node->getChild(TreapNode<>::RightChild), indent + 1);
}

template <class _keyType>
std::ostream &operator<<(std::ostream &out, const Treap<_keyType> &treap)
{
    out << "Treap:\n";
    out << "Size: " << treap.size() << "\n";

    printStructure(out, treap.root);

    return out;
}

Treap<> tr;

int main()
{
    tr.insert(6);
    tr.insert(5);
    tr.insert(2);
    tr.insert(3);
    tr.insert(4);
    tr.erase(3);
    tr.erase(2);
    tr.erase(1);
    tr.insert(1);
    std::cout << tr << "\n";
    std::cout << tr.findByOrder(1)->getKey() << "\n";
    return 0;
}
