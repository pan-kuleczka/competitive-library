#include <bits/stdc++.h>

template <class T = char>
struct SuffixAutomatonNode
{
    std::size_t longestSubstringLength;
    std::map<T, std::shared_ptr<SuffixAutomatonNode<T>>> nextNodes;
    std::weak_ptr<SuffixAutomatonNode<T>> suffixLink;
    bool accepting;

    SuffixAutomatonNode()
    {
        longestSubstringLength = 0;
        nextNodes = std::map<T, std::shared_ptr<SuffixAutomatonNode<T>>>();
        suffixLink = std::weak_ptr<SuffixAutomatonNode<T>>();
        accepting = false;
    }
};

template <class T = char>
class SuffixAutomaton
{
    std::shared_ptr<SuffixAutomatonNode<T>> root;

public:
    SuffixAutomaton()
    {
        root = std::make_shared<SuffixAutomatonNode<T>>();
    }

    SuffixAutomaton(const std::vector<T> &str) : SuffixAutomaton()
    {
        auto lastNode = root;
        for (const T &c : str)
        {
            auto currentBaseNode = lastNode;
            auto currentExpandedNode = std::make_shared<SuffixAutomatonNode<T>>();
            currentExpandedNode->longestSubstringLength = currentBaseNode->longestSubstringLength + 1;
            lastNode = currentExpandedNode;

            while (currentBaseNode != nullptr && currentBaseNode->nextNodes[c] == nullptr)
            {
                // Connect to the new last node
                currentBaseNode->nextNodes[c] = currentExpandedNode;
                currentBaseNode = currentBaseNode->suffixLink.lock();
            }

            while (currentBaseNode != nullptr)
            {
                auto nextExpandedNode = currentBaseNode->nextNodes[c];
                if (nextExpandedNode->longestSubstringLength == currentBaseNode->longestSubstringLength + 1)
                {
                    // No need to split
                    currentExpandedNode->suffixLink = nextExpandedNode;
                    while (currentBaseNode != nullptr && currentBaseNode->nextNodes[c] == nextExpandedNode)
                        currentBaseNode = currentBaseNode->suffixLink.lock();
                    currentExpandedNode = nextExpandedNode;
                }
                else
                {
                    // Split
                    auto newSplitNode = std::make_shared<SuffixAutomatonNode<T>>();
                    newSplitNode->longestSubstringLength = currentBaseNode->longestSubstringLength + 1;
                    newSplitNode->nextNodes = nextExpandedNode->nextNodes;
                    newSplitNode->suffixLink = nextExpandedNode->suffixLink.lock();

                    nextExpandedNode->suffixLink = newSplitNode;
                    currentExpandedNode->suffixLink = newSplitNode;

                    while (currentBaseNode != nullptr && currentBaseNode->nextNodes[c] == nextExpandedNode)
                    {
                        currentBaseNode->nextNodes[c] = newSplitNode;
                        currentBaseNode = currentBaseNode->suffixLink.lock();
                    }

                    currentExpandedNode = newSplitNode;
                }
            }

            currentExpandedNode->suffixLink = root;
        }

        while (lastNode != nullptr)
        {
            lastNode->accepting = true;
            lastNode = lastNode->suffixLink.lock();
        }
    }

    template <class = typename std::enable_if<std::is_same<SuffixAutomaton<T>, SuffixAutomaton<char>>::value>::type>
    SuffixAutomaton(const std::string &str) : SuffixAutomaton(std::vector<char>(str.begin(), str.end())) {}

    bool checkIfAccepting(const std::vector<T> &str) const
    {
        auto state = std::weak_ptr<SuffixAutomatonNode<T>>(root);
        if (state.expired())
            return false;

        for (const T &c : str)
        {
            state = state.lock()->nextNodes[c];
            if (state.expired())
                return false;
        }

        return state.lock()->accepting;
    }

    template <class = typename std::enable_if<std::is_same<SuffixAutomaton<T>, SuffixAutomaton<char>>::value>::type>
    bool checkIfAccepting(const std::string &str) const { return checkIfAccepting(std::vector<char>(str.begin(), str.end())); }
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    SuffixAutomaton<> s(std::string("abaab"));
    std::cout << s.checkIfAccepting("a");
    return 0;
}