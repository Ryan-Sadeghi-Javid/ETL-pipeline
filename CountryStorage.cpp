#include "CountryStorage.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
/* 
    CITATION: 

    This constructor was implemented with assistance from ChatGPT (chat.openai.com). 
    ChatGPT provided a general format for how to make a C++ constructor with the prompt:
    "give me general guidance on how to make a constructor definition in c++"
*/
// Constructor for the CountryStorage class
CountryStorage::CountryStorage() {}
/* 
    CITATION: 

    This destructor was implemented with assistance from ChatGPT (chat.openai.com). 
    ChatGPT provided a general format for how to make a C++ destructor with the prompt:
    "give me general guidance on how to make a destructor definition in c++"
*/
// Destructor for the CountryStorage class
CountryStorage::~CountryStorage() {
    delete_tree(root);
}

// Helper function to locate the index of a certain country in the country_storage array, through the name of the country.
int CountryStorage::index_finder(const std::string& country_name) const {
    for (int i = 0; i < 512; i++) {
        if (country_storage[i].get_country_name() == country_name) {
            return i;
        }
    }
    return -1; // Return -1 if the country does not exist
}

/*
    CITATION:

    This `load` function was modified with assistance from ChatGPT (chat.openai.com) 
    based on the prompt: 
    "My data parsing function is currently not working, please give me general guidence and tips on what 
    possible bugs there might be [followed by the current status of my "load" functon]"
    ChatGPT provided possible bugs and tips for improving the function, such as:
    - Resetting the dynamically allocated arrays (in my case: `years` and `data`) at the start of the function 
      to avoid leftover data.
    - How to use `std::stringstream` in c++ to parse lines of data and separate values by commas.
    - Providing debugging tips, such as adding print statements to track parsing progress.
    - Teaching me how to use the "stod" function in c++.

    PLEASE NOTE: I further prompted ChatGPT to teach me how to use "std::stringstream," this was
    NOT in the inital output. The prompt I used was "please elborate on how to use stringstream in c++"

    PLEASE NOTE: This assistance from ChatGPT was provided in PROJECT 1, however; I reused logic from the load function
    in project 1, into project 3. Hence, I thought it was appropriate to re-cite this function.
*/
// Function to load in time series data from .csv file
void CountryStorage::load(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string country_name, country_code, series_name, series_code;
        // Parse the data
        std::getline(ss, country_name, ',');
        std::getline(ss, country_code, ',');
        std::getline(ss, series_name, ',');
        std::getline(ss, series_code, ',');

        // Populate the time series data
        TimeSeries ts;
        ts.set_series_name(series_name);
        ts.set_series_code(series_code);
        int year = 1960;
        std::string data_value;
        while (std::getline(ss, data_value, ',')) {
            double value = std::stod(data_value);
            ts.add(year, value);
            year++; // Go to the next year
        }
        int i=0, index = 0;
        while(i<512) {
            index = overall_hash(country_code,i);
            if(country_storage[index].empty()) {
                country_storage[index].set_country_name(country_name);
                country_storage[index].set_country_code(country_code);
                country_storage[index].add_series(ts);
                break;
            }
            if(country_storage[index].get_country_code() == country_code) {
                country_storage[index].add_series(ts);
                break;
            }
            i++;
        }
    }
    file.close();
}

// Prints out the name and code of a inputted country and the series names for all time series.
void CountryStorage::list(const std::string& country_name) const {
    int i = index_finder(country_name);
    if (i != -1) {
        country_storage[i].list(); // Calling the list function created in project 2
    } else {
        std::cout << "failure" << std::endl;
    }
}

// Generate the range of a series code
void CountryStorage::range(const std::string& series_code) const {
    double max;
    double min;
    int cnt = 0;
    int index = 0;

    for(int i = 0; i <512;i++) {
        CountryTotal country = country_storage[i];
        for(int j = 0; j<country.get_cur_size(); j++) {
            TimeSeries time_series_cur = country.get_time_series(j); 
            if(time_series_cur.get_series_code() == series_code) { // Find the time series inputted
                double cur_mean = time_series_cur.mean();
                if(cur_mean!=-1){
                    if(index==0){ // Set the inital min and max from the first index
                        min = cur_mean;
                        max = cur_mean;
                    }
                    if(cur_mean > max) { // if we find a mean greater then our previous greatest mean, then replace it
                        max = cur_mean;
                    }
                    if(cur_mean < min) {
                            min = cur_mean;  // if we find a mean less then our previous smallest mean, then replace it
                    }
                    index++;
                    break;
                }
                
            }
        }
    }
    std::cout << min << " " << max << std::endl; // output the min and max nodes
}

