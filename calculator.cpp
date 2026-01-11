#include "calculator.h"
#include "ui_calculator.h"
#include <QPushButton>
#include <cmath>

Calculator::Calculator(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);
    
    // Initialize variables
    reset();
    
    // Connect digit buttons
    for (int i = 0; i <= 9; ++i) {
        QString buttonName = "button" + QString::number(i);
        QPushButton *button = findChild<QPushButton*>(buttonName);
        if (button) {
            connect(button, &QPushButton::clicked, this, &Calculator::digitClicked);
        }
    }
    
    // Connect operator buttons
    connect(ui->buttonAdd, &QPushButton::clicked, this, &Calculator::operatorClicked);
    connect(ui->buttonSubtract, &QPushButton::clicked, this, &Calculator::operatorClicked);
    connect(ui->buttonMultiply, &QPushButton::clicked, this, &Calculator::operatorClicked);
    connect(ui->buttonDivide, &QPushButton::clicked, this, &Calculator::operatorClicked);
    
    // Connect other buttons
    connect(ui->buttonEqual, &QPushButton::clicked, this, &Calculator::equalClicked);
    connect(ui->buttonClear, &QPushButton::clicked, this, &Calculator::clearClicked);
    connect(ui->buttonClearAll, &QPushButton::clicked, this, &Calculator::clearAllClicked);
    connect(ui->buttonPlusMinus, &QPushButton::clicked, this, &Calculator::plusMinusClicked);
    connect(ui->buttonDecimal, &QPushButton::clicked, this, &Calculator::decimalClicked);
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::reset()
{
    currentValue = "0";
    pendingOperator.clear();
    result = 0.0;
    waitingForOperand = true;
    ui->display->setText(currentValue);
}

void Calculator::digitClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;
    
    int digitValue = clickedButton->text().toInt();
    
    if (currentValue == "0" && digitValue == 0) return;
    
    if (waitingForOperand) {
        currentValue.clear();
        waitingForOperand = false;
    }
    
    currentValue += QString::number(digitValue);
    ui->display->setText(currentValue);
}

void Calculator::operatorClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;
    
    QString clickedOperator = clickedButton->text();
    
    if (!pendingOperator.isEmpty() && !waitingForOperand) {
        calculate(currentValue.toDouble(), clickedOperator);
    } else {
        result = currentValue.toDouble();
    }
    
    pendingOperator = clickedOperator;
    waitingForOperand = true;
}

void Calculator::equalClicked()
{
    if (!pendingOperator.isEmpty() && !waitingForOperand) {
        calculate(currentValue.toDouble(), pendingOperator);
        pendingOperator.clear();
        waitingForOperand = true;
    }
}

void Calculator::calculate(double operand, const QString &operation)
{
    if (operation == "+") {
        result += operand;
    } else if (operation == "-") {
        result -= operand;
    } else if (operation == "×") {
        result *= operand;
    } else if (operation == "÷") {
        if (operand == 0.0) {
            ui->display->setText("Error");
            reset();
            return;
        }
        result /= operand;
    }
    
    currentValue = QString::number(result, 'g', 15);
    ui->display->setText(currentValue);
}

void Calculator::clearClicked()
{
    currentValue = "0";
    ui->display->setText(currentValue);
    waitingForOperand = true;
}

void Calculator::clearAllClicked()
{
    reset();
}

void Calculator::plusMinusClicked()
{
    double value = currentValue.toDouble();
    value = -value;
    currentValue = QString::number(value, 'g', 15);
    ui->display->setText(currentValue);
}

void Calculator::decimalClicked()
{
    if (waitingForOperand) {
        currentValue = "0";
        waitingForOperand = false;
    }
    
    if (!currentValue.contains('.')) {
        currentValue += '.';
        ui->display->setText(currentValue);
    }
}