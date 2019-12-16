#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFile>
#include <QMainWindow>

#include "qcustomplot.h"
#include "diffequation.h"
#include "doublevalidator.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void showPlots(void);

    void printSolutions(void);

    ~MainWindow();

private slots:
    void on_calculate_clicked(void);

private:
    Ui::MainWindow *ui;

    QCPLayoutGrid *subLayout;

    DoubleValidator *doubleValidator;

    DiffEquation *diffEquation;
};
#endif // MAINWINDOW_H
