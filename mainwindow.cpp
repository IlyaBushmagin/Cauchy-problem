#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(1018, 568);
    QString backgroundColor("background-color:rgb(239, 239, 239);");
    ui->plots->setBackground(this->palette().background().color());
    ui->solutionsOutput->setStyleSheet(backgroundColor);
    ui->mathExpression->setStyleSheet(backgroundColor);
    ui->x_0->setStyleSheet(backgroundColor);
    ui->y_0->setStyleSheet(backgroundColor);
    ui->x_max->setStyleSheet(backgroundColor);
    ui->h_->setStyleSheet(backgroundColor);
    ui->calculate->setShortcut(Qt::Key_Return);
    int maxLength = 11;
    ui->x_0->setMaxLength(maxLength);
    ui->y_0->setMaxLength(maxLength);
    ui->x_max->setMaxLength(maxLength);
    ui->h_->setMaxLength(maxLength);
    doubleValidator = new DoubleValidator(-9999999999, 9999999999, this);
    ui->x_0->setValidator(doubleValidator);
    ui->y_0->setValidator(doubleValidator);
    ui->x_max->setValidator(doubleValidator);
    ui->h_->setValidator(doubleValidator);
    diffEquation = nullptr;
    ui->statusbar->showMessage(" Ready");
}

void MainWindow::on_calculate_clicked(void)
{
    ui->statusbar->clearMessage();
    ui->statusbar->showMessage(" Calculation...");
    bool x_0_ok, y_0_ok, x_max_ok, h_ok;
    QString mathExpression = ui->mathExpression->text();
    double x_0 = ui->x_0->text().toDouble(&x_0_ok);
    double y_0 = ui->y_0->text().toDouble(&y_0_ok);
    double x_max = ui->x_max->text().toDouble(&x_max_ok);
    double h = ui->h_->text().toDouble(&h_ok);
    if(!mathExpression.isEmpty() && x_0_ok && y_0_ok && x_max_ok && h_ok)
    {
        if(x_0 <= x_max)
        {
            if(h > 0)
            {
                diffEquation = new DiffEquation(mathExpression, x_0, y_0, x_max, h);
                if(diffEquation->calculateSolutions("report/solutions.txt"))
                {
                    printSolutions();
                    showPlots();
                    ui->statusbar->showMessage(" Ready");
                }
                else
                {
                    ui->statusbar->showMessage(" Directory error: /report not found");
                }
                delete diffEquation;
            }
            else
            {
                ui->statusbar->showMessage(" Input value error: h <= 0");
            }
        }
        else
        {
            ui->statusbar->showMessage(" Input value error: x_0 > x_max");
        }
    }
    else
    {
        ui->statusbar->showMessage(" Input value error: missing value");
    }
}

void MainWindow::showPlots(void)
{
    ui->plots->xAxis->setLabel("x");
    ui->plots->yAxis->setLabel("y");
    ui->plots->xAxis->setRange(diffEquation->get_x()->first(), diffEquation->get_x()->last());
    ui->plots->yAxis->setRange(diffEquation->get_y_min(), diffEquation->get_y_max());
    ui->plots->addGraph();
    ui->plots->graph(0)->setPen(QPen(Qt::blue));
    ui->plots->graph(0)->setData(*diffEquation->get_x(), *diffEquation->get_y_Cauchy_Euler());
    ui->plots->addGraph();
    ui->plots->graph(1)->setPen(QPen(Qt::red));
    ui->plots->graph(1)->setData(*diffEquation->get_x(), *diffEquation->get_y_Runge_Kutta());
    ui->plots->replot();
}

void MainWindow::printSolutions(void)
{
    QFile *file = new QFile("report/solutions.txt");
    if(file->open(QFile::ReadOnly))
    {
        ui->solutionsOutput->setText(file->readAll());
    }
    else
    {
        ui->statusbar->showMessage(" File error: /report/solutions.txt not found");
    }
    delete file;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete doubleValidator;
}
