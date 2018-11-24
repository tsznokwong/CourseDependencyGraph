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
    const QString& getSubject() const;
    const int& getCode() const;
    const QString& getExtension() const;

    QString description() const;
};



#endif // COURSECODE_H
