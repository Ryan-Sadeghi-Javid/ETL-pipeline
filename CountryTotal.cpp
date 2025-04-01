#include "CountryTotal.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>

/* 
    CITATION: 

    This constructor was implemented with assistance from ChatGPT (chat.openai.com). 
    ChatGPT provided a general format for how to make a C++ constructor with the prompt:
    "give me general guidance on how to make a constructor definition in c++"
*/
// Constructor for the CountryTotal class
CountryTotal::CountryTotal() : country_name_d(""), country_code_d(""), series(new TimeSeries[2]), capacity(2), cur_size(0) {};

/* 
    CITATION: 

    This destructor was implemented with assistance from ChatGPT (chat.openai.com). 
    ChatGPT provided a general format for how to make a C++ destructor with the prompt:
    "give me general guidance on how to make a destructor definition in c++"
*/
// Destructor for the CountryTotal class
CountryTotal::~CountryTotal() {
    delete[] series;
}

/* 
    CITATION:

    This `resize_double` function was implemented with assistance from ChatGPT (chat.openai.com). 
    ChatGPT provided general guidance on implementing dynamic array resizing, using the prompt:
    "Teach me how to dynamically resize an array in c++"
    Specifically, it suggested:
    - Allocating new arrays with double the current capacity to minimize resizing operations.
    - Copying existing elements from the old arrays (in my case: `series`) to the new arrays.
    - Safely releasing the old memory using `delete[]` to prevent memory leaks.
    - Updating the respective size variable to reflect the new array size.
*/
// Function to increase capacity (double) of the series array
void CountryTotal::resize_double() {
    int new_capacity = capacity*2; // Double the current capacity of the dynamic arrays
    TimeSeries* new_series = new TimeSeries[new_capacity]; // New array to store time series with double the size

    // Copy all of the existing elements from the old arrays to the new arrays
    for(int i = 0;i<cur_size;i++) {
        new_series[i]=series[i];
    }

    // Free the memory of the old arrays to prevent against memory leaks
    delete[] series;

    // Update the pointers to pont to the newly allocated arrays
    series = new_series;
    capacity = new_capacity; // Update the capacity to the new, doubled size
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
    in project 1, into project 2. Hence, I thought it was appropriate to re-cite this function.
*/
// Function to load in time series data from .csv file
void CountryTotal::load(const std::string& country_name, std::ifstream& file) {
    // Free the memory of the old arrays to prepare for loading new data
    delete[] series;

    // Reinitialize the dynamic arrays with a starting capacity of 2
    series = new TimeSeries[2];
    capacity=2; // Setting the new capacty to 2
    cur_size=0; // Setting the array size to 0 - no data in yet


    // Open the given "lab2_multidata.csv" file.    
    // Read the first line of the file (unparsed data)
    std::string line;

    // Reset country name and country code data before loading new data
    country_name_d = "";
    country_code_d = "";
    cur_size = 0;

    // Read the file line by line
    while (std::getline(file, line)) {
        // Parse the unparsed data from the first line using a stringstream
        std::stringstream data_string(line); // Creating the stringstream
        std::string curr_country_name, curr_country_code, series_name, series_code;
        std::getline(data_string, curr_country_name, ','); // Getting the country name
        std::getline(data_string, curr_country_code, ','); // Getting the country code
        std::getline(data_string, series_name, ','); // Getting the series name
        std::getline(data_string, series_code, ','); // Getting the series code
        // Check if the current line matches the requested country
        if (curr_country_name == country_name) {
            // Set the country name and country code for the dataset if matched
            country_name_d = curr_country_name;
            country_code_d = curr_country_code;

            // Resize the `series` array if it has reached capacity
            if (cur_size >= capacity) {
                resize_double();
            }

            // Store the series data in the `series` array
            series[cur_size].set_series_name(series_name);
            series[cur_size].set_series_code(series_code);
        
            int year = 1960;  // Initialize the starting year for the time series data - 1960
            std::string value;  // Temporary string variable to hold various data

            // While loop to read and process data values from the line
            while (std::getline(data_string, value, ',')) { // Seperate by the comma
                double data_value = std::stod(value); // Storing the data as a double as we will preform opreations
                series[cur_size].add(year, data_value); // Add the (year, data) pair to the current series
                year++; // Move to the next year
            }
            cur_size++; // Move to the next time series array
        }
    }
}

// Prints out the name and code of a loaded country and the series names for all time series.
void CountryTotal::list() const {
    std::cout << country_name_d << " " << country_code_d; // Print out the country name and country code

    // Loop through all time series
    for(int i = 0; i<cur_size;i++) {
        std::cout << " " << series[i].get_series_name(); // Print out all series names
    }
    std::cout << std::endl;
}

// Adds a new (year, data) pair to a time series with a given code only if there is valid data at that year
bool CountryTotal::add(const std::string& series_code, int y, int d) {
    // Loop through all time series
    for(int i =0; i<cur_size;i++) {
        if(series[i].get_series_code() == series_code) { // Check if a time series code matches with the desired code
            // Since the "add" function in the Time Series returns a boolean value, we check if we can add a (year,data) pair to the series. (ie: check if we are not overwriting valid data)
            if(series[i].add(y,d)) { 
                return true; // return true if we are able to add the given (year,data) pair
            } else{
                return false; // return false if we are NOT able to add the given (year,data) pair
            }
        } else {
            continue; // skip the current iteration if series code does not match
        }
    }
    return false; // return false if series code does not exist for country
}

// Updates the data value for an existing year, of valid data, to a time series with a given code
bool CountryTotal::update(const std::string& series_code, int y, int d) {
    // Loop through all time series
    for(int i=0; i<cur_size;i++) { 
        if(series[i].get_series_code() == series_code) { // Check if a time series code matches with the desired code
            // Since the "update" function in the Time Series returns a boolean value, we check if we can update a (year,data) pair to the series. (ie: check if we are not overwriting invalid data)
            if(series[i].update(y,d)) {
                return true; // return true if we are able to update the given (year,data) pair
            } else {
                return false; // return false if we are NOT able to add the given (year,data) pair
            }
        } else {
            continue; // skip the current iteration if series code does not match
        }
    }
    return false; // return false if series code does not exist for country
}

// Prints the time series data as (year, data) pairs, for a time series with a given code
void CountryTotal::print(const std::string& series_code) const {
    // Loop through all time series
    for(int i =0; i <cur_size;i++) { 
        if(series[i].get_series_code() == series_code) { // Check if a time series code matches with the desired code
            series[i].print(); // Call the "print" function from the TimeSeries class to print out all of the (year, data) pairs in the desired time series
            return;
        }
    }
    std::cout << "failure" << std::endl; // Print out "failure" if time series code does note exist for country
}

 // Delete the series with the given Code from the data.
bool CountryTotal::delete_series(const std::string& series_code) {
    // Loop through all time series
    for(int i =0; i<cur_size; i++) {
        if(series[i].get_series_code() == series_code) { // Check if a time series code matches with the desired code

            // Shift all of the time series data to delete the desired time series
            for(int j=i;j<cur_size-1; j++) {
                series[j] = series[j+1];
            }
            cur_size--; // Modify the current size to reflect the new data set
            return true; // Return true if we are able to delete data
        }
    }
    return false; // Return false if we are NOT able to delete data
}

// Prints the time series with the biggest mean.
void CountryTotal::biggest() const{
    std::string biggest_year; // String holding the series code with the biggest mean
    int cur_biggest_mean = -100; // Setting the current biggest mean to a really small value
    int cnt = 0; // Counter to check if there was any valid data

    // Loop through all time series data
    for(int i =0; i <cur_size; i++) {
        int cur_mean = series[i].mean(); // Gathering our current series' mean
        if(cur_mean != -1) { // Checking if the current series' mean is NOT -1 (ie: contains valid data)
            cnt++; // Increment counter to show valid data was found
            if(cur_mean > cur_biggest_mean) { // Check if the current mean if greater then our previously found greatest mean
                cur_biggest_mean = cur_mean; // If the current mean is greater then our previously found mean, replace this value
                biggest_year = series[i].get_series_code(); // Collect the series code of the new biggest time series mean
            }
        }
    }
    // If no valid data found, print failure
    if(cnt==0) {
        std::cout << "failure" << std::endl;
    } else { // If valid data found, print the code of the time series with the biggest mean
        std::cout << biggest_year << std::endl;
    }
}

std::string CountryTotal::get_country_name() const {
    return country_name_d;
}

std::string CountryTotal::get_country_code() const {
    return country_code_d;
}

bool CountryTotal::empty() const {
    return country_name_d.empty();
}

void CountryTotal::set_country_name(std::string new_country_name) {
    country_name_d = new_country_name;
}

void CountryTotal::set_country_code(std::string new_country_code) {
    country_code_d = new_country_code;
}

void CountryTotal::add_series(const TimeSeries& ts) {
    if (cur_size == capacity) {
        resize_double();
    }

    series[cur_size] = ts;
    cur_size++;
}

TimeSeries CountryTotal::get_time_series(std::string cur_series_code) const {
    for(int i = 0; i < cur_size; i++) {
        if(series[i].get_series_code() == cur_series_code) {
            return series[i];
        }
    }
    return TimeSeries();
}

TimeSeries CountryTotal::get_time_series(int i) const {
    return series[i];
}

int CountryTotal::get_cur_size() const {
    return cur_size;
}

CountryTotal::CountryTotal(const CountryTotal& new_cs) : country_name_d(new_cs.country_name_d), country_code_d(new_cs.country_code_d), capacity(new_cs.capacity), cur_size(new_cs.cur_size) {
    series = new TimeSeries[capacity];
    for (int i = 0; i < cur_size; i++) {
        series[i] = new_cs.series[i];
    }
}

CountryTotal::CountryTotal(CountryTotal&& new_cs) noexcept : country_name_d(std::move(new_cs.country_name_d)),country_code_d(std::move(new_cs.country_code_d)),series(new_cs.series),capacity(new_cs.capacity),cur_size(new_cs.cur_size) {
    new_cs.series = nullptr;
    new_cs.capacity = 0;
    new_cs.cur_size = 0;
}

CountryTotal& CountryTotal::operator=(const CountryTotal& new_cs) {
    if (this != &new_cs) {
        delete[] series;
        country_name_d = new_cs.country_name_d;
        country_code_d = new_cs.country_code_d;
        capacity = new_cs.capacity;
        cur_size = new_cs.cur_size;
        series = new TimeSeries[capacity];
        for (int i = 0; i < cur_size; i++) {
            series[i] = new_cs.series[i];
        }
    }
    return *this;
}

CountryTotal& CountryTotal::operator=(CountryTotal&& new_cs) noexcept {
    if (this != &new_cs) {
        delete[] series;
        country_name_d = std::move(new_cs.country_name_d);
        country_code_d = std::move(new_cs.country_code_d);
        series = new_cs.series;
        capacity = new_cs.capacity;
        cur_size = new_cs.cur_size;
        new_cs.series = nullptr;
        new_cs.capacity = 0;
        new_cs.cur_size = 0;
    }
    return *this;
}