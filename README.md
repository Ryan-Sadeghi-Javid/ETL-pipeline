🌍 ECE250 Project 5 – Country Relationship ETL Graph
This project implements an ETL-based data pipeline and analytics system in C++ that ingests time-series statistics for countries, transforms them using statistical analysis, and models relationships as a dynamic graph structure. The system enables querying countries based on trends in health, agriculture, economics, or other time-series metrics.

🚀 Overview
The project reads large CSV datasets of country statistics, parses and transforms the data into structured TimeSeries objects, then builds a binary search tree (BST) and graph-based relational model. Relationships between countries are encoded as edges if they share a statistical pattern based on a chosen metric and threshold.

🛠️ Features
Custom ETL Pipeline: Parses, processes, and stores over 500 country records and 60 years of historical data.

Binary Search Tree (BST): Organizes countries based on statistical means to support fast range queries and deletions.

Graph-Based Relationship Engine: Detects relationships between countries based on time-series statistics and forms a bidirectional, labeled graph.

Command-Line Interface: Fully interactive command system supporting data loading, range queries, graph building, relationship analysis, and adjacency/path searches.

Efficient Memory Management: Implements dynamic memory allocation with deep copy/move semantics to optimize performance.

📁 Project Structure
bash
Copy
Edit
├── main.cpp               # Command-line parser and control flow
├── CountryStorage.cpp/h   # Data ingestion and BST building
├── CountryTotal.cpp/h     # Manages individual country time series
├── TimeSeries.cpp/h       # Time series data structure with analytics
├── Graph.cpp/h            # Graph implementation with edge relationships
├── ECE250 Project 5.pdf   # Project spec
📊 Technologies Used
C++ (Object-Oriented Design)

Custom Hashing & Collision Handling

Binary Search Trees

Graph Data Structures (Adjacency Lists, Tuples)

Dynamic Memory Management

Command-Driven Architecture

📌 Example Commands
LOAD filename.csv – Load dataset

BUILD AG.LND.AGRI.ZS – Build BST using agriculture land data

INITIALIZE – Initialize empty graph with country nodes

UPDATE_EDGES AG.LND.AGRI.ZS 80 greater – Add edges between countries with >80% land use for agriculture

ADJACENT CAN – List countries related to Canada

PATH USA IND – Check if a connection exists between the USA and India

RELATIONSHIPS BRA ARG – List shared statistical relationships between Brazil and Argentina

🧠 Learning Highlights
STL containers: unordered_map, set, tuple

File I/O and CSV parsing using sstream

Algorithm design for BFS, data partitioning, and BST traversal

Memory management: destructor, deep copy, move semantics

📜 License
This project is for academic purposes as part of the University of Waterloo’s ECE250 course.
