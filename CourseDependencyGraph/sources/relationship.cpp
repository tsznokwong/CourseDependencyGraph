
#include "relationship.h"

Relationship::Relationship(Type type, QString description):
    type(type),
    description(description) {

}

const Relationship::Type& Relationship::getType() const { return this->type; }
const std::vector<Course*>& Relationship::getEdges() const { return this->edges; }
const QString& Relationship::getDescription() const { return this->description; }
