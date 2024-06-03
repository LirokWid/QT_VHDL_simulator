#include "svgwidget.h"
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSvgItem>
#include <QWheelEvent>
#include <QScrollBar>
#include <QDomDocument>
#include "debugwindow.h"
#include <QRegularExpression>

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

bool SvgWidget::changeElementStyle(const QString &filePath, const QString &elementLabel)
{
    QFile file(filePath);
    file.setPermissions(QFile::ReadOther | QFile::WriteOther); //File has to be writable to be modified

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for reading:" << file.errorString();
        return false;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        qDebug() << "Failed to parse the file.";
        file.close();
        return false;
    }
    file.close();

    QDomElement root = doc.documentElement();
    bool modified = false;
    recursivelyModifyElementStyle(root, elementLabel, modified, false);

    if (modified) {
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qDebug() << "Cannot open file for writing:" << file.errorString();
            return false;
        }
        QTextStream out(&file);
        doc.save(out, 4); // Indentation level of 4 spaces
        file.close();
    } else {
        qDebug() << "Element with label" << elementLabel << "not found.";
    }

    return modified;
}

void SvgWidget::recursivelyModifyElementStyle(QDomElement &element, const QString &elementLabel, bool &modified, bool applyToAllChildren)
{
    if (element.hasAttribute("inkscape:label") && element.attribute("inkscape:label") == elementLabel) {
        applyToAllChildren = true;
    }

    if (applyToAllChildren) {
        QString style = element.attribute("style");

        for (const auto &stylePair : styles_to_modify.styles) {
            QString currentStyleValue = "";

            // Check if style attribute exists
            QRegularExpression re(stylePair.def.name + ":\\s*([^;]+)");
            QRegularExpressionMatch match = re.match(style);
            if (match.hasMatch()) {
                currentStyleValue = match.captured(1);
            }

            // Toggle the style value
            QString newValue;
            if (currentStyleValue == stylePair.def.value) {
                newValue = stylePair.on.value;
            } else {
                newValue = stylePair.def.value;
            }

            if (match.hasMatch()) {
                style.replace(re, stylePair.def.name + ": " + newValue);
            } else {
                if (!style.isEmpty()) {
                    style.append("; ");
                }
                style.append(stylePair.def.name + ": " + newValue);
            }
        }

        element.setAttribute("style", style);
        modified = true;
    }

    QDomNode childNode = element.firstChild();
    while (!childNode.isNull()) {
        if (childNode.isElement()) {
            QDomElement childElement = childNode.toElement();
            recursivelyModifyElementStyle(childElement, elementLabel, modified, applyToAllChildren);
        }
        childNode = childNode.nextSibling();
    }
}

void SvgWidget::highlightItemSlot(const QString &value)
{
    DebugWindow::getInstance()->addDebug("highlithing " + value);
    if(changeElementStyle(fileLocation, value))
    {
        loadSvg(fileLocation);
        qDebug() << "Highlighted successfully.";
    }
    else
    {
        qDebug() << "Failed to add red square.";
    }
}
