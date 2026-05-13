#ifdef _WIN32
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif
#ifndef WINVER
#define WINVER 0x0A00
#endif
#endif

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "httplib.h"

#define private public
#include "BookingSystem.h"
#undef private

namespace
{

    bool parseJsonStringField(const std::string &body, const std::string &field, std::string &value)
    {
        std::string key = "\"" + field + "\"";
        size_t keyPos = body.find(key);
        if (keyPos == std::string::npos)
        {
            return false;
        }

        size_t colonPos = body.find(':', keyPos + key.size());
        if (colonPos == std::string::npos)
        {
            return false;
        }

        size_t quoteStart = body.find('"', colonPos + 1);
        if (quoteStart == std::string::npos)
        {
            return false;
        }

        std::string parsed;
        bool escaping = false;
        for (size_t i = quoteStart + 1; i < body.size(); ++i)
        {
            char ch = body[i];
            if (escaping)
            {
                switch (ch)
                {
                case '"':
                    parsed += '"';
                    break;
                case '\\':
                    parsed += '\\';
                    break;
                case 'n':
                    parsed += '\n';
                    break;
                case 'r':
                    parsed += '\r';
                    break;
                case 't':
                    parsed += '\t';
                    break;
                case 'b':
                    parsed += '\b';
                    break;
                case 'f':
                    parsed += '\f';
                    break;
                default:
                    parsed += ch;
                    break;
                }
                escaping = false;
                continue;
            }

            if (ch == '\\')
            {
                escaping = true;
                continue;
            }

            if (ch == '"')
            {
                value = parsed;
                return true;
            }

            parsed += ch;
        }

        return false;
    }

    bool parseJsonIntField(const std::string &body, const std::string &field, int &value)
    {
        std::string key = "\"" + field + "\"";
        size_t keyPos = body.find(key);
        if (keyPos == std::string::npos)
        {
            return false;
        }

        size_t colonPos = body.find(':', keyPos + key.size());
        if (colonPos == std::string::npos)
        {
            return false;
        }

        size_t valueStart = body.find_first_not_of(" \t\r\n", colonPos + 1);
        if (valueStart == std::string::npos)
        {
            return false;
        }

        size_t valueEnd = valueStart;
        if (body[valueEnd] == '-')
        {
            ++valueEnd;
        }
        while (valueEnd < body.size() && body[valueEnd] >= '0' && body[valueEnd] <= '9')
        {
            ++valueEnd;
        }

        if (valueEnd == valueStart || (body[valueStart] == '-' && valueEnd == valueStart + 1))
        {
            return false;
        }

        try
        {
            value = std::stoi(body.substr(valueStart, valueEnd - valueStart));
            return true;
        }
        catch (...)
        {
            return false;
        }
    }

    std::string escapeJson(const std::string &value)
    {
        std::string escaped;
        escaped.reserve(value.size());

        for (size_t i = 0; i < value.size(); ++i)
        {
            char ch = value[i];
            switch (ch)
            {
            case '\"':
                escaped += "\\\"";
                break;
            case '\\':
                escaped += "\\\\";
                break;
            case '\b':
                escaped += "\\b";
                break;
            case '\f':
                escaped += "\\f";
                break;
            case '\n':
                escaped += "\\n";
                break;
            case '\r':
                escaped += "\\r";
                break;
            case '\t':
                escaped += "\\t";
                break;
            default:
                escaped += ch;
                break;
            }
        }

        return escaped;
    }

    std::string buildErrorJson(const std::string &message)
    {
        return std::string("{\"error\":\"") + escapeJson(message) + "\"}";
    }

    int findShowIndexById(const BookingSystem &system, int showId)
    {
        for (int i = 0; i < system.store.showCount; ++i)
        {
            if (system.store.shows[i].id == showId)
            {
                return i;
            }
        }
        return -1;
    }

    std::string buildRecommendedSeatsJson(const Seat seats[], int count)
    {
        std::string json = "[";

        for (int i = 0; i < count; ++i)
        {
            if (i > 0)
            {
                json += ",";
            }

            json += "{\"row\":";
            json += std::to_string(static_cast<int>(seats[i].r) + 1);
            json += ",\"col\":";
            json += std::to_string(static_cast<int>(seats[i].c) + 1);
            json += "}";
        }

        json += "]";
        return json;
    }

