# Movie Ticket Booking System (Console) - DSA Mini Project

A simple console-based movie booking app designed to demonstrate data structure usage in C++.  
This repository contains code for managing movies, shows, bookings, and waitlists without relying on STL containers.

## Tech
- C++ (standard library limited to strings & iostream; custom data structures used instead of STL containers)
- Custom DSAs implemented for educational purposes:
  - **BST** (binary search tree) for movie title indexing
  - **Hash table** mapping IDs to array indices
  - **Singly linked list** to track bookings per show
  - **Queue** (circular array) for waitlist management


## How to run
Compile and run `src/main.cpp`.

Use a C++ compiler (e.g. `g++`)

  g++ src\main.cpp src\app\BookingSystem.cpp src\data\*.cpp src\dsa\*.cpp src\ui\*.cpp -o app.exe

  ./app.exe