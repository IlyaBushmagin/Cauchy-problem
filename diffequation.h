#ifndef DIFFEQUATION_H
#define DIFFEQUATION_H

#include <QTextStream>
#include <QString>
#include <QVector>
#include <QFile>

#include "mathexpression.h"

class DiffEquation
{
public:
    DiffEquation(const QString &mathExpression, double x_0, double y_0, double x_max, double h);

    bool calculateSolutions(const QString &reportFileName);

    double get_y_max(void);

    double get_y_min(void);

    QVector<double> *get_x(void);

    QVector<double> *get_y_Runge_Kutta(void);

    QVector<double> *get_y_Cauchy_Euler(void);

    ~DiffEquation();

private:
    bool openFile(const QString &reportFileName);

    void writeValues(double x_n, double y_n_Runge_Kutta, double y_n_Cauchy_Euler);

    void closeFile(void);

    QFile *report;

    QTextStream fout;

    MathExpression *mathExpression;

    double x_0, y_0, x_max, y_max, y_min, h;

    QVector<double> *x, *y_Runge_Kutta, *y_Cauchy_Euler;
};

#endif // DIFFEQUATION_H
