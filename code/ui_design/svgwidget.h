#ifndef SVGWIDGET_H
#define SVGWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsSvgItem>
#include <QSlider>



#define BASE_ZOOM 50

class SvgWidget : public QWidget {
    Q_OBJECT
public:
    explicit SvgWidget(QWidget *parent = nullptr);
    void loadSvg(const QString& filePath);
    void clearScene();
    void setZoom(int value);

private slots:
    void zoomChanged(int value);

protected:
    void wheelEvent(QWheelEvent *event) override;

private:
    QGraphicsView *graphicsView;
    QGraphicsSvgItem *svgItem;
    QSlider *zoomSlider;

    //mouse events
    QPoint lastMousePos;
    bool panning;

    int zoomValue = BASE_ZOOM;
};

#endif // SVGWIDGET_H
