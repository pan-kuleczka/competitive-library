#include <bits/stdc++.h>

// Conditional compilation to reduce memory and time requirements (select only the features you need)
#define TREAP_KEY_SUM true     // adds information about sum of the keys in a subtree
#define TREAP_KEY_ADD true     // adds operation adding to all elements in a range
#define TREAP_KEY_SET true     // adds operation setting all elements in a range
#define TREAP_KEY_REVERSE true // adds operation reversing a subtree

std::random_device rd;
std::mt19937 randomGenerator(rd());
std::uniform_int_distribution<unsigned int> keyDistribution(0, UINT_MAX);

template <class _keyType = int>
class ImplicitTreapNode
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

#if TREAP_KEY_ADD
    _keyType additionLazy = _keyType(0);
#endif

#if TREAP_KEY_SET
    std::pair<bool, _keyType> settingLazy = {false, _keyType(0)};
#endif

#if TREAP_KEY_REVERSE
    bool reversed = false;
#endif

    _keyType key;
    unsigned int priority;
    unsigned int subtreeSize = 1;

    ImplicitTreapNode<_keyType> *parent = nullptr;
    ImplicitTreapNode<_keyType> *leftChild = nullptr;
    ImplicitTreapNode<_keyType> *rightChild = nullptr;

    constexpr unsigned int randomPriority() const { return keyDistribution(randomGenerator); }
    void updateNode()
    {
        subtreeSize = 1 + (leftChild ? leftChild->subtreeSize : 0) + (rightChild ? rightChild->subtreeSize : 0);
#if TREAP_KEY_SUM
        subtreeSum = key + (leftChild ? leftChild->key : 0) + (rightChild ? rightChild->key : 0);
#endif
        if (parent)
            parent->updateNode();
    }

    constexpr ImplicitTreapNode<_keyType> *&getChildFromEnum(enum ImplicitTreapNode::Child childType) { return (childType == LeftChild ? leftChild : rightChild); }

public:
    ImplicitTreapNode(const _keyType &_key = _keyType()) :
#if TREAP_KEY_SUM
                                                           subtreeSum(_key),
#endif
                                                           key(_key), priority(randomPriority())
    {
    }

    ImplicitTreapNode(const _keyType &_key, unsigned int _priority) :
#if TREAP_KEY_SUM
                                                                      subtreeSum(_key),
#endif
                                                                      key(_key), priority(_priority)
    {
    }

    constexpr unsigned int getSize() const { return subtreeSize; }
    constexpr unsigned int getPriority() const { return priority; }
    constexpr _keyType getKey() const { return key; }
    constexpr _keyType &getKeyReference() { return key; }
    constexpr ImplicitTreapNode<_keyType> *getParent() { return parent; }
    constexpr ImplicitTreapNode<_keyType> *getChild(enum ImplicitTreapNode::Child childType) { return getChildFromEnum(childType); }

#if TREAP_KEY_SUM
    constexpr _keyType getSum()
    {
        pushUpdates();
        return subtreeSum;
    }
#endif

#if TREAP_KEY_ADD
    void add(const _keyType &delta)
    {
        pushUpdates();
        additionLazy += delta;
    }
#endif

#if TREAP_KEY_SET
    void set(const _keyType &newValue)
    {
        pushUpdates();
        settingLazy = {true, newValue};
    }
#endif

#if TREAP_KEY_REVERSE
    void reverse()
    {
        reversed = !reversed;
    }
