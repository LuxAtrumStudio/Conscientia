#include "pessum_files\pessum_core.h";
#include "pessum_files\logging.h"
#include "pessum_files\lux_reader.h"
#include "pessum_files\rest.h"
#include "pessum_files\conscientia.h"
int main(int argc, char* argv[]) {
	pessum::InitializePessumComponents();
	pessum::conscientia::InitializeConscientia();
	pessum::TerminatePessumComponents();
	return(1);
}