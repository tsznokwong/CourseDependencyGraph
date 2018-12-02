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

    // create dynamically allocated CourseCode with QString.
    // return nullptr if parsing failed.
    static CourseCode* create(QString code);

    // getter
    const QString& getSubject() const;
    const int& getCode() const;
    const QString& getExtension() const;

    // full description of course code
    QString description() const;

    // comparison operator overload
    bool operator==(const CourseCode& rhs) const;
    bool operator>(const CourseCode& rhs) const;
    bool operator>=(const CourseCode& rhs) const;
    bool operator<(const CourseCode& rhs) const;
    bool operator<=(const CourseCode& rhs) const;

};



#endif // COURSECODE_H
