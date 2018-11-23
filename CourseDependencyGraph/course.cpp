
#include "Course.h"

Course::Course(CourseCode courseCode,
               QString title,
               int credit,
               QString description,
               QString attribute,
               QString exclusion,
               QString prerequisite,
               QString corequisite,
               QString colist,
               QString vector,
               CourseCode previousCode,
               std::vector<Semester> offerIn):
    courseCode(courseCode),
    title(title),
    credit(credit),
    description(description),
    attribute(attribute),
    exclusion(Relationship{Relationship::Type::EXCLUSION, exclusion}),
    prerequisite(Relationship{Relationship::Type::PREREQUISITE, prerequisite}),
    corequisite(Relationship{Relationship::Type::COREQUISITE, corequisite}),
    colist(Relationship{Relationship::Type::COLIST, colist}),
    vector(vector),
    previousCode(previousCode),
    offerIn(offerIn) {

}

const CourseCode& Course::getCourseCode() { return this->courseCode; }
const QString& Course::getTitle() { return this->title; }
const int& Course::getCredit() { return this->credit; }
const QString& Course::getDescription() { return this->description; }
const QString& Course::getAttribute() { return this->attribute; }

const Relationship& Course::getExclusion() { return this->exclusion; }
const Relationship& Course::getPrerequisite() { return this->prerequisite; }
const Relationship& Course::getCorequisite() { return this->corequisite; }
const Relationship& Course::getColist() { return this->colist; }

const QString& Course::getVector() { return this->vector; }
const CourseCode& Course::getPreviousCode() { return this->previousCode; }
const std::vector<Semester>& Course::getOfferIn() { return this->offerIn; }
