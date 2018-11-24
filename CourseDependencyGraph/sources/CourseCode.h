#ifndef COURSECODE_H
#define COURSECODE_H

#include <QString>

class CourseCode {
private:
    QString subject;
    int code;
    QString extension;

public:
    CourseCode(QString subject, int code, QString extension = "");

    // getter
    const QString& getSubject();
    const int& getCode();
    const QString& getExtension();

    QString description();
};



#endif // COURSECODE_H
