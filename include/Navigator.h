#pragma once
#include <string>
#include <vector>
#include "Graph.h"

class Navigator {
public:
    bool loadData(const std::string& locationsFile, const std::string& edgesFile);
    void findRoute(const std::string& fromName, const std::string& toName) const;
    void findNearestFacility(const std::string& fromName, const std::string& category) const;
    void listLocations() const;

    std::vector<std::string> getAllLocationNames() const;
    std::string getRouteText(const std::string& fromName, const std::string& toName) const;
    std::string getNearestFacilityText(const std::string& fromName, const std::string& category) const;

private:
    Graph graph;
};
