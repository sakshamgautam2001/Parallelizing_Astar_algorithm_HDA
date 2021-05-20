#include <fstream>
#include <string>
#include <chrono>

#include "map.h"
#include "sequential.h"
#include "hda.h"

using namespace std;

typedef struct query
{
    chrono::_V2::steady_clock::time_point start, stop;
    Result* (*func)(const Map *map, int num_thread);
    Result* answer;
    int num_thread;
    string mode;

    void run(const Map *map)
    {
        start = chrono::steady_clock::now();
        answer = func(map, num_thread);
        stop = chrono::steady_clock::now();
    }

    query(Result* (*func)(const Map *map, int num_thread), string mode, int num_thread)
    {
        this->func = func;
        this->mode = mode;
        this->num_thread = num_thread;
    }

    void display_result()
    {
        cout << mode << " " << num_thread << endl;
        auto time = chrono::duration_cast<chrono::milliseconds>(stop - start).count();
        cout << "Total Time : " << time << "ms" << endl;
        cout << "Length of shortest path : " << answer->shortest_path << endl;
        int s = 0;
        for (int i = 0; i < num_thread; i ++) {
            s += answer->nodes_traversed[i];
        }
        double mean = s*1.0/num_thread;
        cout << "Traversed " << s << " nodes in total" << endl;
        cout << "Overall traversal rate: " << fixed << s*1.0/time*1000 << endl;
        cout << "Traversed " << fixed << mean << " nodes on average" << endl;
        cout << "Average traversal rate: " << fixed << mean / time*1000 << endl;

    }
} query;

int main(int argc, char** argv)
{
    string input_file = "input_cases/graph_3_3.txt";
    ifstream enter(input_file, ifstream::in);
    if (!enter) {
        cout << "Invalid input file: " << input_file << endl;
        return 0;
    }
    Map *map = new Map(enter);
    cout << "Input file loaded: " << input_file << endl;

    vector<query> queries({
                          {&sequential, "Sequential", 1},
                          {&hda_openmp, "HDA OpenMP", 1},
                          {&hda_openmp, "HDA OpenMP", 2},
                          {&hda_openmp, "HDA OpenMP", 4},
                          {&hda_openmp, "HDA OpenMP", 8},
                        });

    for (int i = 0; i < queries.size(); i++)
    {
        queries[i].run(map);
        queries[i].display_result();
    }
}
