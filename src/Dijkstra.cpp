#include "dijkstra.h"
#include <queue>
#include <limits>
#include <algorithm>

PathResult Dijkstra::shortestPath(const Graph& graph, int sourceId, int destId) {
    PathResult result;
    int n = graph.nodeCount();
    if (sourceId < 0 || destId < 0 || sourceId >= n || destId >= n) return result;

    const double INF = std::numeric_limits<double>::infinity();
    std::vector<double> dist(n, INF);
    std::vector<int> parent(n, -1);
    std::vector<bool> visited(n, false);

    using PQItem = std::pair<double,int>;
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<PQItem>> pq;

    dist[sourceId] = 0.0;
    pq.push({0.0, sourceId});

    while (!pq.empty()) {
        auto top = pq.top();
        double d = top.first;
        int u = top.second;
        pq.pop();
        if (visited[u]) continue;
        visited[u] = true;
        if (u == destId) break;

        for (const auto& edge : graph.getNeighbors(u)) {
            int v = edge.first;
            double weight = edge.second;
            if (visited[v]) continue;
            double newDist = dist[u] + weight;
            if (newDist < dist[v]) {
                dist[v] = newDist;
                parent[v] = u;
                pq.push({newDist, v});
            }
        }
    }

    if (dist[destId] == INF) return result;

    std::vector<int> path;
    for (int at = destId; at != -1; at = parent[at]) path.push_back(at);
    std::reverse(path.begin(), path.end());

    result.found = true;
    result.totalDistance = dist[destId];
    result.path = path;
    return result;
}

PathResult Dijkstra::nearestOfCategory(const Graph& graph, int sourceId, const std::string& category) {
    PathResult result;
    int n = graph.nodeCount();
    if (sourceId < 0 || sourceId >= n) return result;

    const double INF = std::numeric_limits<double>::infinity();
    std::vector<double> dist(n, INF);
    std::vector<int> parent(n, -1);
    std::vector<bool> visited(n, false);

    using PQItem = std::pair<double,int>;
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<PQItem>> pq;

    dist[sourceId] = 0.0;
    pq.push({0.0, sourceId});

    int found = -1;
    while (!pq.empty()) {
        auto top = pq.top();
        double d = top.first;
        int u = top.second;
        pq.pop();
        if (visited[u]) continue;
        visited[u] = true;

        if (graph.getCategory(u) == category && u != sourceId) {
            found = u;
            break;
        }

        for (const auto& edge : graph.getNeighbors(u)) {
            int v = edge.first;
            double weight = edge.second;
            if (visited[v]) continue;
            double newDist = dist[u] + weight;
            if (newDist < dist[v]) {
                dist[v] = newDist;
                parent[v] = u;
                pq.push({newDist, v});
            }
        }
    }

    if (found == -1) return result;

    std::vector<int> path;
    for (int at = found; at != -1; at = parent[at]) path.push_back(at);
    std::reverse(path.begin(), path.end());

    result.found = true;
    result.totalDistance = dist[found];
    result.path = path;
    return result;
}
