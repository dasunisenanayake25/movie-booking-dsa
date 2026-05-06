# CineBook: Movie Ticket Booking System

A modern, high-performance movie ticket booking system featuring a **C++ RESTful backend** and a **Glassmorphism-themed Web Frontend**. This project demonstrates the practical application of custom Data Structures and Algorithms (DSA) without relying on the C++ Standard Template Library (STL) for core logic.

## 🚀 Key Features
- **Modern Web UI**: A stunning "dark cinema" interface with glassmorphism, responsive design, and smooth animations.
- **Seat Booking & Management**: Real-time tracking and visualization of seating grids using a custom `SeatingGrid` component.
- **Group Recommendation Engine**: Advanced DSA-based algorithms to find contiguous seating blocks for group bookings.
- **Waitlist Management**: FIFO queue management for fully booked shows.
- **High-Performance Lookups**: Custom Hash Tables mapping IDs to data structures for O(1) retrieval.
- **Efficient Searching**: Binary Search Tree (BST) indexing for instant movie title querying.

## 🏗️ Project Architecture
The system uses a **C++ HTTP Server** (`cpp-httplib`) that exposes RESTful APIs to a **Vanilla JavaScript** frontend.

### Custom DSAs Implemented
- **BSTMovieTitle**: Binary Search Tree for indexing and searching movie titles.
- **HashTableIntToIndex**: Hash map structure mapping IDs to array indices.
- **SinglyLinkedListInt**: List structure tracking bookings per show.
- **QueueArrayWait**: Circular array acting as a Queue for waitlist requests.
- **MergeSortShows**: Tailored Merge Sort for organizing showtimes.
- **SeatingGrid**: 2D Matrix structure for real-time seating states and assignments.

---

## ⚙️ Setup & Execution

### 1. Requirements
- **Compiler**: `g++` (MinGW-w64 recommended for Windows).
- **Libraries**: `ws2_32` (Windows Sockets) for the web server.

### 2. Running the Web Version (Recommended)
The web version provides the full experience with the modern UI.

**Step A: Compile the Web Server**
In your terminal, run:
```sh
g++ -std=c++14 -I./include src/BSTMovieTitle.cpp src/BookingSystem.cpp src/ConsoleIO.cpp src/HashTableIntToIndex.cpp src/Menu.cpp src/MergeSortShows.cpp src/QueueArrayWait.cpp src/SampleData.cpp src/SeatingGrid.cpp src/SinglyLinkedList.cpp src/Store.cpp src/server.cpp -lws2_32 -o server.exe
```
*(Alternatively, use the **Build Web Server** task in VS Code via `Ctrl+Shift+B`)*

**Step B: Run the Server**
```sh
./server.exe
```

**Step C: Open the Frontend**
Open your web browser and navigate to:
[http://localhost:8080](http://localhost:8080)

---

### 3. Running the Console Version
For a pure terminal-based experience.

**Step A: Compile the Console App**
```sh
g++ -I./include src/BSTMovieTitle.cpp src/BookingSystem.cpp src/ConsoleIO.cpp src/HashTableIntToIndex.cpp src/Menu.cpp src/MergeSortShows.cpp src/QueueArrayWait.cpp src/SampleData.cpp src/SeatingGrid.cpp src/SinglyLinkedList.cpp src/Store.cpp src/main.cpp -o app.exe
```
*(Alternatively, use the **Build Console App** task in VS Code)*

**Step B: Run the App**
```sh
./app.exe
```

## 📜 License
Developed for educational purposes to demonstrate the practical application of custom Data Structures and Algorithms in a modern system architecture.