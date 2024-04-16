#include "svg_linker.h"
#include "svg_link_parser.h" // Include the header file for SvgLinkParser


Svg_linker::Svg_linker(SvgLinkParser *parsed_svg)
{
    if(parsed_svg != nullptr)
    {
        this->components = parsed_svg->get_components_list();


    //link_components();
    }
    else
    {
        qDebug() << "Parser instance is null";
    }
}

Svg_linker::~Svg_linker()
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