    std::string buildBookingJson(const BookingSystem &system, int bookingId)
    {
        int bookingIndex = -1;
        if (!system.bookingIdIndex.get(bookingId, bookingIndex))
        {
            return "";
        }

        const Booking &booking = system.store.bookings[bookingIndex];
        int showIndex = findShowIndexById(system, booking.showId);
        int movieIndex = -1;
        int totalPrice = 0;
        std::string datetime;
        std::string movieTitle;

        if (showIndex != -1)
        {
            const Show &show = system.store.shows[showIndex];
            datetime = show.datetime;
            totalPrice = show.price * booking.seatCount;
            movieIndex = findMovieIndexById(system.store, show.movieId);
            if (movieIndex != -1)
            {
                movieTitle = system.store.movies[movieIndex].title;
            }
        }

        std::string json = "{";
        json += "\"id\":";
        json += std::to_string(booking.id);
        json += ",\"showId\":";
        json += std::to_string(booking.showId);
        json += ",\"customerName\":\"";
        json += escapeJson(booking.customerName);
        json += "\",\"seatCount\":";
        json += std::to_string(booking.seatCount);
        json += ",\"status\":\"";
        json += (booking.status == STATUS_CONFIRMED ? "CONFIRMED" : "CANCELLED");
        json += "\",\"movieTitle\":\"";
        json += escapeJson(movieTitle);
        json += "\",\"datetime\":\"";
        json += escapeJson(datetime);
        json += "\",\"totalPrice\":";
        json += std::to_string(totalPrice);
        json += ",\"seats\":[";

        for (int i = 0; i < booking.seatCount; ++i)
        {
            if (i > 0)
            {
                json += ",";
            }

            json += "{\"row\":";
            json += std::to_string(static_cast<int>(booking.seatR[i]) + 1);
            json += ",\"col\":";
            json += std::to_string(static_cast<int>(booking.seatC[i]) + 1);
            json += "}";
        }

        json += "]}";
        return json;
    }

    std::string buildMoviesJson(const BookingSystem &system)
    {
        int movieIds[MAX_MOVIES];
        int movieCount = system.getMovieIds(movieIds);
        std::string json = "[";

        for (int i = 0; i < movieCount; ++i)
        {
            int movieIndex = findMovieIndexById(system.store, movieIds[i]);
            if (movieIndex == -1)
            {
                continue;
            }

            if (json.size() > 1)
            {
                json += ",";
            }

            const Movie &movie = system.store.movies[movieIndex];
            json += "{\"id\":";
            json += std::to_string(movie.id);
            json += ",\"title\":\"";
            json += escapeJson(movie.title);
            json += "\",\"duration\":";
            json += std::to_string(movie.durationMin);
            json += "}";
        }

        json += "]";
        return json;
    }

    std::string buildShowsJson(const BookingSystem &system, int movieId)
    {
        int showIds[MAX_SHOWS];
        int showCount = system.listShowsForMovieSorted(movieId, showIds);
        std::string json = "[";

        for (int i = 0; i < showCount; ++i)
        {
            int showId = showIds[i];
            int showIndex = findShowIndexById(system, showId);
            if (showIndex == -1)
            {
                continue;
            }

            if (json.size() > 1)
            {
                json += ",";
            }

            const Show &show = system.store.shows[showIndex];
            json += "{\"id\":";
            json += std::to_string(show.id);
            json += ",\"datetime\":\"";
            json += escapeJson(show.datetime);
            json += "\",\"price\":";
            json += std::to_string(show.price);
            json += ",\"availableSeats\":";
            json += std::to_string(system.getAvailableCount(show.id));
            json += ",\"waitlistCount\":";
            json += std::to_string(show.waitCount);
            json += "}";
        }

        json += "]";
        return json;
    }

    std::string buildShowStatusJson(const BookingSystem &system, int showId)
    {
        int showIndex = findShowIndexById(system, showId);
        if (showIndex == -1)
        {
            return "";
        }

        const Show &show = system.store.shows[showIndex];
        std::string json = "{";
        json += "\"id\":";
        json += std::to_string(show.id);
        json += ",\"availableSeats\":";
        json += std::to_string(system.getAvailableCount(show.id));
        json += ",\"waitlistCount\":";
        json += std::to_string(show.waitCount);
        json += "}";
        return json;
    }

