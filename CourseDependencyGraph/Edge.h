#ifndef EDGE_H
#define EDGE_H

#include "Relationship.h"

class Course;

class Edge {
private:
    Relationship::Type type;
    Course *course;

public:
    Edge(Course *course, Relationship::Type type);

    // getter
    const Relationship::Type& getType();
    const Course*& getCourse();
};


#endif // EDGE_H
