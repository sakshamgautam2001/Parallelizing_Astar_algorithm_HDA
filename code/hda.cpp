#include <atomic>
#include <unordered_map>
#include <tbb/concurrent_priority_queue.h>
#include <tbb/concurrent_queue.h>

#include "map.h"
#include <omp.h>

using namespace std;

int hashNode(int id, int num_thread)
{
    return id % num_thread;
}


Result* hda_openmp(const Map *map, int num_thread)
{
    bool completed = false;
    omp_set_num_threads(num_thread);
    tbb::concurrent_queue<pair<int, pair<Node *, Node *>>> list[num_thread];

    atomic_int list_elements;
    list_elements = 1;
    list[hashNode(map->start->id, num_thread)].push({map->end->calc_heuristic(map->start), {NULL, map->start}});
    Result* ans = new Result(num_thread);
    ans->shortest_path = INT32_MAX;
#pragma omp parallel
    {
        unordered_map<int, int> g;
        int id = omp_get_thread_num(),count = 0,end_id = map->end->id;
        priority_queue<pair<int, pair<Node *, Node *>>> open_list;

        while (!completed)
        {

            while (1)
            {
                pair<int, pair<Node *, Node *>> element;
                bool has_element = list[id].try_pop(element);
                if (has_element) {
                    open_list.push(element);
                } else {
                    break;
                }
            }

            if (open_list.size() == 0)
            {
                if (list_elements == 0)
                {
                    break;
                }
                continue;
            }

            pair<int, pair<Node *, Node *>> element = open_list.top();
            open_list.pop();

            count++;

            Node *node_prev = element.second.first, *node_curr = element.second.second;
            int g_curr = element.first - map->end->calc_heuristic(node_curr);
            if (g.count(node_curr->id) > 0 && g_curr >= g[node_curr->id])
            {
                list_elements.fetch_add(-1);
                continue;
            }
            else
            {
                g[node_curr->id] = g_curr;
            }

            if (node_curr->id == end_id)
            {
                ans->shortest_path = g[node_curr->id];
                completed = true;
                break;
            }

            vector<pair<int, pair<Node *, Node *>>> successors;
            for (auto it : node_curr->adjacent_list)
            {
                Node *node = it.first;
                if (node_prev != NULL && node_prev->id == node->id)
                {
                    continue;
                }
                int wt = it.second,g_updated = wt + g_curr,f1 = g_updated + map->end->calc_heuristic(node);
                successors.push_back({f1, {node_curr, node}});
            }
            list_elements.fetch_add(successors.size() - 1);
            for (auto it : successors)
            {
                list[hashNode(it.second.second->id, num_thread)].push(it);
            }
        }
        ans->nodes_traversed[id] = count;
    }
    return ans;
}
