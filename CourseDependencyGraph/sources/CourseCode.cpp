#include <QRegularExpression>
#include "CourseCode.h"

CourseCode::CourseCode(QString subject, int code, QString extension):
    subject(subject),
    code(code),
    extension(extension) {

}

CourseCode* CourseCode::create(QString code) {
    QRegularExpression regex {"(?<subject>[A-Z]{4})\\s*(?<code>\\d{4})(?<extension>[A-Z]?)"};
    QRegularExpressionMatch match = regex.match(code);
    if (match.hasMatch()) {
        CourseCode *courseCode = new CourseCode { match.captured("subject"),
                match.captured("code").toInt(),
                match.captured("extension") };
        return courseCode;
    }
    return nullptr;
}

const QString& CourseCode::getSubject() const { return this->subject; }
const int& CourseCode::getCode() const { return this->code; }
const QString& CourseCode::getExtension() const { return this->extension; }

QString CourseCode::description() const {
    return this->subject + QString::number(this->code) + this->extension;
}

bool CourseCode::operator==(const CourseCode& rhs) const {
    return this->description() == rhs.description();
}
bool CourseCode::operator>(const CourseCode& rhs) const {
    return this->description() > rhs.description();
}
bool CourseCode::operator>=(const CourseCode& rhs) const {
    return this->description() >= rhs.description();
}
bool CourseCode::operator<(const CourseCode& rhs) const {
    return this->description() < rhs.description();
}
bool CourseCode::operator<=(const CourseCode& rhs) const {
    return this->description() <= rhs.description();
}
