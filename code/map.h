#ifndef FINAL_MAP_H
#define FINAL_MAP_H

#include <bits/stdc++.h>
#include <cmath>
#include <utility>
#include <fstream>

using namespace std;


class Node
{
public:
    int id, x, y;
    vector<pair<Node *, int>> adjacent_list;
    
    //Constructor
    Node(int node_id)
    {
        id = node_id;
        x = y = -1;
    }

    int calc_heuristic(Node *node)
    {
        return abs(node->x - this->x) + abs(node->y - this->y);
    }
};

class Result
{
public:
    int shortest_path;
    int* nodes_traversed;
    Result (int num_thread) {
        nodes_traversed = new int[num_thread];
    }
    ~Result() {
        delete nodes_traversed;
    }
};

class Map
{
public:
    Node *start, *end;

    vector<Node *> nodes;

    int w, h;

public:
    Map(ifstream& enter)
    {
        enter >> w >> h;

        nodes = vector<Node *>(w * h);

        int startX, startY, endX, endY;
        enter >> startX >> startY >> endX >> endY;

        vector<vector<Node *>> nodeMatrix(h, vector<Node *>(w, nullptr));

        for (int i = 0; i < h; i++)
        {
            for (int j = 0; j < w; j++)
            {
                Node *node = new Node(w * i + j);
                nodes[node->id] = node;
                nodeMatrix[i][j] = node;

                node->x = i;
                node->y = j;
            }
        }

        for (int i = 0; i < w; i++)
        {
            for (int j = 0; j < h; j++)
            {
                int x, y;
                enter >> x >> y;
                int dirs;
                enter >> dirs;
                while (dirs--)
                {
                    char dir;
                    enter >> dir;
                    int x1, y1;
                    if(dir == 'U'){
                      	 x1 = x;
                        y1 = y - 1;
                    }    
                    else if(dir == 'D'){
                        x1 = x;
                        y1 = y + 1;
                    }    
                    else if(dir == 'R'){
                        x1 = x + 1;
                        y1 = y;
                    }    
                    else if(dir == 'L'){
                        x1 = x - 1;
                        y1 = y;
                    }
                    else{
                        throw string("Invalid input");
                    }

                    nodeMatrix[y][x]->adjacent_list.push_back({nodeMatrix[y1][x1], 1});
                
                }
            }
        }

        start = nodeMatrix[startY][startX];

        end = nodeMatrix[endY][endX];
    }
};

#endif //FINAL_MAP_H
