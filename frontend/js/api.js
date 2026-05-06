const API_BASE_URL = (window.location.origin && window.location.origin !== "null") 
    ? window.location.origin 
    : "http://localhost:8080";

async function parseJsonResponse(response) {
    const data = await response.json();

    if (!response.ok) {
        throw new Error(data.error || "Request failed.");
    }

    return data;
}

async function getMovies() {
    const response = await fetch(API_BASE_URL + "/movies");
    return parseJsonResponse(response);
}

async function searchMovie(title) {
    const movies = await getMovies();
    const normalizedTitle = String(title || "").trim().toLowerCase();

    if (!normalizedTitle) {
        return movies;
    }

    return movies.filter(function(movie) {
        return String(movie.title || "").toLowerCase().includes(normalizedTitle);
    });
}

async function getShows(movieId) {
    const url = new URL(API_BASE_URL + "/shows");
    url.searchParams.set("movieId", movieId);

    const response = await fetch(url.toString());
    return parseJsonResponse(response);
}

async function getBooking(bookingId) {
    const response = await fetch(API_BASE_URL + "/booking/" + encodeURIComponent(bookingId));
    return parseJsonResponse(response);
}

async function cancelBooking(bookingId, name) {
    const response = await fetch(API_BASE_URL + "/booking/" + encodeURIComponent(bookingId), {
        method: "DELETE",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({ name: name })
    });

    return parseJsonResponse(response);
}

async function getSeats(showId) {
    const response = await fetch(API_BASE_URL + "/shows/" + encodeURIComponent(showId) + "/seats");
    return parseJsonResponse(response);
}

async function recommendSeats(showId, count) {
    const url = new URL(API_BASE_URL + "/book/recommend");
    url.searchParams.set("showId", showId);
    url.searchParams.set("count", count);

    const response = await fetch(url.toString());
    return parseJsonResponse(response);
}

async function bookSingle(showId, name, row, col) {
    const response = await fetch(API_BASE_URL + "/book/single", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({
            showId: showId,
            name: name,
            row: row,
            col: col
        })
    });

    return parseJsonResponse(response);
}

async function bookGroup(showId, name, seats) {
    const response = await fetch(API_BASE_URL + "/book/group", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({
            showId: showId,
            name: name,
            seats: seats.map(function(seat) {
                return {
                    row: seat.row,
                    col: seat.col
                };
            })
        })
    });

    return parseJsonResponse(response);
}

async function joinWaitlist(showId, name, seatsNeeded) {
    const response = await fetch(API_BASE_URL + "/waitlist", {
        method: "POST",
        headers: {
            "Content-Type": "application/json"
        },
        body: JSON.stringify({
            showId: showId,
            name: name,
            seatsNeeded: seatsNeeded
        })
    });

    return parseJsonResponse(response);
}
