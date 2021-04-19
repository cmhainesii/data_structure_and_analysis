/**
 * course.cpp
 * Charles Haines - 04/2021
 * Southern New Hampshire University
 * 
 * Purpose: Function definitions for course class.
 *
 */

#include "course.hpp"

/**
 * Constructor
 * @param1 std::string - course name
 * @param2 std::string - course number
 * @param3 std::vector<std::string> - prerequisites
 */
Course::Course(std::string name, std::string number, std::vector<std::string> prerequisites) {
    this->name = name;
    this->number = number;
    this->prerequisites = prerequisites;
}

/**
 * Function getCourseName
 * Purpose: return name of course
 */
std::string Course::getCourseName() {
    return this->name;
}

/**
 * Function getCourseNumber()
 * Purpose: return course number
 */
std::string Course::getCourseNumber() {
    return this->number;
}

/**
 * Function getPrerequisites()
 * Purpose: Returns prerequisites vector of course object
 */
std::vector<std::string> Course::getPrerequisites() {
    return this->prerequisites;
}