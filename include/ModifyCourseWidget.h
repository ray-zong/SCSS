#ifndef MODIFYCOURSEWIDGET_H
#define MODIFYCOURSEWIDGET_H

#include <QDialog>

class QLineEdit;
class QSpinBox;

class ModifyCourseWidget : public QDialog
{
    Q_OBJECT

public:
    ModifyCourseWidget(QWidget *parent = nullptr);
    ~ModifyCourseWidget();

    //设置课程号
    void setCourseNumber(int number);
    //设置课程名
    void setCourseName(const QString &name);
    //设置成绩
    void setCourseScore(const QString &score);

    //设置课程号
    int getCourseNumber();
    //设置课程名
    QString getCourseName();
    //设置成绩
    QString getCourseScore();

private:
    //初始化界面
    void initWidget();
    //通过课程号修改成绩
    bool modifyCourseScoreByNumber(int number, const QString &score);

private slots:
    void ok();
    void cancel();

private:
    QSpinBox *m_pSpinBox_number;
    QLineEdit *m_pLineEdit_name;
    QLineEdit *m_pLineEdit_score;
};


#endif
