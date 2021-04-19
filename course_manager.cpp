/**
 * course_manager.cpp
 * Charles Haines - 04/2021
 * Southern New Hampshire University
 * 
 * Purpose: Course manager function definitions.
 */

#include "course_manager.hpp"
#include <fstream>
#include <iostream>

// Destructor - Destroys all course objects and clears the courses vector.
CourseManager::~CourseManager() {
    if (courses.size() > 0) {
        for (Course* currentCourse : courses) {
            delete currentCourse;
        }
    }    

    courses.clear();
}

/** Function loadCoursesFromFile
 * @param1 : std::string - filename - name of input file
 * Purpose: Parse courses file to instantiate course objects
 */
void CourseManager::loadCoursesFromFile(std::string filename){
    // Declare local variables
    std::string currentLine;
    std::ifstream inputFile;
    std::string name, number;
    std::vector<std::string> prerequisites;
    Course* currentCourse = nullptr;
    
    std::vector<int> delimLocations;        // Location of delimiter characters in a line
    int delimCount;                         // Number of delimiter characters in a line
    int startLocation;                      // Start location of parameter in file
    int length;                             // Length of paramter in file
    int count = 0;                              // Debugging purposes. Count number of courses added from file.

    // Check input file for errors
    if (!validateInputFile(filename)) {
        std::cout << "Error reading input file. Check file format and try again." << std::endl;
        return;
    }

    // Attempt to open file
    inputFile.open(filename);

    // If unable to open file, display error message and abort loading courses.
    if(!inputFile.is_open()) {
        std::cout << "Error opening " << filename << "." << std::endl;
        return;
    }

    // Loop through each line in the file
    while (getline(inputFile, currentLine)) {

        // Loop thorough the line and store the index of the delimiters (commas).
        for (auto i = 0; i < currentLine.size(); ++i) {
            if (currentLine.at(i) == ',') {
                delimLocations.push_back(i);
            }
        }

        delimCount = delimLocations.size();

        startLocation = 0; // Start at beginning of line 
        length = delimLocations.at(0) - startLocation; // Use location of next
                                                       // delimiter to determine
                                                       // length of parameter.
        // Store name of course
        number = currentLine.substr(startLocation, length); 

        // Store course number
        startLocation = delimLocations[0] + 1;
        // If at least three parameters, use next delimiter to determine length...
        if(delimCount > 1) {
                length = delimLocations[1] - startLocation;
        }
        else { // Use use length of line since there are not additional parameters
            length = currentLine.size() - startLocation;
        }
        
        name = currentLine.substr(startLocation, length);

        // Process any additional parameters if present (prerequisites)
        if (delimCount >= 2) {
            // Loop through each addional parameter in the line
            for (auto i = 1; i < delimCount; ++i) {
                // Set start location of next parameter
                startLocation = delimLocations.at(i) + 1;

                // If this is the last parameter, set length using size of 
                // current line
                if (i == delimCount - 1) {
                    length = currentLine.size() - startLocation;
                }
                // Else use next delimeter to determine length
                else {
                    length = delimLocations.at(i + 1) - startLocation;
                }

                // Add prerequiste to prerequisites vector
                prerequisites.push_back(currentLine.substr(startLocation, length));
            }
        }

        // Finally, instantiate a new course object and add it to the courses vector
        currentCourse = new Course(name, number, prerequisites);
        courses.push_back(currentCourse);
        count++; // Increase count of courses added to vector

        // Clear delimiter and prerequisite variables before next iteration
        delimLocations.clear();
        prerequisites.clear();
        delimCount = 0;
        
    }

    // Display number of courses added to the vector
    std::cout << "Added " << count << " courses from file." << std::endl << std::endl;
    
    // Sort courses by course number ascending
    sortCourses();


}

/**
 * Function validateInputFiles()
 * @param1 std::string - filename
 * Purpose: Ensure courses file is valid before attempting to instantiate objects
 */
