#include "lux_reader.h"
#include <string>
#include <vector>
#include "logging.h"
#include "pessum_core.h"

namespace pessum {
	namespace luxreader {
		int logloc = 0;
	}
}

void pessum::luxreader::InitializeLuxReader()
{
	logloc = pessum::logging::AddLogLocation("pessum_files/lux_reader/");
}

pessum::luxreader::DataFile pessum::luxreader::LoadLuxDataFile(std::string filedirectory)
{
	RawLuxCode rawcode = GetRawFileData(LUX_DATA, filedirectory);
	DataFile newdata;
	newdata.datafilename = filedirectory;
	if (rawcode.luxcodelines[0] == "-1") {
		pessum::logging::LogLoc(logging::LOG_ERROR, filedirectory + " contains the incorrect file designation", logloc, "LoadLuxDataFile");
		return(newdata);
	}
	bool readType, readTypeTwo, readName, readValue;
	for (unsigned a = 0; a < rawcode.luxcodelines.size(); a++) {
		readType = true;
		readTypeTwo = false;
		readName = false;
		readValue = false;
		std::string value;
		std::string currentLine = rawcode.luxcodelines[a];
		Variable newVariable;
		for (unsigned b = 0; b < rawcode.luxcodelines[a].size(); b++) {
			if (currentLine[b] != ' ') {
				if (readType == true) {
					newVariable.variabletype = newVariable.variabletype + currentLine[b];
				}
				if (readTypeTwo == true) {
					newVariable.variabledefinitiontype = newVariable.variabledefinitiontype + currentLine[b];
				}
				if (readName == true) {
					newVariable.variablename = newVariable.variablename + currentLine[b];
				}
				if (readValue == true) {
					value = value + currentLine[b];
				}
			}
			if (currentLine[b] == ' ') {
				if (readType == true) {
					if (newVariable.variabletype == "vector") {
						readTypeTwo = true;
					}
					else {
						readName = true;
					}
					readType = false;
				}
				else if (readTypeTwo == true) {
					readName = true;
					readTypeTwo = false;
				}
				else if (readName == true) {
					readValue = true;
					readName = false;
				}
				else if (readValue == true) {
					if (newVariable.variabletype == "vector") {
						if (newVariable.variabledefinitiontype == "int") {
							newVariable.intvectorvalues.push_back(stoi(value));
						}
						if (newVariable.variabledefinitiontype == "string") {
							newVariable.stringvectorvalues.push_back(value);
						}
						if (newVariable.variabledefinitiontype == "double") {
							newVariable.doublevectorvalues.push_back(stod(value));
						}
						value = "";
					}
					else {
						readValue = false;
						if (newVariable.variabletype == "int") {
							newVariable.intvalue = stoi(value);
						}
						if (newVariable.variabletype == "string") {
							newVariable.stringvalue = value;
						}
						if (newVariable.variabletype == "double") {
							newVariable.doublevalue = stod(value);
						}
					}
				}
			}
		}
		if (value.size() > 0) {
			if (newVariable.variabletype == "vector") {
				if (newVariable.variabledefinitiontype == "int") {
					newVariable.intvectorvalues.push_back(stoi(value));
				}
				if (newVariable.variabledefinitiontype == "string") {
					newVariable.stringvectorvalues.push_back(value);
				}
				if (newVariable.variabledefinitiontype == "double") {
					newVariable.doublevectorvalues.push_back(stod(value));
				}
				value = "";
			}
			else {
				readValue = false;
				if (newVariable.variabletype == "int") {
					newVariable.intvalue = stoi(value);
				}
				if (newVariable.variabletype == "string") {
					newVariable.stringvalue = value;
				}
				if (newVariable.variabletype == "double") {
					newVariable.doublevalue = stod(value);
				}
			}
		}
		newdata.datafilevariables.push_back(newVariable);
	}
	return(newdata);
}

pessum::luxreader::Hierarchy pessum::luxreader::LoadLuxHierarchyFile(std::string filedirectory)
{
	RawLuxCode rawcode = GetRawFileData(LUX_HIERARCHY, filedirectory);
	Hierarchy newHierarchy;
	Page newPage;
	List newList;
	int currentLevel = 0;
	std::string currentLine = "";
	std::string codeLine = "";
	if (rawcode.luxcodelines[0] == "-1") {
		pessum::logging::LogLoc(logging::LOG_ERROR, filedirectory + " contains the incorrect file designation", logloc, "LoadLuxHierarchyFile");
		return(newHierarchy);
	}
	for (unsigned a = 0; a < rawcode.luxcodelines.size(); a++) {
		currentLine = rawcode.luxcodelines[a];
		codeLine = "";
		if (currentLine[0] == '[') {
			for (unsigned b = 1; b < currentLine.size() && currentLine[b] != ']'; b++) {
				codeLine = codeLine + currentLine[b];
			}
		}
		if (currentLine[currentLine.size() - 1] == '{') {
			if (currentLevel == 0) {
				newHierarchy.hierarchyname = codeLine;
			}
			if (currentLevel == 1) {
				newPage.pagelists.clear();
				newPage.pagename = codeLine;
			}
			if (currentLevel == 2) {
				newList.listitems.clear();
				newList.listname = codeLine;
			}
			currentLevel++;
		}
		if (currentLine[currentLine.size() - 1] == '}') {
			currentLevel--;
			if (currentLevel == 0) {
			}
			if (currentLevel == 1) {
				newHierarchy.hierarchypages.push_back(newPage);
			}
			if (currentLevel == 2) {
				newPage.pagelists.push_back(newList);
			}
		}
		if (currentLine[0] != '[') {
			newList.listitems.push_back(currentLine);
		}
	}
	return(newHierarchy);
}