    std::string buildSeatsJson(const BookingSystem &system, int showId)
    {
        int showIndex = findShowIndexById(system, showId);
        if (showIndex == -1)
        {
            return "";
        }

        const SeatingGrid &seatingGrid = system.showGrids[showIndex];

        // Safety: if seating grid wasn't initialized, return empty string
        if (seatingGrid.rows == 0 || seatingGrid.cols == 0 || seatingGrid.grid == nullptr)
        {
            return "";
        }
        // Extra safety: guard against corrupted dimensions (prevents runaway loops)
        if (seatingGrid.rows < 0 || seatingGrid.cols < 0 || seatingGrid.rows > ROWS || seatingGrid.cols > COLS)
        {
            return "";
        }

        std::string json = "{";
        json += "\"rows\":";
        json += std::to_string(seatingGrid.rows);
        json += ",\"cols\":";
        json += std::to_string(seatingGrid.cols);
        json += ",\"grid\":[";

        for (int r = 0; r < seatingGrid.rows; ++r)
        {
            if (r > 0)
            {
                json += ",";
            }

            json += "[";
            for (int c = 0; c < seatingGrid.cols; ++c)
            {
                if (c > 0)
                {
                    json += ",";
                }

                try
                {
                    if (seatingGrid.grid[r] == nullptr)
                    {
                        return "";
                    }
                    int cell = seatingGrid.grid[r][c];
                    json += (cell == SeatingGrid::AVAILABLE) ? "0" : "1";
                }
                catch (...)
                {
                    return "";
                }
            }
            json += "]";
        }

        json += "],";
        json += "\"availableSeats\":" + std::to_string(system.getAvailableCount(showId)) + ",";
        json += "\"waitlistCount\":" + std::to_string(system.store.shows[showIndex].waitCount);
        json += "}";
        return json;
    }

    bool loadTextFile(const std::string &path, std::string &content)
    {
        std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
        if (!file)
        {
            return false;
        }

        std::ostringstream buffer;
        buffer << file.rdbuf();
        content = buffer.str();
        return true;
    }

} // namespace

