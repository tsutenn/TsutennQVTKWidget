#include "TsutennQVTKWidget.h"

TsutennQVTKWidget::TsutennQVTKWidget(QWidget *parent)
	: QVTKOpenGLNativeWidget(parent)
{
	//ui.setupUi(this);

	this->setAttribute(Qt::WA_AcceptTouchEvents);
	this->installEventFilter(this);
}

TsutennQVTKWidget::~TsutennQVTKWidget()
{
	this->removeEventFilter(this);
}

bool TsutennQVTKWidget::eventFilter(QObject* object, QEvent* event) {

	if (event->type() == QEvent::TouchBegin) {
		return touchBeginEventProcess(event);
	}
	else if (event->type() == QEvent::TouchUpdate) {
		return touchUpdateEventProcess(event);
	}
	else if (event->type() == QEvent::MouseButtonPress) {
		return mousePressEventProcess(event);
	}
	else if (event->type() == QEvent::MouseButtonRelease) {
		return mouseReleaseEventProcess(event);
	}
	else if (event->type() == QEvent::TouchEnd) {
		return touchEndEventProcess(event);
	}
	else if (event->type() == QEvent::MouseButtonDblClick) {
		return mouseDoubleClickEventProcess(event);
	}
	return false;

}

bool TsutennQVTKWidget::touchBeginEventProcess(QEvent* event) {
	QTouchEvent* touchEvent = static_cast<QTouchEvent*>(event);
	touchPoints = touchEvent->touchPoints();

	QList<QPointF> points;
	for (int i = 0; i < touchPoints.count(); i++) {
		points.append(touchPoints.at(i).pos());
	}

	TOUCH_BEGIN(points);
	return true;
}

bool TsutennQVTKWidget::touchUpdateEventProcess(QEvent* event) {
	QTouchEvent* touchEvent = static_cast<QTouchEvent*>(event);
	touchPoints = touchEvent->touchPoints();

	QList<QPointF> points;
	for (int i = 0; i < touchPoints.count(); i++) {
		points.append(touchPoints.at(i).pos());
	}

	TOUCH_UPDATE(points);
	return true;
}

bool TsutennQVTKWidget::touchEndEventProcess(QEvent* event) {
	touchPoints.clear();
	TOUCH_END();
	return true;
}

bool TsutennQVTKWidget::mousePressEventProcess(QEvent* event) {
	if (touchPoints.isEmpty()) {
		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		startPos = mouseEvent->pos();

		MOUSE_BEGIN(startPos);
		processMouseEvent = true;
	}
	return true;
}

bool TsutennQVTKWidget::mouseReleaseEventProcess(QEvent* event) {
	if (processMouseEvent) {
		processMouseEvent = false;

		QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
		endPos = mouseEvent->pos();
		//qDebug() << startPos << ", " << endPos;

		MOUSE_END(startPos, endPos);
	}
	return true;
}

bool TsutennQVTKWidget::mouseDoubleClickEventProcess(QEvent* event) {
	QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
	QPointF Pos = mouseEvent->pos();
	MOUSE_DOUBLE_CLICK(Pos);
	return true;
}