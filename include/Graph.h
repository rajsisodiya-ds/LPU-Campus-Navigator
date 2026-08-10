#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class Graph {
public:
    void addLocation(const std::string& name, const std::string& category);
    void addPath(const std::string& from, const std::string& to, double distance_m);

    int getIdForName(const std::string& name) const;
    std::string getNameForId(int id) const;
    bool hasLocation(const std::string& name) const;

    const std::vector<std::pair<int,double>>& getNeighbors(int id) const;
    int nodeCount() const { return static_cast<int>(names.size()); }
    const std::string& getCategory(int id) const;

private:
    std::unordered_map<std::string,int> nameToId;
    std::vector<std::string> names;
    std::vector<std::string> categories;
    std::vector<std::vector<std::pair<int,double>>> adjacency;
};
