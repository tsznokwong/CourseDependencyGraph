
#include "Semester.h"

Semester::Semester(QString year, QString season):
    year(year),
    season(season) {

}

const QString& Semester::getYear() const { return this->year; }
const QString& Semester::getSeason() const { return this->season; }

QString Semester::description() const {
    return this->year + " " + this->season;
}
