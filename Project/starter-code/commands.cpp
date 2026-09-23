#include "commands.h"
#include "p1_helper.h"
#include <iostream>
#include <sstream>
#include <algorithm>

std::string process_client_command(const std::string& raw_command, std::string& current_user) {
    std::stringstream ss(raw_command);
    std::string command_type;
    ss >> command_type;

    std::transform(command_type.begin(), command_type.end(), command_type.begin(), ::toupper);

    if (command_type == "IAM") {
        std::string username;
        ss >> username;
        if (!username.empty()) {
            current_user = username; // Store active session user
            std::cout << "[Server] User logged in as: " << current_user << std::endl;
            return "210 Welcome " + current_user + "\n";
        } else {
            return "400 Error: Missing username\n";
        }
    } 
    else if (command_type == "CATALOG") {
        std::cout << "[Server] Querying database catalog..." << std::endl;
        
        // Call your helper function to read courses from courses.db
        std::string catalog_data = load_courses_from_db("courses.db"); 
        
        return "200 CATALOG: " + catalog_data + "\n";
    } 
    else if (command_type == "ENROLLMENT") {
        if (current_user.empty()) {
            return "401 Error: Must send IAM <username> first\n";
        }

        std::string course;
        ss >> course;
        if (course.empty()) {
            return "400 Error: Missing course code\n";
        }

        std::cout << "[Server] Attempting enrollment for user " << current_user << " in " << course << std::endl;

        // Call your helper function to write to courses.db / enrollment tables
        bool success = enroll_in_db("courses.db", current_user, course);

        if (success) {
            return "200 Enrolled " + current_user + " successfully in " + course + "\n";
        } else {
            return "402 Error: Enrollment failed (course full or invalid course/user)\n";
        }
    } 
    else if (command_type == "BYE") {
        std::cout << "[Server] Client disconnected." << std::endl;
        return "200 Goodbye\n";
    } 
    
    return "400 Unknown command\n";
}