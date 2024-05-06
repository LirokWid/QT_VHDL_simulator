#include "svgwidget.h"
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSvgItem>
#include <QWheelEvent>
#include <QScrollBar>

SvgWidget::SvgWidget(QWidget *parent)
    : QWidget(parent)
{
    graphicsView = new QGraphicsView(this);
    QGraphicsScene *scene = new QGraphicsScene(this);
    graphicsView->setScene(scene);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(graphicsView);

    zoomSlider = new QSlider(Qt::Horizontal, this);
    zoomSlider->setRange(1, 100);
    zoomSlider->setValue(zoomValue);
    connect(zoomSlider, &QSlider::valueChanged, this, &SvgWidget::zoomChanged);
    layout->addWidget(zoomSlider);

    setLayout(layout);

}


void SvgWidget::loadSvg(const QString& filePath)
{
    clearScene();
    svgItem = new QGraphicsSvgItem(filePath);
    graphicsView->scene()->addItem(svgItem);
    int height = graphicsView->scene()->height();
    graphicsView->fitInView(svgItem, Qt::KeepAspectRatio);
    setZoom(50);

}

void SvgWidget::clearScene()
{
    QGraphicsScene *scene = graphicsView->scene();
    if (!scene)
        return;
    scene->clear();
}

void SvgWidget::setZoom(int value)
{
    zoomValue = value;
    qreal scaleFactor = value / 50.0;
    graphicsView->resetTransform();
    graphicsView->scale(scaleFactor, scaleFactor);
    zoomSlider->setValue(value);
}

void SvgWidget::zoomChanged(int value)
{
    zoomValue = value;
    setZoom(zoomValue);
}

void SvgWidget::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        int dy = event->angleDelta().y();
        int new_zoom = zoomValue + dy;

        int constrained = max(0, min(100, new_zoom));
        setZoom(constrained);

        event->accept();
    } else {
        QWidget::wheelEvent(event);
    }
}
