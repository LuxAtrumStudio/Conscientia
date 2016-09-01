#include "conscientia_advanced.h"
#include <string>
#include <sstream>
#include <vector>
#include <Windows.h>
#include <conio.h>

#include "conscientia.h"
#include "../pessum_core.h"
#include "../logging.h"
#include "../lux_reader.h"

namespace pessum {
	namespace conscientia {
		namespace advanced {
			int logloc = 0;
			int boundloadingbar = 0;
			int firstpage = 0, firstlist = 0, firstitem = 0;
			std::vector<LoadingBar> loadingbars;
		}
	}
}

std::string pessum::conscientia::advanced::AdvancedMenu(std::string filedirectory, int sizex, int sizey, int posx, int posy)
{
	bool running = true, updatedisplay = true;
	int input = -1, currentpage = 0, currentlist = 0, currentitem = 0;
	luxreader::Hierarchy hierarchy;
	hierarchy = luxreader::LoadLuxHierarchyFile(filedirectory);
	GenorateWindow(hierarchy.hierarchyname, sizex, sizey, posx, posy, true, true);
	while (running == true) {
		if (updatedisplay == true) {
			updatedisplay = false;
			DisplayMenu(hierarchy, currentpage, currentlist, currentitem);
		}
		input = int(GetChar());
		if (input == 97 && currentlist > 0) {
			updatedisplay = true;
			currentlist--;
			if (currentitem >= hierarchy.hierarchypages[currentpage].pagelists[currentlist].listitems.size()) {
				currentitem = hierarchy.hierarchypages[currentpage].pagelists[currentlist].listitems.size() - 1;
			}
		}
		if (input == 100 && currentlist < hierarchy.hierarchypages[currentpage].pagelists.size() - 1) {
			updatedisplay = true;
			currentlist++;
			if (currentitem >= hierarchy.hierarchypages[currentpage].pagelists[currentlist].listitems.size()) {
				currentitem = hierarchy.hierarchypages[currentpage].pagelists[currentlist].listitems.size() - 1;
			}
		}
		if (input == 115 && currentitem < hierarchy.hierarchypages[currentpage].pagelists[currentlist].listitems.size() - 1) {
			updatedisplay = true;
			currentitem++;
		}
		if (input == 119 && currentitem > 0) {
			updatedisplay = true;
			currentitem--;
		}
		if (input == 113 && currentpage > 0) {
			updatedisplay = true;
			currentpage--;
			if (currentlist >= hierarchy.hierarchypages[currentpage].pagelists.size()) {
				currentlist = hierarchy.hierarchypages[currentpage].pagelists.size() - 1;
			}
			if (currentitem >= hierarchy.hierarchypages[currentpage].pagelists[currentlist].listitems.size()) {
				currentitem = hierarchy.hierarchypages[currentpage].pagelists[currentlist].listitems.size() - 1;
			}
		}
		if (input == 101 && currentpage < hierarchy.hierarchypages.size() - 1) {
			updatedisplay = true;
			currentpage++;
			if (currentlist >= hierarchy.hierarchypages[currentpage].pagelists.size()) {
				currentlist = hierarchy.hierarchypages[currentpage].pagelists.size() - 1;
			}
			if (currentitem >= hierarchy.hierarchypages[currentpage].pagelists[currentlist].listitems.size()) {
				currentitem = hierarchy.hierarchypages[currentpage].pagelists[currentlist].listitems.size() - 1;
			}
		}
		if (input == 13) {
			TerminateWindow();
			firstitem = 0;
			firstlist = 0;
			firstpage = 0;
			return(hierarchy.hierarchypages[currentpage].pagelists[currentlist].listitems[currentitem]);
		}
		if (input == 27) {
			firstitem = 0;
			firstlist = 0;
			firstpage = 0;
			return("NULL");
		}
	}
	return("NULL");
}

