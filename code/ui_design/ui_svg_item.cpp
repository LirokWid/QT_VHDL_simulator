/*
#include "ui_svg_item.h"
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QSvgRenderer>

ui_svg_item::ui_svg_item(QString svgPath)
{
    this->svgPath = svgPath;
    setFlags(ItemIsMovable);

    //TODO file error checking
    this->renderer = new QSvgRenderer(svgPath);


}


void ui_svg_item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget); // may be needed

    const qreal levelOfDetails = option->levelOfDetailFromTransform(painter->worldTransform());


    //load svg
}
*/
