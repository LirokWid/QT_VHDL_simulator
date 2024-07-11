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

