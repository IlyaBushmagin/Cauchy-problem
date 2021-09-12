#ifndef DOUBLEVALIDATOR_H
#define DOUBLEVALIDATOR_H

#include <QValidator>

class DoubleValidator : public QValidator
{
public:
    DoubleValidator(double bottom, double top, QObject *parent);

    QValidator::State validate(QString &input, int &pos) const;

private:
    double bottom, top;
};

#endif // DOUBLEVALIDATOR_H
