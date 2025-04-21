#include "Graph.h"
#include <iostream>
#include<queue>
#include <cmath>

// Function to initalize all nodes of the graph with current country storage data
void Graph::initalize(CountryTotal countires[512]) {

    // Get rid of existing graph
    node_map.clear();
    adj_list.clear();
    relationship.clear();

    // Loop through country storage array
    for(int i = 0; i <512;i++) {
        // Add countries that aren't empty or have been removed ("EEE" -> removed)
        if(!countires[i].empty() && countires[i].get_country_name() != "EEE") {
            node_map[countires[i].get_country_code()] = countires[i];
        }
    }
    std::cout << "success" << std::endl;    
}

// Update edges based on a certain reltionship inputted by the user
void Graph::update_edges(std::string series_code, double threshold, std::string relation) {
    bool edges_added = false; // Tracking if a min of one d
    for(std::unordered_map<std::string, CountryTotal>::iterator i = node_map.begin(); i!= node_map.end(); i++) {
        for(std::unordered_map<std::string, CountryTotal>::iterator j = node_map.begin(); j!=node_map.end(); j++) {
            if(i == j) {
                continue;
            }

            CountryTotal& country_1 = i->second;
            CountryTotal& country_2 = j->second;

            TimeSeries time_series_1 = country_1.get_time_series(series_code);
            TimeSeries time_series_2 = country_2.get_time_series(series_code);

            double mean_1 = time_series_1.mean();
            double mean_2 = time_series_2.mean();

            if(mean_1==-1 || mean_2==-1) {
                continue;
            }

            bool relation_one = false;
            bool relation_two = false;

            if(relation=="greater") {
                if(mean_1 > threshold) {
                    relation_one = true;
                }
                if(mean_2 > threshold) {
                    relation_two = true;
                }
            }
            else if(relation=="equal") {
                if(std::fabs(mean_1-threshold) < 1E-3) {
                    relation_one = true;
                }
                if(std::fabs(mean_2-threshold) < 1E-3) {
                    relation_two = true;
                }
            } else if(relation=="less"){
                if(mean_1 < threshold) {
                    relation_one = true;
                }
                if(mean_2 < threshold) {
                    relation_two = true;
                }
            }

            if(relation_one && relation_two) {
                std::string series_code_1 = i->first;
                std::string series_code_2 = j->first;
                std::tuple<std::string, double, std::string> relation_tuple = std::make_tuple(series_code,threshold,relation);
                if(relationship[series_code_1][series_code_2].count(relation_tuple) == 0 && relationship[series_code_2][series_code_1].count(relation_tuple) == 0) {
                    relationship[series_code_1][series_code_2].insert(relation_tuple);
                    relationship[series_code_2][series_code_1].insert(relation_tuple);
                    adj_list[series_code_1].insert(series_code_2);
                    adj_list[series_code_2].insert(series_code_1);
                    edges_added = true;
                }
            }
        }

    }
    if(edges_added) {
        std::cout << "success" << std::endl;
    } else{
        std::cout << "failure" << std::endl;
    }
}

// Function to find all adjacent country's to a user inputted country code
void Graph::adjacent(std::string country_code) {
    std::unordered_map<std::string, CountryTotal>::iterator i = node_map.find(country_code);
    if(i == node_map.end()) {
        std::cout << "failure" << std::endl;
        return;
    }
    std::unordered_map<std::string,std::set<std::string>>::iterator adj_list_i = adj_list.find(country_code);
    if(adj_list_i == adj_list.end() || adj_list_i->second.empty()) {
        std::cout << "none" <<std::endl;
        return;
    }
    std::set<std::string> adj_set_i = adj_list_i->second;
    for(std::set<std::string>::iterator j = adj_set_i.begin(); j != adj_set_i.end(); j++) {
        std::unordered_map<std::string, CountryTotal>::iterator adj = node_map.find(*j);
        if(adj != node_map.end()) {
            std::cout << adj->second.get_country_name() << " ";
        }
    }
    std::cout<<std::endl;
}

// Function to check if a path exist between two countrys with BFS
void Graph::path(std::string country_code_1, std::string country_code_2) {
    if(node_map.find(country_code_1) == node_map.end() || node_map.find(country_code_2) == node_map.end()) {
        std::cout << "false" << std::endl;
        return;
    }
    std::set<std::string> visited;
    std::queue<std::string> queue;
    queue.push(country_code_1);
    visited.insert(country_code_1);
    while(!queue.empty()) {
        std::string current_node = queue.front();
        queue.pop();
        if(current_node == country_code_2) {
            std::cout << "true" << std::endl;
            return;
        }
        if(adj_list.find(current_node) != adj_list.end()) {
            for(std::set<std::string>::iterator i = adj_list.find(current_node)->second.begin(); i!=adj_list.find(current_node)->second.end(); i++ ) {
                if(visited.find(*i) == visited.end()) {
                    queue.push(*i);
                    visited.insert(*i);
                }

            }

        }
    }

    std::cout<<"false"<<std::endl;
    return;
}


// Function to print rellationship tuples of two countires
void Graph::relationships(std::string country_code_1, std::string country_code_2) {
    if(relationship.find(country_code_1) != relationship.end() && relationship.at(country_code_1).find(country_code_2) != relationship.at(country_code_1).end() ) {
        for(std::set<std::tuple<std::string, double, std::string>>::iterator i = relationship.at(country_code_1).at(country_code_2).begin(); i != relationship.at(country_code_1).at(country_code_2).end(); i++) {
            std::string series_code = std::get<0>(*i);
            double threshold = std::get<1>(*i);
            std::string relation = std::get<2>(*i);

            std::cout << "(" << series_code << " " << threshold << " " << relation << ") ";
        }
    } else {
        std::cout << "none";
    }
    std::cout << std::endl;
}