
#include "CourseCode.h"

CourseCode::CourseCode(QString subject, int code, QString extension):
    subject(subject),
    code(code),
    extension(extension) {

}

const QString& CourseCode::getSubject() const { return this->subject; }
const int& CourseCode::getCode() const { return this->code; }
const QString& CourseCode::getExtension() const { return this->extension; }

QString CourseCode::description() const {
    return this->subject + QString::number(this->code) + this->extension;
}
