#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>
#include "p1_helper.h"

// Pass user or database

std::string process_client_command(const std::string& raw_command, std::string& current_user);

#endif // COMMAND_PARSER_H