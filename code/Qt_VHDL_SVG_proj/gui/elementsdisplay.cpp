#include "elementsdisplay.h"
#include <QTreeWidget>


ElementsDisplay::ElementsDisplay(QTreeWidget *treeWidget, QWidget *parent)
    : QWidget(parent),
    treeWidget(treeWidget)
{
    idleUi();
}

ElementsDisplay::~ElementsDisplay(){}

void ElementsDisplay::loadTree(const s_components_list &components)
{
    treeWidget->clear();
    treeWidget->setColumnCount(3);
    treeWidget->setHeaderLabels(QStringList() << "Property" << "Value" << "State");
    populateTree(components);
}


void ElementsDisplay::clearTree()
{
    treeWidget->clear();
    idleUi();
}

void ElementsDisplay::populateTree(const s_components_list &components)
{
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(treeWidget);
    rootItem->setText(0, "All Components");



    addElements(rootItem, components.elements);
    addSimulationIOs(rootItem, components.simulation_IOs);
    addSimulationWires(rootItem, components.simulation_wires);
    /*
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
    */
    treeWidget->addTopLevelItem(rootItem);
    treeWidget->expandToDepth(1);
}

void ElementsDisplay::addElements(QTreeWidgetItem *parent, const s_elements &elements)
{
    QTreeWidgetItem *elementsItem = new QTreeWidgetItem(parent);
    elementsItem->setText(0, "Elements");

    for (const auto &element : elements.elements_list)
    {
        QTreeWidgetItem *elementItem = new QTreeWidgetItem(elementsItem);
        elementItem->setText(0, element.name);
        addProperty(elementItem, "Device", element.device);
        addProperty(elementItem, "Label", element.label);
        addProperty(elementItem, "Inputs Number", QString::number(element.inputs_number));
        addProperty(elementItem, "Outputs Number", QString::number(element.outputs_number));
        addStateIcon(elementItem, element.error.is_parse_error);
        // Add inputs and outputs similarly if needed
    }
    //add main error ? TODO
}

void ElementsDisplay::addSimulationIOs(QTreeWidgetItem *parent, const s_sim_I_Os &simIOs)
{
    QTreeWidgetItem *simIOsItem = new QTreeWidgetItem(parent);
    simIOsItem->setText(0, "Simulation I/Os");

    for (const auto &simIO : simIOs.i_os)
    {
        QTreeWidgetItem *simIOItem = new QTreeWidgetItem(simIOsItem);
        simIOItem->setText(0, simIO.name);
        addProperty(simIOItem, "Width", QString::number(simIO.width));

        switch (simIO.type)
        {
        case INPUT:
            addProperty(simIOItem, "Type", "Input");
            break;
        case OUTPUT:
            addProperty(simIOItem, "Type", "Output");
            break;
        case UNDEFINED:
            addProperty(simIOItem, "Type", "Undefined");
            break;
        default:
            addProperty(simIOItem, "Type", "Unknown");
            break;
        }
        addProperty(simIOItem, "Connected To", simIO.connected_to);
        addStateIcon(simIOItem, simIO.error.is_parse_error);
    }
}

void ElementsDisplay::addSimulationWires(QTreeWidgetItem *parent, const s_sim_wires &wires)
{
    QTreeWidgetItem *simWiresItem = new QTreeWidgetItem(parent);
    simWiresItem->setText(0, "Simulation Wires");

    for (const auto &wire : wires.wires)
    {
        QTreeWidgetItem *wireItem = new QTreeWidgetItem(simWiresItem);
        wireItem->setText(0, wire.name);
        addProperty(wireItem, "Width", QString::number(wire.width));
        addProperty(wireItem, "Connected To", wire.connected_to);
        addStateIcon(wireItem, wire.error.is_parse_error);

        if (wire.error.is_parse_error)
        {
            setTextColor(wireItem, Qt::red);
        }
    }
}

void ElementsDisplay::addProperty(QTreeWidgetItem *parent, const QString &property, const QString &value)
{
    QTreeWidgetItem *propertyItem = new QTreeWidgetItem(parent);
    propertyItem->setText(0, property);
    propertyItem->setText(1, value);
}

void ElementsDisplay::addStateIcon(QTreeWidgetItem *item, bool error)
{
    QIcon icon = error ? QIcon(":/icons/error.png") : QIcon(":/icons/ok.png");
    item->setIcon(2, icon);
}

void ElementsDisplay::setTextColor(QTreeWidgetItem *item, const QColor &color)
{
    for (int i = 0; i < item->columnCount(); ++i) {
        item->setForeground(i, QBrush(color));
    }
}

void ElementsDisplay::idleUi()
{
    treeWidget->setColumnCount(1);
    treeWidget->setHeaderLabels(QStringList() << tr("Parser info"));
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(treeWidget);
    rootItem->setText(0, "Ready to parse SVG file.");
    treeWidget->addTopLevelItem(rootItem);
}
