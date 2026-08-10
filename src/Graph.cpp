#include "Graph.h"
#include <iostream>

void Graph::addLocation(const std::string& name, const std::string& category) {
    if (nameToId.count(name)) return;
    int id = static_cast<int>(names.size());
    nameToId[name] = id;
    names.push_back(name);
    categories.push_back(category);
    adjacency.emplace_back();
}

void Graph::addPath(const std::string& from, const std::string& to, double distance_m) {
    if (!nameToId.count(from)) {
        std::cerr << "Warning: unknown location \x27" << from << "\x27 in edge, adding it.\n";
        addLocation(from, "unknown");
    }
    if (!nameToId.count(to)) {
        std::cerr << "Warning: unknown location \x27" << to << "\x27 in edge, adding it.\n";
        addLocation(to, "unknown");
    }
    int a = nameToId[from];
    int b = nameToId[to];
    adjacency[a].push_back({b, distance_m});
    adjacency[b].push_back({a, distance_m});
}

int Graph::getIdForName(const std::string& name) const {
    auto it = nameToId.find(name);
    if (it == nameToId.end()) return -1;
    return it->second;
}

std::string Graph::getNameForId(int id) const {
    if (id < 0 || id >= static_cast<int>(names.size())) return "";
    return names[id];
}

bool Graph::hasLocation(const std::string& name) const {
    return nameToId.count(name) > 0;
}

const std::vector<std::pair<int,double>>& Graph::getNeighbors(int id) const {
    return adjacency.at(id);
}

const std::string& Graph::getCategory(int id) const {
    return categories.at(id);
}
