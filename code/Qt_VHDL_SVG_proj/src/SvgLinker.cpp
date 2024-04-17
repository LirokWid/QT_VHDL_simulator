#include "SvgLinker.h"
#include "SvgParser.h" // Include the header file for SvgParser


/*
 * @brief SvgLinker::SvgLinker
 * @param svg_file
 * Constructor for the SvgLinker class
 * This constructor will call the SvgParser constructor with the svg_file parameter
 * It will then get the components list from the SvgParser instance
 * It will then call the link_components function to link the components
 */
SvgLinker::SvgLinker(QString svg_file) : SvgParser(svg_file)
{
    auto x = all_components;

#if 0
    if(parsed_svg != nullptr)
    {
        this->components = parsed_svg->get_components_list();


    //link_components();
    }
    else
    {
        qDebug() << "Parser instance is null";
    }
#endif
}

SvgLinker::~SvgLinker()
{
}
/*

void Svg_linker::link_components()
{
    QList<component> components = parser_instance->get_all_components();
    for (auto &comp : components)
    {
        if (comp.type == "component")
        {
            std::cout << "Component: " << comp.name << std::endl;
        }
    }
}
*/
