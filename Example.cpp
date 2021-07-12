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
#include <vtkCameraActor.h>
#include <math.h>

#define PI 3.1415926

vtkSmartPointer<vtkCylinderSource> cylinder;
vtkSmartPointer<vtkLight> l0;
vtkSmartPointer<vtkCamera> camera;
vtkSmartPointer<vtkPolyDataMapper> cylinderMapper;
vtkSmartPointer<vtkActor> cylinderActor;
vtkSmartPointer<vtkRenderer> renderer;
vtkSmartPointer<vtkGenericOpenGLRenderWindow> renWin;
QVTKInteractor* iren;
QList<QPointF> predecessor;
qreal zoomPara;

qreal widgetWidth = 1521.0;
qreal widgetHeight = 1021.0;
qreal cameraAngle = 30.000;
qreal cameraPos[3] = { 8, 0, 0 };
qreal focalPos[3] = { 0, 0, 0 };


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
    l0->SetColor(0.7, 0.7, 0.7);
    renderer->AddLight(l0);

    camera = vtkSmartPointer<vtkCamera>::New();
    camera->SetPosition(cameraPos[0], cameraPos[1], cameraPos[2]);
    camera->SetFocalPoint(focalPos[0], focalPos[1], focalPos[2]);
    camera->SetViewUp(0, 1, 0);
    camera->SetViewAngle(cameraAngle);
    renderer->SetActiveCamera(camera);

    renWin = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renWin->AddRenderer(renderer);
}

QPointF getMidPoint(QList<QPointF> list) {
    qreal midX = 0, midY = 0;
    for (int i = 0; i < list.count(); i++) {
        midX += list.at(i).x();
        midY += list.at(i).y();
    }
    return QPointF(midX / list.count(), midY / list.count());
}

qreal getZoomPara(QList<QPointF> list) {
    qreal para = 0;
    QPointF mid = getMidPoint(list);
    for (int i = 0; i < list.count(); i++) {
        para += sqrt(pow(list.at(i).x() - mid.x(), 2.0) + pow(list.at(i).y() - mid.y(), 2.0));
    }
    return para / list.count();
}

Example::Example(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    initVTK();
    //Initialize table widget
    ui.tableWidget->setColumnCount(3);
    ui.tableWidget->setRowCount(11);
    QStringList headers;
    headers << "ID" << "X" << "Y";
    ui.tableWidget->setHorizontalHeaderLabels(headers);
    ui.tableWidget->setColumnWidth(0, 100);
    ui.tableWidget->setColumnWidth(1, 100);
    ui.tableWidget->setColumnWidth(2, 100);
    ui.tableWidget->setItem(0, 0, new QTableWidgetItem("mouse"));
    ui.tableWidget->setItem(0, 1, new QTableWidgetItem(" "));
    ui.tableWidget->setItem(0, 2, new QTableWidgetItem(" "));
    for (int i = 1; i < 11; i++) {
        ui.tableWidget->setItem(i, 0, new QTableWidgetItem(" "));
        ui.tableWidget->setItem(i, 1, new QTableWidgetItem(" "));
        ui.tableWidget->setItem(i, 2, new QTableWidgetItem(" "));
    }

    ui.widget->setRenderWindow(renWin);

    //Set callback function
    iren = QVTKInteractor::New();
    vtkSmartPointer<TsutennCallback> irenRefresh = TsutennCallback::New(ui.widget);
    iren->CreateRepeatingTimer(1);
    iren->AddObserver(vtkCommand::TimerEvent, irenRefresh);
    iren->SetRenderWindow(renWin);

    //Let table widget show the touch information in TsutennQVTKWidget
    void(TsutennQVTKWidget:: * signal1) (QList<QPointF>) = &TsutennQVTKWidget::TOUCH_UPDATE;
    void(Example:: * slot1) (QList<QPointF>) = &Example::exampleSlot;
    QObject::connect(ui.widget, signal1, this, slot1);

    void(TsutennQVTKWidget:: * signal2) (QPointF) = &TsutennQVTKWidget::MOUSE_MOVE;
    void(Example:: * slot2) (QPointF) = &Example::exampleSlot2;
    QObject::connect(ui.widget, signal2, this, slot2);

    QObject::connect(ui.widget, &TsutennQVTKWidget::TOUCH_END, [this] {
        //ui.label->setText("zoomPara");
        });
}

