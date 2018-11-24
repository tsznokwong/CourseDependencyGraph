#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

#include <vector>
#include <QString>

class Course;

class Relationship {
public:
    enum Type {
        EXCLUSION, PREREQUISITE, COREQUISITE, COLIST
    };
private:
    Type type;
    std::vector<Course*> edges;
    QString description;
public:
    Relationship(Type type, QString description);

    // getter
    const Type& getType();
    const std::vector<Course*>& getEdges();
    const QString& getDescription();
};

#endif // RELATIONSHIP_H
