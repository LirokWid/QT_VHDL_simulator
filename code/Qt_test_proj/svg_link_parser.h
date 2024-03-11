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



/*
 * This class will parse the SVG file and extract the electrical links from the attributes Qt_attr
 *  of the SVG elements.
 *
 *  Diagram of the class:
 *  https://app.diagrams.net/#G1hSW0E44PlskYbixKzRJB1GFNPzmR9mme#%7B%22pageId%22%3A%22PhYDsm6gi-UWzlMjqmQs%22%7D
*/

class SvgLinkParser
{
public:

    SvgLinkParser(QString svg_file);
    ~SvgLinkParser();
    int links_count;

private:
    QString svg_file;

    const QString custom_attribute = "sim";



    const QString component_ID      ="ID";
    const QString component_type    ="type";
    const QString component_graphic ="graphic";
    const QString component_input   ="input";
    const QString component_output  ="output";
    const QString component_sel     ="sel";
    const QString undefined         ="undefined";
    const QString not_found         ="not_found";

    QString log_buffer;

    enum e_types {
        label,
        type,
        graphic,
        input,
        output,
        sel,
        wire
    };

    enum e_returns {
        SUCCESS,
        FILE_NOT_FOUND,
        INVALID_ARGUMENT,
        // other error codes
    };

    struct s_io
    {
        QString name;
        uint    width;
        //s_io(QString name, uint width) : name(name), width(width) {}
    };

    /**
     * @brief The s_tree_node_info struct
     *
     * This struct is used to store the information of the node in the tree.
     */
    struct s_tree_node_info
    {
        QString tag_name;
        QString id;
        QString label;
        QString type;

        QString is_element;
        unsigned int component_in_width;
        unsigned int component_out_width;
        //Inputs and outputs
        std::vector<s_io> inputs;
        std::vector<s_io> outputs;

        bool is_parse_error;
        bool has_been_parsed;
        std::vector<QString> error_messages;
    };

    /**
     * @brief The s_tree_node class
     *
     * This class is used to represent the tree structure of the elements of the SVG file.
     */
    struct s_tree_node
    {
        QDomElement element;
        s_tree_node_info infos;
        int level; // Level of the node in the tree
        std::vector<s_tree_node> children;
    };

    int groups_number;
    s_tree_node root;

    typedef enum I_O {
        INPUT,
        OUTPUT,
        UNDIFINED
    }I_O;


    typedef struct
    {
        QString name;
        int width;
        I_O type;
    }sim_I_O;



    typedef struct
    {
        QString label;
        QString type;
        uint16_t attribute_count;
        std::vector<QString> inputs;
        std::vector<QString> outputs;
        std::vector<QString> other_attributes;
    } element_io_list;

    std::vector<element_io_list> component;

    QString get_attr_for_type(e_types type);
    QString get_attr_for_string(QString attr);

    void parse_group_elements(s_tree_node& node);
    void list_attributes(const std::vector<QDomElement>& elements);
    void parse_svg(QString svg_file);

    void parse_by_group(const QDomNode& node, s_tree_node& parentNode, int level);
    void get_group_header(const QDomElement& element, s_tree_node_info& info);
    void generate_tree(const s_tree_node& node, const QString& prefix);
    void print_tree_in_log(QString log_buffer);
    void parse_links(s_tree_node& node);
    QList<sim_I_O> get_simulation_IO(const QDomElement svg_group_xml);
    void find_elements_with_attribute(
        const QDomElement elem_to_look_into,
        const QString attr_name,
        const QString attr_value,
        QList<QDomElement>& found_elements);
};
#endif // SVG_LINK_PARSER_H
