#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "OptionTreeWidget.h"
#include "TakedCourseWidget.h"
#include "SelectableCourseWidget.h"
#include "RetakeCourseWidget.h"
#include "IFileAnalysis.h"
#include "ChooseWidget.h"

#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QDebug>
#include <QStackedWidget>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTextCodec>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pTabWidget(nullptr)
    , m_pOptionTreeWidget(nullptr)
    , m_pRetakeCourseWidget(nullptr)
    , m_pSelectableCourseWidget(nullptr)
    , m_pTakedCourseWidget(nullptr)
    , m_pChooseWidget(nullptr)

{
    ui->setupUi(this);

    setWindowTitle(tr("SCSS(Student Course Selection Guidance System)"));
    //菜单栏
    createMenu();

    //主界面
    createMainWidget();

    //已修课程
    displayTakedCourse();
    //未修课程
    Q_ASSERT(m_pChooseWidget);
    displaySelectableCourse(m_pChooseWidget->currentSpecialty(),
                            m_pChooseWidget->currentTerm());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createMenu()
{
    //文件
    QMenu *pFile = menuBar()->addMenu(tr("&File"));
    {
        //打开
        QMenu *pOpen = pFile->addMenu(tr("Open"));
        //培养方案
        QAction *pTrainingProgram = pOpen->addAction(tr("Training Program"));
        connect(pTrainingProgram, &QAction::triggered, this, &MainWindow::openTrainingProgram);
        //教学计划
        QAction *pTeachingPlan = pOpen->addAction(tr("Teaching Plan"));
        connect(pTeachingPlan, &QAction::triggered, this, &MainWindow::openTeachingPlan);
    }

    //工具
    //QMenu *pTool = menuBar()->addMenu(tr("&Tool"));
    {
        //选项
        //QAction *pOption = pTool->addAction(tr("Option") + "...");
        //connect(pOption, &QAction::triggered, this, &MainWindow::openOptionDialog);
    }
    //帮助
    QMenu *pHelp = menuBar()->addMenu(tr("&Help"));
    {
        //QAction *pLanguage = pHelp->addAction("Language");
        //connect(pLanguage, &QAction::triggered, this, &MainWindow::changeLanguage);

        QAction *pAbout = pHelp->addAction(tr("About"));
        connect(pAbout, &QAction::triggered, this, &MainWindow::aboutSoftware);
    }
}

void MainWindow::createMainWidget()
{
    QWidget *pWidget = this->centralWidget();

    QVBoxLayout *pVLayout = new QVBoxLayout(pWidget);

    m_pChooseWidget = new ChooseWidget(this);
    connect(m_pChooseWidget, &ChooseWidget::currentSpecialtyOrTermChanged, this, &MainWindow::currentSpecialtyOrTermChanged);

    //已修课程
    m_pTakedCourseWidget = new TakedCourseWidget(this);
    //未修课程
    m_pSelectableCourseWidget = new SelectableCourseWidget(this);
    //重修课程
    //m_pRetakeCourseWidget = new RetakeCourseWidget(this);

    m_pTabWidget = new QTabWidget(this);
    m_pTabWidget->addTab(m_pTakedCourseWidget, tr("Taked Course"));
    m_pTabWidget->addTab(m_pSelectableCourseWidget, tr("Selectable Course"));

    pVLayout->addWidget(m_pChooseWidget);
    pVLayout->addWidget(m_pTabWidget);
}

void MainWindow::displayTakedCourse()
{
    Q_ASSERT(m_pTakedCourseWidget);

    m_pTakedCourseWidget->displayTakedCourseInfo();
}

void MainWindow::displaySelectableCourse(int specialty, int term)
{
    Q_ASSERT(m_pSelectableCourseWidget);

    m_pSelectableCourseWidget->displaySelectCourseInfo(specialty, term);
}

void MainWindow::openTeachingPlan()
{
    QString strFileName = QFileDialog::getOpenFileName(this,
                                                       tr("Open Teaching Plan"), qApp->applicationDirPath(), tr("Files (*.txt)"));
    if(strFileName.isEmpty())
        return;

    IFileAnalysis *pFileAnalysis = IFileAnalysis::createFileAnalysis(IFileAnalysis::TeachingPlanFile);

    if(!pFileAnalysis->analysis(strFileName))
    {
        qDebug() << __FILE__ << __LINE__ << "open teaching plan error:" << strFileName;
        return;
    }

    Q_ASSERT(m_pTakedCourseWidget);
    QVector<IPersonalData* > vecTeachingPlan = pFileAnalysis->getData();
    m_pTakedCourseWidget->updateCourseData(vecTeachingPlan);
    m_pTakedCourseWidget->displayTakedCourseInfo();
}

void MainWindow::openTrainingProgram()
{
    QString strFileName = QFileDialog::getOpenFileName(this,
                                                       tr("Open Training Program"), qApp->applicationDirPath(), tr("Files (*.txt)"));
    if(strFileName.isEmpty())
        return;

    IFileAnalysis *pFileAnalysis = IFileAnalysis::createFileAnalysis(IFileAnalysis::TrainingProgramFile);

    if(!pFileAnalysis->analysis(strFileName))
    {
        qDebug() << __FILE__ << __LINE__ << "open training program error:" << strFileName;
        return;
    }

    Q_ASSERT(m_pTakedCourseWidget);
    QVector<IPersonalData* > vecTeachingPlan = pFileAnalysis->getData();
    m_pTakedCourseWidget->updateCourseData(vecTeachingPlan);
    m_pTakedCourseWidget->displayTakedCourseInfo();
}

void MainWindow::changeLanguage()
{
    qDebug() << __FILE__ << __LINE__ << "changeLanguage function";
}

void MainWindow::aboutSoftware()
{
    QMessageBox::about(this, tr("SCSS"), tr("Version:1.0.0\n"
                                            "Rightcopy:Tsinghua University"));
}

void MainWindow::currentOptionChanged(const int, const int)
{
}

void MainWindow::openOptionDialog()
{

}

void MainWindow::currentSpecialtyOrTermChanged(int specialty, int term)
{
    Q_ASSERT(m_pSelectableCourseWidget != NULL);

    m_pSelectableCourseWidget->displaySelectCourseInfo(specialty, term);
}
