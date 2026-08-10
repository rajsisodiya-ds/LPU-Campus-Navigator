#include "Navigator.h"
#include "CSVReader.h"
#include "dijkstra.h"
#include <iostream>
#include <iomanip>
#include <sstream>

bool Navigator::loadData(const std::string& locationsFile, const std::string& edgesFile) {
    auto locations = CSVReader::readLocations(locationsFile);
    auto edges = CSVReader::readEdges(edgesFile);

    if (locations.empty()) {
        std::cerr << "Error: no locations loaded from " << locationsFile << std::endl;
        return false;
    }

    for (const auto& loc : locations) graph.addLocation(loc.name, loc.category);
    for (const auto& e : edges) graph.addPath(e.from, e.to, e.distance_m);

    std::cout << "Loaded " << locations.size() << " locations and "
              << edges.size() << " paths.\n";
    return true;
}

void Navigator::findRoute(const std::string& fromName, const std::string& toName) const {
    std::cout << getRouteText(fromName, toName);
}

void Navigator::findNearestFacility(const std::string& fromName, const std::string& category) const {
    std::cout << getNearestFacilityText(fromName, category);
}

void Navigator::listLocations() const {
    std::cout << "\nAvailable locations:\n";
    for (int i = 0; i < graph.nodeCount(); ++i) {
        std::cout << "  " << graph.getNameForId(i) << "\n";
    }
    std::cout << "\n";
}

std::vector<std::string> Navigator::getAllLocationNames() const {
    std::vector<std::string> names;
    for (int i = 0; i < graph.nodeCount(); ++i) {
        names.push_back(graph.getNameForId(i));
    }
    return names;
}

std::string Navigator::getRouteText(const std::string& fromName, const std::string& toName) const {
    std::ostringstream out;
    int fromId = graph.getIdForName(fromName);
    int toId = graph.getIdForName(toName);

    if (fromId == -1) { out << "Location not found: " << fromName << "\n"; return out.str(); }
    if (toId == -1) { out << "Location not found: " << toName << "\n"; return out.str(); }

    PathResult result = Dijkstra::shortestPath(graph, fromId, toId);
    if (!result.found) {
        out << "No path found between " << fromName << " and " << toName << ".\n";
        return out.str();
    }

    out << "Route: " << fromName << " -> " << toName << "\n";
    out << "----------------------------------------\n";
    for (size_t i = 0; i < result.path.size(); ++i) {
        out << (i + 1) << ". " << graph.getNameForId(result.path[i]) << "\n";
    }
    out << "----------------------------------------\n";
    out << std::fixed << std::setprecision(0);
    out << "Total distance: " << result.totalDistance << " m\n";
    double minutes = result.totalDistance / 80.0;
    out << "Estimated walking time: " << std::setprecision(1) << minutes << " min\n";
    return out.str();
}

std::string Navigator::getNearestFacilityText(const std::string& fromName, const std::string& category) const {
    std::ostringstream out;
    int fromId = graph.getIdForName(fromName);
    if (fromId == -1) { out << "Location not found: " << fromName << "\n"; return out.str(); }

    PathResult result = Dijkstra::nearestOfCategory(graph, fromId, category);
    if (!result.found) {
        out << "No location of category " << category << " reachable from " << fromName << ".\n";
        return out.str();
    }

    out << "Nearest " << category << " from " << fromName << ":\n";
    out << graph.getNameForId(result.path.back()) << "\n";
    out << std::fixed << std::setprecision(0);
    out << "Distance: " << result.totalDistance << " m\n";
    return out.str();
}
