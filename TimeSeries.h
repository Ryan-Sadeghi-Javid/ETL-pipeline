#ifndef TIMESERIES_H
#define TIMESERIES_H
#include <string>

class TimeSeries {

    private:
        // Private member variables, only accessible within the TimeSeries class
        std::string series_name; // Name of the time series
        std::string series_code; // Code associated with the time series
        int* years; // Dynamically allocated array to store the years in the dataset
        double* data; // Dynamically allocated array to store the data values corresponding to the years
        int capacity; // Current capacity of the dynamic arrays
        int cur_size; // Current number of elements in the dataset
        void resize_double(); // Resizes the dynamic arrays to double their current capacity when called
    public:
        TimeSeries(); // Constructor
        ~TimeSeries(); // Destructor
        TimeSeries(const TimeSeries& other); // Copy constructors
        TimeSeries(TimeSeries&& other) noexcept; // Move constructors
        TimeSeries& operator=(const TimeSeries& other); // Copy assignment operator
        TimeSeries& operator=(TimeSeries&& other) noexcept; // Move assignment operator

        bool load(const std::string& filename); // Loads time series data from a file
        void print() const; // Prints the time series data as (year, value) pairs
        bool add(int y, double d); // Adds a new (year, value) pair to the time series, keeping the data sorted
        bool update(int y, double d); // Updates the data value for an existing year, of valid data, in the time series
        double mean() const; // Calculates the mean of the time series data
        void is_monotonic() const; // Checks if dataset is monotonic
        void best_fit() const; // Calculates the slope and y-intercept of the line of best fit
        void set_series_name(std::string new_series_name); // Setter function for the series name private variables
        void set_series_code(std::string new_series_code); // Setter function for the series code private variables
        std::string get_series_name() const; // Getter function for the series name private variables
        std::string get_series_code() const; // Getter function for the series code private variables
};
#endif