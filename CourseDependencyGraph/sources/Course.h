#ifndef COURSE_H
#define COURSE_H

#include <vector>
#include <iostream>

#include "CourseCode.h"
#include "Semester.h"
#include "Relationship.h"

class Course {
private:
    // Course identifier
    CourseCode courseCode;

    // key informations
    QString title;
    int credit;
    QString description;
    QString attribute;

    // Course relationship members
    Relationship exclusion;
    Relationship prerequisite;
    Relationship corequisite;
    Relationship colist;
    Relationship notAvailableAfter;
    Relationship availableAfter;

    // miscellaneous informations
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
    Course(CourseCode courseCode);

    // getter
    const CourseCode& getCourseCode() const;
    const QString& getTitle() const;
    const int& getCredit() const;
    const QString& getDescription() const;
    const QString& getAttribute() const;

    const Relationship& getExclusion() const;
    Relationship& getExclusion();
    const Relationship& getPrerequisite() const;
    Relationship& getPrerequisite();
    const Relationship& getCorequisite() const;
    const Relationship& getColist() const;
    const Relationship& getNotAvailableAfter() const;
    const Relationship& getAvailableAfter() const;

    const QString& getVector() const;
    const QString& getPreviousCode() const;
    const std::vector<Semester>& getOfferIn() const;
    QString getOfferInDescription() const;

    // stream operator
    friend std::ostream& operator<<(std::ostream &os, const Course& course);

    // QString description of all members.
    QString dubugDescription();

    // Course relationship operation
    void addNotAvailableAfter(Course* const course);
    void addAvaiableAfter(Course* const course);
    void linkCourses(const AVLTree<QString, AVLTree<CourseCode, Course* >* > &courses);
};


#endif // COURSE_H