/* 
    CITATION: 

    This function was implemented with assistance from ChatGPT (chat.openai.com). 
    ChatGPT provided a general format for how to make a C++ BST deletion function with the prompt:
    "give me general guidance on how to make a BST deletion function definition in c++"
*/
// Delete the entire BST
void CountryStorage::delete_tree(Node* node) {
    if (!node) { // Base case, if current node is null
        return;
    }
    delete_tree(node->left); // Recursively delete the left subtree
    delete_tree(node->right); // Recursively delete the right subtree
    delete[] node->countries;  // Free dynamically allocated country array
    node->countries = nullptr; // Avoiding dangling pointers
    delete node;
    node = nullptr; // Avoiding dangling pointers
}

/*
    CITATION:

    This `build_tree` function was modified with assistance from ChatGPT (chat.openai.com) 
    based on the prompt: 
    "My build BST function is currently not working, please give me general guidence and tips on what 
    possible bugs there might be [followed by the current status of my "build_tree" functon]"
    ChatGPT provided possible bugs and tips for improving the function, such as:
        - Placing all invalid data to the very left (required further prompting)
*/
// Function to build the BST of a series code
void CountryStorage::build_tree(std::string series_code) {
    // delete any existing tree
    delete_tree(root);
    root = nullptr;

    series_code_cur = series_code;

    double max = -100000000; // Set a really small inital max so that any other max value will overtake it
    double min = 100000000; // Set a really large inital min so that any other min value will overtake it
    for (int i = 0; i < 512; i++) {
        if (country_storage[i].empty()) // skip empty values
        {
            continue;
        }
        for (int j = 0; j < country_storage[i].get_cur_size(); j++) {
            const TimeSeries& ts = country_storage[i].get_time_series(j);
            if (ts.get_series_code() == series_code) {
                double cur_mean = ts.mean(); // Find the time series inputted
                if (cur_mean != -1) { // If we are at a valid mean
                    if (cur_mean < min) // if we find a mean greater then our previous greatest mean, then replace it
                    {
                        min = cur_mean;
                    }
                    if (cur_mean > max){ // if we find a mean less then our previous smallest mean, then replace it
                        max = cur_mean;
                    }
                }
                break;
            }
        }
    }
    
    // Defining the countires array for the node
    std::string* countries = new std::string[512]();
    int cnt = 0;

    // Populate the country array
    for (int i = 0; i < 512; i++) {
        if (!country_storage[i].empty()) {
            countries[cnt] = country_storage[i].get_country_name();
            cnt++;
        }
    }
    // Creating and intialzing the values for the root
    root = new Node();
    root->min = min;
    root->max = max;
    root->countries = countries;
    root->country_cnt = cnt;
    root->left = nullptr;
    root->right = nullptr;

    // Split the node at the root to make the rest of the BST
    split_level(root);

    // Find invalid data and place them in the far left node
    Node* temp = root;
    while(temp->left!=nullptr){
        temp = temp->left;
    }
    if(temp->min!=0){
        Node* tempL = new Node();
        tempL->min=0;
        tempL->max=0;
        temp->left=tempL;

        // Create the invalid country string array
        std::string* countries_invalid = new std::string[512]();
        int index_invalid = 0;
        for (int i = 0; i < 512 && index_invalid < 512; i++) {
            if (country_storage[i].empty()){ // skip empty values
                continue;
            }
            bool series = false;
            double country_mean=-1;
            for (int j = 0; j < country_storage[i].get_cur_size(); j++) {
                TimeSeries ts = country_storage[i].get_time_series(j);
                if (ts.get_series_code() == series_code_cur) { // Find the invalid time series
                    country_mean = ts.mean();
                    series = true;
                    break;
                }
            }

            // if no data exists add the country to the invalid list
            if (!series||country_mean == -1) {
                countries_invalid[index_invalid++]=country_storage[i].get_country_name();
            }
    }
        // Creating and intialzing the values for the root
    tempL->countries = countries_invalid;
    tempL->country_cnt = index_invalid;
    tempL->left = nullptr;
    tempL->right = nullptr;
    }

    std::cout << "success" << std::endl;
}

