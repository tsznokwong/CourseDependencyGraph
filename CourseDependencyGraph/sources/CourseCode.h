#ifndef COURSECODE_H
#define COURSECODE_H

#include <QString>

class CourseCode {
private:
    QString subject;
    int code;
    QString extension;

public:
    CourseCode() = default;
    CourseCode(QString subject, int code, QString extension = "");

    static CourseCode* create(QString code);

    // getter
    const QString& getSubject() const;
    const int& getCode() const;
    const QString& getExtension() const;

    QString description() const;

    // operator overload
    bool operator==(const CourseCode& rhs) const;
    bool operator>(const CourseCode& rhs) const;
    bool operator>=(const CourseCode& rhs) const;
    bool operator<(const CourseCode& rhs) const;
    bool operator<=(const CourseCode& rhs) const;

};



#endif // COURSECODE_H
