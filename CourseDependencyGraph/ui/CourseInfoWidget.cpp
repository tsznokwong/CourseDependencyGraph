#include <QDebug>

#include "CourseInfoWidget.h"
#include "ui_CourseInfoWidget.h"

CourseInfoWidget::CourseInfoWidget(QWidget *parent,
                                   const AVLTree<QString, AVLTree<CourseCode, Course* >* >* courses,
                                   const Course* const course,
                                   const QTreeWidget* const treeWidget) :
    QWidget(parent),
    ui(new Ui::CourseInfoWidget),
    course(course),
    courses(courses),
    treeWidget(treeWidget){
    ui->setupUi(this);

    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
}

CourseInfoWidget::~CourseInfoWidget() {
    delete ui;
}

void CourseInfoWidget::setCourseInfo() {
    if (this->course == nullptr) { return; }

    QString course = this->course->getCourseCode().description() + " (" + QString::number(this->course->getCredit()) + ")";
    course += "\n" + this->course->getTitle();

    QString attributeDescription = this->course->getVector() != "-" ? this->course->getVector() : "" ;
    if (attributeDescription.length() > 0) {
        if (this->course->getAttribute() != "-") {
            attributeDescription += " " + this->course->getAttribute();
        }
    } else {
        attributeDescription = this->course->getAttribute();
    }
    course += "\n" + attributeDescription;

    this->ui->courseLabel->setText(course);

    this->ui->descriptionLabel->setText(this->course->getDescription());

//    if (this->course->getExclusion().getDescription() != "-") {
//        this->ui->exclusionLabel->show();
        this->ui->exclusionLabel->setText("Exclusion: " + this->course->getExclusion().getDescription());
//    } else {
//        this->ui->exclusionLabel->hide();
//    }

//    if (this->course->getPrerequisite().getDescription() != "-") {
//        this->ui->prerequisiteLabel->show();
        this->ui->prerequisiteLabel->setText("Prerequisites: " + this->course->getPrerequisite().getDescription());
//    } else {
//        this->ui->prerequisiteLabel->hide();
//    }

//    if (this->course->getCorequisite().getDescription() != "-") {
//        this->ui->corequisiteLabel->show();
        this->ui->corequisiteLabel->setText("Corequisites: " + this->course->getCorequisite().getDescription());
//    } else {
//        this->ui->corequisiteLabel->hide();
//    }

//    if (this->course->getColist().getDescription() != "-") {
//        this->ui->colistLabel->show();
        this->ui->colistLabel->setText("Colists: " + this->course->getColist().getDescription());
//    } else {
//        this->ui->colistLabel->hide();
//    }

//    if (this->course->getAvailableAfter().getDescription() != "-") {
//        this->ui->availableAfterLabel->show();
        this->ui->availableAfterLabel->setText("Available After: " + this->course->getAvailableAfter().getDescription());
//    } else {
//        this->ui->availableAfterLabel->hide();
//    }

//    if (this->course->getNotAvailableAfter().getDescription() != "-") {
//        this->ui->notAvailableAfterLabel->show();
        this->ui->notAvailableAfterLabel->setText("Not Available After: " + this->course->getNotAvailableAfter().getDescription());
//    } else {
//        this->ui->notAvailableAfterLabel->hide();
//    }

//    if (this->course->getPreviousCode() != "-") {
//        this->ui->previousCodeLabel->show();
        this->ui->previousCodeLabel->setText("Previous Code: " + this->course->getPreviousCode());
//    } else {
//        this->ui->previousCodeLabel->hide();
//    }

    this->ui->offerInLabel->setText("Offer In: " + this->course->getOfferInDescription());
}

void CourseInfoWidget::treeWidgetItemClicked() {
    QList<QTreeWidgetItem *> items = this->treeWidget->selectedItems();
    if (items.size() == 0) { return; }
    QTreeWidgetItem *item = items.first();
    if (item == nullptr) { return; }
    QTreeWidgetItem *subject = item->parent();
    if (subject == nullptr) { return; }

    AVLTree<CourseCode, Course* >* subjectTree = this->courses->find(subject->text(0));
    if (subjectTree == nullptr) { return; }

    CourseCode *courseCode = CourseCode::create(subject->text(0) + item->text((0)));
    if (courseCode == nullptr) { return; }
    this->course = subjectTree->find(*courseCode);
    delete courseCode;

    this->setCourseInfo();
}

