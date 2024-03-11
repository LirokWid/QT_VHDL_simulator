#include "svg_link_parser.h"

SvgLinkParser::SvgLinkParser(QString svg_file)
{
    log_buffer = "";
    links_count = 0;
    root.level = 0;

    if(svg_file.isEmpty())
    {
        qDebug() << "Error: Empty file name";
        return;
    }
    this->svg_file = svg_file;
    parse_svg(this->svg_file);

    //Get the tree of the elements in the SVG file
    generate_tree(root, "  ");
    print_tree_in_log(log_buffer);

    //Generate the electrical links from the SVG file
    parse_links(root);
}

SvgLinkParser::~SvgLinkParser()
{
    //Destructor
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
        if (!doc.setContent(&file))
        {
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
    get_group_header(root.element, root.infos);

    //1.Parse the SVG in different groups where <g> is the parent element and contains an ID
    groups_number = 0;
    parse_by_group(root.element,root,0);

    // Output found groups
    qDebug() << "Found " << groups_number << " groups:";

    file.close();
}

void SvgLinkParser::parse_by_group(const QDomNode& node, s_tree_node& parentNode, int level)
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
                s_tree_node childTreeNode;
                childTreeNode.element = childElement;
                get_group_header(childElement, childTreeNode.infos);
                childTreeNode.level = level + 1; // Increment the level
                parentNode.children.push_back(childTreeNode);
                parse_by_group(childElement, parentNode.children.back(), level + 1);
            }
        }
    }
}

