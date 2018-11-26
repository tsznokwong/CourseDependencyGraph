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
        EXCLUSION, PREREQUISITE, COREQUISITE, COLIST
    };
private:
    Type type;
    std::vector<const Course*> edges;
    QString description;
    std::vector<CourseCode> courseCodes;
public:
    Relationship(Type type, QString description);

    // getter
    const Type& getType() const;
    const std::vector<const Course*>& getEdges() const;
    const QString& getDescription() const;
    const std::vector<CourseCode>& getCourseCodes() const;

    void linkCourses(const AVLTree<QString, AVLTree<CourseCode, Course* >* > &courses);
};

#endif // RELATIONSHIP_H
