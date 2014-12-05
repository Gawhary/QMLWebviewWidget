#ifndef QUICKWIDGET_HPP
# define QUICKWIDGET_HPP

#include <QOpenGLFramebufferObject>

#include <QQuickWindow>

#include <QScopedPointer>

#include <QWidget>

class QuickWidget
  : public QWidget
{
  Q_OBJECT

public:
  QuickWidget(QWidget* = 0);

  QuickWidget(QQuickWindow*, QWidget* = 0);

  QQuickWindow* window() const;
  void setWindow(QQuickWindow*);

protected:
  void init();

  bool event(QEvent*);

  bool eventFilter(QObject*, QEvent*);

private:
  void paintEvent(QPaintEvent*);

private:
  QQuickWindow* window_;

  QScopedPointer<QOpenGLFramebufferObject> fbo_;

private slots:
  void afterRendering();
  void beforeRendering();
};

//////////////////////////////////////////////////////////////////////////////
inline QQuickWindow* QuickWidget::window() const
{
  return window_;
}

//////////////////////////////////////////////////////////////////////////////
inline void QuickWidget::paintEvent(QPaintEvent*)
{
}

#endif // QUICKWIDGET_HPP
