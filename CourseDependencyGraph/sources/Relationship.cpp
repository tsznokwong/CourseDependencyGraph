#include <QRegularExpression>
#include <QDebug>

#include "Relationship.h"

Relationship::Relationship(Type type, QString description):
    type(type),
    description(description) {
    QRegularExpression courses { "(?<subject>[A-Z]{4})\\s*(\\/?\\d{3,4}[A-Z]?)+" };
    QRegularExpressionMatchIterator courseIterator = courses.globalMatch(description);
    while (courseIterator.hasNext()) {
        QRegularExpressionMatch courseMatch = courseIterator.next();
        QString subject = courseMatch.captured("subject");
        QRegularExpression codes { "(?<code>\\d{3,4})(?<extension>[A-Z]?)" };
        QRegularExpressionMatchIterator codeIterator = codes.globalMatch(courseMatch.captured(0));
        while (codeIterator.hasNext()) {
            QRegularExpressionMatch codeMatch = codeIterator.next();
            QString code = codeMatch.captured("code");
            QString extension = codeMatch.captured("extension");
            CourseCode courseCode { subject, code.toInt(), extension};
            this->codes.push_back(courseCode);
        }
    }
    for (CourseCode courseCode: this->codes) {
        qDebug() << courseCode.description();
    }
}

const Relationship::Type& Relationship::getType() const { return this->type; }
const std::vector<Course*>& Relationship::getEdges() const { return this->edges; }
const QString& Relationship::getDescription() const { return this->description; }
const std::vector<CourseCode>& Relationship::getCodes() const { return this->codes; }
