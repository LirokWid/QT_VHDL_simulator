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

QRectF SvgWidget::calculateBoundingBoxRecursive(const QDomElement &element, const QString &elementLabel, QRectF &boundingBox) {
    if (element.hasAttribute("inkscape:label")
        && element.attribute("inkscape:label") == elementLabel)
    {
        QRectF elementBox(element.attribute("x").toDouble(),
                          element.attribute("y").toDouble(),
                          element.attribute("width").toDouble(),
                          element.attribute("height").toDouble());
        boundingBox = boundingBox.united(elementBox);
    }

    QDomNodeList children = element.childNodes();
    for (int i = 0; i < children.count(); ++i)
    {
        QDomElement childElement = children.at(i).toElement();
        if (!childElement.isNull())
        {
            calculateBoundingBoxRecursive(childElement, elementLabel, boundingBox);
        }
    }

    return boundingBox;
}


bool SvgWidget::wrapElementWithRedSquare(const QString &filePath, const QString &elementLabel)
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

    // Check for existing red square and remove it if it exists
    QDomNodeList rectList = root.elementsByTagName("rect");
    for (int i = 0; i < rectList.count(); ++i) {
        QDomElement rect = rectList.at(i).toElement();
        if (rect.hasAttribute("data-label") && rect.attribute("data-label") == elementLabel) {
            root.removeChild(rect);
            qDebug() << "Existing red square removed.";
            break;
        }
    }

    QRectF boundingBox;
    calculateBoundingBoxRecursive(root, elementLabel, boundingBox);

    if (boundingBox.isNull()) {
        qWarning() << "Element with label" << elementLabel << "not found or has no bounding box.";
        return false;
    }

    // Create a new rectangle element to wrap the target element
    QDomElement rectangle = doc.createElement("rect");
    rectangle.setAttribute("x", boundingBox.x());
    rectangle.setAttribute("y", boundingBox.y());
    rectangle.setAttribute("width", boundingBox.width());
    rectangle.setAttribute("height", boundingBox.height());
    rectangle.setAttribute("stroke", "red");
    rectangle.setAttribute("fill", "none");
    rectangle.setAttribute("data-label", elementLabel); // Custom attribute to identify the red square

    // Insert the rectangle as the first child of the root element
    root.insertBefore(rectangle, root.firstChild());

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file for writing:" << filePath;
        qWarning() << "File error:" << file.errorString();
        return false;
    }

    QTextStream stream(&file);
    stream << doc.toString();
    file.close();

    return true;
}

void SvgWidget::highlightItemSlot(const QString &value)
{
    DebugWindow::getInstance()->addDebug("highlithing " + value);
    if(wrapElementWithRedSquare(fileLocation, value))
    {
        loadSvg(fileLocation);
        qDebug() << "Red square added successfully.";
    }
    else
    {
        qDebug() << "Failed to add red square.";
    }
}
