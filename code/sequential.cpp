#include <bits/stdc++.h>

#include "map.h"
#include "sequential.h"

using namespace std;

Result *sequential(const Map *map, int num_thread)
{
    priority_queue<pair<int, Node *>> open_list;

    int end_id = map->end->id,count = 0;
    open_list.push({map->end->calc_heuristic(map->start), map->start});

    vector<int> g(map->h * map->w, INT32_MAX);
    g[map->start->id] = 0;

    while (!open_list.empty())
    {
        count++;
        Node *node_curr = open_list.top().second;
        open_list.pop();

        if (node_curr->id == end_id)
        {
            break;
        }

        for (auto it : node_curr->adjacent_list)
        {
            Node *node = it.first;
            int wt = it.second;

            int g_updated = wt + g[node_curr->id];

            if (g_updated < g[node->id])
            {
                g[node->id] = g_updated;
                int f1 = g_updated + map->end->calc_heuristic(node);
                open_list.push({f1, node});
            }
        }
    }

    Result *ans = new Result(num_thread);
    ans->shortest_path = g[map->end->id];
    ans->nodes_traversed[0] = count;
    return ans;
}
