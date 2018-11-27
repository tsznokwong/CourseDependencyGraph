#ifndef COURSE_H
#define COURSE_H

#include <vector>
#include <iostream>

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
    Relationship notAvailableAfter;
    Relationship availableAfter;

    QString vector;
    QString previousCode;
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
           QString previousCode,
           std::vector<Semester> offerIn);

    // getter
    const CourseCode& getCourseCode() const;
    const QString& getTitle() const;
    const int& getCredit() const;
    const QString& getDescription() const;
    const QString& getAttribute() const;

    const Relationship& getExclusion() const;
    const Relationship& getPrerequisite() const;
    const Relationship& getCorequisite() const;
    const Relationship& getColist() const;
    const Relationship& getNotAvailableAfter() const;
    const Relationship& getAvaialbeAfter() const;

    const QString& getVector() const;
    const QString& getPreviousCode() const;
    const std::vector<Semester>& getOfferIn() const;

    // stream operator
    friend std::ostream& operator<<(std::ostream &os, const Course& course);

    void linkCourses(const AVLTree<QString, AVLTree<CourseCode, Course* >* > &courses);
};


#endif // COURSE_H
