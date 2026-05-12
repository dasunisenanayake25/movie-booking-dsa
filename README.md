# 🎬 CineBook — Cinema Booking System

A full-stack cinema booking system built with a **C++ backend** (REST API via cpp-httplib) and a **vanilla HTML/CSS/JS frontend**. The system supports movie browsing, seat selection, group bookings, waitlist management, and booking cancellations — all powered by custom-built data structures.

---

## 📁 Project Structure

```
.
├── frontend/
│   ├── index.html          # Landing page (movie & show browser)
│   ├── show.html           # Seat selection & booking page
│   ├── booking.html        # Booking confirmation & cancellation
│   ├── css/style.css       # Global styles & design system
│   └── js/api.js           # Frontend API client
├── include/                # C++ header files
├── src/                    # C++ source files
│   ├── server.cpp          # HTTP REST server & route handlers
│   ├── BookingSystem.cpp   # Core booking logic
│   └── ...                 # DSA implementations
└── .vscode/tasks.json      # Build tasks
```

---

## 👥 Team Members & Contributions

### 🎨 Amarasie — Frontend & Seating Grid

**Responsible for:** `frontend/index.html`, `frontend/css/style.css`, `src/SeatingGrid.cpp`, `include/SeatingGrid.h`, `src/HashTableIntToIndex.cpp`, `include/HashTableIntToIndex.h`, `src/SinglyLinkedList.cpp`, `include/SinglyLinkedListInt.h`

**Frontend work:**
- Designed and built the landing page (`index.html`) — Netflix-inspired dark theme with a poster-collage hero, movie cards, and show listings.
- Authored the global CSS design system (`style.css`) with CSS custom properties for typography, color tokens, spacing, and responsive layout.

**DSA Implementation 1 — SeatingGrid (2D Array Grid)**

The `SeatingGrid` class manages the per-show seat layout as a dynamically allocated 2D integer array.

```
SeatingGrid
├── grid[ROWS][COLS]         — 2D array; values: 0=available, 1=selected, 2=booked
├── seatMap (HashTable)      — encodes (row,col) → flat index for O(1) seat lookup
└── selectedSeats (LinkedList) — tracks currently selected seats before confirmation
```

Key operations:

| Method | Description | Complexity |
|---|---|---|
| `selectSeat(row, col)` | Mark a seat as selected | O(1) |
| `deselectSeat(row, col)` | Unmark a selected seat | O(1) |
| `confirmBooking()` | Commit selected seats to booked | O(k) — k = selected seats |
| `cancelSeat(row, col)` | Free a booked seat | O(1) |
| `getAvailableCount()` | Count free seats | O(n·m) |
| `suggestSeats(count)` | Heuristic seat recommendation | O(n·m) |

**DSA Implementation 2 — HashTableIntToIndex (Chained Hash Table)**

Maps integer keys (show ID, booking ID, seat key) to array indices for O(1) average-case lookup. Used in three contexts: show ID index, booking ID index, and per-seat encoding inside `SeatingGrid`.

```
HashTableIntToIndex
└── buckets[]  — array of linked-list chains
    └── Node { key, index, next }
```

| Method | Description | Complexity |
|---|---|---|
| `put(key, index)` | Insert or update mapping | O(1) avg |
| `get(key, outIndex)` | Lookup index by key | O(1) avg |
| `remove(key)` | Delete a mapping | O(1) avg |

**DSA Implementation 3 — SinglyLinkedListInt (Linked List)**

Tracks the list of booking IDs associated with each show, used for iterating and cancelling bookings per show.

```
SinglyLinkedListInt
└── Node { value (bookingId), next }
```

| Method | Description | Complexity |
|---|---|---|
| `insertLast(value)` | Append booking ID | O(1) |
| `removeValue(value)` | Remove by value | O(n) |
| `contains(value)` | Search for a value | O(n) |

---

### 🎥 Dasuni — Movie Lookup & Search

**Responsible for:** `frontend/booking.html`, `frontend/js/api.js`, `src/BSTMovieTitle.cpp`, `include/BSTMovieTitle.h`, `src/MergeSortShows.cpp`, `include/MergeSortShows.h`

**Frontend work:**
- Built the booking confirmation and cancellation page (`booking.html`).

**DSA Implementation 1 — BSTMovieTitle (Binary Search Tree)**

The `BSTMovieTitle` BST indexes movies by their lowercase title for fast exact-match lookup.

```
BSTMovieTitle
└── Node { movieId, title, titleLower, left, right, parent }
```

Key operations:

| Method | Description | Complexity |
|---|---|---|
| `insert(movieId, title)` | Insert movie into BST by title | O(h) |
| `searchExact(title)` | Case-insensitive exact title lookup | O(h) |
| `inOrderList(outIds, count)` | Return all movie IDs sorted alphabetically | O(n) |

