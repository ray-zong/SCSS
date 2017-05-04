#ifndef ABOUTSOFTWARE_H
#define ABOUTSOFTWARE_H

#include <QDialog>

class QLabel;

class AboutSoftware : public QDialog
{
    Q_OBJECT

public:
    explicit AboutSoftware(QWidget *parent = 0);
    ~AboutSoftware();

private:
    void initUI();

private:
    QLabel *m_pText;
};

#endif // MAINWINDOW_H
