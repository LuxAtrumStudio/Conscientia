#ifndef _PESSUM_REST_H_
#define _PESSUM_REST_H_
#include <string>
#include <vector>
#include "pessum_core.h"
namespace pessum {
	//rest namespace allows access to urls and internet
	//api through the use of return json files
	namespace rest {
		//Struct for variables in json files
		struct Variable
		{
			std::string variablename;
			std::vector<Variable> subvariables;
			std::string stringvalue;
			int intvalue;
			double doublevalue;
		};
		//Struct for json file variables
		struct JsonFile
		{
			std::vector<Variable> filevariables;
		};
		//Log location integer index
		extern int logloc;
		//Int index to identify level of json file branches
		extern unsigned int index;
		//Vector of loaded json files
		extern std::vector<JsonFile> loadedjsonfiles;
		//Sets log location for rest logging
		void Initialzierest();
		//Clears loaded json files, freeing up memory
		void Terminaterest();

		//Runs specified python script with script arguments from vector
		void RunScript(std::vector<std::string> scriptargs);
		//Reads script output file, and creates a JsonFile from the raw file
		void InterpretJson(std::string scriptoutputfiledirectory);
		//Reads contents of a variable, and returns a Variable structure from raw variable string
		Variable InterpretVariable(std::string variablename, std::string rawvariable);
		//Access json file from loaded json file vector
		JsonFile GetJsonFile(std::string filename);

		//Returns sub variable searched from variable
		Variable LocateVariable(std::string variablename, Variable searchvariable);
		//Returns sub variable searched from json file
		Variable GetVariable(std::string variablename, std::string jsonfilename);
		//Returns int value searched from json file
		int GetIntVariable(std::string variablename, std::string jsonfilename);
		//Returns double value searched from json file
		double GetDoubleVariable(std::string variablename, std::string jsonfilename);
		//Returns string value searched from json file
		std::string GetStringVariable(std::string variablename, std::string jsonfilename);
	}
}
#endif // !_PESSUM_REST_H_