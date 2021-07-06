# TsutennWidget

一个Qt5下支持多点触控的小部件.

`TsutennWidget`：用于显示VTK实例，继承于`QVTKOpenGLNativeWidget`。实现了对触摸屏多点触控的处理。

`TsutennCallback`：处理`TsutennWidget`的事件。添加至`QVTKInteractor`。**需要实现成员函数：**

``` c++
void touchBeginExecute(QList<QPointF>);
void touchUpdateExecute(QList<QPointF>);
void touchEndExecute(QList<QPointF>);
void mouseBeginExecute(QList<QPointF>);
void mouseEndExecute(QList<QPointF>);
void mouseDoubleClickExecute(QList<QPointF>);
```
