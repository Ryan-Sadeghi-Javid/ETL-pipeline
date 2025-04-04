#include <iostream>
#include <string>
#include "CountryStorage.h"
#include "Graph.h"
int main() {
    CountryStorage cs;
    Graph graph;
    std::string command; // String to contain the users command
    // Infinite loop of user commands until the "exit" command 
    while (std::cin >> command) {
        if (command == "LOAD") { // Handeling the users "LOAD" command
            std::string filename;
            std::cin >> filename;
            cs.load(filename);
            std::cout << "success" << std::endl; // "LOAD" command will always print "success"
        }
        else if (command == "LIST") { // Handeling the users "LIST" command
        
            std::string country_name;
            std::cin >> country_name;
            cs.list(country_name);
        }

        else if (command == "RANGE") { // Handeling the users "RANGE" command
        
            std::string series_code_input;
            std::cin >> series_code_input;
            cs.range(series_code_input);
        }

        else if(command == "BUILD") { // Handeling the users "BUILD" command
            std::string series_code_input;
            std::cin >> series_code_input;
            cs.build_tree(series_code_input);
        }

    else if (command == "FIND") { // Handeling the users "FIND" command
    double mean_value;
    std::string operation;
    std::cin >> mean_value >> operation;
    cs.find(mean_value, operation);
}

    else if (command == "DELETE") { // Handeling the users "DELETE" command
        std::string country_name;
        std::cin >> country_name;
        if(cs.delete_country(country_name)) {
            std::cout<<"success"<<std::endl;            
        } else{
            std::cout<<"failure"<<std::endl;            
        }
    }

    else if (command == "LIMITS") { // Handeling the users "LIMITS" command
        std::string condition;
        std::cin >> condition;
        cs.limit(condition);
    }
    else if(command == "LOOKUP") {
        std::string country_code;
        std::cin >> country_code;
        cs.lookup(country_code);
    }
    else if(command == "REMOVE") {
        std::string country_code;
        std::cin >> country_code;
        cs.remove(country_code);
    }
    else if(command == "INSERT") {
        std::string country_code;
        std::string filename;
        std::cin >> country_code >> filename;
        cs.insert(country_code,filename);
    }
    else if(command == "INITIALIZE") {
        graph.initalize(cs.get_country_storage_array());
    }
    else if(command == "UPDATE_EDGES") {
        std::string series_code_input;
        double threshold;
        std::string relation;
        std::cin >> series_code_input >> threshold >> relation;
        graph.update_edges(series_code_input, threshold, relation);
    }
    else if(command == "ADJACENT") {
        std::string country_code;
        std::cin >> country_code;
        graph.adjacent(country_code);
    }
    else if(command == "PATH") {
        std::string country_code_1;
        std::string country_code_2;
        std::cin >> country_code_1 >> country_code_2;
        graph.path(country_code_1, country_code_2);
    }
    else if(command == "RELATIONSHIPS") {
        std::string country_code_1;
        std::string country_code_2;
        std::cin >> country_code_1 >> country_code_2;
        graph.relationships(country_code_1, country_code_2);
    }
    else if(command == "EXIT") { // Handeling the users "EXIT" command
        return 0;
    }
    }
}