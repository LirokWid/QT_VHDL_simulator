#ifndef MULTITYPESCHRONO_H
#define MULTITYPESCHRONO_H

#include "debugwindow.h"

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
 * @brief A custom Qt widget for visualizing and interacting with time-series data of various numeric types.
 *
 * This widget supports displaying boolean, integer, float, and double data in a graphical format.
 * It allows users to zoom, pan, and fit data to view a specified range of values effectively. The widget
 * features dynamic data point updates, popup information display on mouse hover, and adjustable visible
 * range through a slider control.
 *
 * @tparam T The type of data to be visualized. Supported types include bool, int, float, and double.
 *
 * The widget includes the following features:
 * - Graphical rendering of data points with interactive features.
 * - Customizable y-axis and x-axis scaling and labeling.
 * - Display of statistics such as the type of data and the number of data points.
 * - Ability to dynamically adjust the visible range of data through buttons and a slider.
 * - Popup display that shows the step index and value of the data point under the mouse cursor.
 *
 * @note Some features may require further enhancement, such as adding y-axis steps and visibility corrections
 */
class MultiTypesChrono : public QWidget
{
    Q_OBJECT

public:

    /**
    * @brief Constructor for a single boolean value.
    * @param startValue A boolean value to initialize the graph with.
    * @param parent The parent widget, default is nullptr.
    */
    MultiTypesChrono(bool startValue, QWidget *parent);

    /**
    * @brief Constructor for a single integer value.
    * @param startValue An integer value to initialize the graph with.
    * @param parent The parent widget, default is nullptr.
    */
    MultiTypesChrono(int startValue, QWidget *parent);

    /**
    * @brief Constructor for a single float value.
    * @param startValue A float value to initialize the graph with.
    * @param parent The parent widget, default is nullptr.
    */
    MultiTypesChrono(float startValue, QWidget *parent);

    /**
    * @brief Constructor for a single double value.
    * @param startValue A double value to initialize the graph with.
    * @param parent The parent widget, default is nullptr.
    */
    MultiTypesChrono(double startValue, QWidget *parent);

    /**
     * @brief Constructor for bool data
     * @param startList A QVector of bool values to initialize the graph with.
     * @param parent The parent widget, default is nullptr.
     */
    MultiTypesChrono(QVector<bool> startList, QWidget *parent = nullptr);

    /**
     * @brief Constructor for int data
     * @param startList A QVector of int values to initialize the graph with.
     * @param parent The parent widget, default is nullptr.
     */
    MultiTypesChrono(QVector<int> startList, QWidget *parent = nullptr);

    /**
     * @brief Constructor for float data
     * @param startList A QVector of float values to initialize the graph with.
     * @param parent The parent widget, default is nullptr.
     */
    MultiTypesChrono(QVector<float> startList, QWidget *parent = nullptr);

    /**
     * @brief Constructor for double data
     * @param startList A QVector of double values to initialize the graph with.
     * @param parent The parent widget, default is nullptr.
     */
    MultiTypesChrono(QVector<double> startList, QWidget *parent = nullptr);

    enum e_initType
    {
        BOOL,
        INT,
        FLOAT,
        DOUBLE,
        ERROR
    };


    /**
     * @brief Adds a single data point of the specified type to the graph.
     *
     * The point is converted to double and added to the dataPoints vector.
     * @param point The data point to add. It can be of type `bool`, `int`, `float`, or `double`.
     */
    template <typename T>
    void addPoint(const T& point);

    /**
     * @brief Adds multiple data points to the graph.
     *
     * Takes a QVector of data points and adds them to the dataPoints vector.
     * @param points A QVector of data points to add.
     */
    template <typename T>
    void addPoint(const QVector<T>& points);


    /**
     * @brief Return the type of data in use by the chronogram
     * @return The graph's type of data
     */
    e_initType getDataType() const;

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
    // Constants for the graph display layout and colours
    const int labelOffset = 15;         ///< Offset for the axis labels.
    const int tickSize = 5;             ///< Size of the axis ticks.
    const int textOffset = 10;          ///< Offset for the text labels.
    const int textWidth = 30;           ///< Width of the text labels.
    const int textHeight = 10;          ///< Height of the text labels.
    const int xLabelDensity = 1;        ///< Density of the x-axis labels.
    const int yLabelDensity = 1;        ///< Density of the y-axis labels.

    const int marginLeft = 40;          ///< Left margin of the graph.
    const int marginTop = 50;           ///< Top margin of the graph.
    const int marginBottom = 50;        ///< Bottom margin of the graph.

    const int textRectPadding = 5;      ///< Padding inside the text rectangles.
    const int textRectRadius = 2;       ///< Radius for the rounded corners of the text rectangles.