void Example::exampleSlot(QList<QPointF> points) {
    for (int i = 0; i < points.count() && i < 10; i++) {
        ui.tableWidget->item(i+1, 0)->setText(QString::number(i+1));
        ui.tableWidget->item(i+1, 1)->setText(QString::number(points.at(i).x()));
        ui.tableWidget->item(i+1, 2)->setText(QString::number(points.at(i).y()));
    }
    for (int i = points.count(); i < 10; i++) {
        ui.tableWidget->item(i+1, 0)->setText(" ");
        ui.tableWidget->item(i+1, 1)->setText(" ");
        ui.tableWidget->item(i+1, 2)->setText(" ");
    }
    ui.tableWidget->viewport()->update();
    ui.label->setText(QString::number(zoomPara));
}

void Example::exampleSlot2(QPointF point) {
    ui.tableWidget->item(0, 1)->setText(QString::number(point.x()));
    ui.tableWidget->item(0, 2)->setText(QString::number(point.y()));
}

/*
 * Implement of the execute functions in TsutenCallback 
 */
void TsutennCallback::touchBeginExecute(QList<QPointF> points) {
    predecessor = points;
    zoomPara = getZoomPara(predecessor);
}

void TsutennCallback::touchUpdateExecute(QList<QPointF> points) {
    if (points.count() == 1) {
        l0->SetColor(1.0, 0.0, 0.0);

        if (predecessor.count() == points.count()) {
            qreal dx = (focalPos[0] - cameraPos[0]) *
                (tan((predecessor.at(0).x() - widgetWidth / 2.0) / widgetHeight * cameraAngle * PI / 180.0)
                    - tan((points.at(0).x() - widgetWidth / 2.0) / widgetHeight * cameraAngle * PI / 180.0)
                    );
            qreal dy = (focalPos[0] - cameraPos[0]) *
                (tan((predecessor.at(0).y() - widgetHeight / 2.0) / widgetHeight * cameraAngle * PI / 180.0)
                    - tan((points.at(0).y() - widgetHeight / 2.0) / widgetHeight * cameraAngle * PI / 180.0)
                    );
            cameraPos[1] += dy;
            cameraPos[2] += dx;
            focalPos[1] += dy;
            focalPos[2] += dx;
        }
    }
    else if (points.count() >= 2) {
        if (predecessor.count() == points.count()) {
            qreal cameraAngle0 = cameraAngle;
            cameraAngle *= zoomPara / getZoomPara(points);
            qreal dx = (focalPos[0] - cameraPos[0]) *
                (tan((getMidPoint(predecessor).x() - widgetWidth / 2.0) / widgetHeight * cameraAngle * PI / 180.0)
                    - tan((getMidPoint(points).x() - widgetWidth / 2.0) / widgetHeight * cameraAngle * PI / 180.0)
                    //- tan((getMidPoint(points).x() - widgetWidth / 2.0) / widgetWidth * cameraAngle * PI / 180.0)
                    //+ tan((getMidPoint(points).x() - widgetWidth / 2.0) / widgetWidth * cameraAngle0 * PI / 180.0)
                    );
            qreal dy = (focalPos[0] - cameraPos[0]) *
                (tan((getMidPoint(predecessor).y() - widgetHeight / 2.0) / widgetHeight * cameraAngle * PI / 180.0)
                    - tan((getMidPoint(points).y() - widgetHeight / 2.0) / widgetHeight * cameraAngle * PI / 180.0)
                    //- tan((getMidPoint(points).y() - widgetHeight / 2.0) / widgetWidth * cameraAngle * PI / 180.0)
                    //+ tan((getMidPoint(points).y() - widgetHeight / 2.0) / widgetWidth * cameraAngle0 * PI / 180.0)
                    );
            cameraPos[1] += dy;
            cameraPos[2] += dx;
            focalPos[1] += dy;
            focalPos[2] += dx;
        }
    }
    predecessor = points;
    zoomPara = getZoomPara(predecessor);
    camera->SetPosition(cameraPos[0], cameraPos[1], cameraPos[2]);
    camera->SetFocalPoint(focalPos[0], focalPos[1], focalPos[2]);
    camera->SetViewAngle(cameraAngle);
    renWin->Render();
}

void TsutennCallback::touchEndExecute(QList<QPointF> points) {
    l0->SetColor(0.7, 0.7, 0.7);
    predecessor = *(new QList<QPointF>);
    renWin->Render();
}

void TsutennCallback::mouseBeginExecute(QList<QPointF> points) {
    l0->SetColor(1.0, 1.0, 1.0);
    renWin->Render();
}

void TsutennCallback::mouseEndExecute(QList<QPointF> points) {
    l0->SetColor(0.7, 0.7, 0.7);
    renWin->Render();
}

void TsutennCallback::mouseDoubleClickExecute(QList<QPointF> points) {
    qDebug() << points.at(0);
    renWin->Render();
}

void TsutennCallback::mouseMoveExecute(QList<QPointF> points) {
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
