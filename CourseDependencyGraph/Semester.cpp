
#include "Semester.h"

Semester::Semester(QString year, QString season):
    year(year),
    season(season) {

}

const QString& Semester::getYear() { return this->year; }
const QString& Semester::getSeason() { return this->season; }

QString Semester::description() {
    return this->year + " " + this->season;
}