// Recursively split a node in the BST based on the mean values of the countries in that node
void CountryStorage::split_level(Node* node) {
    if (!node) { // Base case, if current node is null
        return;
    }
    double mid_point = (node->min + node->max) / 2; // Computing the midpoint of the node's range

    // Creating new arrays to store the countires of the left and right child nodes
    std::string* left = new std::string[node->country_cnt]();
    std::string* right = new std::string[node->country_cnt]();
    int left_cnt = 0, right_cnt = 0;

    // Populating the left and right child country  arrays
    for (int i = 0; i < node->country_cnt; i++) {
        double mean_cur = 0;
        for (int j = 0; j < 512; j++) {
            if (country_storage[j].get_country_name() == node->countries[i]) {
                mean_cur = country_storage[j].get_time_series(series_code_cur).mean(); // Find the mean of the country by searching thorugh the country storage array
                break;
            }
        }
        // Assign the country to the left or right array by comparing the mean vs midpoint values (ie: smaller -> left array, greater -> right arary)
        if (mean_cur < mid_point) { 
            left[left_cnt++] = node->countries[i];
        } else {
            right[right_cnt++] = node->countries[i];
        }
    }

    // If EVERY country is in one group, you can not generate any more subtrees, so return
    if (left_cnt == 0 || right_cnt == 0) {
        delete[] left;
        delete[] right;
        return;
    }
    // Creating and intialzing the values for the left child
    node->left = new Node();
    node->left->min = node->min;
    node->left->max = mid_point;
    node->left->countries = left;
    node->left->country_cnt = left_cnt;
    node->left->left = nullptr;
    node->left->right = nullptr;
    // Creating and intialzing the values for the right child
    node->right = new Node();
    node->right->min = mid_point;
    node->right->max = node->max;
    node->right->countries = right;
    node->right->country_cnt = right_cnt;
    node->right->left = nullptr;
    node->right->right = nullptr;

    // Split the newly created left and right child nodes
    split_level(node->left);
    split_level(node->right);
}

// Function to find countires less then, equal to, or greater then to a inputted mean
void CountryStorage::find(double mean, std::string operation) {
    if(!root){ // If no tree is made, print failure
        std::cout << "failure" << std::endl;
        return;
    }

    // "less" operation
    if(operation == "less") {
        for(int i = 0; i<root->country_cnt; i++) {
            for(int j = 0;j<512; j++) {
                if(country_storage[j].get_country_name() == root->countries[i]) {
                    for(int k = 0; k<country_storage[j].get_cur_size(); k++) {
                        TimeSeries ts = country_storage[j].get_time_series(k);
                        if(ts.get_series_code() == series_code_cur) { // Finding the correct time series
                            double country_mean = ts.mean();
                            if(country_mean==-1){ // If invalid, set to 0
                                country_mean = 0;
                            }
                            if(country_mean < mean) { // If mean is less then given threshhold, print out
                                std::cout << root->countries[i] << " ";
                            }
                            break;
                        }
                    }
                    break;
                }
            }
        }
        std::cout << std::endl;
    }

    // "greater" operation
    if(operation == "greater") {
        for(int i = 0; i<root->country_cnt; i++) {
            for(int j = 0;j<512; j++) {
                if(country_storage[j].get_country_name() == root->countries[i]) {
                    for(int k = 0; k<country_storage[j].get_cur_size(); k++) {
                        TimeSeries ts = country_storage[j].get_time_series(k);
                        if(ts.get_series_code() == series_code_cur) { // Finding the correct time series
                            double country_mean = ts.mean();
                            if(country_mean == -1){ // If invalid, set to 0
                                country_mean = 0;
                            }
                            if(country_mean > mean) { // If mean is greater then given threshhold, print out
                                std::cout << root->countries[i] << " ";
                            }
                            break;
                        }
                    }
                    break;
                }
            }
        }
        std::cout << std::endl;
    }

    // "equal" operation
    if(operation == "equal") {
        for(int i = 0; i < root->country_cnt; i++) {
            for(int j = 0;j<512; j++) {
                if(country_storage[j].get_country_name() == root->countries[i]) {
                    for(int k = 0; k<country_storage[j].get_cur_size(); k++) {
                        TimeSeries ts = country_storage[j].get_time_series(k);
                        if(ts.get_series_code() == series_code_cur) { // Finding the correct time series
                            double country_mean = ts.mean();
                            if(country_mean == -1){ // If invalid, set to 0
                                country_mean = 0;
                            }
                            if(fabs(country_mean - mean) < 1E-3) {  // If mean is equal then given threshhold (within the tollerance), print out
                                std::cout << root->countries[i] << " ";
                            }
                            break;
                        }
                    }
                    break;
                }
            }
        }
        std::cout << std::endl;
    }   
}

