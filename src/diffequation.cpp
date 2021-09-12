#include "diffequation.h"

DiffEquation::DiffEquation(const QString &mathExpression, double x_0, double y_0, double x_max, double h)
{
    this->mathExpression = new MathExpression(mathExpression);
    this->x_0 = x_0;
    this->y_0 = y_0;
    this->x_max = x_max;
    this->h = h;
    x = new QVector<double>;
    y_Runge_Kutta = new QVector<double>;
    y_Cauchy_Euler = new QVector<double>;
    report = new QFile;
    y_max = y_0;
    y_min = y_0;
}

bool DiffEquation::calculateSolutions(const QString &reportFileName)
{
    if(openFile(reportFileName))
    {
        x->append(x_0);
        y_Runge_Kutta->append(y_0);
        y_Cauchy_Euler->append(y_0);
        writeValues(x_0, y_0, y_0);
        long long cnt = static_cast<long long>((x_max - x_0) / h);
        double k_1, k_2, k_3, delta_y, x_n = x_0, y_n_Runge_Kutta = y_0, y_n_Cauchy_Euler = y_0;
        while(cnt != 0)
        {
            k_1 = h * mathExpression->calculate(x_n, y_n_Runge_Kutta);
            k_2 = h * mathExpression->calculate(x_n + h / 2, y_n_Runge_Kutta + k_1 / 2);
            k_3 = h * mathExpression->calculate(x_n + h, y_n_Runge_Kutta - k_1 + 2 * k_2);
            delta_y = (k_1 + 4 * k_2 + k_3) / 6;
            y_n_Runge_Kutta = y_n_Runge_Kutta + delta_y;
            if(y_n_Runge_Kutta > y_max)
            {
                y_max = y_n_Runge_Kutta;
            }
            else
            {
                if(y_n_Runge_Kutta < y_min)
                {
                    y_min = y_n_Runge_Kutta;
                }
            }
            delta_y = h / 2 * (mathExpression->calculate(x_n, y_n_Cauchy_Euler)
                               + mathExpression->calculate(x_n + h, y_n_Cauchy_Euler
                               + h * mathExpression->calculate(x_n, y_n_Cauchy_Euler)));
            y_n_Cauchy_Euler = y_n_Cauchy_Euler + delta_y;
            if(y_n_Cauchy_Euler > y_max)
            {
                y_max = y_n_Cauchy_Euler;
            }
            else
            {
                if(y_n_Cauchy_Euler < y_min)
                {
                    y_min = y_n_Cauchy_Euler;
                }
            }
            x_n = x_n + h;
            x->append(x_n);
            y_Runge_Kutta->append(y_n_Runge_Kutta);
            y_Cauchy_Euler->append(y_n_Cauchy_Euler);
            writeValues(x_n, y_n_Runge_Kutta, y_n_Cauchy_Euler);
            cnt--;
        }
        closeFile();
        return true;
    }
    return false;
}

bool DiffEquation::openFile(const QString &reportFileName)
{
    report->setFileName(reportFileName);
    if(report->open(QFile::WriteOnly))
    {
        fout.setDevice(report);
        fout << "+------------+------------+------------+------------+" << endl;
        fout << "|           X| Runge-Kutta|Cauchy-Euler|     Delta Y|" << endl;
        fout << "+------------+------------+------------+------------+" << endl;
        fout.setFieldAlignment(QTextStream::AlignRight);
        return true;
    }
    return false;
}

void DiffEquation::writeValues(double x_n, double y_n_Runge_Kutta, double y_n_Cauchy_Euler)
{
    fout << "|" << qSetFieldWidth(12) << x_n << qSetFieldWidth(0) << "|" << qSetFieldWidth(12) << y_n_Runge_Kutta
         << qSetFieldWidth(0) << "|" << qSetFieldWidth(12) << y_n_Cauchy_Euler
         << qSetFieldWidth(0) << "|" << qSetFieldWidth(12) << fabs(y_n_Runge_Kutta - y_n_Cauchy_Euler)
         << qSetFieldWidth(0) << "|" << endl;
}

void DiffEquation::closeFile(void)
{
    fout << "+------------+------------+------------+------------+" << endl;
    report->flush();
    report->close();
}

double DiffEquation::get_y_max(void)
{
    return y_max;
}

double DiffEquation::get_y_min(void)
{
    return y_min;
}

QVector<double> *DiffEquation::get_x(void)
{
    return x;
}

QVector<double> *DiffEquation::get_y_Runge_Kutta(void)
{
    return y_Runge_Kutta;
}

QVector<double> *DiffEquation::get_y_Cauchy_Euler(void)
{
    return y_Cauchy_Euler;
}

DiffEquation::~DiffEquation()
{
    delete mathExpression;
    delete y_Runge_Kutta;
    delete y_Cauchy_Euler;
    delete report;
    delete x;
}
