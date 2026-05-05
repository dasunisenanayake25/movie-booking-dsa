# Movie Ticket Booking System (Console) - DSA Mini Project

A modern, console-based movie ticket booking app designed to demonstrate fundamental Data Structures and Algorithms (DSA) usage in C++.  
This repository contains code for managing movies, shows, bookings, waitlists, and seating arrangements without relying on C++ STL containers.

## 🚀 Features
- **Seat Booking & Management**: Real-time tracking of seat availability using a custom `SeatingGrid` component.
- **Waitlist Management**: First-In-First-Out (FIFO) queue management for fully booked shows.
- **Fast Lookups**: Hash table-based data retrieval mapping IDs directly to arrays.
- **Fast Searching**: Binary Search Tree (BST) used for efficient movie title querying.
- **Sorting**: Merge Sort used for organizing shows.
- **Custom Structures**: Singly linked lists to track bookings instead of standard vectors.

## 🏗️ Project Architecture
The project follows a modular structure, separating interfaces (`include/`) from implementations (`src/`).

### Directory Structure
- `include/`: Header files (`.h`) containing class definitions and interfaces.
- `src/`: Source files (`.cpp`) containing the core logic and custom DSA implementations.

### Custom DSAs Implemented
- **BSTMovieTitle**: Binary Search Tree for indexing and searching movie titles.
- **HashTableIntToIndex**: Hash map structure mapping IDs to array indices.
- **SinglyLinkedListInt**: List structure tracking bookings per show.
- **QueueArrayWait**: Circular array acting as a Queue for waitlist requests.
- **MergeSortShows**: Implementation of Merge Sort tailored for sorting shows.
- **SeatingGrid**: 2D Matrix structure for real-time seating states and assignments.

## 🛠️ Technology Stack
- **Language**: C++ (Standard library usage strictly limited to strings and iostream)
- **Architecture**: Separated Header & Source Modular Pattern

## ⚙️ How to Compile & Run
The application can be compiled natively using `g++`. It is also pre-configured with a `.vscode/tasks.json` for easy building in VS Code.

### Command Line Compilation
1. Open your terminal or command prompt and navigate to the project root directory.
2. Compile the source code by including the header directory and all source files:
   ```sh
   g++ -I./include src/*.cpp -o app.exe
   ```
3. Run the compiled application:
   ```sh
   ./app.exe
   ```

### VS Code Compilation
1. Open the project folder in VS Code.
2. Press `Ctrl + Shift + B` (Windows/Linux) or `Cmd + Shift + B` (macOS) to run the default build task.
3. Run `./app.exe` in the integrated terminal.

## 📜 License
This project was developed for educational purposes to demonstrate the practical application of custom Data Structures and Algorithms.