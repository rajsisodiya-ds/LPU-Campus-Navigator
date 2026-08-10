#include "Navigator.h"
#include <iostream>
#include <limits>

void printMenu() {
    std::cout << "===== LPU Campus Navigator =====\n";
    std::cout << "1. Find shortest route between two locations\n";
    std::cout << "2. Find nearest facility by category\n";
    std::cout << "3. List all locations\n";
    std::cout << "4. Exit\n";
    std::cout << "Choice: ";
}

int main() {
    Navigator navigator;
    if (!navigator.loadData("data/locations.csv", "data/edges.csv")) {
        std::cerr << "Failed to load campus data. Exiting.\n";
        return 1;
    }

    int choice = 0;
    while (true) {
        printMenu();
        if (!(std::cin >> choice)) break;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 1) {
            std::string from, to;
            std::cout << "From: "; std::getline(std::cin, from);
            std::cout << "To: "; std::getline(std::cin, to);
            navigator.findRoute(from, to);
        } else if (choice == 2) {
            std::string from, category;
            std::cout << "From: "; std::getline(std::cin, from);
            std::cout << "Category (hostel/academic/food/facility/admin/residence/utility/gate): ";
            std::getline(std::cin, category);
            navigator.findNearestFacility(from, category);
        } else if (choice == 3) {
            navigator.listLocations();
        } else if (choice == 4) {
            std::cout << "Goodbye!\n";
            break;
        } else {
            std::cout << "Invalid choice.\n";
        }
    }
    return 0;
}
