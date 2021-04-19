/**
 * course.hpp
 * Charles Haines - 04/2021
 * Southern New Hampshire University
 * 
 * Purpose: Course object class declaration. Course objects
 *          represent a single course including name, number, and
 *          prerequisites.
 */

#ifndef COURSE_HPP
#define COURSE_HPP

#include <string>
#include <vector>

/**
 * Course Object to hold details about an individual course
 */

class Course {
    public:
    Course(std::string name, std::string number, std::vector<std::string> prerequisites);
    std::string getCourseName();
    std::string getCourseNumber();
    std::vector<std::string> getPrerequisites();

    private:
    std::string name;
    std::string number;
    std::vector<std::string> prerequisites;
    

};


#endif