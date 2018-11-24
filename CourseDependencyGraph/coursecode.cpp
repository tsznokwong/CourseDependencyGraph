
#include "CourseCode.h"

CourseCode::CourseCode(QString subject, int code, QString extension):
    subject(subject),
    code(code),
    extension(extension) {

}

const QString& CourseCode::getSubject() { return this->subject; }
const int& CourseCode::getCode() { return this->code; }
const QString& CourseCode::getExtension() { return this->extension; }

QString CourseCode::description() {
    return this->subject + QString::number(this->code) + this->extension;
}
