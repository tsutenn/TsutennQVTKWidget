#ifndef TSUTENNCALLBACK_H
#define TSUTENNCALLBACK_H

#include <vtkCommand.h>
#include <vtkRenderWindowInteractor.h>
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
    virtual void touchBeginExecute(QList<QPointF>);
    virtual void touchUpdateExecute(QList<QPointF>);
    virtual void touchEndExecute(QList<QPointF>);
    virtual void mouseBeginExecute(QList<QPointF>);
    virtual void mouseEndExecute(QList<QPointF>);
    virtual void mouseDoubleClickExecute(QList<QPointF>);
    virtual void mouseMoveExecute(QList<QPointF>);
};

#endif // !TSUTENNCALLBACK_H

