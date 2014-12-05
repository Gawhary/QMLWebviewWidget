#include <QDebug>

#include <QBackingStore>

#include <QCoreApplication>

#include <QPainter>

#include "quickwidget.hpp"

//////////////////////////////////////////////////////////////////////////////
QuickWidget::QuickWidget(QWidget* parent)
  : QWidget(parent, Qt::FramelessWindowHint)
{
  init();
}

//////////////////////////////////////////////////////////////////////////////
QuickWidget::QuickWidget(QQuickWindow* window, QWidget* parent)
  : QWidget(parent, Qt::FramelessWindowHint)
{
  init();

  setWindow(window);
}

//////////////////////////////////////////////////////////////////////////////
void QuickWidget::setWindow(QQuickWindow* window)
{
  if (window_)
  {
    window_->removeEventFilter(this);

    disconnect(window_, 0, this, 0);
  }
  // else do nothing

  if ((window_ = window))
  {
    Q_ASSERT(windowHandle());

    window->setParent(windowHandle());

    window->show();

    window->setOpacity(.0);

    if (window->size().isEmpty())
    {
      window->installEventFilter(this);
    }
    else
    {
      resize(window->size());
    }

    window->setPosition(width(), height());

    connect(window, SIGNAL(afterRendering()), SLOT(afterRendering()),
      Qt::DirectConnection);
    connect(window, SIGNAL(beforeRendering()), SLOT(beforeRendering()),
      Qt::DirectConnection);
  }
  // else do nothing
}

//////////////////////////////////////////////////////////////////////////////
void QuickWidget::init()
{
  window_ = 0;

  setAttribute(Qt::WA_TranslucentBackground);

  setMouseTracking(true);

  winId();
}

//////////////////////////////////////////////////////////////////////////////
bool QuickWidget::event(QEvent* event)
{
  if (window_)
  {
    switch (event->type())
    {
      case QEvent::FocusIn:
      case QEvent::FocusOut:
      case QEvent::FocusAboutToChange:

      case QEvent::KeyPress:
      case QEvent::KeyRelease:

      case QEvent::Wheel:
        QCoreApplication::sendEvent(window_, event);
        return true;

      case QEvent::MouseButtonPress:
      case QEvent::MouseButtonRelease:
      case QEvent::MouseButtonDblClick:
      case QEvent::MouseMove:
      {
        QPointF localPos(static_cast<QMouseEvent*>(event)->localPos());

        localPos.setY(height() - localPos.y());

        QPointF windowPos(static_cast<QMouseEvent*>(event)->windowPos());

        windowPos.setY(height() - windowPos.y());

        QMouseEvent mouseEvent(event->type(), localPos, windowPos,
          static_cast<QMouseEvent*>(event)->button(),
          static_cast<QMouseEvent*>(event)->buttons(),
          static_cast<QMouseEvent*>(event)->modifiers());

        QCoreApplication::sendEvent(window_, &mouseEvent);
        return true;
      }

      case QEvent::Resize:
        if (window_)
        {
          QSize size(static_cast<QResizeEvent*>(event)->size());

          window_->setGeometry(size.width(), size.height(),
            size.width(), size.height());
        }
        // else do nothing
        break;

      case QEvent::Show:
        if (window_)
        {
          window_->show();
        }
        // else do nothing

      default:
        break;
    }
  }

  return QWidget::event(event);
}

//////////////////////////////////////////////////////////////////////////////
bool QuickWidget::eventFilter(QObject*, QEvent* event)
{
  Q_ASSERT(window_);

  switch (event->type())
  {
    case QEvent::Resize:
    {
      QSize size(static_cast<QResizeEvent*>(event)->size());

      resize(size);

      window_->setPosition(size.width(), size.height());

      window_->removeEventFilter(this);
      break;
    }

    default:
      break;
  }

  return false;
}

//////////////////////////////////////////////////////////////////////////////
void QuickWidget::afterRendering()
{
  if (window_ && windowHandle()->isExposed())
  {
#   ifdef QT_OPENGL_ES
    static GLint const fmt(GL_BGRA_EXT);
#   else
    static GLint const fmt(GL_BGRA);
#   endif

    //fbo_->toImage().save("test.png");

    fbo_->bind();

    glReadPixels(0, 0, width(), height(), fmt, GL_UNSIGNED_BYTE,
      static_cast<QImage*>(backingStore()->paintDevice())->bits());

/*
    glBindTexture(GL_TEXTURE_2D, fbo_->texture());

    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE,
      static_cast<QImage*>(backingStore()->paintDevice())->bits());
*/

    backingStore()->flush(rect());
  }
  // else do nothing
}

//////////////////////////////////////////////////////////////////////////////
void QuickWidget::beforeRendering()
{
  if (!fbo_)
  {
    fbo_.reset(new QOpenGLFramebufferObject(size(),
      QOpenGLFramebufferObject::NoAttachment));

    window_->setRenderTarget(fbo_.data());
  }
  // else do nothing
}
