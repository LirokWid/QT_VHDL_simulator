#include "SvgParseManager.h"

#include "ComponentsStruct.h"


SvgParseManager::SvgParseManager(QString svg_file) :
    SvgParser(svg_file)
{
}

SvgParseManager::~SvgParseManager()
{
}

bool SvgParseManager::getGlobalParsingError()
{
    bool error = false;

    error |= all_components.elements.error.is_parse_error;
    error |= all_components.simulation_IOs.error.is_parse_error;
    error |= all_components.simulation_wires.error.is_parse_error;

    return error;
}
