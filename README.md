# TsutennQVTKWidget

一个Qt5下支持多点触控的小部件。（Qt-5.15.2, VTK-9.0.1, Visual Studio 2019）

### TsutennQVTKWidget.h：

用于显示VTK实例，继承 `QVTKOpenGLNativeWidget.h` 。实现了对触摸屏多点触控的处理。可独立使用。

信号：

``` c++
signals:
	void TOUCH_BEGIN(QList<QPointF>);
	void TOUCH_UPDATE(QList<QPointF>);
	void TOUCH_END();
	void MOUSE_BEGIN(QPointF);
	void MOUSE_END(QPointF, QPointF);
	void MOUSE_DOUBLE_CLICK(QPointF);
	void MOUSE_MOVE(QPointF);
```



### **TsutennCallback.h**：

处理 `TsutennQVTKWidget` 事件的回调函数，继承 `vtkCommand.h` ，依赖 `tsutennTask.h `，`slotHelper.h` 和 `TsutennQVTKwidget.h` 。在使用时Interactor绑定的Style会部分失效。

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

触摸开始：

```c++
virtual void touchBeginExecute(QList<QPointF>);
```

触摸更新：

``` c++
virtual void touchUpdateExecute(QList<QPointF>);
```

触摸结束：

```c++
virtual void touchEndExecute(QList<QPointF>);
```

鼠标按下按键：

```c++
virtual void mouseBeginExecute(QList<QPointF>);
```

鼠标在按下按键时移动

```c++
virtual void mouseMoveExecute(QList<QPointF>);
```

鼠标松开按键：

```c++
virtual void mouseEndExecute(QList<QPointF>);
```

鼠标双击：

```c++
virtual void mouseDoubleClickExecute(QList<QPointF>);
```

以上即使没有使用也需要实现，否则会报错。



### **Example.h**：

一个例子程序。

