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
};

#endif // SVGWIDGET_H
