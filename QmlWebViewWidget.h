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
protected:
    QPaintEngine * paintEngine() const ;
    void paintEvent(QPaintEvent * event) ;

private:
    QQuickPaintedItem *m_qquickContainer;

};


class QmlWebViewWidget : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QObject* widget READ widget)
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)


public:
    explicit QmlWebViewWidget(QQuickItem *parent = 0);
    void paint(QPainter *painter);
    QObject *widget() const;

    QUrl url() const;

public slots:
    void setUrl(QUrl arg);
    void setFocus(bool arg);

signals:
    void urlChanged(QUrl arg);
    void loadStarted();
    void loadFinished(bool ok);
    void loadProgress(int progress);

private:
    void handleHoverMoveEvent(QHoverEvent *ev);

    QmlWebViewInternalWidget *m_widget;

protected:
    virtual void geometryChanged(const QRectF & newGeometry,
                                 const QRectF & oldGeometry);
    bool event(QEvent * e) ;
protected slots:
    void updateGeometry();
};

#endif // QMLWEBVIEWWIDGET_H
