/**
 * main.cpp
 * Charles Haines - 04/2021
 * Southern New Hampshire University
 * 
 * Purpose: Main entry point for course planner application
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <fstream>

#include "course.hpp"
#include "course_manager.hpp"


// Forward declare function to print main menu
void printMenu();

// Main function - Display & handle menu input
int main () {
    std::string filename;
    bool quitCommandIssued = false;         // Will be set to true when user issues exit command
    std::string inputBuffer;                // User input storage
    CourseManager manager;                  // Course manager object holds/manages vector of course objects
    

    // Print welcome message
    std::cout << std::setfill('*') << std::setw(40) << "" << std::endl;
    std::cout << std::setfill(' ') << "   Welcome to the Course Planner" << std::endl;
    std::cout << std::setfill('*') << std::setw(40) << "" << std::endl;
    std::cout << std::endl << std::setfill(' ');

    // Continue looping back to the main menu until the quit command is received
    while (!quitCommandIssued) {
        printMenu();
        std::cin >> inputBuffer;

        // Option 1 - Load courses into data structure
        if (inputBuffer.compare("1") == 0) {
            std::cout << "Enter Filename (leave blank for default courses.txt): ";
            std::cin.ignore();
            std::cin.clear();
            std::getline(std::cin, inputBuffer);
            if (inputBuffer.length() > 0) {
                filename = inputBuffer;
            }
            else {
                filename = "courses.txt";
            }

            // In case the user decides to load data twice, clear 
            // the courses vector if it's not empty.
            if (manager.getNumberCourses() > 0) {
                manager.clearCourses();
            }
            manager.loadCoursesFromFile(filename);
        }

        // Option 2 - Print course list
        else if (inputBuffer.compare("2") == 0) {
            manager.printAllCourses();
        }

        // Option 3 - Print Course
        else if (inputBuffer.compare("3") == 0) {
            std::string courseNumber;
            Course* course = nullptr;
            std::cout << "What course do you want to know about? ";
            std::cin >> courseNumber;
            course = manager.findCourse(courseNumber);
            if (course == nullptr) {
                std::cout << "Unable to find course " << courseNumber << ".";
                std::cout << std::endl;
            }
            else {
                // Course found print name and number
                std::cout << course->getCourseNumber() << ", ";
                std::cout << course->getCourseName() << std::endl;

                // Temporarly store current course's prerequisite vector
                std::vector<std::string> prerequisites = course->getPrerequisites();

                if (course->getPrerequisites().size() > 0) {
                    std::cout << "Prerequisites:" << std::endl;
                    Course* currentPrerequisite;

                    for (int i = 0; i < prerequisites.size(); ++i) {
                        currentPrerequisite = manager.findCourse(prerequisites.at(i));
                        // Print info about current prerequisite
                        std::cout << currentPrerequisite->getCourseNumber() << " - ";
                        std::cout << currentPrerequisite->getCourseName() << std::endl;
                    }

                }
            }
            std::cout << std::endl << std::endl;

        }
        else if (inputBuffer.compare("9") == 0) {
            quitCommandIssued = true;
        }

    }
}

// Function to print the main menu options
void printMenu() {

    std::cout << "1. Load Data Structure" << std::endl;
    std::cout << "2. Print Course List" << std::endl;
    std::cout << "3. Print Course" << std::endl;
    std::cout << "9. Exit" << std::endl << std::endl;

    std::cout << "What would you like to do? ";
}