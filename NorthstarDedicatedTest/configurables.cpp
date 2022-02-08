#include <string>
#include "pch.h"
#include "configurables.h"

std::string GetNorthstarPrefix() { return NORTHSTAR_FOLDER_PREFIX; }
std::string GetNorthstarLogName() { return NORTHSTAR_LOG_NAME; }

void parseConfigurables()
{
	char* clachar = strstr(GetCommandLineA(), "-profile=");
	if (clachar)
	{
		std::string cla = std::string(clachar);
		if (strncmp(cla.substr(9, 1).c_str(), "\"", 1))
		{
			int space = cla.find(" ");
			std::string dirname = cla.substr(9, space - 9);
			spdlog::info("Found profile in command line arguments: " + dirname);
			NORTHSTAR_FOLDER_PREFIX = dirname;
		}
		else
		{
			std::string quote = "\"";
			int quote1 = cla.find(quote);
			int quote2 = (cla.substr(quote1 + 1)).find(quote);
			std::string dirname = cla.substr(quote1 + 1, quote2);
			spdlog::info("Found profile in command line arguments: " + dirname);
			NORTHSTAR_FOLDER_PREFIX = dirname;
		}
	}
	else
	{
		spdlog::info("Didnt' find profile in command line arguments. Using default: R2Northstar");
		NORTHSTAR_FOLDER_PREFIX = "R2Northstar";
	}
	clachar = strstr(GetCommandLineA(), "-logname=");
	if (clachar)
	{
		std::string cla = std::string(clachar);
		if (strncmp(cla.substr(9, 1).c_str(), "\"", 1))
		{
			int space = cla.find(" ");
			std::string logfilename = cla.substr(9, space - 9);
			spdlog::info("Found logfilename in command line arguments: " + logfilename);
			NORTHSTAR_LOG_NAME = logfilename;
		}
		else
		{
			std::string quote = "\"";
			int quote1 = cla.find(quote);
			int quote2 = (cla.substr(quote1 + 1)).find(quote);
			std::string logfilename = cla.substr(quote1 + 1, quote2);
			spdlog::info("Found logfilename in command line arguments: " + logfilename);
			NORTHSTAR_LOG_NAME = logfilename;
		}
	}
	else
	{
		spdlog::info("Didnt' find logfilename in command line arguments. Using default: nslog");
		NORTHSTAR_LOG_NAME = "nslog";
	}
}