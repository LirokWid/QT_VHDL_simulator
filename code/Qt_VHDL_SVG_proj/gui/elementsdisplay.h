#ifndef ELEMENTSDISPLAY_H
#define ELEMENTSDISPLAY_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include "ComponentsStruct.h"

/**
 * @class ElementsDisplay
 * @brief A widget that displays parsed information from an SVG file in a tree structure.
 *
 * This class manages the display of components, simulation I/Os, and simulation wires
 * from an SVG file. It organizes the data in a QTreeWidget and supports interaction
 * via mouse clicks to signal element selections.
 */
class ElementsDisplay : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for ElementsDisplay.
     *
     * Initializes the ElementsDisplay widget with a given QTreeWidget and an optional parent widget.
     *
     * @param treeWidget The QTreeWidget that will be used to display the elements.
     * @param parent Optional parent widget (default is nullptr).
     */
    explicit ElementsDisplay(QTreeWidget *treeWidget, QWidget *parent = nullptr);

    /**
     * @brief Destructor for ElementsDisplay.
     */
    ~ElementsDisplay();

    /**
     * @brief Loads the parsed data into the tree widget.
     *
     * Populates the tree widget with components, simulation I/Os, and wires from the parsed SVG file.
     *
     * @param components A structure containing parsed components, simulation I/Os, and wires.
     */
    void loadTree(const s_components_list &components);

    /**
     * @brief Clears the tree widget.
     *
     * Clears all the data displayed in the tree widget and resets the UI to an idle state.
     */
    void clearTree();

signals:
    /**
     * @brief Signal emitted when an element in the tree is clicked.
     *
     * @param value The text value of the clicked item in the tree.
     */
    void elementClicked(const QString &value);

private:

    s_components_list *components;  ///< A pointer to the structure containing the parsed components.
    QVBoxLayout *mainLayout;        ///< The main layout for the widget.
    QTreeView *place;               ///< The QTreeView to place additional elements (if any).
    QTreeWidget *treeWidget;        ///< The QTreeWidget that displays the elements.

    /**
     * @brief Adds an error state icon to a tree item.
     *
     * Adds an error or success icon to a given QTreeWidgetItem depending on the error state.
     *
     * @param item The tree item to which the icon will be added.
     * @param error A boolean indicating whether an error is present (true if there is an error).
     */
    void addStateIcon(QTreeWidgetItem *item, bool error);

    /**
     * @brief Adds a property and its value as child items to a tree item.
     *
     * @param parent The parent tree item to which the property will be added.
     * @param property The name of the property.
     * @param value The value of the property.
     */
    void addProperty(QTreeWidgetItem *parent, const QString &property, const QString &value);

    /**
     * @brief Populates the tree widget with components data.
     *
     * @param components The structure containing components to populate the tree.
     */
    void populateTree(const s_components_list &components);

    /**
     * @brief Adds elements data to a tree item.
     *
     * @param parent The parent tree item under which elements will be added.
     * @param elements The structure containing elements data.
     */
    void addElements(QTreeWidgetItem *parent, const s_elements &elements);

    /**
     * @brief Resets the UI to an idle state.
     *
     * Clears the tree widget and sets up a default message indicating the widget is ready for new data.
     */
    void idleUi();

    /**
     * @brief Adds simulation I/Os to a tree item.
     *
     * @param parent The parent tree item under which simulation I/Os will be added.
     * @param simIOs The structure containing simulation I/Os data.
     */
    void addSimulationIOs(QTreeWidgetItem *parent, const s_sim_I_Os &simIOs);

    /**
     * @brief Adds simulation wires to a tree item.
     *
     * @param parent The parent tree item under which simulation wires will be added.
     * @param wires The structure containing simulation wires data.
     */
    void addSimulationWires(QTreeWidgetItem *parent, const s_sim_wires &wires);

    /**
     * @brief Sets the text color of a tree item.
     *
     * Changes the color of text for all columns in a given tree item.
     *
     * @param item The tree item whose text color will be set.
     * @param color The color to apply to the text.
     */
    void setTextColor(QTreeWidgetItem *item, const QColor &color);

private slots:
    /**
     * @brief Handles item click events in the tree.
     *
     * Emits the elementClicked signal when an item is clicked, passing the item's text.
     *
     * @param item The clicked tree item.
     * @param column The column of the clicked item.
     */
    void handleItemClicked(QTreeWidgetItem *item, int column);
};

#endif // ELEMENTSDISPLAY_H
