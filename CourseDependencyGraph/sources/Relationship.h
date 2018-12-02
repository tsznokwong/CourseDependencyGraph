#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

#include <vector>
#include <QString>

#include "sources/CourseCode.h"
#include "sources/AVLTree.tpp"

class Course;

class Relationship {
public:
    enum Type {
        EXCLUSION, PREREQUISITE, COREQUISITE, COLIST, NOT_AVAILABLE_AFTER, AVAILABLE_AFTER
    };
private:
    Type type;
    std::vector<Course*> edges;
    QString description;
    std::vector<CourseCode> courseCodes;

    void updateDescription();

public:
    Relationship(Type type, QString description);

    // getter
    const Type& getType() const;
    const std::vector<Course*>& getEdges() const;
    std::vector<Course*>& getEdges();
    const QString& getDescription() const;
    const std::vector<CourseCode>& getCourseCodes() const;

    // Course relationship operation
    void addEdge(const CourseCode courseCode, Course* const course);
    void linkCourses(const AVLTree<QString, AVLTree<CourseCode, Course* >* > &courses);
};

#endif // RELATIONSHIP_H
