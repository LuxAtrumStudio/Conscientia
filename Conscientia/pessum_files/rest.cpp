#include "rest.h"
#include <string>
#include <vector>
#include "logging.h"
#include "pessum_core.h"

namespace pessum {
	namespace rest {
		int logloc = 0;
		unsigned int index = 1;
		std::vector<JsonFile> loadedjsonfiles;
	}
}

void pessum::rest::Initialzierest()
{
	logloc = logging::AddLogLocation("pessum_files/rest/");
}

void pessum::rest::Terminaterest()
{
	loadedjsonfiles.clear();
}

void pessum::rest::RunScript(std::vector<std::string> scriptargs)
{
	if (scriptargs.size() <= 1) {
		pessum::logging::LogLoc(logging::LOG_ERROR, "Filed to enter url to script arguments", logloc, "RunScript");
	}
	else {
		if (scriptargs[0] == "") {
			scriptargs[0] = "get_page";
		}
		std::string transferlocation = scriptargs[0] + ".tran";
		std::ofstream transferdata(transferlocation.c_str());
		if (transferdata.is_open()) {
			for (unsigned a = 1; a < scriptargs.size(); a++) {
				if (a != scriptargs.size() - 1) {
					transferdata << scriptargs[a] << std::endl;
				}
				else {
					transferdata << scriptargs[a];
				}
			}
			transferdata.close();
			std::string scriptlocation = scriptargs[0] + ".py";
			system(scriptlocation.c_str());
		}
		else {
			pessum::logging::LogLoc(logging::LOG_ERROR, "Unable to genorate transfer data file for script: " + scriptargs[0], logloc, "RunScript");
		}
	}
}

void pessum::rest::InterpretJson(std::string scriptoutputfiledirectory)
{
	index = 1;
	std::string raw = "";
	std::ifstream load(scriptoutputfiledirectory.c_str());
	if (load.is_open()) {
		std::string line = "";
		while (getline(load, line)) {
			for (unsigned a = 0; a < line.size(); a++) {
				if (line[a] == '[') {
					line[a] = '\0';
				}
				if (line[a] == ']') {
					line[a] = '\0';
				}
			}
			raw = raw + line;
		}
		load.close();
	}
	else {
		pessum::logging::LogLoc(logging::LOG_ERROR, "Unable to open script output directory: " + scriptoutputfiledirectory, logloc, "InterpretJson");
	}
	std::string current = "";;
	JsonFile filejson;
	filejson.filevariables.push_back(InterpretVariable(scriptoutputfiledirectory, raw));
	loadedjsonfiles.push_back(filejson);
}

pessum::rest::Variable pessum::rest::InterpretVariable(std::string variablename, std::string rawvariable)
{
	Variable newlevel;
	newlevel.variablename = variablename;
	std::string strLineA, strLineB;
	bool loadName = false, loadVal = false, stringVal = false, action = false, nameStored = false;
	for (index; index < rawvariable.size() - 1; index++) {
		if (rawvariable[index] == '{') {
			index++;
			Variable level = InterpretVariable(strLineA, rawvariable);
			newlevel.subvariables.push_back(level);
			index = index + 1;
			strLineA = "";
			strLineB = "";
			loadName = false;
			loadVal = false;
			stringVal = false;
			action = false;
			nameStored = false;
		}
		if (rawvariable[index] == '"' && loadName == false && action == false && nameStored == false) {
			loadName = true;
			action = true;
		}
		else if (rawvariable[index] == '"' && loadName == true && action == false) {
			loadName = false;
			nameStored = true;
		}
		if (rawvariable[index] == ':' && rawvariable[index + 1] != '{' && action == false && loadVal == false) {
			loadVal = true;
			action = true;
		}
		if (rawvariable[index] == '\"' && loadVal == true && action == false) {
			stringVal = true;
			action = true;
		}
		if (rawvariable[index] == ',' && loadVal == true || rawvariable[index] == '}' && loadVal == true) {
			Variable newVariable;
			newVariable.variablename = strLineA;
			newVariable.stringvalue = strLineB;
			loadVal == false;
			if (stringVal == false) {
				bool doubleCheck = false;
				for (unsigned b = 0; b < strLineB.size(); b++) {
					if (strLineB[b] == '.') {
						doubleCheck = true;
					}
				}
				if (doubleCheck == false) {
					std::string::size_type returnSize;
					newVariable.intvalue = stoi(strLineB, &returnSize);
				}
				else if (doubleCheck == true) {
					std::string::size_type returnSize;
					newVariable.doublevalue = stod(strLineB, &returnSize);
				}
			}
			else if (stringVal == true) {
				stringVal = false;
			}
			newlevel.subvariables.push_back(newVariable);
			strLineA = "";
			strLineB = "";
			loadName = false;
			loadVal = false;
			stringVal = false;
			action = false;
			nameStored = false;
		}
		if (loadName == true && action == false) {
			strLineA = strLineA + rawvariable[index];
		}
		if (loadVal == true && action == false) {
			strLineB = strLineB + rawvariable[index];
		}
		if (rawvariable[index] == '}' && action == false) {
			return(newlevel);
		}
		action = false;
	}
	return (newlevel);
}

