#include <bits/stdc++.h>
#include <bits/extc++.h>
using namespace std;

template <typename K, typename V, typename Comp = std::less<K>>
using pdbs_map = __gnu_pbds::tree<
    K, V, Comp,
    __gnu_pbds::rb_tree_tag,
    __gnu_pbds::tree_order_statistics_node_update>;

template <typename K, typename Comp = std::less<K>>
using pbds_set = pdbs_map<K, __gnu_pbds::null_type, Comp>;

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    pbds_set<int> s;
    s.insert(1);
    s.insert(2);
    s.insert(3);

    cout << s.order_of_key(2) << "\n";
    cout << *s.find_by_order(1) << "\n";

    return 0;
}