/*
    CITATION:

    This `delete_country` function was modified with assistance from ChatGPT (chat.openai.com) 
    based on the prompt: 
    "My node deletion function is currently not working, please give me general guidence and tips on what 
    possible bugs there might be [followed by the current status of my "delete_country" functon]"
    ChatGPT provided possible bugs and tips for improving the function, such as:
        - Deleting the entire node (required further prompting)
*/
// Function to delete a certain country from all nodes in the BST
bool CountryStorage::delete_country(std::string country_name) {
    if (!root) {  // if no BST exists, print out "failure"
        return false;
    }
    Node* parent = nullptr;
    Node* curr_node = root; // Start traversing from the root node
    double cur_mean = 0; // keeping track of the current countrys mean
    for (int j = 0; j < 512; j++) {
        if ((country_storage[j].get_country_name() == country_name)) {
            for (int k = 0; k < country_storage[j].get_cur_size(); k++) {
                TimeSeries ts = country_storage[j].get_time_series(k); // If country name matches, iterate through all time series data
                if (ts.get_series_code() == series_code_cur) { // Finding the correct time series
                    cur_mean = ts.mean(); // Finding the mean
                    if (cur_mean == -1) { // if invalid, set to mean to 0
                        cur_mean = 0;
                    }
                    break;
                }
            }
            break;
        }
    }

    // Travering through BST
    while (curr_node) {
        bool node_found = false;

        // Checking if the country exists in the current node we are on
        for (int i = 0; i < curr_node->country_cnt; i++) {
            if (curr_node->countries[i]==country_name) {
                node_found = true;
                break;
            }
        }
        if (node_found) { // If the country exists
            for (int i = 0; i < curr_node->country_cnt; i++) {
                if (curr_node->countries[i]==country_name) {
                    // Deleting the country from the country string array
                    for (int j = i; j < curr_node->country_cnt - 1; j++) {
                        curr_node->countries[j] = curr_node->countries[j + 1];
                    }
                    curr_node->country_cnt--; // Decrementing the country count
                    break;
                }
            }
            if (curr_node->country_cnt == 0) { // Deleting the ENTIRE node if the country count becomes 0
                if (!parent) {
                    delete curr_node;
                    root = nullptr;
                } else {
                    if (parent->left == curr_node) {
                        parent->left = nullptr;
                    } else if (parent->right == curr_node) {
                        parent->right = nullptr;
                    }
                    delete curr_node;
                }
            }
        } else{ // If country was NOT found, print failure
            return false;
        }
        // Determine what node to traverse through next based on the midpoint
        parent = curr_node;
        if (cur_mean < (curr_node->min+curr_node->max) / 2) {
            curr_node = curr_node->left;
        } else {
            curr_node = curr_node->right;
        }
    }
    return true;
}

// Function to print out the countires in the leftmost or rightmost node, depending on if the condition is lowest or highest respectivly
void CountryStorage::limit(std::string condition) {
    if(!root) { // If no tree exists, print out "failure"
        std::cout<< "failure" <<std::endl;
        return;
    }
    
    Node* curr_node = root; // Start traverseal from the root node

    if(condition=="lowest") { // If the condition is "lowest"
        while(curr_node->left) { // Travel to the leftmost node
            curr_node=curr_node->left;
        }

        // Print out all the countires in the leftmost node
        for(int i = 0; i<curr_node->country_cnt; i++) {
            std::cout<<curr_node->countries[i] << " ";
        }
        std::cout << std::endl;
        return;
    }
    else if(condition=="highest") { // If the condition is "highest"
        while(curr_node->right) { // Travel to the rightmost node
            curr_node=curr_node->right;
        }

        // Print out all the countires in the rightmost node
        for(int i = 0; i<curr_node->country_cnt; i++) {
            std::cout<<curr_node->countries[i] << " ";
        }
        std::cout << std::endl;
        return;
    }
}

// Converts 3 three letter country code into an index
int CountryStorage::primary_hash(std::string country_code) {
    int w = (country_code[0]-'A') * 676 + (country_code[1]-'A') * 26 + (country_code[2]-'A');
    return w%512;
}