    const int pointRadius = 2;          ///< Radius of the data points.
    const int minDisplayedSteps = 2;    ///< Minimum number of displayed steps.

    const int popupDisplayRadius = 15;  ///< popup visibility distance from point

    const QColor backgroundColor = Qt::black;   ///< Background color of the graph.
    const QColor axisColor = Qt::white;         ///< Color of the graph axes.
    const QColor graphColor = Qt::red;          ///< Color of the graph line.
    const QColor rectBorderColor = Qt::white;   ///< Color of the borders of text rectangles.
    ///////////////////////////////////

    int visibleRange_X = 10; ///< Range of visible X data points.
    int visibleRange_Y = 10; ///< Range of visible Y data points.
    int yAxisCurrentMax;
    int yAxisCurrentMin;

    int offset_X = 0;   ///< Current offset of the graph.
    int offset_Y = 0;   ///< Current offset of the graph

    int Y_dataRange;

    double stepPixelNb_X = 60.f; ///< Number of pixels per step.
    double stepPixelNb_Y = 30.f; ///< Number of pixels per step.

    e_initType initType; ///< Stores the type of the data (BOOL, INT, FLOAT, DOUBLE).

    QSlider *slider_X; ///< Slider for navigating through the data points.
    QSlider *slider_Y; ///< Slider for navigating through the data points.
    QVBoxLayout *Vlayout; ///< Vertical layout for the widget.

    //To store the data points
    QVector<double> dataPoints; ///< Vector of data points.

    int dataMax;
    int dataMin;
    int nbPoints;
    int height;
    int width;

    QPushButton *plusButton_X;  ///< Button to increase the visible range.
    QPushButton *minusButton_X; ///< Button to decrease the visible range.
    QPushButton *fitButton_X;   ///< Button to fit the graph to the data points.

    QPushButton *plusButton_Y;  ///< Button to increase the visible range.
    QPushButton *minusButton_Y; ///< Button to decrease the visible range.
    QPushButton *fitButton_Y;   ///< Button to fit the graph to the data points.

    QHBoxLayout *buttonLayout_X; ///< Horizontal layout for the buttons.
    QVBoxLayout *buttonLayout_Y; ///< Horizontal layout for the buttons.

    QString buttonStyle = R"(
    QPushButton {
        background-color:#ededed;
        border-radius:5px;
        border:1px solid #000000;
        color:#222222;
        font-family:Arial;
        font-size:15px;
        font-weight:bold;
        padding:3px 6px;
        text-decoration:none;
    }
    QPushButton:hover {
        background-color:#bababa;
    }
    QPushButton:active {
        position:relative;
        top:1px;
    }
    )"; ///< Style
    QLabel *popupLabel;

    bool isDragging = false; ///< Flag indicating if the graph is being dragged.
    QPoint dragStartPoint; ///< Starting point of the drag.
    QPoint dragEndPoint; ///< Ending point of the drag.

    bool isRightClicking = false; ///< Flag indicating if the right mouse button is being clicked.
    bool isFirstRightClick = false; ///< Flag indicating if it's the first right mouse click.
    QPoint rightClickStartPoint; ///< Starting point of the right mouse click.

    /**
     * @brief Appends a data point of the specified type.
     *
     * Converts the data point to double and appends it to the dataPoints vector.
     * @param point The data point to append.
     */
    template <typename T>
    void appendPoint(const T& point);

    /**
     * @brief Draws the backFground scale of the graph.
     * @param painter The QPainter used for drawing.
     */
    void drawBackScale(QPainter *painter);

    /**
     * @brief Updates the range of the slider based on the data points.
     */
    void updateSliderRanges();

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
    void calculatePixelPerStep();

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


    /**
     * @brief convert a pixel number in a step number
     * @param x the pixel x coordinate
     * @return the step corresponding to the x coordinate
     */
    int getStepFromX(int x);

    /**
     * @brief Initializes the graph with its graphical settings and buttons
     */
    void initGraph();

    /**
    * @brief Gets the maximum value from a QVector of double.
    * @param vec The QVector from which to find the maximum value.
    * @return The maximum value found in the QVector.
    */
    double getMax(const QVector<double> &vec);

    /**
    * @brief Gets the minimum value from a QVector of double.
    * @param vec The QVector from which to find the minimum value.
    * @return The minimum value found in the QVector.
    */
    double getMin(const QVector<double> &vec);

    /**
    * @brief Calculates and updates the minimum and maximum sizes for the data points.
    *
    * This function analyzes the data points to determine their minimum and maximum
    * values and sets the respective attributes accordingly.
    */
    void getMinMaxSize();

    /**
    * @brief Converts the initialization type to a corresponding string.
    * @param type The initialization type to convert.
    * @return A QString representing the type, such as "bool", "int", "float", or "double".
    */
    QString getTypeString(e_initType type);

    /**
    * @brief Draws the boolean data points on the graph.
    * @param painter The QPainter used for rendering the boolean data.
    *
    * This function visually draws boolean data points on the graph using the
    * designated painter.
    */
    void drawBoolData(QPainter *painter);

    /**
    * @brief Draws the data points on the graph.
    * @param painter The QPainter used for rendering the data points.
    *
    * This function handles the rendering of various data points onto the graph,
    * considering their type and scaling.
    */
    void drawData(QPainter *painter);

    /**
    * @brief Calculates the height in pixels for a specific data point.
    * @param point The data point for which to calculate the height.
    * @return The calculated height in pixels for the given data point.
    */
    double pointHeightForValue(double point);

    /**
    * @brief Draws the X-axis on the graph.
    * @param leftPoint The left endpoint of the X-axis.
    * @param rightPoint The right endpoint of the X-axis.
    * @param painter The QPainter used for rendering the X-axis.
    */
    void drawXaxis(const QPoint leftPoint, const QPoint rightPoint, QPainter *painter);

    /**
     * @brief Draws the Y-axis on the graph.
     * @param topPoint The top endpoint of the Y-axis.
     * @param bottomPoint The bottom endpoint of the Y-axis.
     * @param painter The QPainter used for rendering the Y-axis.
     */
    void drawYaxis(const QPoint topPoint, const QPoint bottomPoint, QPainter *painter);

    /**
     * @brief Gets the height of the zero pixel position on the graph.
     * @return The height of the zero pixel position in pixels.
     */
    int getZeroPxHeight();

    /**
     * @brief Updates the current minimum and maximum values for the Y-axis based on data.
     *
     * This function recalculates the Y-axis limits based on the data points currently
     * being visualized, ensuring that the graph accurately represents the data range.
     */
    void getYcurrentMinMax();