pessum::rest::JsonFile pessum::rest::GetJsonFile(std::string filename)
{
	for (unsigned a = 0; a < loadedjsonfiles.size(); a++) {
		if (loadedjsonfiles[a].filevariables[0].variablename == filename) {
			return(loadedjsonfiles[a]);
		}
	}
}

pessum::rest::Variable pessum::rest::LocateVariable(std::string variablename, Variable searchvariable)
{
	Variable result, check;
	result.variablename == "NULL";
	if (variablename == searchvariable.variablename) {
		return(searchvariable);
	}
	for (unsigned a = 0; a < searchvariable.subvariables.size(); a++) {
		check = LocateVariable(variablename, searchvariable.subvariables[a]);
		if (check.variablename == variablename) {
			result = check;
			return(result);
		}
	}
	return (result);
}

pessum::rest::Variable pessum::rest::GetVariable(std::string variablename, std::string jsonfilename)
{
	Variable result;
	JsonFile file = GetJsonFile(jsonfilename);
	for (unsigned a = 0; a < file.filevariables.size(); a++) {
		result = LocateVariable(variablename, file.filevariables[a]);
	}
	if (result.variablename == "NULL") {
		result.intvalue = 0;
		result.doublevalue = 0.0;
		result.stringvalue = "";
		result.subvariables.clear();
	}
	return (result);
}

int pessum::rest::GetIntVariable(std::string variablename, std::string jsonfilename)
{
	Variable result;
	JsonFile file = GetJsonFile(jsonfilename);
	for (unsigned a = 0; a < file.filevariables.size(); a++) {
		result = LocateVariable(variablename, file.filevariables[a]);
	}
	if (result.variablename == "NULL") {
		result.intvalue = 0;
		result.doublevalue = 0.0;
		result.stringvalue = "";
		result.subvariables.clear();
	}
	return (result.intvalue);
}

double pessum::rest::GetDoubleVariable(std::string variablename, std::string jsonfilename)
{
	Variable result;
	JsonFile file = GetJsonFile(jsonfilename);
	for (unsigned a = 0; a < file.filevariables.size(); a++) {
		result = LocateVariable(variablename, file.filevariables[a]);
	}
	if (result.variablename == "NULL") {
		result.intvalue = 0;
		result.doublevalue = 0.0;
		result.stringvalue = "";
		result.subvariables.clear();
	}
	return (result.doublevalue);
}

std::string pessum::rest::GetStringVariable(std::string variablename, std::string jsonfilename)
{
	Variable result;
	JsonFile file = GetJsonFile(jsonfilename);
	for (unsigned a = 0; a < file.filevariables.size(); a++) {
		result = LocateVariable(variablename, file.filevariables[a]);
	}
	if (result.variablename == "NULL") {
		result.intvalue = 0;
		result.doublevalue = 0.0;
		result.stringvalue = "";
		result.subvariables.clear();
	}
	return (result.stringvalue);
}