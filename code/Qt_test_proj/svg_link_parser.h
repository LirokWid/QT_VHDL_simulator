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
    int elements_count;
    int links_count;
private:
    const QString custom_attribute = "Qt_attr";

    const QString component_ID      ="component_ID";
    const QString component_type    ="component_type";
    const QString component_graphic ="component_graphic";
    const QString component_input   ="component_input";
    const QString component_output  ="component_output";
    const QString component_sel     ="component_sel";
    const QString undefined         ="undefined";
    const QString not_found         ="not_found";

    typedef struct
    {
        QString label;
        QString type;
        std::vector<QString> inputs;
        std::vector<QString> outputs;
        std::vector<QString> other_attributes;
    } element_io_list;
    std::vector<element_io_list> elements;

    std::vector<QDomElement> parse_group_elements(QDomElement group);
    void list_attributes(const std::vector<QDomElement>& elements);
    void parseSvg(QString svg_file);
    std::vector<QDomElement> parse_by_group(const QDomDocument& doc);
};
#endif // SVG_LINK_PARSER_H