#endif

    void pushUpdates()
    {
#if TREAP_KEY_REVERSE
        if (reversed)
        {
            ImplicitTreapNode<_keyType> *leftChild = detachChild(ImplicitTreapNode<>::LeftChild);
            ImplicitTreapNode<_keyType> *rightChild = detachChild(ImplicitTreapNode<>::RightChild);
            if (leftChild)
                leftChild->reverse();
            if (rightChild)
                rightChild->reverse();
            attachChild(ImplicitTreapNode<>::LeftChild, rightChild);
            attachChild(ImplicitTreapNode<>::RightChild, leftChild);
            reversed = false;
        }
#endif
#if TREAP_KEY_ADD
        if (additionLazy != _keyType(0))
        {
            key += additionLazy;
            sum = ...
            if (leftChild)
                leftChild->add(additionLazy);
            if (rightChild)
                rightChild->add(additionLazy);
            additionLazy = _keyType(0);
        }
#endif
#if TREAP_KEY_SET
        if (settingLazy.first)
        {
            key = settingLazy.second;
            sum = ...
            if (leftChild)
                leftChild->set(settingLazy.second);
            if (rightChild)
                rightChild->set(settingLazy.second);
            settingLazy = {false, _keyType(0)};
        }
#endif
    }

    void attachChild(enum ImplicitTreapNode::Child childType, ImplicitTreapNode<_keyType> *newChild)
    {
        ImplicitTreapNode<_keyType> *&child = getChildFromEnum(childType);
        if (child)
            throw std::runtime_error("TreapNode error: Potential memory leak caused by an unsafe replacement of a child");
        if (newChild->getParent())
            throw std::runtime_error("TreapNode error: Potential memory leak caused by a node having two parents");

        child = newChild;
        child->parent = this;

        updateNode();
    }

    ImplicitTreapNode<_keyType> *detachChild(enum ImplicitTreapNode::Child childType)
    {
        ImplicitTreapNode<_keyType> *&child = getChildFromEnum(childType);
        ImplicitTreapNode<_keyType> *detached = child;
        child = nullptr;
        detached->parent = nullptr;

        updateNode();
        return detached;
    }

    ~ImplicitTreapNode()
    {
        if (parent)
            throw std::runtime_error("TreapNode error: Potential memory leak caused by deletion of an owned child");
        if (leftChild)
            leftChild->parent = nullptr;
        delete leftChild;
        if (rightChild)
            rightChild->parent = nullptr;
        delete rightChild;
    }
};

template <class _keyType = int>
class ImplicitTreap
{
    std::pair<ImplicitTreapNode<_keyType> *, ImplicitTreapNode<_keyType> *> splitNode(ImplicitTreapNode<_keyType> *node, std::size_t position)
    {
        if (!node)
            return {nullptr, nullptr};

        node->pushUpdates();

        std::size_t leftChildSubtreeSize = (node->getChild(ImplicitTreapNode<>::LeftChild) ? node->getChild(ImplicitTreapNode<>::LeftChild)->getSize() : 0);

        if (position <= leftChildSubtreeSize)
        {
            ImplicitTreapNode<_keyType> *left = node->detachChild(ImplicitTreapNode<>::LeftChild);
            std::pair<ImplicitTreapNode<_keyType> *, ImplicitTreapNode<_keyType> *> leftSplit = splitNode(left, position);

            node->attachChild(ImplicitTreapNode<>::LeftChild, leftSplit.second);

            return {leftSplit.first, node};
        }
        else
        {
            ImplicitTreapNode<_keyType> *right = node->detachChild(ImplicitTreapNode<>::RightChild);
            std::pair<ImplicitTreapNode<_keyType> *, ImplicitTreapNode<_keyType> *> rightSplit = splitNode(right, position - leftChildSubtreeSize - 1);

            node->attachChild(ImplicitTreapNode<>::RightChild, rightSplit.first);

            return {node, rightSplit.second};
        }
    }

    ImplicitTreapNode<_keyType> *mergeNodes(ImplicitTreapNode<_keyType> *first, ImplicitTreapNode<_keyType> *second)
    {

        if (!first)
            return second;

        if (!second)
            return first;

        ImplicitTreapNode<_keyType> *lesserNode = first;
        lesserNode->pushUpdates();

        ImplicitTreapNode<_keyType> *greaterNode = second;
        greaterNode->pushUpdates();

        if (lesserNode->getPriority() < greaterNode->getPriority())
        {
            greaterNode->attachChild(
                ImplicitTreapNode<>::LeftChild,
                mergeNodes(
                    lesserNode,
                    greaterNode->detachChild(ImplicitTreapNode<>::LeftChild)));

            return greaterNode;
        }
        else
        {
            lesserNode->attachChild(
                ImplicitTreapNode<>::RightChild,
                mergeNodes(
                    lesserNode->detachChild(ImplicitTreapNode<>::RightChild),
                    greaterNode));

            return lesserNode;
        }
    }

