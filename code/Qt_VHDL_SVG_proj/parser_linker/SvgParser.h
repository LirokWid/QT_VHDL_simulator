/**
 * @author LiroKwid
 * @date 02-2024
 * @file svg_link_parser.h
 * @brief Header file for SvgLinkParser class.
 *
 * This file contains the declaration of the SvgLinkParser class, which is responsible for parsing SVG files and extracting electrical links.
 */

#ifndef SVGPARSER_H
#define SVGPARSER_H


#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include <QXmlStreamAttributes>
#include <QString>
#include <QDomDocument>

#include "ComponentsStruct.h"

#include "debugwindow.h"

#define DEBUG

#define DEFAULT_WIDTH 1
#define DEFAULT_INPUT_NAME "in"
#define DEFAULT_OUTPUT_NAME "out"
#define DEFAULT_WIRE_NAME "wire_"

#define FORMAT_ERROR -10

#define FLAG_NAME_FROM_INKSCAPE "inkscape_label"

#define CUSTOM_ATTRIBUTE "sim"

#define NAME_ATTRIBUTE "inkscape:label"

// Macro that returns the concatenation of the CUSTOM_ATTRIBUTE and the string passed as argument
#define ATTR_FOR_STR(attr) (QString(CUSTOM_ATTRIBUTE) + ":" + attr)

/**
 * @brief The SvgParser class
 *
 * This class parses an SVG file and extracts electrical links from Qt_attr attributes of SVG elements.
 */
class SvgParser
{
protected:

    /**
     * @brief Enumeration for the names of the type attribute.
     */
    enum e_types
    {
        label,   ///< Label attribute type.
        type,    ///< Type attribute type.
        graphic, ///< Graphic attribute type.
        input,   ///< Input attribute type.
        output,  ///< Output attribute type.
        inputs,  ///< Inputs attribute type.
        outputs, ///< Outputs attribute type.
        sel,     ///< Selection attribute type.
        wire     ///< Wire attribute type.
    };

    /**
     * @brief Enumeration for return codes.
     */
    enum e_returns
    {
        SUCCESS,         ///< Success return code.
        FILE_NOT_FOUND,  ///< File not found return code.
        INVALID_ARGUMENT ///< Invalid argument return code.
        // other error codes
    };

    enum e_states
    {
        TO_BE_FOUND, ///< To be found state.
    };

    /**
     * @brief Struct for storing input/output information.
     */
    struct s_io
    {
        QString name; ///< Name of the input/output.
        uint width;   ///< Width of the input/output.
    };

    /**
     * @brief Struct for storing tree node information.
     */
    struct s_tree_node_info
    {
        QString tag_name;                    ///< Tag name of the node.
        QString id;                          ///< ID attribute of the node.
        QString label;                       ///< Label attribute of the node.
        QString type;                        ///< Type attribute of the node.
        unsigned int component_in_width;     ///< Input width of the component.
        unsigned int component_out_width;    ///< Output width of the component.
        QList<s_io> inputs;            ///< Vector of input information.
        QList<s_io> outputs;           ///< Vector of output information.
        bool is_parse_error;                 ///< Flag indicating parsing error.
        bool has_been_parsed;                ///< Flag indicating node has been parsed.
        QList<QString> error_messages; ///< Vector of error messages.
    };

    /**
     * @brief Struct for representing tree nodes.
     */
    struct s_tree_node
    {
        QDomElement element;               ///< XML element of the node.
        s_tree_node_info infos;            ///< Information of the node.
        int level;                         ///< Level of the node in the tree.
        QList<s_tree_node> children; ///< Children nodes.
    };

    int groups_number; ///< Number of groups.
    s_tree_node root;  ///< Root node of the tree.

    s_components_list all_components; ///< All components private variable.


private:
    QString svg_file; ///< File name of the SVG file.

    const QString custom_attribute = CUSTOM_ATTRIBUTE; ///< Custom attribute prefix.

    DebugWindow *debug;

    QString log_buffer; ///< Buffer for log messages.
    /**
     * @brief Get the attribute name for a given type.
     * @param type The type of the attribute.
     * @return The attribute name.
     */
    QString attr(e_types type);

    /**
     * @brief Get the attribute name for a given string.
     * @param attr The attribute string.
     * @return The attribute name.
     */
    QString attr_name_for_str(QString attr);

