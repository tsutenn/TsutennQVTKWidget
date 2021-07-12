#ifndef TSUTENNQVTKWIDGET_H
#define TSUTENNQVTKWIDGET_H

#include <QWidget>
#include <QVTKOpenGLNativeWidget.h>
#include <qevent.h>
#include <qpoint.h>
#include <qdebug.h>
#include <qobject.h>
//#include "ui_TsutennQVTKWidget.h"

class TsutennQVTKWidget : public QVTKOpenGLNativeWidget
{
	Q_OBJECT

public:
	TsutennQVTKWidget(QWidget* parent = Q_NULLPTR);
	~TsutennQVTKWidget();

	bool eventFilter(QObject* object, QEvent*);

private:
	//Ui::TsutennQVTKWidget ui;

	bool touchBeginEventProcess(QEvent*);
	bool touchUpdateEventProcess(QEvent*);
	bool touchEndEventProcess(QEvent*);
	bool mousePressEventProcess(QEvent*);
	bool mouseReleaseEventProcess(QEvent*);
	bool mouseDoubleClickEventProcess(QEvent*);
	bool mouseMoveEventProcess(QEvent*);
	bool processMouseEvent = false;
	QPointF startPos, endPos;
	QList<QTouchEvent::TouchPoint> touchPoints;

signals:
	void TOUCH_BEGIN(QList<QPointF>);
	void TOUCH_UPDATE(QList<QPointF>);
	void TOUCH_END();
	void MOUSE_BEGIN(QPointF);
	void MOUSE_END(QPointF, QPointF);
	void MOUSE_DOUBLE_CLICK(QPointF);
	void MOUSE_MOVE(QPointF);
};

#endif // !TSUTENNQVTKWIDGET_H

