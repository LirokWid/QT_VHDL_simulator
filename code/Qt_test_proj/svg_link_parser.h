#ifndef SVG_LINK_PARSER_H
#define SVG_LINK_PARSER_H

#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include <QXmlStreamAttributes>
#include <QString>
#include <QDomDocument>

#include <vector>



/*
 * This class will parse the SVG file and extract the electrical links from the attributes Qt_attr
 *  of the SVG elements.
*/

class SvgLinkParser
{
public:

    SvgLinkParser(QString svg_file);
    int links_count;

private:
    QString svg_file;

    const QString custom_attribute = "RISCVsim";


    const QString component_ID      ="ID";
    const QString component_type    ="type";
    const QString component_graphic ="graphic";
    const QString component_input   ="input";
    const QString component_output  ="output";
    const QString component_sel     ="sel";
    const QString undefined         ="undefined";
    const QString not_found         ="not_found";

    QString log_buffer;

    struct TreeNodeInfo
    {
        QString tag_name;
        QString id;
    };

    struct TreeNode
    {
        QDomElement element;
        TreeNodeInfo info;
        int level; // Level of the node in the tree
        std::vector<TreeNode> children;
    };

    int groups_number;
    TreeNode root;


    typedef struct
    {
        QString label;
        QString type;
        uint16_t attribute_count;
        std::vector<QString> inputs;
        std::vector<QString> outputs;
        std::vector<QString> other_attributes;
    } element_io_list;
    std::vector<element_io_list> elements;

    void parse_group_elements(QDomElement group);
    void list_attributes(const std::vector<QDomElement>& elements);
    void parse_svg(QString svg_file);

    void parse_by_group(const QDomNode& node, TreeNode& parentNode, int level);
    void extractNodeInfo(const QDomElement& element, TreeNodeInfo& info);
    void generate_tree(const TreeNode& node, const QString& prefix);
    void print_tree_in_log();

};
#endif // SVG_LINK_PARSER_H