void pessum::luxreader::SaveLuxDataFile(std::string filedirectory, DataFile ouputdata)
{
	RawLuxCode outputluxcode;
	for (unsigned a = 0; a < ouputdata.datafilevariables.size(); a++) {
		std::string outputline;
		outputline = ouputdata.datafilevariables[a].variabletype + " ";
		if (ouputdata.datafilevariables[a].variabletype == "vector") {
			outputline = outputline + ouputdata.datafilevariables[a].variabledefinitiontype + " ";
		}
		outputline = outputline + ouputdata.datafilevariables[a].variablename;
		if (ouputdata.datafilevariables[a].variabletype == "int") {
			outputline = outputline + " " + std::to_string(ouputdata.datafilevariables[a].intvalue);
		}
		if (ouputdata.datafilevariables[a].variabletype == "string") {
			outputline = outputline + " " + ouputdata.datafilevariables[a].stringvalue;
		}
		if (ouputdata.datafilevariables[a].variabletype == "double") {
			outputline = outputline + " " + std::to_string(ouputdata.datafilevariables[a].doublevalue);
		}
		if (ouputdata.datafilevariables[a].variabletype == "vector") {
			if (ouputdata.datafilevariables[a].variabledefinitiontype == "int") {
				for (unsigned b = 0; b < ouputdata.datafilevariables[a].intvectorvalues.size(); b++) {
					outputline = outputline + " " + std::to_string(ouputdata.datafilevariables[a].intvectorvalues[b]);
				}
			}
			if (ouputdata.datafilevariables[a].variabledefinitiontype == "string") {
				for (unsigned b = 0; b < ouputdata.datafilevariables[a].stringvectorvalues.size(); b++) {
					outputline = outputline + " " + ouputdata.datafilevariables[a].stringvectorvalues[b];
				}
			}
			if (ouputdata.datafilevariables[a].variabledefinitiontype == "double") {
				for (unsigned b = 0; b < ouputdata.datafilevariables[a].doublevectorvalues.size(); b++) {
					outputline = outputline + " " + std::to_string(ouputdata.datafilevariables[a].doublevectorvalues[b]);
				}
			}
		}
		outputluxcode.luxcodelines.push_back(outputline);
	}
	OuputRawFileData(LUX_DATA, filedirectory, outputluxcode);
}

pessum::luxreader::RawLuxCode pessum::luxreader::GetRawFileData(LuxFileType filetype, std::string filedirectory)
{
	std::string line;
	RawLuxCode rawcode;
	std::ifstream load(filedirectory.c_str());
	if (load.is_open()) {
		getline(load, line);
		if (line != "#LISTFILE" && filetype == LUX_LIST) {
			rawcode.luxcodelines.clear();
			rawcode.luxcodelines.push_back("-1");
			return(rawcode);
		}
		if (line != "#DATAFILE" && filetype == LUX_DATA) {
			rawcode.luxcodelines.clear();
			rawcode.luxcodelines.push_back("-1");
			return(rawcode);
		}
		if (line != "#CODEFILE" && filetype == LUX_CODE) {
			rawcode.luxcodelines.clear();
			rawcode.luxcodelines.push_back("-1");
			return(rawcode);
		}
		if (line != "#HIERARCHYFILE" && filetype == LUX_HIERARCHY) {
			rawcode.luxcodelines.clear();
			rawcode.luxcodelines.push_back("-1");
			return(rawcode);
		}
		while (getline(load, line)) {
			rawcode.luxcodelines.push_back(line);
		}
		load.close();
	}
	else {
		pessum::logging::LogLoc(logging::LOG_ERROR, "Unable to open: " + filedirectory, logloc, "GetRawFileData");
	}
	std::string cleanLine;
	bool tabSpace;
	for (unsigned a = 0; a < rawcode.luxcodelines.size(); a++) {
		line = rawcode.luxcodelines[a];
		cleanLine = "";
		tabSpace = true;
		for (unsigned b = 0; b < line.size(); b++) {
			if (tabSpace == true && line[b] != '	') {
				tabSpace = false;
			}
			if (tabSpace == false) {
				cleanLine = cleanLine + line[b];
			}
		}
		rawcode.luxcodelines[a] = cleanLine;
	}
	return(rawcode);
}

void pessum::luxreader::OuputRawFileData(LuxFileType filetype, std::string filedirectory, RawLuxCode rawoutputcode)
{
	std::ofstream outputfile(filedirectory.c_str());
	if (outputfile.is_open()) {
		if (filetype == LUX_DATA) {
			outputfile << "#DATAFILE\n";
		}
		if (filetype == LUX_HIERARCHY) {
			outputfile << "#HIERARCHYFILE\n";
		}
		if (filetype == LUX_CODE) {
			outputfile << "#CODEFILE\n";
		}
		if (filetype == LUX_LIST) {
			outputfile << "#LISTFILE\n";
		}
		for (unsigned a = 0; a < rawoutputcode.luxcodelines.size(); a++) {
			outputfile << rawoutputcode.luxcodelines[a] << "\n";
		}
		outputfile.close();
	}
}