void SvgLinkParser::generate_tree(const s_tree_node& node, const QString& prefix)
{
    QString adjusted_prefix = prefix;
    QString log_content; // Accumulate log output

    if (!prefix.isEmpty()) {
        log_content += prefix;
        log_content += "|";
    }
    log_content += "- " + node.infos.tag_name;

    if (!node.infos.id.isEmpty()) {
        log_content += " (id: " + node.infos.id + ")";
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

void SvgLinkParser::print_tree_in_log(QString buffer)
{
    qDebug() << "SVG Groups Tree:";
    QStringList lines = buffer.split("\n", Qt::SkipEmptyParts);

    // Print log content from first line to last line
    for (int i = 0; i < lines.size(); ++i)
    {
        qDebug().noquote().nospace() << lines[i];
    }

    qDebug() << "End of SVG Groups Tree";
    //reset the log buffer
    buffer = "";
}

void SvgLinkParser::parse_links(s_tree_node& node)
{
    //Parse the current node's element
    parse_group_elements(node);

    qDebug() << "Parsing links for node: " << node.infos.tag_name << " (id: " << node.infos.id << ") at level " << node.level << " ...";
    //Recursively call parse_tree_nodes()
    for (s_tree_node& child : node.children)
    {
        parse_links(child);
    }
}

QString SvgLinkParser::get_attr_for_string(QString attr)
{
    return custom_attribute + ":" + attr;
}

QString SvgLinkParser::get_attr_for_type(e_types type)
{
    switch (type)
    {
    case e_types::label:
        return custom_attribute+":label";
        break;
    case e_types::type:
        return custom_attribute+":type";
        break;
    case e_types::graphic:
        return custom_attribute+":graphic";
        break;
    case e_types::input:
        return custom_attribute+":input";
        break;
    case e_types::output:
        return custom_attribute+":output";
        break;
    case e_types::sel:
        return custom_attribute+":sel";
        break;
    case e_types::wire:
        return custom_attribute+":wire";
        break;
    default:
        return "";
        break;

    }
}

void SvgLinkParser::get_group_header(const QDomElement& element, s_tree_node_info& infos)
{
    //We should consistently have those infos

    //For the device type(MUX, AND, OR, etc)
    if(element.hasAttribute(get_attr_for_string("device")))
    {
        infos.type = element.attribute(get_attr_for_string("device"));
    }else{
        infos.type = "";
        infos.is_parse_error = true;
        infos.error_messages.push_back("No device attribute found");
    }

    //For the i/o width
    if(element.hasAttribute(get_attr_for_string("width")))
    {
        QString str_width = element.attribute(get_attr_for_string("width"));
        //Width is in the form "x,y" where x is the inputs width and y the outputs width
        QStringList widths = str_width.split(",");
        if(widths.size() == 2)
        {
            infos.component_in_width = widths[0].toInt();
            infos.component_out_width = widths[1].toInt();
        }else{
            infos.component_in_width = 0;
            infos.component_out_width = 0;
            infos.is_parse_error = true;
            infos.error_messages.push_back("Invalid width attribute format");
        }
    }else{
        infos.component_in_width = 0;
        infos.component_out_width = 0;
        infos.is_parse_error = true;
        infos.error_messages.push_back("No width attribute found");
    }
}

void SvgLinkParser::parse_group_elements(s_tree_node& node)
{
    QDomElement node_xml_data = node.element;//Store the group element svg data
    s_tree_node_info *parsed_data = &node.infos;//Store parsed data here

    parsed_data->is_parse_error = false;
    parsed_data->has_been_parsed = false;

    if(node_xml_data.tagName() == "svg")
    {
        //Main element, contains groups, wires, inputs and outputs
        //We should only have one svg element
        if(parsed_data->has_been_parsed)
        {
            qDebug() << "Error: Main element tried parsing again";
            return;
        }

        //Parse the simulator inputs and outputs
        QList<sim_I_O> sim_I_Os;
        sim_I_Os = get_simulation_IO(node_xml_data);


        /*
        //Parse the wires
        QList<QDomElement> wires_found;
        find_elements_with_attribute(
            node_xml_data,
            get_attr_for_type(e_types::wire),
            wires_found);

        for(QDomElement wire : wires_found)
        {
            //Parse the wire
            if(wire.hasAttribute(get_attr_for_string("")))
            {
                //Get the label
                QString label = wire.attribute(get_attr_for_type(e_types::label));
            }else{
                //Error
            }
            //Get the input and output
            //Get the input and output width
            //Get the input and output type
            //Add the wire to the list of wires
        }
*/
        //No header for the main element, hard code the values
        parsed_data->type = "main";
        parsed_data->component_in_width = 0;
        parsed_data->component_out_width = 0;

        parsed_data->has_been_parsed = true;


    }else if(node_xml_data.tagName() == "g")
    {
        //Group element, contains components
    }else if(node_xml_data.tagName() == "")

    //Q_ASSERT(node_svg_data.tagName() == "g");
    // if(node_svg_data.tagName() != "g")
    // {
    //     qDebug() << "Error: Not a group node";
    // }

    //Firstly, parse the main group attributes (device, width)
    get_group_header(node_xml_data, *parsed_data);

    //For every element in a node
    //TODO : (if the node isn't a group of nodes)
    //Find if the element is a component
    //If it is, add it to the list of components
    for(int i = 0; node_xml_data.isNull(); node_xml_data.nextSiblingElement())
    {//For each node in the group
        int breakpoint = 0;
        breakpoint++;

        qDebug() << "Parsing node " << breakpoint;

        //TODO: SIGSEGV here
        QDomElement element = node_xml_data.childNodes().item(i).toElement();

        //Get the element type by comparing to the list of predefined types

        // Node is a text containing the label U1, U2, etc
        if (element.hasAttribute(get_attr_for_type(label)))
        {
            parsed_data->label = element.attribute(get_attr_for_type(label));
            qDebug() << "ID:" << parsed_data->label;
        }

        // Node is a text containing the type
        else if (element.hasAttribute(get_attr_for_type(type)))
        {
            parsed_data->type = element.attribute(get_attr_for_type(type));
            qDebug() << "Type:" << parsed_data->type;
        }

        // Node is a text containing the graphic
        else if (element.hasAttribute(get_attr_for_type(graphic)))
        {//Do nothing yet for the graphic
            qDebug() << "Graphic";
        }

        // Node is an input
        else if (element.hasAttribute(get_attr_for_type(input)))
        {
            s_io new_input;
            new_input.name = element.attribute(get_attr_for_type(input));
            //The input width should be contained in the main group attr sim:width=10,10
            //We need to get the width and height of the main group
            //And overide if the input has a width attribute
            //Get already parsed widht
            if(element.hasAttribute(get_attr_for_string("width")))
            {
                new_input.width = element.attribute(get_attr_for_string("width")).toInt();
                qDebug() << "Input width overidden by attribute";
            }else{
                if(parsed_data->component_in_width != 0)
                {
                    new_input.width = parsed_data->component_in_width;
                }else{
                    new_input.width = 0;
                    parsed_data->is_parse_error = true;
                    parsed_data->error_messages.push_back("No width attribute found for input");
                }
            }
        }

        // Node is an output
        else if (element.hasAttribute(get_attr_for_type(output)))
        {
            s_io new_output;
            new_output.name = element.attribute(get_attr_for_type(output));
            //Same as for input
            if(element.hasAttribute(get_attr_for_string("width")))
            {
                new_output.width = element.attribute(get_attr_for_string("width")).toInt();
                qDebug() << "Input width overidden by attribute";
            }else{
                if(parsed_data->component_in_width != 0)
                {
                    new_output.width = parsed_data->component_out_width;
                }else{
                    new_output.width = 0;
                    parsed_data->is_parse_error = true;
                    parsed_data->error_messages.push_back("No width attribute found for input");
                }
            }
        }
        //qDebug() << "Element:" << element.tagName() << "id:" << element.attribute("id")<< "custom_attr:" << element.attribute(custom_attribute);
    }
}

QList<SvgLinkParser::sim_I_O> SvgLinkParser::get_simulation_IO(const QDomElement svg_group_xml)
{

    QList<QDomElement> inputs_found;

    QList<sim_I_O> sim_I_Os;

    find_elements_with_attribute(
        svg_group_xml,
        get_attr_for_type(type),
        "sim_input",
        inputs_found);


    for(const QDomElement& sim_input : inputs_found)
    {
        //Parse the name
        QString name = DEFAULT_INPUT_NAME;
        if(sim_input.hasAttribute(IO_NAME_ATTRIBUTE))
        {
            name = sim_input.attribute(IO_NAME_ATTRIBUTE);
        }else{
            qDebug() << "Warning: No name attribute found for input";
        }

        //Parse the width
        int width = DEFAULT_WIDTH;
        //Parse the input
        if(sim_input.hasAttribute(get_attr_for_string("width")))
        {//Overide the width if it's found
            //Get the width
            width = sim_input.attribute(get_attr_for_string("width")).toInt();
        }else{
            qDebug() << "Warning: No width attribute found for input";
        }
        sim_I_Os.append({name, width, INPUT});
    }

    QList<QDomElement> outputs_found;
    find_elements_with_attribute(
        svg_group_xml,
        get_attr_for_type(type),
        "sim_output",
        outputs_found);

    for(const QDomElement& sim_output : outputs_found)
    {
        //Parse the name
        QString name = DEFAULT_OUTPUT_NAME;
        if(sim_output.hasAttribute(IO_NAME_ATTRIBUTE))
        {
            name = sim_output.attribute(IO_NAME_ATTRIBUTE);
        }else{
            qDebug() << "Warning: No name attribute found for output";
        }

        //Parse the width
        int width = DEFAULT_WIDTH;
        //Parse the input
        if(sim_output.hasAttribute(get_attr_for_string("width")))
        {//Overide the width if it's found
            //Get the width
            width = sim_output.attribute(get_attr_for_string("width")).toInt();
        }else{
            qDebug() << "Warning: No width attribute found for output";
        }
        sim_I_Os.append({name, width, OUTPUT});
    }
    return sim_I_Os;
}

void SvgLinkParser::find_elements_with_attribute(const QDomElement elem_to_look_into, const QString attr_name, const QString attr_value, QList<QDomElement>& found_elements)
{
    //Find attributes with a fixed value
    QDomElement child = elem_to_look_into.firstChildElement();
    while( !child.isNull() )
    {
        if( child.attributes().contains(attr_name))
        {
            if(child.attribute(attr_name) == attr_value)
            {
                qDebug() << child.attribute(attr_name) << "==" << attr_value;
                found_elements.append(child);
            }
        }
        child = child.nextSiblingElement();
    }
}