void pessum::conscientia::advanced::DisplayMenu(luxreader::Hierarchy hierarchy, int currentpage, int currentlist, int currentitem)
{
	ClearWindow();
	char verticalchar = char(179);
	char horizontalchar = char(196);
	char intersectchar = char(193);
	std::stringstream vss, hss, iss;
	std::string vertical;
	std::string horizontal;
	std::string intersect;
	vss << verticalchar;
	vss >> vertical;
	hss << horizontalchar;
	hss >> horizontal;
	iss << intersectchar;
	iss >> intersect;
	std::string line, outputline;
	int sizex = virtualwindows[boundwindow].sizex - 2;
	int sizey = virtualwindows[boundwindow].sizey - 2;
	int pagewidth = sizex / hierarchy.hierarchypages.size();
	int listwidth = sizex / hierarchy.hierarchypages[currentpage].pagelists.size();
	int pagesdisplayed = hierarchy.hierarchypages.size();
	int listsdisplayed = hierarchy.hierarchypages[currentpage].pagelists.size();
	int itemsdisplayed = hierarchy.hierarchypages[currentpage].pagelists[currentlist].listitems.size();
	int x = 0, y = 1;
	if (pagewidth < (sizex - 4) / 5) {
		pagewidth = (sizex - 3) / 4;
		pagesdisplayed = 4;
	}
	if (listwidth < sizex / 9) {
		listwidth = sizex / 8;
		listsdisplayed = 8;
	}
	while (currentpage >= pagesdisplayed + firstpage - 1 && pagesdisplayed + firstpage < hierarchy.hierarchypages.size()) {
		firstpage++;
	}
	while (currentpage <= firstpage && firstpage > 0) {
		firstpage--;
	}
	while (currentlist >= listsdisplayed + firstlist - 1 && listsdisplayed + firstlist < hierarchy.hierarchypages[currentpage].pagelists.size()) {
		firstlist++;
	}
	while (currentlist <= firstlist && firstlist > 0) {
		firstlist--;
	}
	while (currentitem >= itemsdisplayed + firstitem - 1 && itemsdisplayed + firstitem < hierarchy.hierarchypages[currentpage].pagelists[currentlist].listitems.size()) {
		firstitem++;
	}
	while (currentitem <= firstitem && firstitem > 0) {
		firstitem--;
	}
	for (int a = 0; a < pagewidth - 1; a++) {
		line = line + horizontal;
	}
	for (int a = firstpage; a < pagesdisplayed + firstpage; a++) {
		Print(hierarchy.hierarchypages[a].pagename, x + FindTextCenter(hierarchy.hierarchypages[a].pagename, pagewidth), y);
		if (currentpage != a) {
			Print(line, x + 1, y + 1);
		}
		x = x + pagewidth;
		if (a != pagesdisplayed - 1) {
			Print(vertical, x, y);
			Print(intersect, x, y + 1);
		}
	}
	x = 0;
	for (int a = firstlist; a < listsdisplayed + firstlist; a++) {
		y = 3;
		if (hierarchy.hierarchypages[currentpage].pagelists[a].listname != "") {
			if (a == currentlist) {
				outputline = "[>" + hierarchy.hierarchypages[currentpage].pagelists[a].listname + "<]";
			}
			else {
				outputline = "<" + hierarchy.hierarchypages[currentpage].pagelists[a].listname + ">";
			}
			Print(outputline, FindTextCenter(outputline, listwidth) + x, y);
		}
		y++;
		for (int b = firstitem; b < itemsdisplayed + firstitem && b < hierarchy.hierarchypages[currentpage].pagelists[a].listitems.size(); b++) {
			if (a == currentlist && b == currentitem) {
				outputline = "[" + hierarchy.hierarchypages[currentpage].pagelists[currentlist].listitems[currentitem] + "]";
				Print(outputline, FindTextCenter(outputline, listwidth) + x, y);
			}
			else {
				outputline = hierarchy.hierarchypages[currentpage].pagelists[a].listitems[b];
				Print(outputline, FindTextCenter(outputline, listwidth) + x, y);
			}
			y++;
		}
		x = x + listwidth;
	}
	Update();
}

int pessum::conscientia::advanced::AdvancedLoadingBar(std::string process, bool decimal)
{
	LoadingBar newbar;
	newbar.name = process;
	newbar.percent = 0;
	newbar.windowpointer = virtualwindows.size();
	newbar.decimal = decimal;
	loadingbars.push_back(newbar);
	int basex = virtualwindows[0].sizex;
	int basey = virtualwindows[0].sizey;
	GenorateWindow(process, (basex / 2), 3, (basex / 2) - (basex / 4), (basey / 2) - 1, true, true);
	BindLoadingBar(loadingbars.size() - 1);
	return(loadingbars.size() - 1);
}

void pessum::conscientia::advanced::UpdateLoadingBarRaw(double percent, int loadingbar)
{
	loadingbars[loadingbar].percent = percent;
	char block = char(219);
	int size = virtualwindows[loadingbars[loadingbar].windowpointer].sizex - 2;
	double blockworth;
	if (loadingbars[loadingbar].decimal == true) {
		blockworth = (double)1 / (double)size;
	}
	else if (loadingbars[loadingbar].decimal == false) {
		blockworth = (double)100 / (double)size;
	}
	ClearWindow();
	std::string bar = "";
	while (percent > blockworth) {
		bar = bar + block;
		percent = percent - blockworth;
	}
	Print(bar, 1, 1);
}

void pessum::conscientia::advanced::UpdateLoadingBarAdd(double addpercent, int loadingbar)
{
	loadingbars[loadingbar].percent = loadingbars[loadingbar].percent + addpercent;;
	char block = char(219);
	int size = virtualwindows[loadingbars[loadingbar].windowpointer].sizex - 2;
	double percent = loadingbars[loadingbar].percent + addpercent;
	double blockworth;
	if (loadingbars[loadingbar].decimal == true) {
		blockworth = (double)1 / (double)size;
	}
	else if (loadingbars[loadingbar].decimal == false) {
		blockworth = (double)100 / (double)size;
	}
	ClearWindow();
	std::string bar = "";
	double mathpercent = percent;
	while (percent > blockworth) {
		bar = bar + block;
		percent = percent - blockworth;
	}
	Print(bar, 1, 1);
}

void pessum::conscientia::advanced::TermianteLoadingBar(int loadingbar)
{
	TerminateWindow(loadingbars[boundloadingbar].windowpointer);
	loadingbars.erase(loadingbars.begin() + boundloadingbar);
	if (boundloadingbar > 1) {
		boundloadingbar--;
	}
	else {
		boundloadingbar = 0;
	}
}

void pessum::conscientia::advanced::BindLoadingBar(int pointer)
{
	boundloadingbar = pointer;
}