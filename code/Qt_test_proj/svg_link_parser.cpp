#include "svg_link_parser.h"

SvgLinkParser::SvgLinkParser(QString svg_file)
{
    log_buffer = "";
    if(svg_file.isEmpty())
    {
        qDebug() << "Error: Empty file name";
        return;
    }
    this->svg_file = svg_file;
    parse_svg(this->svg_file);

    //Get the tree of the elements in the SVG file
    generate_tree(root, "  ");
    print_tree_in_log();
}


void SvgLinkParser::parse_svg(QString svg_file)
{
    // Open the SVG file
    QFile file(svg_file);
    QDomDocument doc;
    try
    {
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            throw std::runtime_error("Failed to open SVG file");
        }
        if (!doc.setContent(&file)) {
            qDebug() << "Failed to parse SVG file as XML.";
        }
    }
    catch(const std::exception& e)
    {
        qDebug() << "Error: " << e.what();
        return;
    }


    // Create the root node of the tree and get the main SVG element infos
    root.element = doc.documentElement();
    extractNodeInfo(root.element, root.info);

    //1.Parse the SVG in different groups where <g> is the parent element and contains an ID
    groups_number = 0;
    parse_by_group(root.element,root,0);

    // Output found groups
    qDebug() << "Found " << groups_number << " groups:";

    file.close();
}

void SvgLinkParser::parse_by_group(const QDomNode& node, TreeNode& parentNode, int level)
{//Recursively find all groups nodes in a node and return a vector of QDomElements
    QDomNodeList children = node.childNodes();
    for (int i = 0; i < children.count(); ++i)
    {
        QDomNode childNode = children.at(i);
        if (childNode.isElement())
        {
            QDomElement childElement = childNode.toElement();
            if (!childElement.isNull() && childElement.tagName() == "g")
            {
                ++groups_number;
                TreeNode childTreeNode;
                childTreeNode.element = childElement;
                extractNodeInfo(childElement, childTreeNode.info);
                childTreeNode.level = level + 1; // Increment the level
                parentNode.children.push_back(childTreeNode);
                parse_by_group(childElement, parentNode.children.back(), level + 1);
            }
        }
    }
}

void SvgLinkParser::extractNodeInfo(const QDomElement& element, TreeNodeInfo& info)
{
    info.tag_name = element.tagName();
    info.id = element.attribute("id", "");
}

void SvgLinkParser::generate_tree(const TreeNode& node, const QString& prefix)
{
    QString adjusted_prefix = prefix;
    QString log_content; // Accumulate log output

    if (!prefix.isEmpty()) {
        log_content += prefix;
        log_content += "|";
    }
    log_content += "- " + node.info.tag_name;

    if (!node.info.id.isEmpty()) {
        log_content += " (id: " + node.info.id + ")";
    }
    log_content += " - Level: " + QString::number(node.level);

    //Accumulate log output into a buffer
    log_buffer += log_content + "\n";


    for (size_t i = 0; i < node.children.size(); ++i) {
        if (i == node.children.size() - 1) {
            adjusted_prefix += "   "; // Adjust prefix for the last child
        } else {
            adjusted_prefix += "|  "; // Adjust prefix for other children
        }
        generate_tree(node.children[i], adjusted_prefix);
        adjusted_prefix = prefix; // Reset prefix for next child
    }
}

void SvgLinkParser::print_tree_in_log()
{
    qDebug() << "SVG Groups Tree:";

    QStringList lines = log_buffer.split("\n", Qt::SkipEmptyParts);

    // Print log content from first line to last line
    for (int i = 0; i < lines.size(); ++i)
    {
        qDebug().noquote().nospace() << lines[i];
    }

    qDebug() << "End of SVG Groups Tree";
    //reset the log buffer
    log_buffer = "";
}

void SvgLinkParser::parse_group_elements(QDomElement group)
{
    element_io_list new_element;

    for(int i = 0; i < group.childNodes().size(); ++i)
    {//For each node in the group
        QDomNode node = group.childNodes().item(i);
        if(node.isElement())
        {
            QDomElement element = node.toElement();
            if (element.hasAttribute(custom_attribute))
            {
                if (element.hasAttribute(custom_attribute))
                {
                    // Compare the component name with predefined constants
                    if (element.attribute(custom_attribute).compare(component_ID) == 0)
                    {
                        new_element.label = element.attribute(component_ID);
                        qDebug() << "ID:" << new_element.label;
                    }
                    else if (element.attribute(custom_attribute).compare(component_type) == 0)
                    {
                        new_element.type = element.attribute(component_type);
                        qDebug() << "Type:" << new_element.type;
                    }
                    else if (element.attribute(custom_attribute).compare(component_input) == 0)
                    {
                        new_element.inputs.push_back(element.attribute(component_input));
                        qDebug() << "Input:" << new_element.inputs.back();
                    }
                    else if (element.attribute(custom_attribute).compare(component_output) == 0)
                    {
                        new_element.outputs.push_back(element.attribute(component_output));
                        qDebug() << "Output:" << new_element.outputs.back();
                    }
                    else if (element.attribute(custom_attribute).compare(component_sel) == 0)
                    {
                        new_element.other_attributes.push_back(element.attribute(component_sel));
                        qDebug() << "Other:" << new_element.other_attributes.back();
                    }
                    else {
                        new_element.other_attributes.push_back("ATTRIBUTE NOT RECOGNIZED");
                        qDebug() << "Other:" <<  new_element.other_attributes.back();
                    }
                }
            }
            qDebug() << "Element:" << element.tagName() << "id:" << element.attribute("id")<< "custom_attr:" << element.attribute(custom_attribute);
        }
    }
    this->elements.push_back(new_element);
}
