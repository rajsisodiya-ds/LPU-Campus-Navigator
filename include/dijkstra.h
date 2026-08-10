#pragma once
#include <vector>
#include <string>
#include "Graph.h"

struct PathResult {
    bool found = false;
    double totalDistance = 0.0;
    std::vector<int> path;
};

class Dijkstra {
public:
    static PathResult shortestPath(const Graph& graph, int sourceId, int destId);
    static PathResult nearestOfCategory(const Graph& graph, int sourceId, const std::string& category);
};
