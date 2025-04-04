#ifndef COUNTRYSTORAGE_H
#define COUNTRYSTORAGE_H
#include "CountryTotal.h"

// Struct to represent the node in the BST
struct Node {
    double min; // Min value in subtree
    double max; // Max value in subtree
    Node* left; // Pointer to left child
    Node* right; // Pointer to right child
    std::string* countries; // Array of country names
    int country_cnt; // Num of countires in nodes
};

// Class that handles multilple countires
class CountryStorage {
    // Private member variables, only accessible within the CountryStorage class
    private:
        CountryTotal country_storage[512];
        Node* root = nullptr;
        std::string series_code_cur;
        int primary_hash(std::string country_code);
        int secondary_hash(std::string country_code);
        int overall_hash(std::string country_code, int i);
    public:
        CountryStorage(); // Constructor
        ~CountryStorage(); // Destructor
        int index_finder(const std::string& country_name) const;
        void load(const std::string& filename);
        void list(const std::string& country_name) const;
        void range(const std::string& series_code) const;
        void split_level(Node* node);
        void build_tree(std::string country_name);
        void delete_tree(Node* node);
        void find(double mean, std::string operation);
        bool delete_country(std::string country_name);
        void limit(std::string condition);
        void lookup(std::string country_code);
        void remove(std::string country_code);
        void insert(std::string country_code, std::string filename);
        CountryTotal* get_country_storage_array();

};

#endif