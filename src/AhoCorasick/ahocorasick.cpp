#include <bits/stdc++.h>
#define IOSTREAM_BOOST true
using namespace std;

struct AhoCorasickNode
{
    int character;
    AhoCorasickNode *parent;
    AhoCorasickNode *moveTable[26];
    AhoCorasickNode *desc[26];
    AhoCorasickNode *suffixLink;
    bool wordEnding;

    static int charToIndex(char c)
    {
        c = tolower(c);
        if (c < 'a' || c > 'z')
            throw runtime_error("AHO_BAD_CHAR");
        c -= 'a';
        return c;
    }

    AhoCorasickNode(AhoCorasickNode *_parent, char c = 'a', bool ending = false)
    {
        parent = _parent;
        character = c;
        wordEnding = ending;
        for (int i = 0; i < 26; i++)
            moveTable[i] = nullptr, desc[i] = nullptr;
        suffixLink = nullptr;
    }

    AhoCorasickNode *findSuffixLink()
    {
        if (suffixLink == nullptr)
        {
            if (parent == nullptr) //root
                suffixLink = this;
            else if (parent->parent == nullptr) //child of root
                suffixLink = parent;
            else
                suffixLink = parent->findSuffixLink()->move(character);
        }
        return suffixLink;
    }

    AhoCorasickNode *move(char c)
    {
        int k = charToIndex(c);
        if (moveTable[k] == nullptr)
        {
            if (desc[k] != nullptr)
                moveTable[k] = desc[k];
            else
            {
                if (parent == nullptr)
                    moveTable[k] = this;
                else
                    moveTable[k] = findSuffixLink()->move(c);
            }
        }
        return moveTable[k];
    }

    void findSuffixLinkRecursive()
    {
        findSuffixLink();
        for (int i = 0; i < 26; i++)
            if (desc[i] != nullptr)
                desc[i]->findSuffixLinkRecursive();
    }

    ~AhoCorasickNode()
    {
        for (int i = 0; i < 26; i++)
            if (desc[i] != nullptr)
                delete desc[i];
    }
};

struct AhoCorasick
{
    AhoCorasickNode *root;
    AhoCorasickNode *state;

    void clear()
    {
        delete root;
        root = new AhoCorasickNode(nullptr);
        state = root;
    }

    AhoCorasickNode *addChar(AhoCorasickNode *node, const char c, bool ending = false)
    {
        if (node == nullptr)
            return nullptr;
        int k = root->charToIndex(c);
        if (node->desc[k] == nullptr)
            node->desc[k] = new AhoCorasickNode(node, c, ending);
        return node->desc[k];
    }

    AhoCorasickNode *addString(const string &str)
    {
        AhoCorasickNode *cstate = root;
        for (int i = 0; i < str.length(); i++)
        {
            bool ending = (i == str.length() - 1);
            cstate = addChar(cstate, str[i], ending);
        }
        return cstate;
    }

    void calcSuffixLinks()
    {
        root->findSuffixLinkRecursive();
    }

    void move(char c)
    {
        state = state->move(c);
    }

    bool isEnding()
    {
        return state->wordEnding;
    }

    AhoCorasick()
    {
        root = new AhoCorasickNode(nullptr);
        state = root;
    }
    ~AhoCorasick()
    {
        delete root;
    }
};

AhoCorasick ac;

int n;
string s;

int main()
{
#if IOSTREAM_BOOST
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
#endif

    cin >> n;
    for (int i = 0; i < n; i++)
    {
        cin >> s;
        ac.addString(s);
    }
    ac.calcSuffixLinks();

    return 0;
}