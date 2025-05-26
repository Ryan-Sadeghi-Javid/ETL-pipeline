# 🌍 ECE250 Project 5 – Country Relationship ETL Graph

A high-performance ETL-based data pipeline and analytics system written in C++. This project processes time-series country statistics, transforms them using statistical analysis, and models relationships between countries as a dynamic, queryable graph. It supports interactive CLI-based analysis for exploring trends in health, agriculture, economics, and other key metrics.

---

## 🚀 Overview

This project reads large CSV datasets of country-level time-series data, parses and transforms them into structured `TimeSeries` objects, and builds:
- A **Binary Search Tree (BST)** to support fast range queries and country lookups
- A **Graph-based model** where edges represent statistically similar relationships between countries

---

## 🛠️ Features

- **Custom ETL Pipeline**  
  Parses, processes, and stores over 500 country records and 60 years of historical data from raw CSVs

- **Binary Search Tree (BST)**  
  Organizes countries based on statistical means for efficient range queries and deletions

- **Graph-Based Relationship Engine**  
  Detects pairwise country relationships using time-series similarity and builds a bidirectional, labeled graph

- **Command-Line Interface (CLI)**  
  Fully interactive system supporting data loading, BST construction, graph generation, relationship analysis, and traversal queries

- **Efficient Memory Management**  
  Implements dynamic allocation with custom destructors, deep copy, and move semantics to ensure optimal performance

---

## 📊 Technologies & Concepts

- `C++` (Object-Oriented Design)
- Custom hashing and collision handling
- Binary Search Trees
- Graphs (Adjacency Lists, Weighted Edges, Tuples)
- Dynamic memory management
- Command-driven architecture

---

## 🧪 Example Commands

```text
LOAD filename.csv                        # Load country data from file
BUILD AG.LND.AGRI.ZS                     # Build BST using agricultural land (% of total) data
INITIALIZE                               # Initialize graph with country nodes
UPDATE_EDGES AG.LND.AGRI.ZS 80 greater   # Connect countries with >80% land used for agriculture
ADJACENT CAN                             # List countries related to Canada
PATH USA IND                             # Check if a path exists between USA and India
RELATIONSHIPS BRA ARG                    # List shared metrics between Brazil and Argentina
