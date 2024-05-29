#ifndef ELEMENTSDISPLAY_H
#define ELEMENTSDISPLAY_H

#include <QWidget>
#include <QVBoxLayout>

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "ComponentsStruct.h"

class ElementsDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit ElementsDisplay(
        QTreeWidget *treeWidget,
        QWidget *parent = nullptr);

    ~ElementsDisplay();

    void loadTree(const s_components_list &components);
    void clearTree();

private:


    s_components_list *components;
    QVBoxLayout *mainLayout;
    QTreeView *place;
    QTreeWidget *treeWidget;

    void addStateIcon(QTreeWidgetItem *item, bool error);
    void addProperty(QTreeWidgetItem *parent, const QString &property, const QString &value);
    void populateTree(const s_components_list &components);
    void addElements(QTreeWidgetItem *parent, const s_elements &elements);
    void idleUi();
    void addSimulationIOs(QTreeWidgetItem *parent, const s_sim_I_Os &simIOs);
    void addSimulationWires(QTreeWidgetItem *parent, const s_sim_wires &wires);
    void setTextColor(QTreeWidgetItem *item, const QColor &color);
};

#endif // ELEMENTSDISPLAY_H
