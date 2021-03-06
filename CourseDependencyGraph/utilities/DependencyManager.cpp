#include <QDebug>
#include <QRegularExpression>
#include <iostream>

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

        // csv field line
        // SUBJECT,CODE,TITLE,CREDIT,ATTRIBUTE,EXCLUSION STR,PREREQUISITE STR,COREQUISITE STR,DESCRIPTION,COLIST WITH STR,VECTOR,PREVIOUS CODE,OFFER IN
        for (int column = 0; column < record.count(); ++column) {
            if (courseData->fields[column] == "SUBJECT") {
                subject = record[column];
            } else if (courseData->fields[column] == "CODE") {
                QRegularExpression regex {"(?<code>\\d{4})(?<extension>[A-Z]?)"};
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

/**
 * @brief DependencyManager::linkCourses.
 * Adding pointers to course relationship and reverse relationship of exclusion and prerequisites.
 */
void DependencyManager::linkCourses() {
    std::vector<QString> subjects;
    this->courses.toKeyVector(subjects);
    for (QString subject: subjects) {
        AVLTree<CourseCode, Course* >* subjectTree = this->courses.find(subject);
        std::vector<CourseCode> courseCodes;
        subjectTree->toKeyVector(courseCodes);

        for (CourseCode courseCode: courseCodes) {
            Course* course = subjectTree->find(courseCode);
            course->linkCourses(this->courses);

            for (Course *targetCourse: course->getPrerequisite().getEdges()) {
                targetCourse->addAvaiableAfter(course);
            }
            for (Course *targetCourse: course->getExclusion().getEdges()) {
                targetCourse->addNotAvailableAfter(course);
            }

        }
    }
}

Course* DependencyManager::findCourse(QString code){
	QString subject = code.left(4);
	if (!courses.contains(subject)) return nullptr;
	CourseCode* courseCode = CourseCode::create(code);
	if (!courses.find(subject)->contains(*courseCode)){
		delete courseCode;
		return nullptr;
	}
	Course* course = courses.find(subject)->find(*courseCode);
	delete courseCode;
	return course;
}
