#include "TimeSeries.h"
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
// Constructor for the TimeSeries class
TimeSeries::TimeSeries() : years(new int[2]), data(new double[2]), capacity(2), cur_size(0) {}

/* 
    CITATION: 

    This destructor was implemented with assistance from ChatGPT (chat.openai.com). 
    ChatGPT provided a general format for how to make a C++ destructor with the prompt:
    "give me general guidance on how to make a destructor definition in c++"
*/
// Destructor for the TimeSeries class
TimeSeries::~TimeSeries() {
    delete[] years;
    delete[] data;
}

/* 
    CITATION: 

    This following move and copy constructors were implemented with assistance from ChatGPT (chat.openai.com):
    "give me general guidance on how to make a destructor definition in c++"
    ChatGPT provided a general format for how to make C++ move and copy constructors with the prompt:
    "give me general guidance on how to make move and copy constructors in c++"
*/
// Copy constructor for the TimeSeries class
TimeSeries::TimeSeries(const TimeSeries& new_ts) : years(new int[new_ts.capacity]), data(new double[new_ts.capacity]),capacity(new_ts.capacity), cur_size(new_ts.cur_size),series_name(new_ts.series_name),series_code(new_ts.series_code) {
    // Loop through all elements in the "years" and "data" array
    for (int i = 0; i < cur_size; i++) {
        years[i] = new_ts.years[i]; // Copy each year value
        data[i] = new_ts.data[i]; // Copy each data value
    }
}
/* 
    CITATION: 

    This following move and copy constructors were implemented with assistance from ChatGPT (chat.openai.com):
    "give me general guidance on how to make a destructor definition in c++"
    ChatGPT provided a general format for how to make C++ move and copy constructors with the prompt:
    "give me general guidance on how to make move and copy constructors in c++"
*/
// Move constructor for the TimeSeries class
TimeSeries::TimeSeries(TimeSeries&& new_ts) noexcept : years(new_ts.years), data(new_ts.data),capacity(new_ts.capacity), cur_size(new_ts.cur_size),series_name(std::move(new_ts.series_name)), series_code(std::move(new_ts.series_code)) {
    // Set the old object's pointers to nullptr so it doesn't free them
    new_ts.years = nullptr;
    new_ts.data = nullptr;

    // Reset the other attributes of the old object
    new_ts.capacity = 0;
    new_ts.cur_size = 0;
}
/* 
    CITATION: 

    This following move and copy constructors were implemented with assistance from ChatGPT (chat.openai.com):
    "give me general guidance on how to make a destructor definition in c++"
    ChatGPT provided a general format for how to make C++ move and copy constructors with the prompt:
    "give me general guidance on how to make move and copy constructors in c++"

    PLEASE NOTE: I further prompted ChatGPT asking about copy assigment operator's for assistance with the prompt:
    "give me general guidance on implementing a copy assigment operator"
*/
// Copy assignment operator for the TimeSeries class
TimeSeries& TimeSeries::operator=(const TimeSeries& new_ts) {
    // check for self-assignment - don't copy if assigning to itself
    // free existing memory before overwriting it
    if (this!= &new_ts) {
        delete[] years;
        delete[] data;

        // Allocate new memory for years and data arrays
        years = new int[new_ts.capacity];
        data = new double[new_ts.capacity];

        // Copy over data from new_ts
        capacity = new_ts.capacity;
        cur_size = new_ts.cur_size;
        series_name = new_ts.series_name;
        series_code = new_ts.series_code;
        // Copy the years and data values from new_ts
        for (int i = 0; i < cur_size; i++) {
            years[i] = new_ts.years[i];
            data[i] = new_ts.data[i];
        }
    }
    return *this;
}
/* 
    CITATION: 

    This following move and copy constructors were implemented with assistance from ChatGPT (chat.openai.com):
    "give me general guidance on how to make a destructor definition in c++"
    ChatGPT provided a general format for how to make C++ move and copy constructors with the prompt:
    "give me general guidance on how to make move and copy constructors in c++"

    PLEASE NOTE: I further prompted ChatGPT asking about move assigment operator's for assistance with the prompt:
    "give me general guidance on implementing a move assigment operator"
*/
// Move assignment operator for the TimeSeries class
TimeSeries& TimeSeries::operator=(TimeSeries&& new_ts) noexcept {
    if (this != &new_ts) {
        // Free the existing memory before taking ownership of new_ts's data
        delete[] years;
        delete[] data;

        // Copy the pointers from new_ts
        years = new_ts.years;
        data = new_ts.data;

        // Copy data from new_ts
        capacity = new_ts.capacity;
        cur_size = new_ts.cur_size;

        // Move the series name and code - avoids deep copy
        series_name = std::move(new_ts.series_name);
        series_code = std::move(new_ts.series_code);

        // Nullify new_ts's pointers so its destructor doesn't delete moved data
        new_ts.years = nullptr;
        new_ts.data = nullptr;
        // Reset new_ts's data since its contents were moved
        new_ts.capacity = 0;
        new_ts.cur_size = 0;
    }
    return *this;
}
/* 
    CITATION:

    This `resize_double` function was implemented with assistance from ChatGPT (chat.openai.com). 
    ChatGPT provided general guidance on implementing dynamic array resizing, using the prompt:
    "Teach me how to dynamically resize an array in c++"
    Specifically, it suggested:
    - Allocating new arrays with double the current capacity to minimize resizing operations.
    - Copying existing elements from the old arrays (in my case: `years` and `data`) to the new arrays.
    - Safely releasing the old memory using `delete[]` to prevent memory leaks.
    - Updating the respective size variable to reflect the new array size.
*/
// Function to increase capacity (double) of the years and data array
void TimeSeries::resize_double() {
    int new_capacity = capacity * 2; // Double the current capacity of the dynamic arrays
    int* new_years = new int[new_capacity]; // New array to store years with double the size
    double* new_data = new double[new_capacity]; // New array to store data with double the size

    // Copy all of the existing elements from the old arrays to the new arrays
    for (int i = 0; i < cur_size; i++) {
        new_years[i] = years[i];
        new_data[i] = data[i];
    }

    // Free the memory of the old arrays to prevent against memory leaks
    delete[] years;
    delete[] data;

    // Update the pointers to pont to the newly allocated arrays
    years = new_years;
    data = new_data;

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
*/
// Function to load in time series data from .csv file
bool TimeSeries::load(const std::string& filename) {
    // Free the memory of the old  arrays to prepare for loading new data
    delete[] years;
    delete[] data;

    // Reinitialize the dynamic arrays with a starting capacity of 2
    years = new int[2];
    data = new double[2];
    capacity = 2; // Setting the new capacty to 2
    cur_size = 0; // Setting the array size to 0 - no data in yet
    
    // Open the file directed by the filename
    std::ifstream file(filename);

    // Read the first line of the file (unparsed data)
    std::string line;
   if (!std::getline(file,line)) {
        return false; // Return false if the file contains no valid data
    } 

    // Parse the unparsed data from the first line using a stringstream
    std::stringstream data_string(line); // Creating the stringstream
    std::string country_name,country_code,series_namet,series_codet;
    std::getline(data_string, country_name, ','); // Getting the country name
    std::getline(data_string, country_code, ','); // Getting the country code
    std::getline(data_string, series_namet, ','); // Getting the series name
    std::getline(data_string, series_codet, ','); // Getting the series code
    series_name = series_codet;
    series_code = series_codet;
    int year = 1960; // Initialize the starting year for the time series data - 1960
    std::string value_string; // Temporary string variable to hold various data

    // While loop to read and process data values from the line
    while (std::getline(data_string, value_string, ',')) { // Seperate by the comma
            double value = std::stod(value_string); // Storing the data as a double as we will preform opreations

                // Check and resize the dynamic array if required
                if (cur_size == capacity) { 
                    resize_double();
                }

                years[cur_size] = year; // Store the current array in the "years" array
                data[cur_size] = value; // Store the current data in the "data" array
                cur_size++; // Add one to the array size
                year++; // Add one to the current year
    }
    file.close(); // Close the file after all data has been read
    return true; // Return true if valid data was loaded
}

// Function to print all data in the time series
void TimeSeries::print() const {
    int cnt = 0; // Counter to store the number of valid data found

    // Loop through all data
    for (int i = 0; i < cur_size; i++) {
        if(data[i] != -1) { // Checking if the current data is valid, if it is, print out that data
            std::cout << "(" << years[i] << "," << data[i] << ") ";
            cnt++; // Add one to the counter if valid data is found
        } else {
            continue; // Skip to the next iteration if data is not valid
        }
    }
    if(cnt==0) { // Print out "failure" if the counter is at 0 (ie: no valid data found)
        std::cout << "failure" << std::endl;
    } else { // Otherwise print a new line
        std::cout << std::endl;
    }
}

// Function to add data manually to the time series
bool TimeSeries::add(int y, double d) {
    // Loop through all data to check if the year already exists
    for (int i = 0; i < cur_size; i++) {
        if (years[i] == y) { // Check if any of the years in the "years" array matches with the year to be added
            if(data[i] != -1) { // If the data written to the year to be added is valid, then return false
                return false;
            } else {
                data[i] = d; // If the data written to the year to be added is invalid, then overwrite the invalid data
                return true;
            }
        }
    }
    // Check if the arrays are at capacity, if so, increase the size
    if (cur_size == capacity) {
        resize_double();
    }

    // Finding the correct position to insert the new year being added to keep the array sorted
    int pos=0;
    while(pos<cur_size && years[pos]<y){ // Increment position until the correct spot is found
        pos++;
    }

    // Shift elements to make space for the new year and data
    for(int i = cur_size; i>pos; i--) {
        years[i] = years[i-1]; // Move the year at index i-1 to index i
        data[i]= data[i-1]; // Move the data at index i-1 to index i
    }

    // Set the new year to it's desired position now that all data has been shifted
    years[pos] = y;
    data[pos]=d;
    cur_size++; // Increase the current array size to refelect the new data entry
    return true;
}

// Function to update a piece of VALID data in the array
bool TimeSeries::update(int y, double d) {
    // Loop through the array to find the year to be updated
    for (int i = 0; i < cur_size; i++) {
        if (years[i]==y) { // Check if any of the years matches the year to be updated
            if(data[i]==-1) { // If the data to be updated is invalid, return false, as invalid data can not be updated
                return false;
            }
            data[i]=d; // If the data is NOT invalid, update it with the new piece of data
            return true;
        }
    }
    return false;
}

// Function to calculate the mean of the current dataset
double TimeSeries::mean() const {
    int cnt = 0; // Counter to keep track of the valid data points
    double sum = 0; // Counter to keep track of the sum of all valid data
    // Loop through all elements in the data array
    for (int i = 0; i < cur_size; i++) {
        if(data[i] != -1) { // If the data is valid, then add it's value to the array, and add one to the counter
            sum += data[i];
            cnt++; // Increment the counter if valid data is found
        }
    }
    double final = sum / cnt; // Calculate the mean by dividing the total sum of data by the number of valid data points
    
    if(cnt==0) { // If there are no valid data points, print "failure"
        return -1;
    } else { // If there are valid data points, print the mean
        return final;
    }
}

// Function to check if the current dataset is monotonic
void TimeSeries::is_monotonic() const {
    int cnt = 0; // Counter to keep track of the valid data points
    bool increasing = true; // Boolean to keep track if the series is monotonic
    bool decreasing = true; // Boolean to keep track if the series is monotonic
    // Loop through all elements in the data array STARTING from the second element
    for (int i=1; i<cur_size; i++) { 
        if(data[i] != -1) { // Check if the data is valid
        // If the current value is less than the previous value, then it is NOT increasing
        if (data[i] < data[i - 1]) {
            increasing = false;
        }
        // If the current value is greater than the previous value, then it is NOT decreasing
        if (data[i] > data[i - 1]) { 
            decreasing = false;
        }
        cnt++; // Increment the counter if valid data is found
        }
    }
    // If there was no valid data (in this case less then two valid data points since array starts at i=1), print "failure"
    if(cnt == 0) {
        std::cout << "failure" << std::endl;
    // If either increasing or decreasing is true (ie: the dataset is increasing or decreasing), then the series is monotonic
    } else if (increasing || decreasing) {
        std::cout << "series is monotonic" << std::endl;
    // If none of these are true, the series is not monotonic
    } else {
        std::cout << "series is not monotonic" << std::endl;
    }
}

// Function to get the slope and intercept for the line of best fit of the current dataset
void TimeSeries::best_fit() const {
    double m = 0; // Slope of the line of best fit
    double b = 0; // Y-intercept of the line of best fit
    int cnt = 0; // Counter to keep track of the valid data points
    double x = 0; // Sum of all years
    double y = 0; // Sum of all data values
    double xy = 0; // Sum of years multiplied by data for every valid point
    double x2 = 0; // Sum of years squared for every valid point
    // Loop through all elements in the data array
    for (int i = 0; i < cur_size;i++) {
        if(data[i] != -1) { // Check if the data is valid
        // Add all data from current iteration to the variables defined above
            x += years[i];
            y += data[i];
            xy += years[i] * data[i];
            x2 += years[i] * years[i];
            cnt++;
        }
    }
    m = (cnt*xy - x*y) /(cnt*x2 - x*x); // Calculate the slope - m - of the line of best fit
    b = (y - m*x) / cnt; // Calculate the y-intercept - b - of the line of best fit

    if(cnt == 0) { // If there are no valid data points, print "failure"
        std::cout << "failure" << std::endl;
    } else { // Otherwise, print out the slope and y-intercept of the dataset
        std::cout << "slope is " << m << " intercept is " << b << std::endl;
    }
}

// Setter function for the series name
void TimeSeries::set_series_name(std::string new_series_name) {
    series_name = new_series_name;
}

// Setter function for the series code
void TimeSeries::set_series_code(std::string new_series_code) {
    series_code = new_series_code;
}

// Getter function for the series name
std::string TimeSeries::get_series_name() const {
    return series_name;
}

// Setter function for the series name
std::string TimeSeries::get_series_code() const {
    return series_code;
}