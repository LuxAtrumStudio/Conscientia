#include "pessum_core.h"
#include "logging.h"
#include "lux_reader.h"
#include "rest.h"
#include "conscientia_files\conscientia.h"
#include "conscientia_files\conscientia_advanced.h"

void pessum::InitializePessumComponents()
{
	logging::InitializeLogging();
	luxreader::InitializeLuxReader();
	rest::Initialzierest();
	conscientia::InitializeConscientia();
}

void pessum::TerminatePessumComponents()
{
	rest::Terminaterest();
	logging::TerminateLogging();
	conscientia::TerminateConscientia();
}