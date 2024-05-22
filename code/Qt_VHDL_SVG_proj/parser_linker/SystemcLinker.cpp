#include "SystemcLinker.h"

#include "ComponentsStruct.h"

/*
 * @brief SystemcLinker::SystemcLinker
 * @param svg_file
 * Constructor for the SystemcLinker class
 * This constructor will call the SvgParser constructor with the svg_file parameter
 * It will then get the components list from the SvgParser instance
 * It will then call the link_components function to link the components
 */
SystemcLinker::SystemcLinker(QString svg_file) :
    SvgParser(svg_file)
{
    s_components_list components = all_components;

    //First, create the module which will be simulated by SystemC
    for(s_element& elem  : (components.elements.elements_list))
    {
        create_sysc_module(elem);
    }
}

SystemcLinker::~SystemcLinker()
{
}

bool SystemcLinker::getGlobalParsingError()
{
    bool error = false;

    error |= all_components.elements.error.is_parse_error;
    error |= all_components.simulation_IOs.error.is_parse_error;
    error |= all_components.simulation_wires.error.is_parse_error;

    return error;
}


void SystemcLinker::create_sysc_module(const s_element& elem)
{
    //SC_MODULE()
    //TODO
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
