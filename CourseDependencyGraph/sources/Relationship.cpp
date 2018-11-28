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
            this->courseCodes.push_back(courseCode);
        }
    }

}

const Relationship::Type& Relationship::getType() const { return this->type; }
const std::vector<Course*>& Relationship::getEdges() const { return this->edges; }
std::vector<Course*>& Relationship::getEdges() { return this->edges; }
const QString& Relationship::getDescription() const { return this->description; }
const std::vector<CourseCode>& Relationship::getCourseCodes() const { return this->courseCodes; }

void Relationship::updateDescription() {
    if (this->courseCodes.size() == 0) {
        this->description = "-";
        return;
    }
    this->description = "";
    for (unsigned int iterator = 0; iterator < this->courseCodes.size(); ++iterator) {
        this->description += this->courseCodes[iterator].description() +
                (iterator < this->courseCodes.size() - 1 ? ", " : "");
    }
}

void Relationship::addEdge(const CourseCode courseCode, Course* const course) {
    this->edges.push_back(course);
    this->courseCodes.push_back(courseCode);
    this->updateDescription();
}

void Relationship::linkCourses(const AVLTree<QString, AVLTree<CourseCode, Course* >* > &courses) {
    for (CourseCode courseCode: this->courseCodes) {
        if (!courses.contains(courseCode.getSubject())) {
            qDebug() << "Subject Not Found: " << courseCode.getSubject();
            continue;
        }
        if (!courses.find(courseCode.getSubject())->contains(courseCode)) {
            qDebug() << "Course Code Not Found: " << courseCode.description();
            continue;
        }
        Course *course = courses.find(courseCode.getSubject())->find(courseCode);
        this->edges.push_back(course);
    }
}
