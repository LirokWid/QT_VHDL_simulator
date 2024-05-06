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
    graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(graphicsView);

    zoomSlider = new QSlider(Qt::Horizontal, this);
    zoomSlider->setRange(1, 100);
    zoomSlider->setValue(50);
    connect(zoomSlider, &QSlider::valueChanged, this, &SvgWidget::zoomChanged);
    layout->addWidget(zoomSlider);

    setLayout(layout);

}


void SvgWidget::loadSvg(const QString& filePath)
{
    clearScene();
    svgItem = new QGraphicsSvgItem(filePath);
    graphicsView->scene()->addItem(svgItem);
    graphicsView->fitInView(svgItem, Qt::KeepAspectRatio);
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
    qreal scaleFactor = value / 50.0;
    graphicsView->resetTransform();
    graphicsView->scale(scaleFactor, scaleFactor);
}

void SvgWidget::zoomChanged(int value)
{
    setZoom(value);
}

void SvgWidget::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
        qreal scaleFactor = 1.15;
        if (event->angleDelta().y() < 0) {
            scaleFactor = 1.0 / scaleFactor;
        }
        graphicsView->scale(scaleFactor, scaleFactor);
        event->accept();
    } else {
        QWidget::wheelEvent(event);
    }
}
