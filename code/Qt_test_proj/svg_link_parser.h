/**
 * @file svg_link_parser.h
 * @brief Header file for SvgLinkParser class.
 *
 * This file contains the declaration of the SvgLinkParser class, which is responsible for parsing SVG files and extracting electrical links.
 */

#ifndef SVG_LINK_PARSER_H
#define SVG_LINK_PARSER_H


#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include <QXmlStreamAttributes>
#include <QString>
#include <QDomDocument>

#include <vector>

#define DEFAULT_WIDTH 1
#define DEFAULT_INPUT_NAME "in"
#define DEFAULT_OUTPUT_NAME "out"

#define IO_NAME_ATTRIBUTE "inkscape:label"

/**
 * @brief The SvgLinkParser class
 *
 * This class parses an SVG file and extracts electrical links from Qt_attr attributes of SVG elements.
 */
class SvgLinkParser
{
public:
    /**
     * @brief SvgLinkParser constructor.
     * @param svg_file The file path of the SVG file to parse.
     */
    SvgLinkParser(QString svg_file);

    /**
     * @brief SvgLinkParser destructor.
     */
    ~SvgLinkParser();

    int links_count;

private:
    QString svg_file; 								///< File name of the SVG file.

    const QString custom_attribute = "sim"; 		///< Custom attribute prefix.

    const QString component_ID = "ID"; 				///< Component ID attribute name.
    const QString component_type = "type"; 			///< Component type attribute name.
    const QString component_graphic = "graphic"; 	///< Component graphic attribute name.
    const QString component_input = "input"; 		///< Component input attribute name.
    const QString component_output = "output"; 		///< Component output attribute name.
    const QString component_sel = "sel"; 			///< Component selection attribute name.
    const QString undefined = "undefined"; 			///< Undefined attribute value.
    const QString not_found = "not_found"; 			///< Not found attribute value.

    QString log_buffer; 							///< Buffer for log messages.

    /**
     * @brief Enumeration for the names of the type attribute.
     */
    enum e_types {
        label, ///< Label attribute type.
        type, ///< Type attribute type.
        graphic, ///< Graphic attribute type.
        input, ///< Input attribute type.
        output, ///< Output attribute type.
        sel, ///< Selection attribute type.
        wire ///< Wire attribute type.
    };

    /**
     * @brief Enumeration for return codes.
     */
    enum e_returns {
        SUCCESS, ///< Success return code.
        FILE_NOT_FOUND, ///< File not found return code.
        INVALID_ARGUMENT ///< Invalid argument return code.
        // other error codes
    };

    /**
     * @brief Struct for storing input/output information.
     */
    struct s_io
    {
        QString name; ///< Name of the input/output.
        uint width; ///< Width of the input/output.
    };

    /**
     * @brief Struct for storing tree node information.
     */
    struct s_tree_node_info
    {
        QString tag_name; 					///< Tag name of the node.
        QString id; 						///< ID attribute of the node.
        QString label; 						///< Label attribute of the node.
        QString type; 						///< Type attribute of the node.
        unsigned int component_in_width; 	///< Input width of the component.
        unsigned int component_out_width; 	///< Output width of the component.
        std::vector<s_io> inputs; 			///< Vector of input information.
        std::vector<s_io> outputs; 			///< Vector of output information.
        bool is_parse_error; 				///< Flag indicating parsing error.
        bool has_been_parsed; 				///< Flag indicating node has been parsed.
        std::vector<QString> error_messages;///< Vector of error messages.
    };

    /**
     * @brief Struct for representing tree nodes.
     */
    struct s_tree_node
    {
        QDomElement element; 				///< XML element of the node.
        s_tree_node_info infos; 			///< Information of the node.
        int level; 							///< Level of the node in the tree.
        std::vector<s_tree_node> children; 	///< Children nodes.
    };

    int groups_number; 	///< Number of groups.
    s_tree_node root; 	///< Root node of the tree.

    typedef enum I_O {
        INPUT, 		///< Input type.
        OUTPUT, 	///< Output type.
        UNDEFINED 	///< Undefined type.
    } I_O;

    typedef struct
    {
        QString name; 	///< Name of the component.
        int width; 		///< Width of the component.
        I_O type; 		///< Type of the component.
    } sim_I_O;

    /**
     * @brief Struct for storing element IO information.
     */
    typedef struct
    {
        QString label; 							///< Label of the element.
        QString type; 							///< Type of the element.
        uint16_t attribute_count; 				///< Count of attributes.
        std::vector<QString> inputs; 			///< Vector of input attributes.
        std::vector<QString> outputs; 			///< Vector of output attributes.
        std::vector<QString> other_attributes; 	///< Vector of other attributes.
    } element_io_list;

    /**
     * @brief Struct for storing components list.
     */
    typedef struct
    {
        QList<element_io_list> elements; 		///< List of elements.
    } components_list;

    components_list all_components; 			///< All components list.

    /**
     * @brief Get the attribute name for a given type.
     * @param type The type of the attribute.
     * @return The attribute name.
     */
    QString get_attr_for_type(e_types type);

    /**
     * @brief Get the attribute name for a given string.
     * @param attr The attribute string.
     * @return The attribute name.
     */
    QString get_attr_for_string(QString attr);

    /**
     * @brief Parse group elements.
     * @param node The tree node to parse.
     */
    void parse_group_elements(s_tree_node& node);

    /**
     * @brief List attributes.
     * @param elements The vector of elements.
     */
    void list_attributes(const std::vector<QDomElement>& elements);

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
    void parse_by_group(const QDomNode& node, s_tree_node& parentNode, int level);

    /**
     * @brief Get group header.
     * @param element The XML element.
     * @param info The node information.
     */
    void get_group_header(const QDomElement& element, s_tree_node_info& info);

    /**
     * @brief Generate tree.
     * @param node The tree node.
     * @param prefix The prefix for formatting.
     */
    void generate_tree(const s_tree_node& node, const QString& prefix);

    /**
     * @brief Print tree in log.
     * @param log_buffer The log buffer.
     */
    void print_tree_in_log(QString log_buffer);

    /**
     * @brief Parse links.
     * @param node The tree node.
     */
    void parse_links(s_tree_node& node);

    /**
     * @brief Get simulation IO.
     * @param svg_group_xml The SVG group XML.
     * @return List of simulation IO.
     */
    QList<sim_I_O> get_simulation_IO(const QDomElement svg_group_xml);

    /**
     * @brief Find elements with attribute.
     * @param elem_to_look_into The element to search into.
     * @param attr_name The attribute name.
     * @param attr_value The attribute value.
     * @param found_elements The list of found elements.
     */
    void find_elements_with_attribute(
        const QDomElement elem_to_look_into,
        const QString attr_name,
        const QString attr_value,
        QList<QDomElement>& found_elements);
};

#endif // SVG_LINK_PARSER_H
