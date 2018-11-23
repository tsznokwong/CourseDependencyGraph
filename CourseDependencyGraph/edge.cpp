
#include "Edge.h"

Edge::Edge(Course *course, Relationship::Type type):
    type(type),
    course(course) {

}

const Relationship::Type& Edge::getType() { return this->type; }
const Course* const & Edge::getCourse() { return this->course; }
