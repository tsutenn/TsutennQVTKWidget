#ifndef TSUTENNCALLBACK_H
#define TSUTENNCALLBACK_H

#include <vtkCommand.h>
#include "TsutennQVTKWidget.h"
#include "slotHelper.h"
#include "tsutennTask.h"
class TsutennCallback :
    public vtkCommand
{
public:
    static TsutennCallback* New(TsutennQVTKWidget* parent = Q_NULLPTR);
    vtkTypeMacro(TsutennCallback, vtkCommand);

    void setParent(TsutennQVTKWidget* parent);

    void Execute(vtkObject* caller, unsigned long vtkNotUsed(eventId), void* vtkNotUsed(callData));

private:
    slotHelper* callback_slot;

    /*
     * Implement these methods!
     */
    void touchBeginExecute(QList<QPointF>);
    void touchUpdateExecute(QList<QPointF>);
    void touchEndExecute(QList<QPointF>);
    void mouseBeginExecute(QList<QPointF>);
    void mouseEndExecute(QList<QPointF>);
    void mouseDoubleClickExecute(QList<QPointF>);
};

#endif // !TSUTENNCALLBACK_H