    ImplicitTreapNode<_keyType> *insertNode(ImplicitTreapNode<_keyType> *node, std::size_t position)
    {
        if (!node)
            return root;

        node->pushUpdates();

        if (node->getSize() > 1)
            throw std::runtime_error("TreapNode error: Trying to insert a node with children");

        auto rootSplit = splitNode(root, position);

        ImplicitTreapNode<_keyType> *leftMerge = mergeNodes(rootSplit.first, node);

        return mergeNodes(leftMerge, rootSplit.second);
    }

    std::pair<bool, ImplicitTreapNode<_keyType> *> eraseNode(std::size_t position)
    {
        auto rootSplit = splitNode(root, position);
        auto rightSplit = splitNode(rootSplit.second, position + 1);

        ImplicitTreapNode<_keyType> *newRoot = mergeNodes(rootSplit.first, rightSplit.second);

        if (rightSplit.first != nullptr)
        {
            delete rightSplit.first;
            return {true, newRoot};
        }
        return {false, newRoot};
    }

    ImplicitTreapNode<_keyType> *findNodeByPosition(ImplicitTreapNode<_keyType> *node, std::size_t position)
    {
        if (!node)
            return nullptr;

        if (position >= node->getSize())
            return nullptr;

        node->pushUpdates();

        ImplicitTreapNode<_keyType> *leftChild = node->getChild(ImplicitTreapNode<>::LeftChild);
        ImplicitTreapNode<_keyType> *rightChild = node->getChild(ImplicitTreapNode<>::RightChild);

        std::size_t leftSize = (leftChild ? leftChild->getSize() : 0);

        if (position < leftSize)
            return findNodeByPosition(leftChild, position);
        else if (position == leftSize)
            return node;
        else
            return findNodeByPosition(rightChild, position - leftSize - 1);
    }

    std::array<ImplicitTreapNode<_keyType> *, 3> splitByRange(std::size_t begin, std::size_t end)
    {
        if (!root)
            return {nullptr, nullptr, nullptr};
        if (end > root->getSize())
            throw std::runtime_error("ImplicitTreap error: range operation end out of bounds");
        if (begin > end)
            throw std::runtime_error("ImplicitTreap error: range operation begin > end");

        std::pair<ImplicitTreapNode<_keyType> *, ImplicitTreapNode<_keyType> *> leftSplit = splitNode(root, begin);
        std::pair<ImplicitTreapNode<_keyType> *, ImplicitTreapNode<_keyType> *> rightSplit = splitNode(leftSplit.second, end);

        return {leftSplit.first, rightSplit.first, rightSplit.second};
    }

