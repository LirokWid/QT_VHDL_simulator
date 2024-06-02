// ComponentsStructs.h

#ifndef COMPONENTSSTRUCTS_H
#define COMPONENTSSTRUCTS_H

#include <QString>
#include <QList>

// Enum for I/O type
typedef enum I_O
{
    INPUT,    ///< Input type.
    OUTPUT,   ///< Output type.
    UNDEFINED ///< Undefined type.
} e_IO_type;

// Struct for parsing errors
struct s_parse_error
{
    bool is_parse_error = false;           ///< Flag indicating parsing error.
    QList<QString> error_messages;          ///< Error messages.
};

// Struct for element I/O information
struct s_element_io
{
    QString name;         ///< Name of the I/O.
    int width;            ///< Width of the I/O.
    QString connected_to; ///< Connected to.
};

// Struct for simulation I/O information
struct s_sim_I_O
{
    QString name;         ///< Name of the component.
    int width;            ///< Width of the component.
    e_IO_type type;       ///< Type of the I/O.
    QString connected_to; ///< Connected to (if output).
    s_parse_error error;  ///< Error.
};

// Struct for storing all the simulation I/Os
struct s_sim_I_Os
{
    QList<s_sim_I_O> i_os; ///< List of I/Os.
    s_parse_error error;   ///< Error.
};

// Struct for storing wire information
struct s_sim_wire
{
    QString name;         ///< Wire name.
    int width;            ///< Wire width.
    QString connected_to; ///< Connected to.
    s_parse_error error;  ///< Error.
};

// Struct for storing all the wires
struct s_sim_wires
{
    QList<s_sim_wire> wires; ///< List of wires.
    s_parse_error error;     ///< Error.
};

// Struct for storing element information
struct s_element
{
    QString name;                      ///< Name of the element.
    QString device;                    ///< Device of the element.
    QString label;                     ///< Label of the element.
    QList<s_element_io> inputs;        ///< List of inputs.
    QList<s_element_io> outputs;       ///< List of outputs.
    int inputs_number;                 ///< Number of inputs.
    int outputs_number;                ///< Number of outputs.
    s_parse_error error;               ///< Error.
};

// Struct for storing all elements
struct s_elements
{
    QList<s_element> elements_list; ///< List of elements.
    s_parse_error error;            ///< Error.
};

// Struct for storing components list
struct s_components_list
{
    s_elements elements;          ///< List of elements.
    s_sim_I_Os simulation_IOs;    ///< List of simulation I/Os.
    s_sim_wires simulation_wires; ///< List of simulation wires.
};

#endif // COMPONENTSSTRUCTS_H
