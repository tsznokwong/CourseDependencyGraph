
#include "relationship.h"

Relationship::Relationship(Type type, QString description):
    type(type),
    description(description) {

}

const Relationship::Type& Relationship::getType() { return this->type; }
const std::vector<Course*>& Relationship::getEdges() { return this->edges; }
const QString& Relationship::getDescription() { return this->description; }
