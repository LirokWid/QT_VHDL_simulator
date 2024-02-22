#include "svg_link_parser.h"

SvgLinkParser::SvgLinkParser(QString svg_file)
{
    parseSvg(svg_file);
}

void SvgLinkParser::list_attributes(const std::vector<QDomElement>& elements)
{
    for (const QDomElement& element : elements) {
        qDebug() << "Attributes of group:" << element.attribute("id");
        QDomNamedNodeMap attributes = element.attributes();
        for (int i = 0; i < attributes.size(); ++i) {
            QDomAttr attribute = attributes.item(i).toAttr();
            qDebug() << attribute.nodeName() << ":" << attribute.nodeValue();
        }
    }
}
void SvgLinkParser::parseSvg(QString svg_file) {
    // Open the SVG file
    QFile file(svg_file);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file:" << file.errorString();
        return;
    }

    // Create a new XML reader
    QDomDocument doc;
    if (!doc.setContent(&file)) {
        qDebug() << "Failed to parse SVG file as XML.";
    }

    //1.Parse the SVG in different groups where <g> is the parent element and contains an ID
    std::vector<QDomElement> subgroups = parse_by_group(doc);


    //2. Parse the elements of each <g> element
    for(const QDomElement& subgroup : subgroups)
    {
        parse_group_elements(subgroup);



        QDomNamedNodeMap subgroup_attributes = subgroup.attributes();
        for( int i = 0 ; i < subgroup_attributes.length() ; ++i )
        {
            QDomNode attribute = subgroup_attributes.item(i);
            qDebug() << "Attribute:" << attribute.nodeName() << "Value:" << attribute.nodeValue();
        }


        /*
        //Extract element attributes
        for(const QDomElement& attribute:subgroup.attributes())
        {
            if (attribute.name().toString() == custom_attribute)
            {//If we are in a element that contains the custom attribute
                //Append the element ID to the list
                if (xml.attributes().hasAttribute("id"))
                {
                    QString value = xml.attributes().value("id").toString();

                    // Compare the component name with predefined constants
                    if (value.compare(component_ID) == 0) {
                        element.label = value;
                        qDebug() << "ID:" << element.label;
                    }
                    else if (value.compare(component_type) == 0) {
                        element.type = value;
                        qDebug() << "Type:" << element.type;
                    }
                    else if (value.compare(component_input) == 0) {
                        element.inputs.push_back(value);
                        qDebug() << "Input:" << element.inputs.back();
                    }
                    else if (value.compare(component_output) == 0) {
                        element.outputs.push_back(value);
                        qDebug() << "Output:" << element.outputs.back();
                    }
                    else if (value.compare(component_sel) == 0) {
                        element.other_attributes.push_back(value);
                        qDebug() << "Other:" << element.other_attributes.back();
                    }
                    else {
                        element.other_attributes.push_back("ATTRIBUTE NOT RECOGNIZED");
                        qDebug() << "Other:" << element.other_attributes.back();
                    }
                }
            }
        }
*/

    }

    //list_attributes(subgroups);


    file.close();
}

std::vector<QDomElement> SvgLinkParser::parse_by_group(const QDomDocument& doc)
{
    std::vector<QDomElement> childGroups;
    QDomElement root = doc.documentElement();// Returns the svg main element

    //Get first group inside of the main group
    QDomElement g_group     = root.firstChildElement("g");
    QDomElement first_group = g_group.firstChildElement("g");

    qDebug() << "First group:" << first_group.tagName() << "id:" << first_group.attribute("id");
    Q_ASSERT(!first_group.isNull() && "No groups found in the SVG file");


    // Iterate over all child elements of the root
    for (QDomElement elem = first_group; !elem.isNull(); elem = elem.nextSiblingElement())
    {
        // If the element is a group and has an ID, add it to the list
        if (elem.tagName() == "g" && !elem.attribute("id").isEmpty())
        {
            childGroups.push_back(elem);
        }
    }
    //Returns a vector of QDomElements that are all the groups of the SVG that have a valid ID
    return childGroups;
}

std::vector<QDomElement> SvgLinkParser::parse_group_elements(QDomElement group)
{
    for(int i = 0; i < group.childNodes().size(); ++i)
    {
        QDomNode node = group.childNodes().item(i);
        if(node.isElement())
        {
            QDomElement element = node.toElement();
            qDebug() << "Element:" << element.tagName() << "id:" << element.attribute("id")<< "custom_attr:" << element.attribute(custom_attribute);
        }
    }
}

void listAttributes(const QXmlStreamAttributes& attributes, int indent = 0)
{
    foreach (const QXmlStreamAttribute& attribute, attributes)
    {
        QString attributeName = attribute.name().toString();
        QString attributeValue = attribute.value().toString();
        QString indentation = QString(indent, ' ');
        qDebug().noquote() << indentation << "Attribute:" << attributeName << "Value:" << attributeValue;
    }
}
