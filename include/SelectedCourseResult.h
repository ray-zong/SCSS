#ifndef SELECTEDCOURSERESULT_H
#define SELECTEDCOURSERESULT_H

#include <QDialog>

#include "DataTypeDefine.h"


class QLabel;


class SelectedCourseResult : public QDialog
{
public:
    SelectedCourseResult(QWidget* parent = nullptr);
    ~SelectedCourseResult();

    void displaySelectedCourse(const QVector<DB_SpecialtyCourse> &);

private:
    void initWidget();

private:
    QLabel *m_pLabel;
}; 

#endif //SELECTEDCOURSERESULT_H
