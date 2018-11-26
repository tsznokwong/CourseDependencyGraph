#include <QDebug>
#include <QRegularExpression>

#include "DependencyManager.h"
#include "CSVLoader.h"

DependencyManager::DependencyManager():
    courses() {

}

void DependencyManager::loadCSV() {
    CSVLoader::Data* courseData = CSVLoader::load();

    for (const QStringList &record: courseData->records) {
        if (record.count() != courseData->fields.count()) {
            qDebug() << "Record size not match" << endl;
            qDebug() << "Fields" << courseData->fields << endl;
            qDebug() << "Record" << record << endl;
            continue;
        }

        QString subject;
        int code = -1;
        QString extension;
        QString title;
        int credit = -1;
        QString description;
        QString attribute;
        QString exclusion;
        QString prerequisite;
        QString corequisite;
        QString colist;
        QString vector;
        QString previousCode;
        std::vector<Semester> offerIn;

        //SUBJECT,CODE,TITLE,CREDIT,ATTRIBUTE,EXCLUSION STR,PREREQUISITE STR,COREQUISITE STR,DESCRIPTION,COLIST WITH STR,VECTOR,PREVIOUS CODE,OFFER IN
        for (int column = 0; column < record.count(); ++column) {
            if (courseData->fields[column] == "SUBJECT") {
                subject = record[column];
            } else if (courseData->fields[column] == "CODE") {
                QRegularExpression regex {"(?<code>\\d{4})(?<extension>\\w?)"};
                QRegularExpressionMatch match = regex.match(record[column]);
                if (match.hasMatch()) {
                    code = match.captured("code").toInt();
                    extension = match.captured("extension");
                }
            } else if (courseData->fields[column] == "TITLE") {
                title = record[column];
            } else if (courseData->fields[column] == "CREDIT") {
                credit = record[column].toInt();
            } else if (courseData->fields[column] == "ATTRIBUTE") {
                attribute = record[column];
            } else if (courseData->fields[column] == "EXCLUSION STR") {
                exclusion = record[column];
            } else if (courseData->fields[column] == "PREREQUISITE STR") {
                prerequisite = record[column];
            } else if (courseData->fields[column] == "COREQUISITE STR") {
                corequisite = record[column];
            } else if (courseData->fields[column] == "DESCRIPTION") {
                description = record[column];
            } else if (courseData->fields[column] == "COLIST WITH STR") {
                colist = record[column];
            } else if (courseData->fields[column] == "VECTOR") {
                vector = record[column];
            } else if (courseData->fields[column] == "PREVIOUS CODE") {
                previousCode = record[column];
            } else if (courseData->fields[column] == "OFFER IN") {
                QRegularExpression regex {"(?<year>\\d{4}-\\d{2}) (?<season>Fall|Summer|Spring|Winter)"};
                QRegularExpressionMatchIterator iterator = regex.globalMatch(record[column]);
                while (iterator.hasNext()) {
                    QRegularExpressionMatch match = iterator.next();
                    Semester semester { match.captured("year"), match.captured("season") };
                    offerIn.push_back(semester);
                }
            }
        }
        Course *course = new Course { CourseCode(subject, code, extension),
                title,
                credit,
                description,
                attribute,
                exclusion,
                prerequisite,
                corequisite,
                colist,
                vector,
                previousCode,
                offerIn };
		if (!this->courses.contains(subject))
            this->courses.add(subject, new AVLTree<CourseCode, Course*>());
        this->courses.find(subject)->add(course->getCourseCode(), course);

    }
    delete courseData;
}

void DependencyManager::linkCourses() {
    std::vector<QString> subjects;
    this->courses.toKeyVector(subjects);
    for (QString subject: subjects) {
        std::vector<CourseCode> courses;
        this->courses.find(subject)->toKeyVector(courses);
        for (CourseCode courseCode: courses) {
            this->courses.find(subject)->find(courseCode)->linkCourses(this->courses);
        }
    }
}


