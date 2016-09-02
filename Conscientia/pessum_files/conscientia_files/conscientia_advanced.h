#ifndef _PESSUM_FILES_CONSCIENTIA_FILES_CONSCIENTIA_ADVANCED_H_
#define _PESSUM_FILES_CONSCIENTIA_FILES_CONSCIENTIA_ADVANCED_H_
#include <string>
#include <vector>
#include <Windows.h>
#include "../lux_reader.h"
#include "../pessum_core.h"
#include "conscientia.h"
namespace pessum {
	namespace conscientia {
		namespace advanced {
			//Data structure to store the data in a loading bar
			struct LoadingBar {
				double percent;
				std::string name;
				int windowpointer;
				bool decimal;
			};
			//Log location integer index
			extern int logloc;
			//Vector of all active loading bars
			extern std::vector<LoadingBar> loadingbars;
			//Integer pointer for the current active loading bar in loadingbars
			extern int boundloadingbar;
			//Used to determin what pages, lists, and items are displayed in a menu
			extern int firstpage, firstlist, firstitem;
			//Given a file directory to a lux hierarchy file, return the selected item from the file structure
			std::string AdvancedMenu(std::string filedirectory = "NULL", int sizex = virtualwindows[0].sizex - 4, int sizey = virtualwindows[0].sizey - 4, int posx = 2, int posy = 2);
			//Displays UI for a lux hierarchy menu selection
			void DisplayMenu(luxreader::Hierarchy hierarchy, int currentpage = 0, int currentlist = 0, int currentitem = 0);
			//Initalizes a loading bar
			int AdvancedLoadingBar(std::string process, bool decimal = true);
			//Sets the loading bars percent to the value and updates the bar
			void UpdateLoadingBarRaw(double percent, int loadingbar = boundloadingbar);
			//Adds the value to the loading bars percent and updates the bar
			void UpdateLoadingBarAdd(double percent, int loadingbar = boundloadingbar);
			//Closes the loading bar window, and erases it from the active loading bars
			void TermianteLoadingBar(int loadingbar = boundloadingbar);
			//Sets the current active loading bar
			void BindLoadingBar(int pointer);
		}
	}
}
#endif // !_PESSUM_FILES_CONSCIENTIA_FILES_CONSCIENTIA_ADVANCED_H_