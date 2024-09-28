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
 * This class parses an SVG file and extracts electrical links from attributes of SVG elements.
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
     * @brief Gets the custom attribute name for a specific component type.
     *
     * Returns the attribute name corresponding to the provided component type.
     *
     * @param type The type of the component.
     * @return The custom attribute name for the given component type.
     */
    QString attr(e_types type);
    
    /**
     * @brief Constructs a custom attribute name.
     *
     * Generates a custom attribute name by prepending a defined namespace to the input string.
     *
     * @param attr The input attribute name.
     * @return The full custom attribute name.
     */
    QString attr_name_for_str(QString attr);

    /**
     * @brief
     * Parses a single SVG element and extracts information related to the device type, name, label,
     * inputs, and outputs. Adds the parsed element to the provided list of elements.
     *
     * @param svg_group_xml The QDomElement representing the SVG element to be parsed.
     * @param elements Reference to the structure where parsed elements will be stored.
     */
    void parse_one_element(const QDomElement svg_group_xml, s_elements &elements);

    /**
     * @brief Parses an individual element in the SVG file.
     *
     * Processes a specific element in the SVG file to extract its attributes and component information.
     * It can ether parse a <svg> or a <g> tag.
     *
     * @param node The tree node corresponding to the element being parsed.
     */
    void parse_element(s_tree_node &node);

    /**
     * @brief List attributes.
     * @param elements The vector of elements.
     */
    void list_attributes(const QList<QDomElement> &elements);

    /**
     * @brief Parse SVG file.
     * Opens the SVG file and processes its content, parsing the file into groups and building a tree structure of the elements.
     * @param svg_file The SVG file to parse.
     */
    void parse_svg(QString svg_file);

    /**
     * @brief Recursively parses the SVG document by group.
     *
     * Finds all group (<g>) elements in the SVG file, storing them in a tree structure.
     *
     * @param node The current XML node being processed.
     * @param parentNode The parent node in the tree structure.
     * @param level The depth level of the current node in the tree.
     */
    void parse_by_group(const QDomNode &node, s_tree_node &parentNode, int level);

    /**
     * @brief Extracts group header information from a DOM element.
     *
     * Parses a group element (<g>) in the SVG file to retrieve its attributes (e.g., device type, width).
     *
     * @param element The current DOM element being processed.
     * @param infos The structure to store the extracted information.
     */
    void get_group_header(const QDomElement &element, s_tree_node_info &info);

    /**
     * @brief Generates a tree structure from parsed SVG groups.
     *
     * Outputs a visual representation of the tree structure of SVG elements, including their tag names and IDs.
     *
     * @param node The current node in the tree structure.
     * @param prefix The prefix used to format the tree structure display.
     */
    void generate_tree(const s_tree_node &node, const QString &prefix);

    /**
     * @brief Prints the generated tree structure to the log.
     * Outputs the parsed tree structure of SVG elements to the log window.
     *
     * @param buffer The accumulated log content representing the SVG groups tree.
     */
    void print_tree_in_log(QString log_buffer);

    /**
     * @brief Parses the components of the SVG file.
     * Recursively processes the tree nodes and extracts component information from each element.
     *
     * @param node The current node in the tree structure being parsed.
     */
    void parse_components(s_tree_node &node);

    /**
     * @brief Parse simulation IO.
     * Parses simulation inputs and outputs (IOs) from the given SVG element. Adds the parsed IOs
     * to the provided list, distinguishing between inputs and outputs based on the attributes in the XML.
     *
     * @param svg_group_xml The QDomElement representing the SVG element that contains simulation IOs.
     * @param parsed_IOs Reference to the structure where parsed IOs will be stored.
     */
    void parse_simulation_IOs(const QDomElement svg_group_xml, s_sim_I_Os &parsed_IOs);

    /**
     * @brief Parse outputs from string
     * Parses a list of output names and their corresponding widths from a string formatted as
     * 'name:width,name:width'. Adds the parsed outputs to the provided list.
     *
     * @param outputs_string A QString containing the outputs in 'name:width' format.
     * @param outputs_list Reference to the list where parsed outputs will be stored.
     * @return The number of outputs parsed, or an error code if the format is invalid.
     */
    int get_list_of_outputs_name_and_width(QString outputs_string, QList<s_element_io> &outputs_list);

    /**
     * @brief Parse inputs from string
     * Parses a list of input names, their corresponding widths, and connected components from
     * a string formatted as 'name:width:connected_to,name:width:connected_to'. Adds the parsed inputs to the provided list.
     *
     * @param inputs_string A QString containing the inputs in 'name:width:connected_to' format.
     * @param inputs_list Reference to the list where parsed inputs will be stored.
     * @return The number of inputs parsed, or an error code if the format is invalid.
     */
    int get_list_of_inputs_name_and_width(QString outputs_string, QList<s_element_io> &inputs_list);
    
    /**
     * @brief Get attribute value if it exist
     * Checks if the specified XML element contains the given attribute and, if found, retrieves
     * the attribute's value.
     *
     * @param xml The QDomElement to check.
     * @param str_to_get Reference to the QString where the attribute value will be stored.
     * @param attr_name The name of the attribute to check for.
     * @return True if the attribute is found, false otherwise.
     */
    bool check_and_get_attr(const QDomElement &xml, QString &str_to_get, QString attr_name);

    /**
     * @brief Parse simulation wires.
     * Parses the wires in the simulation from the given SVG element, extracting connection and name
     * information. Adds the parsed wires to the provided structure.
     *
     * @param svg_group_xml The QDomElement representing the SVG element that contains simulation wires.
     * @param parsed_wires Reference to the structure where parsed wires will be stored.
     */
    void parse_simulation_wires(const QDomElement svg_group_xml, s_sim_wires &parsed_wires);

    /**
     * @brief Find elements with attribute.
     * Searches the given XML element for child elements that contain a specific attribute with a
     * matching value. Adds any matching elements to the provided list.
     *
     * @param elem_to_look_into The QDomElement to search within.
     * @param attr_name The name of the attribute to match.
     * @param attr_value The value of the attribute to match.
     * @param found_elements Reference to the list where found elements will be stored.
     */
    void list_matching_attr_with_value(
        const QDomElement elem_to_look_into,
        const QString attr_name,
        const QString attr_value,
        QList<QDomElement> &found_elements);

    /**
     * @brief Attach an error message to a structure.
     * Adds an error message to the given error structure. Marks the structure as having a parse error.
     *
     * @param error The structure where the error message will be added.
     * @param errorMessage The error message to be added.
     */
    void add_error_message(s_parse_error &error, const QString &errorMessage);

    /**
     * @brief list matching attribute
     * Searches the given XML element for child elements that contain a specific attribute. Adds
     * any matching elements to the provided list.
     *
     * @param elem_to_look_into The QDomElement to search within.
     * @param attr_name The name of the attribute to match.
     * @param found_elements Reference to the list where found elements will be stored.
     * @return The number of matching elements found.
     */
    int list_matching_attr(const QDomElement elem_to_look_into, const QString attr_name, QList<QDomElement> &found_elements);

public:
    /**
     * @brief Constructor for SvgParser class
     * Initializes the parsing process by loading the SVG file, generating a tree structure,
     * and extracting component information.
     *
     * @param svg_file The path to the SVG file to be parsed.
     */
    SvgParser(QString svg_file);

    /**
     * @brief SvgParser destructor.
     * Cleans up resources used by the parser.
     */
    ~SvgParser();

    // Getter function for components_list
    const s_components_list& get_components_list() const {
        return all_components;
    }
};

#endif // SVGPARSER_H
