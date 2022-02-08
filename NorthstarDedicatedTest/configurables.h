#pragma once
#include <string>

static std::string NORTHSTAR_FOLDER_PREFIX;
static std::string NORTHSTAR_LOG_NAME;

std::string GetNorthstarPrefix();
std::string GetNorthstarLogName();
void parseConfigurables();