> h = tree height. Balanced case: O(log n). Worst case: O(n).

**DSA Implementation 2 — MergeSortShows (Merge Sort)**

Sorts show indices by datetime so shows are listed in chronological order.

```
MergeSortShows::sortIndices(arr[], count, store)
  └── mergeSort → divide & conquer → merge by show datetime
```

| Method | Description | Complexity |
|---|---|---|
| `sortIndices(arr, count, store)` | Sort show index array by datetime | O(n log n) |

---

### ⚙️ Danith — Waitlist, Server & Core Backend

**Responsible for:** `frontend/show.html`, `src/server.cpp`, `src/BookingSystem.cpp`, `src/QueueArrayWait.cpp`, `src/Store.cpp`, `src/SampleData.cpp`, `src/ConsoleIO.cpp`, `src/Menu.cpp`, `src/main.cpp`, and all corresponding headers.

**Frontend work:**
- Implemented `api.js`, the JavaScript fetch-based client that wires the frontend pages to all backend REST endpoints.
- Built the interactive seat selection page (`show.html`) with real-time seat state rendering (available / selected / booked) and group booking flow.

**Backend work:**
- Built the full REST API server using cpp-httplib (`server.cpp`) with 10+ endpoints covering movies, shows, seats, bookings, and the waitlist.
- Implemented the `BookingSystem` class — the central orchestrator that coordinates all DSA components to handle bookings, cancellations, seat allocation, and waitlist processing.
- Seeded sample data (`SampleData.cpp`) and built the console I/O and menu system for standalone CLI mode.

**DSA Implementation — QueueArrayWait (Circular Array Queue)**

Manages the per-show waitlist. Each show has its own queue of pending seat requests.

```
QueueArrayWait
├── data[]   — fixed-capacity WaitRequest array
├── front    — dequeue pointer
└── rear     — enqueue pointer  (circular wraparound)
```

| Method | Description | Complexity |
|---|---|---|
| `enQueue(request)` | Add customer to waitlist | O(1) |
| `deQueue(outValue)` | Remove front of waitlist | O(1) |
| `peek(outValue)` | Inspect front without removing | O(1) |
| `isFull() / isEmpty()` | Capacity checks | O(1) |

---

## 🌐 REST API Endpoints

| Method | Endpoint | Description |
|---|---|---|
| GET | `/movies` | List all movies |
| GET | `/shows?movieId=<id>` | List shows for a movie (sorted by datetime) |
| GET | `/shows/:id/seats` | Get seat map for a show |
| GET | `/book/recommend?showId=&seats=` | Get recommended seat suggestions |
| POST | `/book/single` | Book a single specific seat |
| POST | `/book/group` | Book a group (auto seat allocation) |
| GET | `/booking/:id` | View a booking |
| DELETE | `/booking/:id` | Cancel a booking |
| POST | `/waitlist` | Join the waitlist for a show |

---

## 🛠️ Build & Run

**Requirements:** C++17 compiler (g++ / clang++), cpp-httplib (bundled in `include/httplib.h`)

```bash
# Compile the REST API server
g++ -std=c++14 -Wall -Wextra -pedantic src/server.cpp src/BookingSystem.cpp src/SeatingGrid.cpp src/BSTMovieTitle.cpp src/MergeSortShows.cpp src/QueueArrayWait.cpp src/HashTableIntToIndex.cpp src/SinglyLinkedList.cpp src/Store.cpp src/SampleData.cpp -I include -lws2_32 -o server.exe


# Run the server (default port 8080)
./server.exe

# Open the frontend
open frontend/index.html

# Compile the console app
g++ -std=c++17 -Wall -Wextra -pedantic src/main.cpp src/BookingSystem.cpp src/SeatingGrid.cpp src/BSTMovieTitle.cpp src/MergeSortShows.cpp src/QueueArrayWait.cpp src/HashTableIntToIndex.cpp src/SinglyLinkedList.cpp src/Store.cpp src/SampleData.cpp src/ConsoleIO.cpp src/Menu.cpp -I include -o app.exe


# Run the console app
./app.exe
```

---

## 🗂️ Data Structure Summary

| Structure | File | Used For | Implemented By |
|---|---|---|---|
| 2D Array Grid | `SeatingGrid` | Per-show seat layout | Amarasie |
| Hash Table | `HashTableIntToIndex` | ID → index lookup | Amarasie |
| Singly Linked List | `SinglyLinkedListInt` | Per-show booking list | Amarasie |
| Binary Search Tree | `BSTMovieTitle` | Movie title search | Dasuni |
| Merge Sort | `MergeSortShows` | Chronological show listing | Dasuni |
| Circular Queue | `QueueArrayWait` | Per-show waitlist | Danith |

