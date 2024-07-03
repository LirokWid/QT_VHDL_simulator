#ifndef MULTITYPESCHRONO_H
#define MULTITYPESCHRONO_H

#include <QApplication>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QVector>
#include <QDebug>
#include <QPushButton>
#include <QLabel>

/**
 * @class MultiTypesChrono
 * @brief A QWidget-based class for visualizing boolean data points on a graph.
 */
class MultiTypesChrono : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for MultiTypesChrono.
     * @param parent The parent widget.
     */
    MultiTypesChrono(QWidget *parent = nullptr);

    /**
     * @brief Adds a boolean data point to the graph.
     * @param point The boolean data point to be added.
     */
    void addPoint(bool point);

protected:
    /**
     * @brief Handles the paint event to draw the graph.
     * @param event The paint event.
     */
    void paintEvent(QPaintEvent* event) override;

    /**
     * @brief Handles the resize event to adjust the graph layout.
     * @param event The resize event.
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief Handles the mouse press event for dragging and zooming.
     * @param event The mouse press event.
     */
    void mousePressEvent(QMouseEvent *event) override;

    /**
     * @brief Handles the mouse move event for dragging and zooming.
     * @param event The mouse move event.
     */
    void mouseMoveEvent(QMouseEvent *event) override;

    /**
     * @brief Handles the mouse release event for dragging and zooming.
     * @param event The mouse release event.
     */
    void mouseReleaseEvent(QMouseEvent *event) override;

    /**
     * @brief Handles the mouse going out of the widget
     * @param event The mouse leaving event
     */
    void leaveEvent(QEvent *event) override;

private:
    // Constants for the graph layout
    const int labelOffset = 15; ///< Offset for the axis labels.
    const int tickSize = 5; ///< Size of the axis ticks.
    const int textOffset = 10; ///< Offset for the text labels.
    const int textWidth = 30; ///< Width of the text labels.
    const int textHeight = 10; ///< Height of the text labels.
    const int xLabelDensity = 1; ///< Density of the x-axis labels.

    const int marginLeft = 40; ///< Left margin of the graph.
    const int marginTop = 50; ///< Top margin of the graph.
    const int marginBottom = 50; ///< Bottom margin of the graph.

    const int textRectPadding = 5; ///< Padding inside the text rectangles.
    const int textRectRadius = 2; ///< Radius for the rounded corners of the text rectangles.

    int visibleRange = 10; ///< Range of visible data points.
    const int pointRadius = 2; ///< Radius of the data points.
    const int minDisplayedSteps = 2; ///< Minimum number of displayed steps.

    const int popupDisplayRadius = 15; ///< popup visible if close to the point

    const QColor backgrounColor = Qt::black; ///< Background color of the graph.
    const QColor axisColor = Qt::white; ///< Color of the graph axes.
    const QColor graphColor = Qt::blue; ///< Color of the graph line.
    const QColor rectBorderColor = Qt::white; ///< Color of the border for text rectangles.

    int currentOffset = 0; ///< Current offset of the graph.
    double stepPixelNb = 60.f; ///< Number of pixels per step.

    QSlider *slider; ///< Slider for navigating through the data points.
    QVBoxLayout *Vlayout; ///< Vertical layout for the widget.
    QVector<bool> boolDataPoints; ///< Vector of boolean data points.
    QPushButton *plusButton; ///< Button to increase the visible range.
    QPushButton *minusButton; ///< Button to decrease the visible range.
    QPushButton *fitButton; ///< Button to fit the graph to the data points.
    QHBoxLayout *buttonLayout; ///< Horizontal layout for the buttons.    
    QLabel *popupLabel;

    /**
     * @brief Draws the background scale of the graph.
     * @param painter The QPainter used for drawing.
     */
    void drawBackScale(QPainter *painter);

    /**
     * @brief Updates the range of the slider based on the data points.
     */
    void updateSliderRange();

    /**
     * @brief Calculates the visible range of the graph.
     */
    void calculateVisibleRange();

    /**
     * @brief Initializes the boolean data points with a specified number of points.
     * @param nbPoints The number of points to initialize.
     */
    void initializeBoolDataPoints(int nbPoints);

    /**
     * @brief Calculates the number of pixels per step.
     */
    void calculateStepPixelNb();

    /**
     * @brief Draws text on the graph.
     * @param painter The QPainter used for drawing.
     * @param text The text to draw.
     * @param x The x-coordinate of the text position.
     * @param y The y-coordinate of the text position.
     */
    void drawText(QPainter &painter, const QString &text, int x, int y);

    /**
     * @brief Draws text inside a rectangle on the graph.
     * @param painter The QPainter used for drawing.
     * @param text The text to draw.
     * @param x The x-coordinate of the rectangle position.
     * @param y The y-coordinate of the rectangle position.
     */
    void drawTextInBox(QPainter &painter, const QString &text, int x, int y);

    /**
     * @brief Draws a popup to display a point coordinates
     * @param cursorPos position to show
     */
    void showPopupAtCursor(QPoint cursorPos);

    bool isDragging = false; ///< Flag indicating if the graph is being dragged.
    QPoint dragStartPoint; ///< Starting point of the drag.
    QPoint dragEndPoint; ///< Ending point of the drag.

    bool isRightClicking = false; ///< Flag indicating if the right mouse button is being clicked.
    bool isFirstRightClick = false; ///< Flag indicating if it's the first right mouse click.
    QPoint rightClickStartPoint; ///< Starting point of the right mouse click.

    int getStepFromX(int x);
private slots:
    /**
     * @brief Slot to handle the plus button click event.
     */
    void handlePlusButton();

    /**
     * @brief Slot to handle the minus button click event.
     */
    void handleMinusButton();

    /**
     * @brief Slot to handle the fit button click event.
     */
    void handleFitButton();
};

#endif // MULTITYPESCHRONO_H
