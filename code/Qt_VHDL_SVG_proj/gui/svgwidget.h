/**
 * @file SvgWidget.h
 * @brief This file contains the SvgWidget class, which provides functionality for loading, displaying, and interacting with SVG files in a QWidget using QGraphicsView and QGraphicsSvgItem.
 */

#ifndef SVGWIDGET_H
#define SVGWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsSvgItem>
#include <QSlider>
#include <QDomDocument>

#define BASE_ZOOM 50

/**
 * @class SvgWidget
 * @brief The SvgWidget class allows for loading, viewing, and interacting with SVG files using Qt's graphics framework.
 *
 * The class provides functions to load an SVG, apply zoom, highlight elements, and modify styles of specific elements in the SVG.
 */
class SvgWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Constructs the SvgWidget object.
     * @param parent The parent widget, defaults to nullptr.
     */
    explicit SvgWidget(QWidget *parent = nullptr);

    /**
     * @brief Loads an SVG file and displays it within the widget.
     * @param filePath The path to the SVG file to be loaded.
     */
    void loadSvg(const QString& filePath);

    /**
     * @brief Clears the currently loaded SVG from the scene.
     */
    void clearSvg();

    /**
     * @brief Clears the current QGraphicsScene.
     */
    void clearScene();

    /**
     * @brief Sets the zoom level for viewing the SVG.
     * @param value The new zoom value (from 1 to 100).
     */
    void setZoom(int value);

public slots:
    /**
     * @brief Highlights a specific item in the SVG based on its label.
     * @param value The label of the item to be highlighted.
     */
    void highlightItemSlot(const QString &value);

private slots:
    /**
     * @brief Slot that handles zoom changes when the zoom slider is adjusted.
     * @param value The new zoom level.
     */
    void zoomChanged(int value);

protected:
    /**
     * @brief Handles mouse wheel events to zoom in and out when the control key is held.
     * @param event The wheel event.
     */
    void wheelEvent(QWheelEvent *event) override;

private:
    /**
     * @struct s_style
     * @brief Represents a style with default and "on" states.
     */
    struct s_style_lists
    {
        struct s_style
        {
            struct
            {
                QString name;  /**< The name of the style attribute (e.g., "stroke"). */
                QString value; /**< The value of the style attribute (e.g., "#000000"). */
            } def, on;  /**< Default and "on" state of the style. */
        };
        QList<s_style> styles; /**< A list of styles to be modified. */
    };

    /// List of styles to modify in the SVG elements.
    const s_style_lists styles_to_modify =
        {
            QList<s_style_lists::s_style>
            {
                {{"stroke", "#000000"}, {"stroke", "#ff0000"}}, /**< Stroke color */
                {{"color", "black"}, {"color", "red"}}           /**< Text color */
            }
        };

    QGraphicsView *graphicsView;   /**< The QGraphicsView used to display the SVG. */
    QGraphicsSvgItem *svgItem;     /**< The QGraphicsSvgItem representing the SVG in the scene. */
    QSlider *zoomSlider;           /**< The zoom slider used to control the zoom level. */
    QString fileLocation;          /**< The location of the currently loaded SVG file. */

    QPoint lastMousePos;           /**< The last recorded mouse position for panning. */
    bool panning;                  /**< Boolean indicating whether the view is in panning mode. */
    int zoomValue = BASE_ZOOM;     /**< The current zoom value. */

    /**
     * @brief Recursively changes the stroke color of elements in the SVG.
     * @param element The root element of the SVG.
     * @param elementLabel The label of the element to modify.
     * @return True if successful, false otherwise.
     */
    bool changeStrokeColorRecursive(QDomElement &element, const QString &elementLabel);

    /**
     * @brief Modifies the stroke color of a specific element in the SVG.
     * @param filePath The path to the SVG file.
     * @param elementLabel The label of the element to modify.
     * @return True if the element was successfully modified, false otherwise.
     */
    bool changeElementStrokeColor(const QString &filePath, const QString &elementLabel);

    /**
     * @brief Toggles the style value between default and "on" states.
     * @param currentValue The current value of the style.
     * @param defaultValue The default value of the style.
     * @param onValue The "on" value of the style.
     * @return The toggled value.
     */
    QString toggleStyleValue(const QString &currentValue, const QString &defaultValue, const QString &onValue);

    /**
     * @brief Recursively modifies styles of elements in the SVG.
     * @param element The root element of the SVG.
     * @param elementLabel The label of the element to modify.
     * @return True if the style was successfully modified, false otherwise.
     */
    bool changeStyleRecursive(QDomElement &element, const QString &elementLabel);

    /**
     * @brief Modifies the style of a specific element in the SVG.
     * @param filePath The path to the SVG file.
     * @param elementLabel The label of the element to modify.
     * @return True if the element was successfully modified, false otherwise.
     */
    bool changeElementStyle(const QString &filePath, const QString &elementLabel);

    /**
     * @brief Modifies the style of an element and its children.
     * @param element The element to modify.
     * @param elementLabel The label of the element.
     * @param mainElemModified Reference to a flag indicating if the main element has been modified.
     * @param modified Reference to a flag indicating if any modifications were made.
     * @param applyToAllChildren Flag indicating whether to apply the modification to all children.
     */
    void modifyElementStyle(QDomElement &element, const QString &elementLabel, bool &mainElemModified, bool &modified, bool applyToAllChildren);

    /**
     * @brief Recursively applies color modifications to elements.
     * @param element The root element of the SVG.
     * @param elementLabel The label of the element to modify.
     * @param mainElemModified Reference to a flag indicating if the main element has been modified.
     * @param modified Reference to a flag indicating if any modifications were made.
     * @param applyToAllChildren Flag indicating whether to apply the modification to all children.
     */
    void recursiveElemColoring(QDomElement &element, const QString &elementLabel, bool &mainElemModified, bool &modified, bool applyToAllChildren);

    /**
     * @brief Locates elements in the SVG by name.
     * @param root The root element of the SVG.
     * @param elemName The name of the element to locate.
     * @return A list of matching QDomElement objects.
     */
    QList<QDomElement> locateElemFromName(QDomElement &root, const QString elemName);
};

#endif // SVGWIDGET_H
