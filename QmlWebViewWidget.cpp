#include "QmlWebViewWidget.h"
//#include "../config.h"
#include <QTimer>
#include <QWebView>
#include <QWebFrame>
#include <QQuickWindow>
#include <qdebug.h>

QmlWebViewInternalWidget::QmlWebViewInternalWidget(QQuickPaintedItem *parent) :
    QWebView(0)
{
    m_qquickContainer = parent;
    setFocusPolicy(Qt::WheelFocus);
    setAttribute(Qt::WA_WState_Created, true);// don't create window on setVisible(true)
    setVisible(true);// if not visible some operations will not done.
}

QPaintEngine *QmlWebViewInternalWidget::paintEngine() const
{
    return NULL;
}

void QmlWebViewInternalWidget::paintEvent(QPaintEvent *event)
{
    QWebView::paintEvent(event);
    m_qquickContainer->update(event->rect());
}


QmlWebViewWidget::QmlWebViewWidget(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
    setOpaquePainting(true);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);

    setFlag(QQuickItem::ItemAcceptsDrops, true);
    setFlag(QQuickItem::ItemHasContents, true);


    m_widget = new QmlWebViewInternalWidget(this);

    connect(m_widget, SIGNAL(urlChanged(QUrl)), this, SIGNAL(urlChanged(QUrl)) );

    m_attachedObject = NULL;
    m_attachedObjectName = "";

    QObject::connect(this, &QQuickPaintedItem::widthChanged, this, [this]{
        QQuickWindow *w = this->window();
        if(w){
            QObject::connect(w, &QQuickWindow::xChanged,
                             this, &QmlWebViewWidget::updateGeometry,
                             (Qt::ConnectionType)(Qt::AutoConnection|Qt::UniqueConnection));
        }
    });
}

void QmlWebViewWidget::paint(QPainter *painter)
{
    m_widget->render(painter, QPoint(), QRegion(),
                     QWidget::DrawWindowBackground | QWidget::DrawChildren);
}

QObject* QmlWebViewWidget::widget() const
{
    return m_widget;
}

QUrl QmlWebViewWidget::url() const
{
    return m_widget->url();
}


QObject *QmlWebViewWidget::attachedObject()
{
    return m_attachedObject;
}

QString QmlWebViewWidget::attachedObjectName()
{
    return m_attachedObjectName;
}

void QmlWebViewWidget::setUrl(QUrl arg)
{

    connect(m_widget->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), SLOT(attachObject()));

    QWebSettings::globalSettings( )->setAttribute( QWebSettings::PrivateBrowsingEnabled, true );
    QWebSettings::setMaximumPagesInCache( 0 );
    QWebSettings::setObjectCacheCapacities( 0, 0, 0 );
    QWebSettings::clearMemoryCaches( );

    m_widget->setUrl(arg);
}

void QmlWebViewWidget::setFocus(bool arg)
{
    QQuickPaintedItem::setFocus(arg);
    if(arg){
        m_widget->setFocus();
        forceActiveFocus();
    }
    else{
        m_widget->clearFocus();
    }
    setActiveFocusOnTab(arg);
}

void QmlWebViewWidget::setAttachedObject(QObject *attachedObject)
{
    m_attachedObject = attachedObject;
}

void QmlWebViewWidget::setAttachedObjectName(QString attachedObjectName)
{
    m_attachedObjectName = attachedObjectName;
}

void QmlWebViewWidget::attachObject()
{
    qDebug() << "QmlWebViewWidget::attachObject() - Attaching an object to JS";
    if(m_attachedObject != NULL)
        m_widget->page()->mainFrame()->addToJavaScriptWindowObject(m_attachedObjectName, m_attachedObject);

    qDebug() << "QmlWebViewWidget::attachObject() - Object name: " << m_widget->page()->mainFrame()->objectName();
    qDebug() << "QmlWebViewWidget::attachObject() - Object attached";
}

QVariant QmlWebViewWidget::evaluateJavaScript(QString scriptSource)
{
    return m_widget->page()->mainFrame()->evaluateJavaScript(scriptSource);
}

void QmlWebViewWidget::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    qDebug() << "QmlWebViewWidget::geometryChanged, " << newGeometry;
    if(newGeometry == oldGeometry)
        return;
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    updateGeometry();
}


bool QmlWebViewWidget::event(QEvent *e)
{
//    qDebug() << "QmlWebViewWidget::event" << e->type();

    if(e->type() == QEvent::HoverEnter ||
       e->type() == QEvent::HoverMove ||
       e->type() == QEvent::HoverLeave ){

        handleHoverMoveEvent((QHoverEvent*)e);
        return true;
    }

    if( e->type() == QEvent::MouseButtonPress ||
        e->type() == QEvent::MouseButtonDblClick )
        setFocus(true);

    if(m_widget->event(e))
        return true;
    return QQuickPaintedItem::event(e);
}

void QmlWebViewWidget::updateGeometry()
{
    QPointF newPos(0, 0);
    newPos = mapToItem(0, newPos);
    qDebug() << "QmlWebViewWidget::updateGeometry, top left mapped to window: " << newPos;

    if(this->window()){
    newPos += this->window()->position();
    qDebug() << "QmlWebViewWidget::updateGeometry, top left mapped to screen: " << newPos;
    }
    QRectF absRect(newPos, contentsBoundingRect().size());
    m_widget->setGeometry(absRect.toRect());
}

void QmlWebViewWidget::handleHoverMoveEvent(QHoverEvent* ev)
{
    QMouseEvent me(QEvent::MouseMove, ev->posF(),
                   Qt::NoButton, Qt::NoButton, ev->modifiers());
    me.setAccepted(ev->isAccepted());
    me.setTimestamp(ev->timestamp());
    m_widget->event(&me);
    setCursor(m_widget->cursor());
}
