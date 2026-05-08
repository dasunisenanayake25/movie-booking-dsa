# 🎬 CineBook: Premium Movie Booking System

CineBook is a high-performance, modern movie ticket booking system featuring a **C++ RESTful backend** and a **luxury Noir/Gold Web Frontend**. This project is a comprehensive demonstration of custom **Data Structures and Algorithms (DSA)**, built entirely from scratch without relying on the C++ Standard Template Library (STL) for core logic.

---

## ✨ Key Features

- **💎 Premium Experience**: A stunning "Dark Luxury" interface with glassmorphism, responsive design, and cinematic typography.
- **💺 Dynamic Seating Grid**: Interactive 2D seating visualization with real-time state tracking.
- **🤖 Group Recommendation**: Smart algorithms that automatically find the best available contiguous seating blocks for groups.
- **⏳ Waitlist System**: Efficient FIFO queue management for handling bookings when a show reaches full capacity.
- **⚡ High-Performance Lookups**: O(1) data retrieval using custom Hash Tables for show and booking indices.
- **🔍 Instant Search**: Binary Search Tree (BST) indexing for lighting-fast movie title lookups.

---

## 🏗️ System Architecture

CineBook follows a client-server architecture where a **C++ HTTP Server** (`cpp-httplib`) serves as the backend engine, exposing RESTful APIs to a **Vanilla JavaScript/CSS3** frontend.

### 🧠 Custom Data Structures Implemented

| Structure | Description | Purpose |
| :--- | :--- | :--- |
| **SeatingGrid** | 2D Matrix / Matrix-based logic | Real-time seat assignment and visualization. |
| **BSTMovieTitle** | Binary Search Tree | Optimized O(log n) searching for movie titles. |
| **HashTable** | Chaining-based Hash Map | O(1) lookup for Shows and Bookings by ID. |
| **QueueArray** | Circular Array Queue | FIFO management for the ticket waitlist. |
| **SinglyLinkedList**| Dynamic Linked List | Tracking all active bookings linked to a specific show. |
| **MergeSort** | Tailored Sort Algorithm | Efficiently organizing showtimes by datetime. |

---

## 🚀 Getting Started

### 📋 Prerequisites
- **C++ Compiler**: `g++` (MinGW-w64 recommended).
- **Network**: Windows Sockets (`ws2_32`) for the HTTP server.

### 🛠️ Installation & Execution

#### **Web Version (Full Experience)**
The web version provides the premium UI and interactive seat map.

1. **Compile the Server**:
   ```bash
   g++ -std=c++14 -I./include src/*.cpp -lws2_32 -o server.exe
   ```
2. **Run the Server**:
   ```bash
   ./server.exe
   ```
3. **Open in Browser**:
   Navigate to [http://localhost:8080](http://localhost:8080)

#### **Console Version**
For a lightweight terminal-based experience.

1. **Compile the App**:
   ```bash
   g++ -I./include src/BSTMovieTitle.cpp src/BookingSystem.cpp src/ConsoleIO.cpp src/HashTableIntToIndex.cpp src/Menu.cpp src/MergeSortShows.cpp src/QueueArrayWait.cpp src/SampleData.cpp src/SeatingGrid.cpp src/SinglyLinkedList.cpp src/Store.cpp src/main.cpp -o app.exe
   ```
2. **Run the App**:
   ```bash
   ./app.exe
   ```

---

## 👥 Members Contributions

The success of CineBook is the result of collaborative efforts across backend logic and frontend design:

### **Amarasie**
- **Core DSA**: Implemented the `SeatingGrid` matrix logic and the high-performance `HashTableIntToIndex` for ID-based lookups.
- **Seat Logic**: Developed the underlying logic for seat selection, state tracking, and availability checks.
- **Frontend Design**: Led the aesthetic design and layout of the `index.html` dashboard, establishing the core Noir/Gold visual language.

### **Dasuni**
- **Core DSA**: Implemented the `BSTMovieTitle` for optimized movie searching and the `MergeSortShows` algorithm for chronological show organization.
- **System Objects**: Designed and implemented the core `Show` and `Movie` data structures.
- **Frontend Functionality**: Developed interactive frontend components and integrated search features.

### **Danith**
- **Integration**: Developed the `server.cpp` RESTful API layer and established the communication bridge between C++ and JavaScript.
- **System Logic**: Orchestrated the `BookingSystem` core logic, coordinating interactions between various DSA components.
- **Waitlist Logic**: Implemented the `QueueArrayWait` system for handling booking overflows.
- **Project Lead**: Managed the overall system architecture, data initialization, and frontend-backend synchronization.

---

## 📜 License
CineBook is developed for educational purposes to demonstrate the practical application of Data Structures and Algorithms in a modern, professional system architecture.
