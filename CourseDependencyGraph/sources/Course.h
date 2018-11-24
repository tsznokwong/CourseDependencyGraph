#ifndef COURSE_H
#define COURSE_H

#include <vector>

#include "CourseCode.h"
#include "Semester.h"
#include "Relationship.h"

class Course {
private:
    CourseCode courseCode;

    QString title;
    int credit;
    QString description;
    QString attribute;

    Relationship exclusion;
    Relationship prerequisite;
    Relationship corequisite;
    Relationship colist;

    QString vector;
    CourseCode previousCode;
    std::vector<Semester> offerIn;


public:
    Course(CourseCode courseCode,
           QString title,
           int credit,
           QString description,
           QString attribute,
           QString exclusion,
           QString prerequisite,
           QString corequisite,
           QString colist,
           QString vector,
           CourseCode previousCode,
           std::vector<Semester> offerIn);

    // getter
    const CourseCode& getCourseCode();
    const QString& getTitle();
    const int& getCredit();
    const QString& getDescription();
    const QString& getAttribute();

    const Relationship& getExclusion();
    const Relationship& getPrerequisite();
    const Relationship& getCorequisite();
    const Relationship& getColist();

    const QString& getVector();
    const CourseCode& getPreviousCode();
    const std::vector<Semester>& getOfferIn();

};


#endif // COURSE_H
