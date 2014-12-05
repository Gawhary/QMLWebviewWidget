#ifndef QMLWEBVIEWWIDGET_H
#define QMLWEBVIEWWIDGET_H

#include <QQuickPaintedItem>
#include <QWidget>

#include <QWebView>


class QmlWebViewInternalWidget : public QWebView
{
    friend class QmlWebViewWidget;
    Q_OBJECT
private:
    explicit QmlWebViewInternalWidget(QQuickPaintedItem *parent);

public slots:
    void show(){}
protected:
    QPaintEngine * paintEngine() const ;
    void paintEvent(QPaintEvent * event) ;
    virtual bool event(QEvent * e);

private:
    QQuickPaintedItem *m_qquickContainer;

};


class QmlWebViewWidget : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QObject* widget READ widget)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QObject *attachedObject READ attachedObject WRITE setAttachedObject NOTIFY attachedObjectChanged)
    Q_PROPERTY(QString attachedObjectName READ attachedObjectName WRITE setAttachedObjectName NOTIFY attachedObjectNameChanged)
    Q_PROPERTY(bool focus READ focus WRITE setFocus NOTIFY focusChanged)


public:
    explicit QmlWebViewWidget(QQuickItem *parent = 0);
    void paint(QPainter *painter);
    QObject *widget() const;

    QUrl url() const;
    QObject *attachedObject();
    QString attachedObjectName();
    bool focus() const;

public slots:
    void setUrl(QUrl arg);
    void setFocus(bool arg);
    void setAttachedObject(QObject *attachedObject);
    void setAttachedObjectName(QString attachedObjectName);
    void attachObject();
    QVariant evaluateJavaScript(QString scriptSource);

signals:
    void urlChanged(QUrl arg);
    void attachedObjectChanged(QObject *arg);
    void attachedObjectNameChanged(QString arg);
    void focusChanged(bool arg);

private:
    void handleHoverMoveEvent(QHoverEvent *ev);

    QmlWebViewInternalWidget *m_widget;
    QObject *m_attachedObject;
    QString m_attachedObjectName;
    bool m_focus;
    QRectF m_geometry;

protected:
    virtual void geometryChanged(const QRectF & newGeometry,
                                 const QRectF & oldGeometry);
    bool event(QEvent * e) ;
protected slots:
    void updateGeometry();
};

#endif // QMLWEBVIEWWIDGET_H