    /**
     * @brief parse one group element containing a component.
     * @param svg_group_xml
     * @param element_io
     */
    void parse_one_element(const QDomElement svg_group_xml, s_elements &elements);

    /**
     * @brief Parse an element from an xml group tag:
     * It can ether parse a <svg> or a <g> tag.
     *
     * @param node The tree node to parse.
     */
    void parse_element(s_tree_node &node);

    /**
     * @brief List attributes.
     * @param elements The vector of elements.
     */
    void list_attributes(const QList<QDomElement> &elements);

    /**
     * @brief Parse SVG file.
     * @param svg_file The SVG file to parse.
     */
    void parse_svg(QString svg_file);

    /**
     * @brief Parse by group.
     * @param node The XML node to parse.
     * @param parentNode The parent node.
     * @param level The level of the node in the tree.
     */
    void parse_by_group(const QDomNode &node, s_tree_node &parentNode, int level);

    /**
     * @brief Get group header.
     * @param element The XML element.
     * @param info The node information.
     */
    void get_group_header(const QDomElement &element, s_tree_node_info &info);

    /**
     * @brief Generate tree.
     * @param node The tree node.
     * @param prefix The prefix for formatting.
     */
    void generate_tree(const s_tree_node &node, const QString &prefix);

    /**
     * @brief Print tree in log.
     * @param log_buffer The log buffer.
     */
    void print_tree_in_log(QString log_buffer);

    /**
     * @brief Parse links.
     * @param node The tree node.
     */
    void parse_components(s_tree_node &node);

    /**
     * @brief Parse simulation IO.
     * @param svg_group_xml The SVG group XML.
     * @param parsed_IOs The parsed IOs.
     */
    void parse_simulation_IOs(const QDomElement svg_group_xml, s_sim_I_Os &parsed_IOs);

    /**
     * @brief Get a list of outputs name, width for sim:outputs attributes
     * @param outputs_string
     * @param out_struct
     * @return Number of outputs found if success, -1 if error
     */
    int get_list_of_outputs_name_and_width(QString outputs_string, QList<s_element_io> &outputs_list);

    /**
     * @brief Get a list of outputs name, width for sim:outputs attributes
     * @param outputs_string
     * @param out_struct
     * @return Number of outputs found if success, -1 if error
     */
    int get_list_of_inputs_name_and_width(QString outputs_string, QList<s_element_io> &inputs_list);
    
    /**
     * @brief Get attribute value if it exist
     * @param xml The XML element to check
     * @param str_to_get The string to get fron the attribute
     * @param attr_name The attribute name
     */
    bool check_and_get_attr(const QDomElement &xml, QString &str_to_get, QString attr_name);

    /**
     * @brief Parse simulation wires.
     * @param svg_group_xml The SVG group XML.
     * @param parsed_wires The parsed wires.
     */
    void parse_simulation_wires(const QDomElement svg_group_xml, s_sim_wires &parsed_wires);

    /**
     * @brief Find elements with attribute.
     * @param elem_to_look_into The element to search into.
     * @param attr_name The attribute name.
     * @param attr_value The attribute value.
     * @param found_elements The list of found elements.
     */
    void list_matching_attr_with_value(
        const QDomElement elem_to_look_into,
        const QString attr_name,
        const QString attr_value,
        QList<QDomElement> &found_elements);

    /**
     * @brief Attach an error message to a structure.
     * @param sim_IO The simulation IO structure.
     * @param errorMessage The error message.
     */
    void add_error_message(s_parse_error &error, const QString &errorMessage);

    /**
     * @brief list_matching_attr
     * @param elem_to_look_into
     * @param attr_name
     * @param found_elements
     * @return number of elements found
     */
    int list_matching_attr(const QDomElement elem_to_look_into, const QString attr_name, QList<QDomElement> &found_elements);

public:
    /**
     * @brief SvgParser constructor.
     * @param svg_file The file path of the SVG file to parse.
     */
    SvgParser(QString svg_file);

    /**
     * @brief SvgParser destructor.
     */
    ~SvgParser();

    // Getter function for components_list
    const s_components_list& get_components_list() const {
        return all_components;
    }
};

#endif // SVGPARSER_H
