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
    //int height = graphicsView->scene()->height();         // not used
    graphicsView->fitInView(svgItem, Qt::KeepAspectRatio);
    setZoom(zoomValue);
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

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cannot open file for reading:" << file.errorString();
        return false;
    }

    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        qDebug() << "Failed to parse the file.";
        file.close();
        return false;
    }
    file.close();

    QDomElement element = doc.documentElement();
    bool modified = false;
    bool mainElemModified = false;


    while(!mainElemModified)
    {
        QDomNode childNode = element.firstChild();
        while (!childNode.isNull())
        {
            if (childNode.isElement())
            {
                QDomElement childElement = childNode.toElement();
                modifyElementStyle(childElement, elementLabel, mainElemModified, modified, false);
            }
            childNode = childNode.nextSibling();
        }
        //modifyElementStyle(root, elementLabel, mainElemModified, modified, false);
    }

    if (modified)
    {
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            qDebug() << "Cannot open file for writing:" << file.errorString();
            return false;
        }
        QTextStream out(&file);
        doc.save(out, 4); // Indentation level of 4 spaces
        file.close();
    }
    else
    {
        qDebug() << "Element with label" << elementLabel << "not found.";
    }

    return modified;
}

void SvgWidget::modifyElementStyle(QDomElement &element, const QString &elementLabel, bool &mainElemModified, bool &modified, bool applyToAllChildren)
{

    if (( element.hasAttribute("inkscape:label") && element.attribute("inkscape:label") == elementLabel))
    //|| modified)
    {
        applyToAllChildren = true;
    }

    if (applyToAllChildren)
    {// Togle the style for the current element
        QString style = element.attribute("style");

        for (const auto &stylePair : styles_to_modify.styles)
        {
            QString currentStyleValue = "";

            // Check if style attribute exists
            QRegularExpression re(stylePair.def.name + ":\\s*([^;]+)");
            QRegularExpressionMatch match = re.match(style);
            if (match.hasMatch())
            {
                currentStyleValue = match.captured(1);
            }

            // Toggle the style value
            QString newValue;
            if (currentStyleValue == stylePair.def.value)
            {
                newValue = stylePair.on.value;
            }
            else
            {
                newValue = stylePair.def.value;
            }

            if (match.hasMatch())
            {
                style.replace(re, stylePair.def.name + ": " + newValue);
            }
            else
            {
                if (!style.isEmpty())
                {
                    style.append("; ");
                }
                style.append(stylePair.def.name + ": " + newValue);
            }
        }

        element.setAttribute("style", style);
        modified = true;
    }

    if(mainElemModified != modified)
    {//We found the parent elem containing the label
        mainElemModified = true;

        //Recursively apply the style to all the children
        QDomNode childNode = element.firstChild();
        while (!childNode.isNull())
        {
            if (childNode.isElement())
            {
                QDomElement childElement = childNode.toElement();
                modifyElementStyle(childElement, elementLabel, mainElemModified, modified, applyToAllChildren);
            }
            childNode = childNode.nextSibling();
        }
    }
}

void SvgWidget::recursiveElemColoring(QDomElement &element, const QString &elementLabel, bool &mainElemModified, bool &modified, bool applyToAllChildren)
{
    if (element.hasAttribute("inkscape:label") && element.attribute("inkscape:label") == elementLabel)
    {
        applyToAllChildren = true;
    }

    if (applyToAllChildren)
    {
        // Toggle the style for the current element
        QString style = element.attribute("style");

        for (const auto &stylePair : styles_to_modify.styles)
        {
            QString currentStyleValue = "";

            // Check if style attribute exists
            QRegularExpression re(stylePair.def.name + ":\\s*([^;]+)");
            QRegularExpressionMatch match = re.match(style);
            if (match.hasMatch())
            {
                currentStyleValue = match.captured(1);
            }

            // Toggle the style value
            QString newValue;
            if (currentStyleValue == stylePair.def.value)
            {
                newValue = stylePair.on.value;
            }
            else
            {
                newValue = stylePair.def.value;
            }

            if (match.hasMatch())
            {
                style.replace(re, stylePair.def.name + ": " + newValue);
            }
            else
            {
                if (!style.isEmpty())
                {
                    style.append("; ");
                }
                style.append(stylePair.def.name + ": " + newValue);
            }
        }

        element.setAttribute("style", style);
        modified = true;
    }

    // Recursively apply the style to all children
    QDomNode childNode = element.firstChild();
    while (!childNode.isNull())
    {
        if (childNode.isElement())
        {
            QDomElement childElement = childNode.toElement();
            recursiveElemColoring(childElement, elementLabel, mainElemModified, modified, applyToAllChildren);
        }
        childNode = childNode.nextSibling();
    }
}


QList<QDomElement> SvgWidget::locateElemFromName(QDomElement &root, const QString elemName)
{
    QList<QDomElement> matchingElements;

    // Helper function to recursively search and collect matching elements
    std::function<void(QDomElement &)> searchAndCollect = [&](QDomElement &element)
    {
        if (element.hasAttribute("inkscape:label") && element.attribute("inkscape:label") == elemName)
        {
            matchingElements.append(element);
        }

        QDomNode childNode = element.firstChild();
        while (!childNode.isNull())
        {
            if (childNode.isElement())
            {
                QDomElement childElement = childNode.toElement();
                searchAndCollect(childElement);
            }
            childNode = childNode.nextSibling();
        }
    };

    // Start the search from the root element
    searchAndCollect(root);

    return matchingElements;
}


void SvgWidget::highlightItemSlot(const QString &value)
{
    DebugWindow::getInstance()->addDebug("highlithing " + value);


    QFile file(fileLocation);
    file.setPermissions(QFile::ReadOther | QFile::WriteOther); //File has to be writable to be modified

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cannot open file for reading:" << file.errorString();
    }

    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        qDebug() << "Failed to parse the file.";
        file.close();
    }
    file.close();

    QDomElement root = doc.documentElement();

    QList<QDomElement> elementsToColor = locateElemFromName(root, value);



    bool mainElemModified = false;
    for (QDomElement &elem : elementsToColor)
    {
        QDomDocument doc;
        QDomElement clone = elem.cloneNode(true).toElement();
        doc.appendChild(clone);
        qDebug() << "Element:" << elem.tagName() << "Label:" << elem.attribute("inkscape:label");
        qDebug() << doc.toString();

        bool modified = false;
        recursiveElemColoring(elem, value, mainElemModified, modified, true);
    }


/*
    QList<QDomElement> found_elems;
    found_elems = locateElemFromName(element, value);

    if (found_elems.size() == 1)
    {
        if (changeElementStyle(fileLocation, value))
        {
            loadSvg(fileLocation);
            qDebug() << "Highlighted successfully.";
        }
        else
        {
            qDebug() << "Failed to add red square.";
        }
    }
    else
    {
        DebugWindow::getInstance()->addError("found " + QString::number(found_elems.size()) + " elements instead of 1");
    }
*/





}
