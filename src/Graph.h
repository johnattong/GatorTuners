#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <vector>

template <typename T>
class Graph {
    std::map<T, std::list<T>> adjList; // Adjacency list to store the graph

public:
    // Function to add an edge between vertices u and v of the graph
    // Function to add an edge between vertices u and v of the graph
    void add_edge(T u, T v) {
        if (u == nullptr || v == nullptr) {
            return; // Don't add the edge if v is nullptr
        }
        // Check if edge from u to v already exists
        if (std::find(adjList[u].begin(), adjList[u].end(), v) == adjList[u].end()) {
            // Add edge from u to v
            adjList[u].push_back(v);
        }
        // Check if edge from v to u already exists
        if (std::find(adjList[v].begin(), adjList[v].end(), u) == adjList[v].end()) {
            // Add edge from v to u because the graph is undirected
            adjList[v].push_back(u);
        }
    }

    // Function to print the adjacency list representation of the graph
    void print() {
        std::cout << "Adjacency list for the Graph: " << std::endl;
        // Iterate over each vertex
        for (const auto& i : adjList) {
            // Print the vertex
            std::cout << i.first << " -> ";
            // Iterate over the connected vertices
            for (const auto& j : i.second) {
                // Print the connected vertex
                std::cout << j << " ";
            }
            std::cout << std::endl;
        }
    }

    vector<T> getDFS() {
        vector<T> result;
        set<T> visited;

        function<void(T)> dfs = [&](T node) {
            if (visited.find(node) != visited.end()) return;

            visited.insert(node);
            result.push_back(node);

            for (const auto& neighbor : adjList[node]) {
                dfs(neighbor);
            }
        };

        for (const auto& pair : adjList) {
            if (visited.find(pair.first) == visited.end()) {
                dfs(pair.first);
            }
        }

        return result;
    }

    double jaccardSimilarity(T u, T v) {
        if (adjList.find(u) == adjList.end() || adjList.find(v) == adjList.end()) {
            return 0.0; // One or both nodes don't exist
        }

        std::set<T> setU(adjList[u].begin(), adjList[u].end());
        std::set<T> setV(adjList[v].begin(), adjList[v].end());

        // Calculate intersection
        std::vector<T> intersection;
        std::set_intersection(setU.begin(), setU.end(), setV.begin(), setV.end(),
                              std::back_inserter(intersection));

        // Calculate union
        std::vector<T> union_set;
        std::set_union(setU.begin(), setU.end(), setV.begin(), setV.end(),
                       std::back_inserter(union_set));

        // Jaccard similarity = size of intersection / size of union
        return static_cast<double>(intersection.size()) / static_cast<double>(union_set.size());
    }
};

#endif //GRAPH_H