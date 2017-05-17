#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QDockWidget;
class QStackedWidget;
class OptionTreeWidget;
class RetakeCourseWidget;
class SelectableCourseWidget;
class TakedCourseWidget;
class ChooseWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    //菜单栏
    void createMenu();
    //主界面：显示课程信息
    void createMainWidget();
    //显示已修课程
    void displayTakedCourse();
    //显示未修课程 index: 专业
    void displaySelectableCourse(int specialty, int term);

private slots:
    //打开教学计划
    void openTeachingPlan();
    //打开培养方案
    void openTrainingProgram();
    //语言切换
    void changeLanguage();
    //关于
    void aboutSoftware();
    //显示状态改变
    void currentOptionChanged(const int, const int);
    //打开工具栏
    void openOptionDialog();
    //专业或者学期改变
    void currentSpecialtyOrTermChanged(int specialty, int term);
    //修改已修课程
    void takedCourseChanged();

private:
    Ui::MainWindow *ui;

    QTabWidget *m_pTabWidget;
    OptionTreeWidget *m_pOptionTreeWidget;
    RetakeCourseWidget *m_pRetakeCourseWidget;
    SelectableCourseWidget *m_pSelectableCourseWidget;
    TakedCourseWidget *m_pTakedCourseWidget;
    ChooseWidget *m_pChooseWidget;
};

#endif // MAINWINDOW_H
