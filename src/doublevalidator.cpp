#include "doublevalidator.h"

DoubleValidator::DoubleValidator(double bottom, double top, QObject *parent) : QValidator(parent)
{
    this->bottom = bottom;
    this->top = top;
}

 QValidator::State DoubleValidator::validate(QString &input, int &pos) const
{
    for(auto it = input.begin(); it != input.end(); it++)
    {
        if(!((*it >= "0" && *it <= "9") || *it == "-" || *it == "."))
        {
            return QValidator::Invalid;
        }
    }
    if(input.isEmpty() || input == "-")
    {
        return QValidator::Intermediate;
    }
    else
    {
        if(input.length() >= 2 && input.at(0) == "0" && !(input.at(1) == "."))
        {
            return QValidator::Invalid;
        }
        else
        {
            if(input.length() >= 3 && input.at(0) == "-" && input.at(1) == "0" && !(input.at(2) == "."))
            {
                return QValidator::Invalid;
            }
        }
    }
    bool ok;
    double num = input.toDouble(&ok);
    if(ok && (num >= bottom) && (num <= top))
    {
        return  QValidator::Acceptable;
    }
    else
    {
        return  QValidator::Invalid;
    }
}
