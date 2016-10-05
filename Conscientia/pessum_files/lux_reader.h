#ifndef _PESSUM_FILES_LUX_READER_H_
#define _PESSUM_FILES_LUX_READER_H_
#include <string>
#include <vector>
#include "pessum_core.h"
namespace pessum {
	//luxreader namespace provides support for lux files, and reading the data files
	//from lux code to be used in a program.
	namespace luxreader {
		//Struct for raw lux code loading
		struct RawLuxCode
		{
			std::vector<std::string> luxcodelines;
		};
		//Structs for lux menu loading
		struct List
		{
			std::string listname;
			std::vector<std::string> listitems;
		};
		struct Page
		{
			std::string pagename;
			std::vector<List> pagelists;
		};
		struct Hierarchy
		{
			std::string hierarchyname;
			std::vector<Page> hierarchypages;
		};
		//Structs for lux data loading
		struct Variable
		{
			std::string variablename, variabletype, variabledefinitiontype;
			int intvalue;
			std::vector<int> intvectorvalues;
			double doublevalue;
			std::vector<double> doublevectorvalues;
			std::string stringvalue;
			std::vector<std::string> stringvectorvalues;
		};
		struct DataFile
		{
			std::string datafilename;
			std::vector<Variable> datafilevariables;
		};
		//Defines different types of Lux Code files for the file reader
		enum LuxFileType
		{
			LUX_DATA,
			LUX_HIERARCHY,
			LUX_CODE,
			LUX_LIST
		};
		//Sets log location for lux reader logging
		extern int logloc;

		//Sets the logging location code(must be called)
		void InitializeLuxReader();

		//Loads the data from a lux data file, and returns in a DataFile struct
		DataFile LoadLuxDataFile(std::string filedirectory);
		//Loads the data from a lux hierarchy file, and returns in a Hierarchy struct
		Hierarchy LoadLuxHierarchyFile(std::string filedirectory);
		//Loads the data from a lux list file, and returns in a vector of strings
		std::vector<std::string> LoadLuxListFile(std::string filedirectory);

		//Takes a DataFile structure, and converts it to RawLuxCode, to output
		void SaveLuxDataFile(std::string filedirectory, DataFile ouputdata);

		//Loads the raw lux code data from specified file
		RawLuxCode GetRawFileData(LuxFileType filetype, std::string filedirectory);
		//Saves a raw lux code data to specifed file
		void OuputRawFileData(LuxFileType filetype, std::string filedirectory, RawLuxCode rawoutputcode);
	}
}
#endif // !_PESSUM_FILES_LUX_READER_H_