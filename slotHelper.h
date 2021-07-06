#ifndef SLOTHELPER_H
#define SLOTHELPER_H

#include <qobject.h>
#include <qqueue.h>
#include "tsutennTask.h"
#include "TsutennQVTKWidget.h"
class slotHelper :
    public QObject
{
	Q_OBJECT

private:
    QQueue<tsutennTask*> tasks;
	QPointF touchPos;
	QPointF mouseStartPos;
	QPointF mouseEndPos;

public:
	slotHelper(TsutennQVTKWidget* parent = Q_NULLPTR);
	~slotHelper();
	bool hasTask() {
		return !this->tasks.isEmpty();
	}
	tsutennTask* getTask() {
		return this->tasks.dequeue();
	}

public slots:
	void touchBeginEventProcessor(QList<QPointF>);
	void touchUpdateEventProcessor(QList<QPointF>);
	void toucEndEventProcessor();
	void mouseBeginEventProcessor(QPointF);
	void mouseEndEventProcessor(QPointF, QPointF);
	void mouseDoubleClickEventProcessor(QPointF);
};

#endif // !SLOTHELPER_H
