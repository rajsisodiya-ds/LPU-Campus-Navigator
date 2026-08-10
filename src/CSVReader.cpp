#include "CSVReader.h"
#include <fstream>
#include <sstream>
#include <iostream>

static std::vector<std::string> splitCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, ',')) {
        fields.push_back(field);
    }
    if (!line.empty() && line.back() == ',') {
        fields.push_back("");
    }
    return fields;
}

std::vector<LocationRecord> CSVReader::readLocations(const std::string& filepath) {
    std::vector<LocationRecord> locations;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: could not open " << filepath << std::endl;
        return locations;
    }

    std::string line;
    bool isHeader = true;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (isHeader) { isHeader = false; continue; }

        auto fields = splitCSVLine(line);
        if (fields.size() < 3) continue;

        LocationRecord rec;
        rec.id = std::stoi(fields[0]);
        rec.name = fields[1];
        rec.category = fields[2];
        rec.notes = fields.size() > 3 ? fields[3] : "";
        locations.push_back(rec);
    }
    return locations;
}

std::vector<EdgeRecord> CSVReader::readEdges(const std::string& filepath) {
    std::vector<EdgeRecord> edges;
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: could not open " << filepath << std::endl;
        return edges;
    }

    std::string line;
    bool isHeader = true;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (isHeader) { isHeader = false; continue; }

        auto fields = splitCSVLine(line);
        if (fields.size() < 3) continue;

        EdgeRecord rec;
        rec.from = fields[0];
        rec.to = fields[1];
        rec.distance_m = std::stod(fields[2]);
        edges.push_back(rec);
    }
    return edges;
}