int main()
{
    BookingSystem system;
    httplib::Server server;

    server.set_default_headers({{"Access-Control-Allow-Origin", "*"},
                                {"Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS"},
                                {"Access-Control-Allow-Headers", "Content-Type"},
                                {"Access-Control-Max-Age", "86400"}});

    server.Options(R"(.*)", [](const httplib::Request &, httplib::Response &res)
                   { res.status = 204; });

    server.Get("/", [](const httplib::Request &, httplib::Response &res)
               {
        std::string content;
        if (!loadTextFile("frontend/index.html", content)) {
            res.status = 404;
            res.set_content(buildErrorJson("frontend/index.html not found"), "application/json");
            return;
        }

        res.set_content(content, "text/html; charset=UTF-8"); });

    server.Get("/ping", [](const httplib::Request &, httplib::Response &res)
               { res.set_content(R"({"status":"ok"})", "application/json"); });

    server.Get("/movies", [&system](const httplib::Request &, httplib::Response &res)
               { res.set_content(buildMoviesJson(system), "application/json"); });

    server.Get("/shows", [&system](const httplib::Request &req, httplib::Response &res)
               {
        if (!req.has_param("movieId")) {
            res.status = 400;
            res.set_content(R"({"error":"movieId is required"})", "application/json");
            return;
        }

        int movieId = 0;
        try {
            movieId = std::stoi(req.get_param_value("movieId"));
        } catch (...) {
            res.status = 400;
            res.set_content(R"({"error":"movieId must be an integer"})", "application/json");
            return;
        }

        if (!system.hasMovie(movieId)) {
            res.status = 404;
            res.set_content(R"({"error":"movie not found"})", "application/json");
            return;
        }

        res.set_content(buildShowsJson(system, movieId), "application/json"); });

    server.Get(R"(/shows/(\d+)/seats)", [&system](const httplib::Request &req, httplib::Response &res)
               {
        // Ensure CORS header present for this route specifically
        res.set_header("Access-Control-Allow-Origin", "*");

        int showId = 0;
        try {
            showId = std::stoi(req.matches[1].str());
        } catch (...) {
            res.status = 400;
            res.set_content(buildErrorJson("invalid show id"), "application/json");
            return;
        }

        // Extra safety: verify the show index and that it matches the requested id
        int showIndex = findShowIndexById(system, showId);
        if (showIndex == -1 || system.store.shows[showIndex].id != showId) {
            // Return a well-formed empty grid JSON so frontend doesn't hang
            res.status = 200;
            res.set_content(R"({"rows":10,"cols":12,"grid":[]})", "application/json");
            return;
        }

        std::string json = buildSeatsJson(system, showId);
        if (json.empty()) {
            // Return empty grid JSON (200) so frontend can handle gracefully
            res.status = 200;
            res.set_content(R"({"rows":10,"cols":12,"grid":[]})", "application/json");
            return;
        }

        res.set_content(json, "application/json"); });

    server.Post("/book/single", [&system](const httplib::Request &req, httplib::Response &res)
                {
        int showId = 0;
        int row = 0;
        int col = 0;
        std::string name;

        if (!parseJsonIntField(req.body, "showId", showId) ||
            !parseJsonStringField(req.body, "name", name) ||
            !parseJsonIntField(req.body, "row", row) ||
            !parseJsonIntField(req.body, "col", col)) {
            res.status = 400;
            res.set_content(buildErrorJson("invalid request body"), "application/json");
            return;
        }

        int bookingId = system.bookSingle(showId, name, row, col);
        if (bookingId == 0) {
            res.status = 400;
            res.set_content(buildErrorJson("booking failed"), "application/json");
            return;
        }

        std::string showStatus = buildShowStatusJson(system, showId);
        std::string json = "{\"bookingId\":" + std::to_string(bookingId) + ",\"showStatus\":" + showStatus + "}";
        res.set_content(json, "application/json"); });

    server.Post("/book/group", [&system](const httplib::Request &req, httplib::Response &res)
                {
        int showId = 0;
        std::string name;
        std::string body = req.body;

        if (!parseJsonIntField(body, "showId", showId) ||
            !parseJsonStringField(body, "name", name)) {
            res.status = 400;
            res.set_content(buildErrorJson("invalid request body"), "application/json");
            return;
        }

        // Parse seats array from JSON body
        Seat seats[MAX_SEATS_IN_BOOKING];
        int seatCount = 0;
        
        size_t seatsPos = body.find("\"seats\":");
        if (seatsPos == std::string::npos) {
            res.status = 400;
            res.set_content(buildErrorJson("seats array required"), "application/json");
            return;
        }

        size_t arrayStart = body.find('[', seatsPos);
        size_t arrayEnd = body.find(']', arrayStart);
        if (arrayStart == std::string::npos || arrayEnd == std::string::npos) {
            res.status = 400;
            res.set_content(buildErrorJson("invalid seats array"), "application/json");
            return;
        }

        // Parse each seat object in the array
        std::string arrayContent = body.substr(arrayStart + 1, arrayEnd - arrayStart - 1);
        size_t searchPos = 0;
        
        while (seatCount < MAX_SEATS_IN_BOOKING) {
            size_t objStart = arrayContent.find('{', searchPos);
            if (objStart == std::string::npos) break;
            
            size_t objEnd = arrayContent.find('}', objStart);
            if (objEnd == std::string::npos) break;
            
            std::string objStr = arrayContent.substr(objStart, objEnd - objStart + 1);
            
            int row = 0, col = 0;
            if (parseJsonIntField(objStr, "row", row) && 
                parseJsonIntField(objStr, "col", col)) {
                seats[seatCount].r = row - 1;
                seats[seatCount].c = col - 1;
                seatCount++;
                searchPos = objEnd + 1;
            } else {
                break;
            }
        }

        if (seatCount == 0) {
            res.status = 400;
            res.set_content(buildErrorJson("no valid seats in array"), "application/json");
            return;
        }

        int bookingId = system.bookGroupSeats(showId, name, seats, seatCount);
        if (bookingId == 0) {
            res.status = 400;
            res.set_content(buildErrorJson("group booking failed"), "application/json");
            return;
        }

        std::string showStatus = buildShowStatusJson(system, showId);
        std::string json = "{\"bookingId\":" + std::to_string(bookingId) + ",\"showStatus\":" + showStatus + "}";
        res.set_content(json, "application/json"); });

    server.Get("/book/recommend", [&system](const httplib::Request &req, httplib::Response &res)
               {
        if (!req.has_param("showId") || !req.has_param("count")) {
            res.status = 400;
            res.set_content(buildErrorJson("showId and count are required"), "application/json");
            return;
        }

        int showId = 0;
        int count = 0;
        try {
            showId = std::stoi(req.get_param_value("showId"));
            count = std::stoi(req.get_param_value("count"));
        } catch (...) {
            res.status = 400;
            res.set_content(buildErrorJson("showId and count must be integers"), "application/json");
            return;
        }

        if (count < 1 || count > MAX_SEATS_IN_BOOKING) {
            res.status = 400;
            res.set_content(buildErrorJson("count is out of range"), "application/json");
            return;
        }

        Seat seats[MAX_SEATS_IN_BOOKING];
        if (!system.recommendSeats(showId, count, seats)) {
            res.status = 404;
            res.set_content("{\"seats\":[]}", "application/json");
            return;
        }

        std::string seatsJson = buildRecommendedSeatsJson(seats, count);
        std::string json = "{\"seats\":" + seatsJson + "}";
        res.set_content(json, "application/json"); });

    server.Get(R"(/booking/(\d+))", [&system](const httplib::Request &req, httplib::Response &res)
               {
        int bookingId = 0;
        try {
            bookingId = std::stoi(req.matches[1].str());
        } catch (...) {
            res.status = 400;
            res.set_content(buildErrorJson("invalid booking id"), "application/json");
            return;
        }

        std::string json = buildBookingJson(system, bookingId);
        if (json.empty()) {
            res.status = 404;
            res.set_content(buildErrorJson("booking not found"), "application/json");
            return;
        }

        res.set_content(json, "application/json"); });

    server.Delete(R"(/booking/(\d+))", [&system](const httplib::Request &req, httplib::Response &res)
                  {
        int bookingId = 0;
        std::string name;

        try {
            bookingId = std::stoi(req.matches[1].str());
        } catch (...) {
            res.status = 400;
            res.set_content(buildErrorJson("invalid booking id"), "application/json");
            return;
        }

        if (!parseJsonStringField(req.body, "name", name)) {
            res.status = 400;
            res.set_content(buildErrorJson("invalid request body"), "application/json");
            return;
        }

        // Get showId before cancellation
        int bookingIndex = -1;
        if (!system.bookingIdIndex.get(bookingId, bookingIndex)) {
            res.status = 404;
            res.set_content(buildErrorJson("booking not found"), "application/json");
            return;
        }
        int showId = system.store.bookings[bookingIndex].showId;

        bool success = system.cancelBooking(bookingId, name) != 0;
        std::string showStatus = buildShowStatusJson(system, showId);
        std::string json = "{\"success\":" + std::string(success ? "true" : "false") + ",\"showStatus\":" + showStatus + "}";
        res.set_content(json, "application/json"); });

    server.Post("/waitlist", [&system](const httplib::Request &req, httplib::Response &res)
                {
        int showId = 0;
        int seatsNeeded = 0;
        std::string name;

        if (!parseJsonIntField(req.body, "showId", showId) ||
            !parseJsonStringField(req.body, "name", name) ||
            !parseJsonIntField(req.body, "seatsNeeded", seatsNeeded)) {
            res.status = 400;
            res.set_content(buildErrorJson("invalid request body"), "application/json");
            return;
        }

        bool success = system.joinWaitlist(showId, name, seatsNeeded) != 0;
        std::string showStatus = buildShowStatusJson(system, showId);
        std::string json = "{\"success\":" + std::string(success ? "true" : "false") + ",\"showStatus\":" + showStatus + "}";
        res.set_content(json, "application/json"); });

    server.set_mount_point("/", "./frontend");

    std::cout << "NoirCinema HTTP server listening on http://localhost:8080" << std::endl;

    if (!server.listen("0.0.0.0", 8080))
    {
        std::cerr << "Failed to start NoirCinema HTTP server on port 8080" << std::endl;
        return 1;
    }

    return 0;
}
