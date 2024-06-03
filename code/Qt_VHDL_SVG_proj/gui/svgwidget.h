#ifndef SVGWIDGET_H
#define SVGWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsSvgItem>
#include <QSlider>
#include <QDomDocument>



#define BASE_ZOOM 50

class SvgWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SvgWidget(QWidget *parent = nullptr);
    void loadSvg(const QString& filePath);
    void clearSvg();
    void clearScene();
    void setZoom(int value);

public slots:
    void highlightItemSlot(const QString &value);

private slots:
    void zoomChanged(int value);

protected:
    void wheelEvent(QWheelEvent *event) override;

private:

    struct s_style_lists
    {
        struct s_style
        {
            struct
            {
                QString name;
                QString value;
            } def, on;
        };
        QList<s_style> styles;
    };

    const s_style_lists styles_to_modify =
        {
        QList<s_style_lists::s_style>
        {
            {{"stroke", "#000000"}, {"stroke", "#ff0000"}}, // Stroke color
            {{"color", "black"}, {"color", "red"}}          // Text color
        }
    };

    QGraphicsView *graphicsView;
    QGraphicsSvgItem *svgItem;
    QSlider *zoomSlider;
    QString fileLocation;

    //mouse events
    QPoint lastMousePos;
    bool panning;

    int zoomValue = BASE_ZOOM;


    bool changeStrokeColorRecursive(QDomElement &element, const QString &elementLabel);
    bool changeElementStrokeColor(const QString &filePath, const QString &elementLabel);
    QString toggleStyleValue(const QString &currentValue, const QString &defaultValue, const QString &onValue);
    bool changeStyleRecursive(QDomElement &element, const QString &elementLabel);
    bool changeElementStyle(const QString &filePath, const QString &elementLabel);
    void recursivelyModifyElementStyle(QDomElement &element, const QString &elementLabel, bool &modified, bool applyToAllChildren);
};

#endif // SVGWIDGET_H
