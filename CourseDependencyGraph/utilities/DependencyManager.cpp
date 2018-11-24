#include <QDebug>
#include <QRegularExpression>

#include "DependencyManager.h"
#include "CSVLoader.h"

DependencyManager::DependencyManager() {

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
                QRegularExpression regex {"(\\d{4})(\\w?)"};
                QRegularExpressionMatch match = regex.match(record[column]);
                if (match.hasMatch()) {
                    code = match.captured(1).toInt();
                    extension = match.captured(2);
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
                QRegularExpression regex {"(\\d{4}-\\d{2}) (Fall|Summer|Spring|Winter)"};
                QRegularExpressionMatchIterator iterator = regex.globalMatch(record[column]);
                while (iterator.hasNext()) {
                    QRegularExpressionMatch match = iterator.next();
                    Semester semester { match.captured(1), match.captured(2) };
                    offerIn.push_back(semester);
                }
            }
            Course course { CourseCode(subject, code, extension),
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
        }

    }

    delete courseData;
}