    void mergeRanges(std::array<ImplicitTreapNode<_keyType> *, 3> ranges)
    {
        ImplicitTreapNode<_keyType> *leftMerge = mergeNodes(ranges[0], ranges[1]);
        root = mergeNodes(leftMerge, ranges[2]);
    }

#if TREAP_KEY_SUM
    _keyType getRangeSumOperation(std::size_t begin, std::size_t end)
    {
        _keyType result = 0;
        std::array<ImplicitTreapNode<_keyType> *, 3> splitTreap = splitByRange(begin, end);

        if (splitTreap[1])
        {
            result = splitTreap[1]->getSum();
        }

        mergeRanges(splitTreap);
        return result;
    }
#endif

#if TREAP_KEY_ADD
    void addRangeOperation(std::size_t begin, std::size_t end, const _keyType &delta)
    {
        std::array<ImplicitTreapNode<_keyType> *, 3> splitTreap = splitByRange(begin, end);

        if (splitTreap[1])
        {
            splitTreap[1]->add(delta);
        }

        mergeRanges(splitTreap);
    }
#endif

#if TREAP_KEY_SET
    void setRangeOperation(std::size_t begin, std::size_t end, const _keyType &newValue)
    {
        std::array<ImplicitTreapNode<_keyType> *, 3> splitTreap = splitByRange(begin, end);

        if (splitTreap[1])
        {
            splitTreap[1]->set(newValue);
        }

        mergeRanges(splitTreap);
    }
#endif

#if TREAP_KEY_REVERSE
    void reverseRangeOperation(std::size_t begin, std::size_t end)
    {
        std::array<ImplicitTreapNode<_keyType> *, 3> splitTreap = splitByRange(begin, end);

        if (splitTreap[1])
        {
            splitTreap[1]->reverse();
        }

        mergeRanges(splitTreap);
    }
#endif

public:
    ImplicitTreapNode<_keyType> *root;

    ImplicitTreap(ImplicitTreapNode<_keyType> *_root = nullptr) : root(_root) {}
    ImplicitTreap(const std::vector<_keyType> &keys)
    {
        // TO DO : create bst from a vector
    }

    constexpr unsigned int size() const { return (root ? root->getSize() : 0); }

    void insert(ImplicitTreapNode<_keyType> *node, std::size_t position)
    {
        // Inserts node before the element at position position
        // If position is greater than the size of the array, it is inserted at the end

        root = insertNode(node, position);
    }

    void insert(_keyType key, std::size_t position)
    {
        // Inserts a new ImplicitTreapNode with key key before the element at position position
        // If position is greater than the size of the array, it is inserted at the end

        insert(new ImplicitTreapNode<_keyType>(key), position);
    }

    void push_back(ImplicitTreapNode<_keyType> *node)
    {
        // Appends node to the array

        insert(node, (root ? root->getSize() : 0));
    }

    void push_back(_keyType key)
    {
        // Appends a new ImplicitTreapNode with key key to the array

        push_back(new ImplicitTreapNode<_keyType>(key));
    }

    bool erase(std::size_t position)
    {
        // Attempts to erase element on position position
        // Returns whether the operation succeeded

        std::pair<bool, ImplicitTreapNode<_keyType> *> result = eraseNode(position);
        root = result.second;

        return result.first;
    }

    ImplicitTreapNode<_keyType> *findByPosition(std::size_t position)
    {
        // Returns pointer to the node with given position in a treap
        // If the node does not exist (position >= size()) returns nullptr instead

        return findNodeByPosition(root, position);
    }

    _keyType &operator[](std::size_t position)
    {
        ImplicitTreapNode<_keyType> node = findNodeByPosition(position);
        if (!node)
            throw std::runtime_error("ImplicitTreap error: operator[] out of bounds");
        return node.getKeyReference();
    }

    std::pair<ImplicitTreap<_keyType>, ImplicitTreap<_keyType>> split(std::size_t position)
    {
        // Splits treap into two in a std::pair:
        // - first with positions less than position
        // - second with positions greater or equal to position

        std::pair<ImplicitTreap<_keyType *>, ImplicitTreap<_keyType *>> splitResult = splitNode(root, position);

        return {ImplicitTreap<_keyType>(splitResult.first), ImplicitTreap<_keyType>(splitResult.second)};
    }

