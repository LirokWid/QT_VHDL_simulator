// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause
/*
#ifndef VIEW_H
#define VIEW_H

#include <QFrame>
#include <QGraphicsView>
#include <QGraphicsSvgItem>
#include <QSvgWidget>

QT_BEGIN_NAMESPACE
class QLabel;
class QSlider;
class QToolButton;
QT_END_NAMESPACE

class View;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(View *v) : QGraphicsView(), view(v) { }

protected:

private:
    View *view;
};

class View : public QFrame
{
    Q_OBJECT
public:
    explicit View(const QString &name, QWidget *parent = nullptr);

    QGraphicsView *view() const;

public slots:
    void zoomIn();
    void zoomOut();
    void zoomInBy(int level);
    void zoomOutBy(int level);

private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void togglePointerMode();

private:
    GraphicsView    *graphicsView;
    QLabel          *label;
    QLabel          *label2;
    QToolButton     *selectModeButton;
    QToolButton     *dragModeButton;
    QToolButton     *antialiasButton;
    QToolButton     *printButton;
    QToolButton     *resetButton;
    QSlider         *zoomSlider;
};

#endif // VIEW_H
*/
