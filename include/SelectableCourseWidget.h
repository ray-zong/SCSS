#ifndef SELECTABLECOURSEWIDGET_H
#define SELECTABLECOURSEWIDGET_H

#include <QWidget>

class QTableWidget;
class QLabel;
class QSqlDatabase;
class IPersonalData;

class SelectableCourseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SelectableCourseWidget(QWidget *parent = 0);
    ~SelectableCourseWidget();

    //专业
    void setCurrentSpecialty(int specialty);
    //学期
    void setCurrentTerm(int term);
    //已修课程
    void setTakedCourseData(const QVector<IPersonalData* >&);

private:
    QTableWidget *createTableWidget();
    //打开数据库
    bool createConnection(QSqlDatabase &db);
    //选课
    bool selectCourseAnalysis();

private slots:

private:
    QLabel *m_pLabel_advise;
    QTableWidget *m_pTableWidget;
    QVector<IPersonalData *> m_vecPersonalData;

    int m_nSpecialty;
    int m_nTerm;
};

#endif // SELECTABLECOURSEWIDGET_H
