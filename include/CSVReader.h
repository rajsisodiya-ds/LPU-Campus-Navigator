#pragma once
#include <string>
#include <vector>

struct LocationRecord {
    int id;
    std::string name;
    std::string category;
    std::string notes;
};

struct EdgeRecord {
    std::string from;
    std::string to;
    double distance_m;
};

class CSVReader {
public:
    static std::vector<LocationRecord> readLocations(const std::string& filepath);
    static std::vector<EdgeRecord> readEdges(const std::string& filepath);
};
