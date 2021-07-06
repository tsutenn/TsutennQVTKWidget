#ifndef TSUTENNTASK_H
#define TSUTENNTASK_H

#include <qlist.h>

class tsutennTask
{
private:
	int type = -1;
	QList<QPointF> points;
public:
	tsutennTask(int t, QList<QPointF> l) {
		this->type = t;
		this->points = l;
	}
	int getType() {
		return this->type;
	}
	QList<QPointF> getPoints() {
		return this->points;
	}
};

#endif // !TSUTENNTASK_H
