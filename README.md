# TsutennQVTKWidget

一个Qt5下支持多点触控的小部件.

**TsutennQVTKWidget.h**：用于显示VTK实例，继承 `QVTKOpenGLNativeWidget.h` 。实现了对触摸屏多点触控的处理。可独立使用。

信号：

``` c++
signals:
	void TOUCH_BEGIN(QList<QPointF>);
	void TOUCH_UPDATE(QList<QPointF>);
	void TOUCH_END();
	void MOUSE_BEGIN(QPointF);
	void MOUSE_END(QPointF, QPointF);
	void MOUSE_DOUBLE_CLICK(QPointF);
```



**TsutennCallback.h**：处理 `TsutennQVTKWidget` 事件的回调函数，继于 `vtkCommand.h` 依赖 `tsutennTask.h `，`slotHelper.h` 和 `TsutennQVTKwidget.h` 。在使用时Interactor绑定的Style会部分失效。

**设置作用的 TsutennQVTKWidget**

声明时：

``` c++
static TsutennCallback* TsutennCallback::New(ui.widget)
```

声明后：

```c++
void setParent(TsutennQVTKWidget* parent);
```

**需要实现以下成员函数：**

``` c++
void touchBeginExecute(QList<QPointF>);
void touchUpdateExecute(QList<QPointF>);
void touchEndExecute(QList<QPointF>);
void mouseBeginExecute(QList<QPointF>);
void mouseEndExecute(QList<QPointF>);
void mouseDoubleClickExecute(QList<QPointF>);
```



**Example.h**：一个例子程序。

