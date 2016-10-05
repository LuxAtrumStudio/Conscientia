#include "logging.h"
#include <string>
#include <fstream>
#include <ctime>
#include "pessum_core.h"

namespace pessum {
	namespace logging {
		std::ofstream logfile;
		std::vector<std::string> loglocationbindings;
	}
}

bool pessum::logging::InitializeLogging(std::string outputfile)
{
	logfile.open(outputfile.c_str(), std::ios::out);
	if (logfile.is_open()) {
		std::string outputline;
		time_t logopentime = time(NULL);
		outputline = "Opened log: " + outputfile;
		Log(LOG_SUCCESS, outputline, "pessum_files/logging.cpp/InitializeLogging");
		LogTimeStamp(true);
		return(true);
	}
	else {
		return(false);
	}
}

void pessum::logging::Log(LogType type, std::string logstring, std::string logfilelocation)
{
	std::string logline = InterpretType(type) + logstring + ">>" + logfilelocation;
	if (logfile.is_open()) {
		logfile << logline << std::endl;
	}
}

void pessum::logging::LogLoc(LogType type, std::string logstring, int logfilelocation, std::string functionname)
{
	std::string logline = InterpretType(type) + logstring + ">>" + loglocationbindings[logfilelocation] + functionname;
	if (logfile.is_open()) {
		logfile << logline << std::endl;
	}
}

std::string pessum::logging::InterpretType(LogType type)
{
	std::string logtypeline = "";
	if (type == LOG_ERROR) {
		logtypeline = "ERROR>>";
	}
	else if (type == LOG_WARNING) {
		logtypeline = "WARNING>>";
	}
	else if (type == LOG_SUCCESS) {
		logtypeline = "SUCCESS>>";
	}
	else if (type == LOG_DATA) {
		logtypeline = "DATA>>";
	}
	else if (type == LOG_INFORMATION) {
		logtypeline = "INFORMATION>>";
	}
	else if (type == LOG_DEVELOPMENT_CHECK) {
		logtypeline = "DEVCHECK>>";
	}
	else {
		logtypeline = "LOG TYPE NOT RECOGNIZED";
	}
	return(logtypeline);
}

int pessum::logging::AddLogLocation(std::string loglocationstring)
{
	int loglocationindex = loglocationbindings.size();
	loglocationbindings.push_back(loglocationstring);
	return loglocationindex;
}

void pessum::logging::LogTimeStamp(bool date)
{
	time_t currenttime;
	time(&currenttime);
	std::string timestring = ctime(&currenttime);
	std::string newtime = "";
	if (date == true) {
		for (unsigned a = 0; a < timestring.size() - 1; a++) {
			newtime = newtime + timestring[a];
		}
	}
	else {
		for (unsigned a = 11; a < timestring.size() - 6; a++) {
			newtime = newtime + timestring[a];
		}
	}
	Log(LOG_INFORMATION, newtime, "TimeStamp");

}

void pessum::logging::TerminateLogging()
{
	if (logfile.is_open()) {
		time_t logclosetime = time(NULL);
		Log(LOG_SUCCESS, "Terminated log file", "Pessum/logging.cpp/TerminateLogging");
		LogTimeStamp(true);
		logfile.close();
	}
	loglocationbindings.clear();
}