/**
 * course_manager.hpp
 * Charles Haines - 04/2021
 * Southern New Hampshire University
 * 
 * Purpose: Course manager class declaration. Course manager is used
 * to store and manage a vector of course objects
 */


#ifndef COURSE_MANAGER_HPP
#define COURSE_MANAGER_HPP

#include <string>
#include <vector>

#include "course.hpp"

class CourseManager {
    public:
        virtual ~CourseManager();
        void loadCoursesFromFile(std::string filename);
        void printAllCourses();
        Course* findCourse(std::string courseNumber);
        int getNumberCourses();
        void clearCourses();

        
    private:
    bool validateInputFile(std::string filename);
    void sortCourses();
    std::vector<Course*> courses;


};

#endif