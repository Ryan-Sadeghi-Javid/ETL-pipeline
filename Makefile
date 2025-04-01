all: main.cpp TimeSeries.cpp CountryTotal.cpp
	g++ -std=c++17 main.cpp TimeSeries.cpp CountryTotal.cpp CountryStorage.cpp