bool CourseManager::validateInputFile(std::string filename) {
    std::vector<std::string> validCourses;    // Store valid course names to compare with prereq's
    std::fstream inputFile;                   
    std::string currentLine;

    // Following variables are used to parse parameters from the input file line
    // Will be used in conjuction with std::string.substr(startLocation, length)
    int startLocation;            
    int length;
    int delimCount = 0; // Count number of commas in each line


    inputFile.open(filename);
    
    // If file isn't open, display error and return.
    if (!inputFile.is_open()) {
        std::cout << "Unable to open input file " << filename << "." << std::endl;
        return false;
    }
    
    // Loop through each line in the file
    while( std::getline(inputFile, currentLine)) {
        if (currentLine.size() > 0) {
            // Loop through each character in the line looking for delimiters (commas)
            for (int i = 0; i < currentLine.size(); ++i) {
                if (currentLine.at(i) == ',') {
                    ++delimCount;

                    if (delimCount == 1) {
                        // Store first parameter as a valid course.
                        validCourses.push_back(currentLine.substr(0, i));
                    }
                }
            }
            if (delimCount <= 0) { // If line has no commas, line is not valid.
                std::cout << "Error: Invalid file format" << std::endl;
                return false;
            }
            else { // If execution reaches here, reset delimCount to zero before
                   // the next iteration.
                delimCount = 0;
            }
        }
        
    }

    // If execution reaches here, each line in the input file has at least two parameters
    // (course number and name)
    // Next we'll check each course that has prerequisites and verify that the
    // prerequisites are valid courses.
    
    // Close and reopen input file to start back at line 1 again
    inputFile.close();
    inputFile.open(filename);

    if (!inputFile.is_open()) {
        std::cout << "Error reading opening input file " << filename << "." << std::endl;
        return false;
    }

    // Loop through each line in the file again
    while (std::getline(inputFile, currentLine)) {
        delimCount = 0;       // Number of commas
        std::vector<int> delimIndexes; // Indicies of commas for current line
        std::string currentCourse;     // Current prerequisite being checked
        bool validPrereq = false;      // Will be set to true if current prerequisite
                                       // matches a valid course number in the file.


        // Count delimiters for the line again and store the indicies
        for (int i = 0; i < currentLine.size(); ++i) {
            if (currentLine.at(i) == ',') {
                ++delimCount;
                delimIndexes.push_back(i);
            }
        }

        // If the course has prerequisites...
        if (delimCount >= 2) {
            // Loop through each prerequisite and make sure it's a valid course
            for (int i = 1; i < delimIndexes.size(); ++i) {
                startLocation = delimIndexes.at(i) + 1;
                if ( i == delimIndexes.size() - 1) {
                    // If this is the last parameter, 
                    // use the end of the line to determine the 
                    // length of the parameter
                    length = currentLine.size() - startLocation;
                }
                else {
                    // Else use the next delimiter to find the length
                    length = delimIndexes.at(i + 1) - startLocation;
                }
                
                // Separate current course from other parameters on the line
                currentCourse = currentLine.substr(startLocation, length);

                // Finally, loop thorugh validCourses and try to match currentCourse.
                // If a match isn't found, the prerequisite is invalid and validation
                // returns unsuccessful.
                for (std::string validCourse : validCourses) {
                    if (validCourse.compare(currentCourse) == 0) {
                        validPrereq = true;
                        break;
                    }
                }

                if (!validPrereq) {
                    std::cout << "Unable to recognize prerequisite " << currentCourse;
                    std::cout << "as a valid course." << std::endl;
                    return false;
                }
                
            }
        }
    }
    // If execution reaches here, all tests have passed. Input file is valid!
    return true;

}

/**
 * Function printAllCourses()
 * Purpose: Prints information on all courses in the vector
 */
void CourseManager::printAllCourses() {
    std::cout << "Here's a sample schedule:" << std::endl << std::endl;
    for (Course* currentCourse : courses) {
        std::cout << currentCourse->getCourseNumber() << ", ";
        std::cout << currentCourse->getCourseName() << std::endl;
    }
    std::cout << std::endl;
}

/**
 * Function sortCourses()
 * Purpose: Sort courses in ascending order by course number
 */
void CourseManager::sortCourses() {
    int i;
    int j = 0;
    Course* temp = nullptr;
    for (i = 1; i < courses.size(); ++i) {
        j = i;
        while (j > 0 && courses.at(j)->getCourseNumber() < courses.at(j - 1)->getCourseNumber()) {
            // Swap i and j
            temp = courses.at(j);
            courses.at(j) = courses.at(j-1);
            courses.at(j-1) = temp;
            temp = nullptr;
            j--;
        }
    }
}

/**
 * Function findCourse()
 * @param1 std::string number - course number
 * Purpose: find a course object given it's course number
 */
Course* CourseManager::findCourse(std::string number) {
    Course* course = nullptr;

    // Convert string to all uppercase for comparision
    for (int i = 0; i < number.length(); ++i) {
        number.at(i) = std::toupper(number.at(i));
    }

    for (Course* currentCourse : courses) {
        if (currentCourse->getCourseNumber().compare(number) == 0) {
            // If number matches, course found, return this course object
            course = currentCourse;
            break;
        }

    }
    return course;
}

/**
 * Function getNumberCourses()
 * Purpose: Returns the current size of the courses vector
 */
int CourseManager::getNumberCourses() {
    return static_cast<int>(courses.size());
}

/**
 * Function clearCourses()
 * Purpose: Removes all course objects from the courses vector
 */
void CourseManager::clearCourses() {
    if (courses.size() >= 1) {
        for (Course* currentCourse : courses) {
            delete currentCourse;
        }
    }
    courses.clear();
}