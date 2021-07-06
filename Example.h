#pragma once

#include <QtWidgets/QMainWindow>
#include <qlist.h>
#include <qpoint.h>
#include "ui_Example.h"

class Example : public QMainWindow
{
    Q_OBJECT

public:
    Example(QWidget *parent = Q_NULLPTR);

private:
    Ui::ExampleClass ui;

public slots:
    void exampleSolt(QList<QPointF>);
};