private slots:
    /**
     * @brief Slot to handle the slider value change event.
     */
    void handlePlusButton_X();

    /**
     * @brief Slot to handle the minus button click event.
     */
    void handleMinusButton_X();

    /**
     * @brief Slot to handle the fit button click event.
     */
    void handleFitButton_X();

    /**
     * @brief Slot to handle the plus button click event.
     */
    void handlePlusButton_Y();

    /**
     * @brief Slot to handle the minus button click event.
     */
    void handleMinusButton_Y();

    /**
     * @brief Slot to handle the fit button click event.
     */
    void handleFitButton_Y();
};

/**
 * @brief Converts a QVector of various numeric types to QVector<double>.
 *
 * This function uses template specialization to handle different numeric types.
 * If the type is arithmetic, it converts the value to double and appends it
 * to the output vector. If the type is not convertible, a warning is logged.
 *
 * @tparam T The type of the input QVector.
 * @param inputVector The QVector to be converted.
 * @return QVector<double> The converted QVector containing double values.
 */
template <typename T>
QVector<double> convertToDoubleVector(const QVector<T>& inputVector)
{
    QVector<double> doubleVector;
    doubleVector.reserve(inputVector.size()); // Reserve space to avoid multiple allocations

    for (const T& value : inputVector)
    {
        if constexpr (std::is_arithmetic_v<T>) // Check if T is an arithmetic type
        {
            doubleVector.append(static_cast<double>(value));
        }
        else
        {
            qDebug() << "Type cannot be converted to double.";
        }
    }

    return doubleVector;
}

template <typename T>
void MultiTypesChrono::addPoint(const T& point)
{
    appendPoint(point);
}

template <typename T>
void MultiTypesChrono::addPoint(const QVector<T>& points)
{
    for (const T& point : points)
    {
        appendPoint(point);
    }
}

template <typename T>
void MultiTypesChrono::appendPoint(const T& point)
{
    bool validType = false;
    bool warningType = false;

    if constexpr (std::is_same<T, bool>::value)
    {         
        validType = (initType == BOOL);
        if (validType)
            dataPoints.append(static_cast<double>(point));
    }
    else if constexpr (std::is_same<T, int>::value)
    {
        validType = (initType == INT);
        if (validType)
            dataPoints.append(static_cast<double>(point));
    }
    else if constexpr (std::is_same<T, float>::value)
    {
        validType = (initType == FLOAT);
        if (validType)
            dataPoints.append(static_cast<double>(point));
    }
    else if constexpr (std::is_same<T, double>::value)
    {
        validType = (initType == DOUBLE);
        if (validType)
            dataPoints.append(point);
    }

    // Common update logic
    if (validType)
    {
        updateSliderRange();
        getMinMaxSize();
        update();
    }
    else
    {
        DebugWindow::getInstance()->addError("Wrong type used, graph type is " + getTypeString(initType) + ". Input is of type " + typeid(T).name());
    }
}


#endif // MULTITYPESCHRONO_H
