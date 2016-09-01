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
			struct LoadingBar {
				double percent;
				std::string name;
				int windowpointer;
				bool decimal;
			};
			extern int logloc;
			extern std::vector<LoadingBar> loadingbars;
			extern int boundloadingbar;
			extern int firstpage, firstlist, firstitem;
			//TODO(Arden):Divide up AdvancedMenu and DisplayMenu into multiple functions
			std::string AdvancedMenu(std::string filedirectory = "NULL", int sizex = virtualwindows[0].sizex - 4, int sizey = virtualwindows[0].sizey - 4, int posx = 2, int posy = 2);
			void DisplayMenu(luxreader::Hierarchy hierarchy, int currentpage = 0, int currentlist = 0, int currentitem = 0);
			int AdvancedLoadingBar(std::string process, bool decimal = true);
			void UpdateLoadingBarRaw(double percent, int loadingbar = boundloadingbar);
			void UpdateLoadingBarAdd(double percent, int loadingbar = boundloadingbar);
			void TermianteLoadingBar(int loadingbar = boundloadingbar);
			void BindLoadingBar(int pointer);
		}
	}
}
#endif // !_PESSUM_FILES_CONSCIENTIA_FILES_CONSCIENTIA_ADVANCED_H_