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

private:
    void setupUi();
    void populateTree(const s_components_list &components);
    void clearTree();

    s_components_list *components;
    QVBoxLayout *mainLayout;
    QTreeView *place;
    QTreeWidget *treeWidget;
};

#endif // ELEMENTSDISPLAY_H