// Secondary hash used for collisions if mulitple countries have the same hash index
int CountryStorage::secondary_hash(std::string country_code) {
    int w = (country_code[0]-'A') * 676 + (country_code[1]-'A') * 26 + (country_code[2]-'A');
    int i = (w/512)%512;
    if(i%2==0){
        i++;
    }
    return i;
}

// Overall hash function given from the project design used for double hashing: primiary + i * secondary
int CountryStorage::overall_hash(std::string country_code, int i) {
    return (primary_hash(country_code) + i*secondary_hash(country_code))%512;
}

// Preforms a lookup in the hash table for a given country code
void CountryStorage::lookup(std::string country_code) {
    int i = 0;
    int index = 0;

    while(i<512) { // Attempts the probing method 512 times
        index = overall_hash(country_code,i); // Getting current hash index

        // If we are at an empty slot, the country does not exist
        if(country_storage[index].empty()) {
            std::cout<<"failure"<<std::endl;
            return;
        }

        // If we have found the country code
        if(country_storage[index].get_country_code() == country_code) {
            std::cout<<"index "<<index<< " searches " << (i+1) << std::endl;
            return;
        }

        // "EEE" is the preset name for removed values from the hash table
        if(country_storage[index].get_country_name() == "EEE") {
            i++;
            continue;
        }
        i++; // Cont' probing if no match is found
    }
    std::cout<<"failure"<<std::endl;
}

// Removes a country from hash table bassed on country code
void CountryStorage::remove(std::string country_code) {
    int index = 0;
    int i = 0;
    bool code_found = false;
    std::string country_name;

    // Attempts the probing method 512 times
    while(i<512) {
        index = overall_hash(country_code,i); // Getting current hash index

        // If we are at an empty slot, the country does not exist
        if(country_storage[index].empty()) {
            std::cout<<"failure"<<std::endl;
            return;
        }

        // If a country matches, then begin removal process
        if(country_storage[index].get_country_code() == country_code) {
            country_name = country_storage[index].get_country_name();
            country_storage[index].set_country_name("EEE"); // Marking deleted entries as "EEE" (used in other functions)
            country_storage[index].set_country_code(""); // Empty country code
            code_found=true; // We have found a country to remove
            break;
        }
        i++;
    }

    // If no country is found, then print "failure"
    if(!code_found) {
        std::cout<<"failure"<<std::endl;
        return;
    }

    // Remove from tree if the tree exists at this moment
    if(root!=nullptr) {
        delete_country(country_name);
    }
    std::cout << "success" << std::endl;
}

// Inserts new country from a GIVEN CSV based on country code
void CountryStorage::insert(std::string country_code, std::string filename) {
    int i = 0;
    int index = 0;
    int insert_index=-1000;
    bool already_in_table = false;
    // Attempts the probing method 512 times
    while(i<512) {
        index = overall_hash(country_code,i);
        if(country_storage[index].empty() || (country_storage[index].get_country_name()=="EEE")) { // If we find an empty or prev. removed slot, make that the index of the new element to add
            if(insert_index==-1000) {
                insert_index=index;
            }
            break;
        }

        // If country already exists
        if(country_storage[index].get_country_code()==country_code) {
            already_in_table=true;
        }
        i++;
    }
    if(already_in_table) {
        std::cout<<"failure"<<std::endl;
        return;
    }

    // Open file and parse
    std::ifstream file(filename);
    std::string line;
    std::string country_name;

    while(std::getline(file,line)){
        std::stringstream ss(line);
        std::string country_name_d, country_code_d, series_name_d, series_code_d;
        std::getline(ss, country_name_d, ',');
        std::getline(ss, country_code_d, ',');
        if(country_code_d!=country_code){
            continue;
        }

        std::getline(ss, series_name_d, ',');
        std::getline(ss, series_code_d, ',');

        // Creating new time series
        TimeSeries ts;
        ts.set_series_name(series_name_d);
        ts.set_series_code(series_code_d);
        int year = 1960;
        std::string data_value;
        while(std::getline(ss,data_value,',')) {
            double value = std::stod(data_value);
            ts.add(year,value);
            year++;
        }
        country_name = country_name_d;

        // Inserting country into hash table at the prev. computed index
        country_storage[insert_index].set_country_name(country_name);
        country_storage[insert_index].set_country_code(country_code);
        country_storage[insert_index].add_series(ts);
    }
    file.close();
    std::cout<<"success"<<std::endl;
}