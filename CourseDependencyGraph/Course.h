#ifndef COURSE_H
#define COURSE_H

#include <vector>

#include "CourseCode.h"
#include "Semester.h"

class Course {
private:
    CourseCode courseCode;

    QString title;
    int credit;
    QString description;
    QString attribute;

    QString exclusionDescription;
    QString prerequisiteDescription;
    QString corequisiteDescription;
    QString colistDescription;

    std::vector<Course*> exclusion;
    std::vector<Course*> prerequisite;
    std::vector<Course*> corequisite;
    std::vector<Course*> colist;

    QString vector;
    CourseCode previousCode;
    std::vector<Semester> offerIn;


public:
    Course();

    // getter
    const CourseCode& getCourseCode();
    const QString& getTitle();
    const int& getCredit();
    const QString& getDescription();
    const QString& getAttribute();
    const QString& getExclusionDescription();
    const QString& getPrerequisiteDescription();
    const QString& getCorequisiteDescription();
    const QString& getColistDescription();
    const std::vector<Course*>& getExclusion();
    const std::vector<Course*>& getPrerequisite();
    const std::vector<Course*>& getCorequisite();
    const std::vector<Course*>& getColist();
    const QString& getVector();
    const CourseCode& getPreviousCode();
    const std::vector<Semester>& getOfferIn();

};



#endif // COURSE_H