    void merge(const ImplicitTreap<_keyType> &other)
    {
        // Merges current treap with other, new treap is appended to this one

        root = mergeNodes(root, other.root);
    }

#if TREAP_KEY_SUM
    _keyType getRangeSum(std::size_t begin, std::size_t end)
    {
        // Returns sum of elements in a range from begin (inclusive) to end (exclusive) in the array

        return getRangeSumOperation(begin, end);
    }
#endif

#if TREAP_KEY_ADD
    void addRange(std::size_t begin, std::size_t end, const _keyType &delta)
    {
        // Adds delta to elements in a range from begin (inclusive) to end (exclusive) in the array

        addRangeOperation(begin, end, delta);
    }
#endif

#if TREAP_KEY_SET
    void setRange(std::size_t begin, std::size_t end, const _keyType &newValue)
    {
        // Sets elements in a range from begin (inclusive) to end (exclusive) in the array to newValue

        setRangeOperation(begin, end, newValue);
    }
#endif

#if TREAP_KEY_REVERSE
    void reverseRange(std::size_t begin, std::size_t end)
    {
        // Reverses elements in a range from begin (inclusive) to end (exclusive) in the array

        reverseRangeOperation(begin, end);
    }
#endif

    ~ImplicitTreap()
    {
        if (root)
            delete root;
    }
};

template <class _keyType>
void printStructure(std::ostream &out, ImplicitTreapNode<_keyType> *node, unsigned int indent = 0)
{
    for (unsigned int i = 0; i < indent; ++i)
        out << '\t';

    if (!node)
    {
        out << "-\n";
        return;
    }

    node->pushUpdates();

    out << node->getKey() << " (P = " << node->getPriority() << ")\n";
    printStructure(out, node->getChild(ImplicitTreapNode<>::LeftChild), indent + 1);
    printStructure(out, node->getChild(ImplicitTreapNode<>::RightChild), indent + 1);
}

template <class _keyType>
void printImplicitArray(std::ostream &out, ImplicitTreapNode<_keyType> *node, bool initialCall = true)
{
    if (!node)
        return;

    node->pushUpdates();

    printImplicitArray(out, node->getChild(ImplicitTreapNode<>::LeftChild), false);
    out << node->getKey() << " ";
    printImplicitArray(out, node->getChild(ImplicitTreapNode<>::RightChild), false);

    if (initialCall)
        out << "\n";
}

template <class _keyType>
std::ostream &operator<<(std::ostream &out, const ImplicitTreap<_keyType> &treap)
{
    out << "ImplicitTreap:\n";
    out << "Size: " << treap.size() << "\n";

    printImplicitArray(out, treap.root);

    return out;
}

ImplicitTreap<> imtr;

int main()
{
    std::cout << "Enter size: ";
    int n;
    std::cin >> n;
    for (int i = 0; i < n; ++i)
        imtr.push_back(0);
    std::cout << imtr << "\n";

    std::string command;
    int args[3];

    while (true)
    {

        std::cin >> command;

        if (command == "EXIT")
            break;

#if TREAP_KEY_SUM
        else if (command == "SUM")
        {
            std::cin >> args[0] >> args[1];
            std::cout << "SUM ON (" << args[0] << ", " << args[1] << "] = " << imtr.getRangeSum(args[0], args[1]) << "\n";
        }
#endif

#if TREAP_KEY_ADD
        else if (command == "ADD")
        {
            std::cin >> args[0] >> args[1] >> args[2];
            imtr.addRange(args[0], args[1], args[2]);
            std::cout << "ADDED " << args[2] << " ON (" << args[0] << ", " << args[1] << "]\n";
            std::cout << imtr << "\n";
        }
#endif

#if TREAP_KEY_SET
        else if (command == "SET")
        {
            std::cin >> args[0] >> args[1] >> args[2];
            imtr.setRange(args[0], args[1], args[2]);
            std::cout << "SET " << args[2] << " ON (" << args[0] << ", " << args[1] << "]\n";
            std::cout << imtr << "\n";
        }
#endif

#if TREAP_KEY_REVERSE
        else if (command == "REVERSE" || command == "REV")
        {
            std::cin >> args[0] >> args[1];
            imtr.reverseRange(args[0], args[1]);
            std::cout << "REVERSED (" << args[0] << ", " << args[1] << "]\n";
            std::cout << imtr << "\n";
        }
#endif
        else
            std::cout << "UNKNOWN COMMAND.\n";
    }

    return 0;
}
