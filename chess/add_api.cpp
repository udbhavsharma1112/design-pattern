#include <iostream>
#include "httplib.h" // Include cpp-httplib header
#include "json.hpp" // Include nlohmann/json for JSON parsing

using json = nlohmann::json;

int main() {
    httplib::Server svr;

    // Define the API endpoint
    svr.Post("/add", [](const httplib::Request &req, httplib::Response &res) {
        try {
            // Parse the JSON body
            auto body_json = json::parse(req.body);

            // Extract the numbers from the body
            double num1 = body_json["num1"].get<double>();
            double num2 = body_json["num2"].get<double>();
            double num3 = body_json["num3"].get<double>();

            // Calculate the sum
            double sum = num1 + num2 + num3;

            // Create the response JSON
            json response_json;
            response_json["sum"] = sum;

            // Send the response
            res.set_content(response_json.dump(), "application/json");
        } catch (const std::exception &e) {
            // Handle errors
            res.status = 400;
            json error_json;
            error_json["error"] = "Invalid request: " + std::string(e.what());
            res.set_content(error_json.dump(), "application/json");
        }
    });

    std::cout << "Server is running at http://localhost:8080\n";

    // Start the server
    svr.listen("localhost", 8080);

    return 0;
}
