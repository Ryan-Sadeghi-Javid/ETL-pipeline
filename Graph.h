#ifndef GRAPH_H
#define GRAPH_H

#include "CountryStorage.h"
#include <vector>
#include <tuple>
#include <unordered_map>
#include <set>

class Graph {
    private:
        std::unordered_map<std::string, CountryTotal> node_map; // Each node's data
        std::unordered_map<std::string, std::set<std::string>> adj_list; // Adjancy list that maps a country code to its corresponding country object
        std::unordered_map<std::string, std::unordered_map<std::string,std::set<std::tuple<std::string, double, std::string>>>> relationship; // Maps a pair of countries to a set of tuples that represents the series code
    public:

        // Each of the functions in the design document
        void initalize(CountryTotal countires[512]);
        void update_edges(std::string series_code, double threshold, std::string relation);
        void adjacent(std::string country_code);
        void path(std::string country_code_1, std::string country_code_2);
        void relationships(std::string country_code_1, std::string country_code_2);
};


#endif