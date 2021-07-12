#include "TsutennCallback.h"

//slot helper
slotHelper::slotHelper(TsutennQVTKWidget* parent) : QObject(parent) {
	void(TsutennQVTKWidget:: * TouchBegin) (QList<QPointF>) = &TsutennQVTKWidget::TOUCH_BEGIN;
	void(TsutennQVTKWidget:: * TouchUpdate) (QList<QPointF>) = &TsutennQVTKWidget::TOUCH_UPDATE;
	void(TsutennQVTKWidget:: * TouchEnd) (void) = &TsutennQVTKWidget::TOUCH_END;
	void(TsutennQVTKWidget:: * MouseBegin) (QPointF) = &TsutennQVTKWidget::MOUSE_BEGIN;
	void(TsutennQVTKWidget:: * MouseEnd) (QPointF, QPointF) = &TsutennQVTKWidget::MOUSE_END;
	void(TsutennQVTKWidget:: * MouseDoubleClick) (QPointF) = &TsutennQVTKWidget::MOUSE_DOUBLE_CLICK;
	void(TsutennQVTKWidget:: * MouseMove) (QPointF) = &TsutennQVTKWidget::MOUSE_MOVE;

	void(slotHelper:: * tbep) (QList<QPointF>) = &slotHelper::touchBeginEventProcessor;
	void(slotHelper:: * tuep) (QList<QPointF>) = &slotHelper::touchUpdateEventProcessor;
	void(slotHelper:: * teep) (void) = &slotHelper::toucEndEventProcessor;
	void(slotHelper:: * mbep) (QPointF) = &slotHelper::mouseBeginEventProcessor;
	void(slotHelper:: * meep) (QPointF, QPointF) = &slotHelper::mouseEndEventProcessor;
	void(slotHelper:: * mdcep) (QPointF) = &slotHelper::mouseDoubleClickEventProcessor;
	void(slotHelper:: * mmep) (QPointF) = &slotHelper::mouseMoveEventProcessor;

	QObject::connect(parent, TouchBegin, this, tbep);
	QObject::connect(parent, TouchUpdate, this, tuep);
	QObject::connect(parent, TouchEnd, this, teep);
	QObject::connect(parent, MouseBegin, this, mbep);
	QObject::connect(parent, MouseEnd, this, meep);
	QObject::connect(parent, MouseDoubleClick, this, mdcep);
	QObject::connect(parent, MouseMove, this, mmep);
}

slotHelper::~slotHelper() {
	QObject::disconnect(this);
}

void slotHelper::touchBeginEventProcessor(QList<QPointF> points) {
	this->tasks.enqueue(new tsutennTask(0, points));
}

void slotHelper::touchUpdateEventProcessor(QList<QPointF> points) {
	this->tasks.enqueue(new tsutennTask(1, points));
}

void slotHelper::toucEndEventProcessor() {
	QList<QPointF> points;
	this->tasks.enqueue(new tsutennTask(2, points));
}
void slotHelper::mouseBeginEventProcessor(QPointF startpos) {
	QList<QPointF> points;
	points.append(startpos);
	this->tasks.enqueue(new tsutennTask(3, points));
}
void slotHelper::mouseEndEventProcessor(QPointF startpos, QPointF endpos) {
	QList<QPointF> points;
	points.append(startpos);
	points.append(endpos);
	this->tasks.enqueue(new tsutennTask(4, points));
}

void slotHelper::mouseDoubleClickEventProcessor(QPointF clickpos) {
	QList<QPointF> points;
	points.append(clickpos);
	this->tasks.enqueue(new tsutennTask(5, points));
}

void slotHelper::mouseMoveEventProcessor(QPointF movepos) {
	QList<QPointF> points;
	points.append(movepos);
	this->tasks.enqueue(new tsutennTask(6, points));
}


//tsutenn callback
TsutennCallback* TsutennCallback::New(TsutennQVTKWidget* parent) {
	TsutennCallback* tc = new TsutennCallback;
	tc->callback_slot = new slotHelper(parent);
	return tc;
}

void TsutennCallback::setParent(TsutennQVTKWidget* parent) {
	this->callback_slot->setParent(parent);
}

void TsutennCallback::Execute(vtkObject* caller, unsigned long vtkNotUsed(eventId), void* vtkNotUsed(callData)) {
	while (this->callback_slot->hasTask()) {
		tsutennTask* task = this->callback_slot->getTask();
		switch (task->getType()) {
		case 0:
			qDebug() << "Touch begin";
			touchBeginExecute(task->getPoints());
			break;
		case 1:
			touchUpdateExecute(task->getPoints());
			break;
		case 2:
			qDebug() << "Touch End";
			touchEndExecute(task->getPoints());
			break;
		case 3:
			qDebug() << "Mouse press";
			mouseBeginExecute(task->getPoints());
			break;
		case 4:
			qDebug() << "Mouse release";
			mouseEndExecute(task->getPoints());
			break;
		case 5:
			qDebug() << "Mouse double click";
			mouseDoubleClickExecute(task->getPoints());
			break;
		case 6:
			mouseMoveExecute(task->getPoints());
			break;
		}
	}
}
