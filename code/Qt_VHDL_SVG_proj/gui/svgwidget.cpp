#include "svgwidget.h"
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSvgItem>
#include <QWheelEvent>
#include <QScrollBar>
#include <QDomDocument>
#include "debugwindow.h"

#define min(X, Y) (((X) < (Y)) ? (X) : (Y))
#define max(X, Y) (((X) > (Y)) ? (X) : (Y))

SvgWidget::SvgWidget(QWidget *parent)
    : QWidget(parent)
{
    graphicsView = new QGraphicsView(this);
    QGraphicsScene *scene = new QGraphicsScene(this);
    graphicsView->setScene(scene);
    graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(graphicsView);

    zoomSlider = new QSlider(Qt::Vertical, this);
    zoomSlider->setRange(1, 100);
    zoomSlider->setValue(zoomValue);
    connect(zoomSlider, &QSlider::valueChanged, this, &SvgWidget::zoomChanged);
    layout->addWidget(zoomSlider);

    setLayout(layout);
}


void SvgWidget::loadSvg(const QString& filePath)
{
    this->fileLocation = filePath;
    clearScene();
    svgItem = new QGraphicsSvgItem(filePath);
    graphicsView->scene()->addItem(svgItem);
    int height = graphicsView->scene()->height();
    graphicsView->fitInView(svgItem, Qt::KeepAspectRatio);
    setZoom(50);
}

void SvgWidget::clearSvg()
{
    clearScene();
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
bool SvgWidget::changeElementColorRecursive(QDomElement &element, const QString &elementLabel) {
    if (element.hasAttribute("inkscape:label") && element.attribute("inkscape:label") == elementLabel) {
        element.setAttribute("fill", "red");
        return true;
    }

    QDomNodeList children = element.childNodes();
    for (int i = 0; i < children.count(); ++i) {
        QDomElement childElement = children.at(i).toElement();
        if (!childElement.isNull() && changeElementColorRecursive(childElement, elementLabel)) {
            return true;
        }
    }
    return false;
}

bool SvgWidget::changeSvgElementColorToRed(const QString &filePath, const QString &elementLabel)
{
    QFile file(filePath);
    file.setPermissions(QFile::ReadOther | QFile::WriteOther); //File has to be writable to be modified

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for reading:" << filePath;
        qWarning() << "File error:" << file.errorString();
        return false;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        qWarning() << "Failed to parse SVG content.";
        file.close();
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();
    bool elementFound = changeElementColorRecursive(root, elementLabel);

    if (!elementFound) {
        qWarning() << "Element with label" << elementLabel << "not found.";
        return false;
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for writing:" << filePath;
        qWarning() << "File error:" << file.errorString();
    }

    QTextStream stream(&file);
    stream << doc.toString();
    file.close();

    return true;
}

void SvgWidget::highlightItemSlot(const QString &value)
{
    DebugWindow::getInstance()->addDebug("highlithing " + value);
    if(changeSvgElementColorToRed(fileLocation, value))
    {
        loadSvg(fileLocation);
    }
}
