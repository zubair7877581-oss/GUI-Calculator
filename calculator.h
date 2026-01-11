#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Calculator;
}
QT_END_NAMESPACE

class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:
    void digitClicked();
    void operatorClicked();
    void equalClicked();
    void clearClicked();
    void clearAllClicked();
    void plusMinusClicked();
    void decimalClicked();

private:
    Ui::Calculator *ui;
    
    QString currentValue;
    QString pendingOperator;
    double result;
    bool waitingForOperand;
    
    void calculate(double operand, const QString &operation);
    void reset();
};

#endif // CALCULATOR_H