#include "elementsdisplay.h"
#include <QTreeWidget>


ElementsDisplay::ElementsDisplay(QTreeWidget *treeWidget, QWidget *parent)
    : QWidget(parent)
{
    this->treeWidget = treeWidget;
    setupUi();
}

ElementsDisplay::~ElementsDisplay(){}

void ElementsDisplay::loadTree(const s_components_list &components)
{
    clearTree();
    populateTree(components);
}

void ElementsDisplay::setupUi()
{
    treeWidget->setColumnCount(2);
    treeWidget->setHeaderLabels(QStringList() << "Property" << "Value");
}

void ElementsDisplay::clearTree()
{
    treeWidget->clear();
}

void ElementsDisplay::populateTree(const s_components_list &components)
{
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(treeWidget);
    rootItem->setText(0, "All Components");

    // Elements
    QTreeWidgetItem *elementsItem = new QTreeWidgetItem(rootItem);
    elementsItem->setText(0, "Elements");
    for (const auto &element : components.elements.elements_list)
    {
        QTreeWidgetItem *elementItem = new QTreeWidgetItem(elementsItem);
        elementItem->setText(0, element.name);
        elementItem->addChild(new QTreeWidgetItem(QStringList() << "Device" << element.device));
        elementItem->addChild(new QTreeWidgetItem(QStringList() << "Label" << element.label));
        elementItem->addChild(new QTreeWidgetItem(QStringList() << "Inputs Number" << QString::number(element.inputs_number)));
        elementItem->addChild(new QTreeWidgetItem(QStringList() << "Outputs Number" << QString::number(element.outputs_number)));
        // ... Add inputs and outputs in similar way
    }

    // Simulation I/Os
    QTreeWidgetItem *simIOsItem = new QTreeWidgetItem(rootItem);
    simIOsItem->setText(0, "Simulation I/Os");
    for (const auto &simIO : components.simulation_IOs.i_os)
    {
        QTreeWidgetItem *simIOItem = new QTreeWidgetItem(simIOsItem);
        simIOItem->setText(0, simIO.name);
        simIOItem->addChild(new QTreeWidgetItem(QStringList() << "Width" << QString::number(simIO.width)));
        simIOItem->addChild(new QTreeWidgetItem(QStringList() << "Type" << QString::number(simIO.type)));
        simIOItem->addChild(new QTreeWidgetItem(QStringList() << "Connected To" << simIO.connected_to));
    }

    // Simulation Wires
    QTreeWidgetItem *simWiresItem = new QTreeWidgetItem(rootItem);
    simWiresItem->setText(0, "Simulation Wires");
    for (const auto &wire : components.simulation_wires.wires)
    {
        QTreeWidgetItem *wireItem = new QTreeWidgetItem(simWiresItem);
        wireItem->setText(0, wire.name);
        wireItem->addChild(new QTreeWidgetItem(QStringList() << "Width" << QString::number(wire.width)));
        wireItem->addChild(new QTreeWidgetItem(QStringList() << "Connected To" << wire.connected_to));
    }

    treeWidget->addTopLevelItem(rootItem);
    treeWidget->expandAll();
}
