#ifndef SELECTABLECOURSEWIDGET_H
#define SELECTABLECOURSEWIDGET_H

#include <QWidget>
#include <QMap>

#include "DataTypeDefine.h"

class QTableWidget;
class QLabel;
class QSqlDatabase;
class IPersonalData;
class QCheckBox;
class QPushButton;
class SelectedCourseResult;
class QTabWidget;

class SelectableCourseWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SelectableCourseWidget(QWidget *parent = 0);
    ~SelectableCourseWidget();

    //显示选课信息
    bool displaySelectCourseInfo(int specialty, int term);

    //已修课程
    void setTakedCourseData(const QVector<IPersonalData* >&);

private:
    QTableWidget *createTableWidget();
    //打开数据库
    bool createConnection(QSqlDatabase &db);
    //选课按钮
    QWidget *addCheckWidget(int, int, int);
    //为课组设置课程内容
    void setSelectableCourseByGroup(int group, const QVector<DB_SpecialtyCourse> &vecSpecialtyCourse);

private slots:
    void checkboxClicked();

    void checkSelectedCourse();

private:
    QLabel *m_pLabel_advise;
    QPushButton *m_pPushButton;
    QTabWidget *m_pTabWidget;
    SelectedCourseResult *m_pSelectedCourseResult;
    QVector<IPersonalData *> m_vecPersonalData;

    int m_nSpecialty;
    int m_nTerm;
    //当前点击映射到index,group,row
    QMap<QCheckBox *, QVector3D> m_mapCheckboxToGroupRow;
    //已修课程学分分组（学分分组，课程号）
    QMap<int, QVector<DB_SpecialtyCourse> > m_mapGroupToTakedCourse;
    //未修课程学分分组
    QMap<int, QVector<DB_SpecialtyCourse> > m_mapGroupToSelectableCourse;
    //学分分组
    QMap<int, DB_SpecialtyCreditGroup > m_mapGroupTospecialtyCreditGroup;
    //当前选课数据
    QMap<int, QVector<DB_SpecialtyCourse> > m_mapGroupToSpecialtyCourse;
};

#endif // SELECTABLECOURSEWIDGET_H
