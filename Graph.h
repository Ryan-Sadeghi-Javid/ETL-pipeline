#ifndef GRAPH_H
#define GRAPH_H

#include "CountryStorage.h"
#include <vector>
#include <tuple>
#include <unordered_map>
#include <set>

class Graph {
    private:
        std::unordered_map<std::string, std::unordered_map<std::string, std::set<std::tuple<std::string, double, std::string>>>> adj_list;
    public:
        void node(std::string country_code, std::string country_name);
        void edges(std::vector<std::string> codes, std::string series_code, std::string relation, double limit);
        std::vector<std::string> adjacent(std::string code);
        std::vector<std::tuple<std::string, double, std::string>> reltionship(std::string code_one, std::string code_two);
        bool path(std::string code_one, std::string code_two);
};


#endif