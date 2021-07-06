#include "Example.h"

#include <QtWidgets/QApplication>
#include <qdebug.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkLight.h>
#include <vtkCamera.h>
#include <vtkCallbackCommand.h>
#include <QVTKInteractor.h>
#include "TsutennCallback.h"

vtkSmartPointer<vtkCylinderSource> cylinder;
vtkSmartPointer<vtkLight> l0;
vtkSmartPointer<vtkCamera> camera;
vtkSmartPointer<vtkPolyDataMapper> cylinderMapper;
vtkSmartPointer<vtkActor> cylinderActor;
vtkSmartPointer<vtkRenderer> renderer;
vtkSmartPointer<vtkGenericOpenGLRenderWindow> renWin;
QVTKInteractor* iren;

void initVTK() {
    cylinder = vtkSmartPointer<vtkCylinderSource>::New();
    cylinder->SetHeight(3.0);
    cylinder->SetRadius(1.0);
    cylinder->SetResolution(10);

    cylinderMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    cylinderActor = vtkSmartPointer<vtkActor>::New();
    cylinderActor->SetMapper(cylinderMapper);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(cylinderActor);

    l0 = vtkSmartPointer<vtkLight>::New();
    l0->SetLightTypeToHeadlight();
    l0->SetColor(0.5, 0.5, 0.5);
    renderer->AddLight(l0);

    /*
    camera = vtkSmartPointer<vtkCamera>::New();
    camera->SetClippingRange(6, -6);
    camera->SetFocalPoint(0, 0, 0);
    camera->SetPosition(0, 6, 0);
    camera->ComputeViewPlaneNormal();
    camera->SetViewUp(1, 0, 0);
    camera->SetViewAngle(45);
    renderer->SetActiveCamera(camera);
    */

    renWin = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renWin->AddRenderer(renderer);
}

Example::Example(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    initVTK();

    ui.widget->setRenderWindow(renWin);

    //Set callback function
    iren = QVTKInteractor::New();
    vtkSmartPointer<TsutennCallback> irenRefresh = TsutennCallback::New(ui.widget);
    iren->CreateRepeatingTimer(1);
    iren->AddObserver(vtkCommand::TimerEvent, irenRefresh);
    iren->SetRenderWindow(renWin);

    //Initialize table widget
    ui.tableWidget->setColumnCount(3);
    ui.tableWidget->setRowCount(10);
    QStringList headers;
    headers << "ID" << "X" << "Y";
    ui.tableWidget->setHorizontalHeaderLabels(headers);
    ui.tableWidget->setColumnWidth(0, 80);
    ui.tableWidget->setColumnWidth(1, 120);
    ui.tableWidget->setColumnWidth(2, 120);
    for (int i = 0; i < 10; i++) {
        ui.tableWidget->setItem(i, 0, new QTableWidgetItem(" "));
        ui.tableWidget->setItem(i, 1, new QTableWidgetItem(" "));
        ui.tableWidget->setItem(i, 2, new QTableWidgetItem(" "));
    }

    //Let table widget show the touch information in TsutennQVTKWidget
    void(TsutennQVTKWidget:: * TouchSignal) (QList<QPointF>) = &TsutennQVTKWidget::TOUCH_UPDATE;
    void(Example:: * ThisSolt) (QList<QPointF>) = &Example::exampleSolt;
    QObject::connect(ui.widget, TouchSignal, this, ThisSolt);
    QObject::connect(ui.widget, &TsutennQVTKWidget::TOUCH_END, [this] {
        ui.label->setText("None");
        });
}

void Example::exampleSolt(QList<QPointF> points) {
    for (int i = 0; i < points.count() && i < 10; i++) {
        ui.tableWidget->item(i, 0)->setText(QString::number(i));
        ui.tableWidget->item(i, 1)->setText(QString::number(points.at(i).x()));
        ui.tableWidget->item(i, 2)->setText(QString::number(points.at(i).y()));
    }
    for (int i = points.count(); i < 10; i++) {
        ui.tableWidget->item(i, 0)->setText(" ");
        ui.tableWidget->item(i, 1)->setText(" ");
        ui.tableWidget->item(i, 2)->setText(" ");
    }
    ui.tableWidget->viewport()->update();
    ui.label->setText(QString::number(points.count()));
}

/*
 * Implement of the execute functions in TsutenCallback 
 */
void TsutennCallback::touchBeginExecute(QList<QPointF> points) {

}

void TsutennCallback::touchUpdateExecute(QList<QPointF> points) {
    if (points.count() == 1)
        l0->SetColor(0.5, 1.0, 0.5);
    else if (points.count() == 2)
        l0->SetColor(1.0, 1.0, 0.5);
    else
        l0->SetColor(1.0, 0.5, 0.5);
    renWin->Render();
}

void TsutennCallback::touchEndExecute(QList<QPointF> points) {
    l0->SetColor(0.5, 0.5, 0.5);
    renWin->Render();
}

void TsutennCallback::mouseBeginExecute(QList<QPointF> points) {
    l0->SetColor(1.0, 1.0, 1.0);
    renWin->Render();
}

void TsutennCallback::mouseEndExecute(QList<QPointF> points) {
    l0->SetColor(0.5, 0.5, 0.5);
    renWin->Render();
}

void TsutennCallback::mouseDoubleClickExecute(QList<QPointF> points) {
    qDebug() << points.at(0);
    renWin->Render();
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    Example w;
    w.show();
    renWin->Render();
    return a.exec();
}
