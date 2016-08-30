#include "pessum_core.h"
#include "logging.h"
#include "lux_reader.h"
#include "rest.h"

void pessum::InitializePessumComponents()
{
	logging::InitializeLogging();
	luxreader::InitializeLuxReader();
	rest::Initialzierest();
}

void pessum::TerminatePessumComponents()
{
	rest::Terminaterest();
	logging::TerminateLogging();
}