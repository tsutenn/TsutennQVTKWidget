#pragma once

#include <QtWidgets/QMainWindow>
#include <qlist.h>
#include <qpoint.h>
#include "TsutennCallback.h"
#include "ui_Example.h"

class Example : public QMainWindow, public TsutennCallback
{
    Q_OBJECT

public:
    Example(QWidget *parent = Q_NULLPTR);

private:
    Ui::ExampleClass ui;
    
public slots:
    void exampleSlot(QList<QPointF>);
    void exampleSlot2(QPointF);
};
