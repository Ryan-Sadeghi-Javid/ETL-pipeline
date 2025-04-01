#ifndef COUNTRYTOTAL_H
#define COUNTRYTOTAL_H
#include <string>
#include "TimeSeries.h"

class CountryTotal {
    private:
        // Private member variables, only accessible within the CountryTotal class
        std::string country_name_d; // Name of the country (data)
        std::string country_code_d; // Code associated with the country (data)
        TimeSeries* series; // Dynamically allocated array to store the time series in the dataset
        int capacity; // Current capacity of the dynamic arrays
        int cur_size; // Current number of elements in the dataset
        void resize_double(); // Resizes the dynamic arrays to double their current capacity when called

    public:
        CountryTotal(); // Constructor
        ~CountryTotal(); // Destructor
        void load(const std::string& country_name, std::ifstream& file); // Loads ALL time series data from a given country from "lab2_multidata.csv"
        void list() const; // Prints out the name and code of a loaded country and the series names for all time series.
        bool add(const std::string& series_code, int y, int d); // Adds a new (year, data) pair to a time series with a given code
        bool update(const std::string& series_code, int y, int d);  // Updates the data value for an existing year, of valid data, to a time series with a given code
        void print(const std::string& series_code) const;  // Prints the time series data as (year, data) pairs, for a time series with a given code
        bool delete_series(const std::string& series_code); // Delete the series with the given Code from the data.
        void biggest() const; // Prints the time series with the biggest mean.
        std::string get_country_name() const;
        std::string get_country_code() const;
        bool empty() const;
        void set_country_name(std::string country_name);
        void set_country_code(std::string country_code);
        void add_series(const TimeSeries& ts);
        int get_cur_size() const;
        TimeSeries get_time_series(std::string cur_series_code) const;
        TimeSeries get_time_series(int i) const;
        CountryTotal(const CountryTotal& other);
        CountryTotal(CountryTotal&& other) noexcept;
        CountryTotal& operator=(const CountryTotal& other);
        CountryTotal& operator=(CountryTotal&& other) noexcept;
};

#endif