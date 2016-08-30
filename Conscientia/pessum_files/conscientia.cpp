#include "conscientia.h"
#include <string>
#include <sstream>
#include <vector>
#include <Windows.h>
#include "pessum_core.h"
#include "logging.h"

namespace pessum {
	namespace conscientia {
		int logloc = 0;
		std::vector<Window> virtualwindows;
		int boundwindow = 0;
		int currentbuffer = 0;
		HANDLE loadbuffer, displaybuffer;
		_CONSOLE_SCREEN_BUFFER_INFO consoleinfo;
		int firstpage = 0, firstlist = 0, firstitem = 0;
		std::vector<int> loadingbars;
	}
}

void pessum::conscientia::InitializeConscientia()
{
	logloc = pessum::logging::AddLogLocation("pessum_files/conscientia/");
	loadbuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	displaybuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	if (SetConsoleActiveScreenBuffer(displaybuffer) == false) {
		pessum::logging::LogLoc(pessum::logging::LOG_ERROR, "Failed to set display buffer", logloc, "InitializeConscientia");
	}
	else {
		pessum::logging::LogLoc(pessum::logging::LOG_SUCCESS, "Set display buffer", logloc, "InitalizeConscientia");
	}
	if (GetConsoleScreenBufferInfo(displaybuffer, &consoleinfo) == false) {
		pessum::logging::LogLoc(pessum::logging::LOG_ERROR, "Failed to get display buffer info", logloc, "InitializeConscientia");
	}
	else {
		pessum::logging::LogLoc(pessum::logging::LOG_SUCCESS, "Got display buffer info", logloc, "InitalizeConscientia");
	}
	Window declaration;
	declaration.name = "stdscr";
	declaration.posx = 0;
	declaration.posy = 0;
	declaration.sizex = consoleinfo.dwSize.X;
	declaration.sizey = consoleinfo.dwSize.Y;
	declaration.cursorx = 0;
	declaration.cursory = 0;
	declaration.border = false;
	declaration.title = false;
	virtualwindows.push_back(declaration);
}

void pessum::conscientia::GenorateWindow(std::string name, int sizex, int sizey, int posx, int posy, bool border, bool title)
{
	Window declaration;
	declaration.name = name;
	declaration.posx = posx;
	declaration.posy = posy;
	declaration.sizex = sizex;
	declaration.sizey = sizey;
	declaration.cursorx = 0;
	declaration.cursory = 0;
	declaration.border = border;
	declaration.title = title;
	if (border == true) {
		declaration.cursorx++;
		declaration.cursory++;
	}
	if (title == true && border == false) {
		declaration.cursory++;
	}
	virtualwindows.push_back(declaration);
	pessum::logging::LogLoc(pessum::logging::LOG_SUCCESS, "Genorated virtual window: " + name, logloc, "GenorateWindow");
	BindWindow(virtualwindows.size() - 1);
	if (border == true) {
		DrawBorder();
	}
	if (title == true) {
		DrawTitle();
	}
}

void pessum::conscientia::ClearWindowAll()
{
	for (unsigned a = 0; a < virtualwindows.size(); a++) {
		ClearWindow(a);
	}
}

int pessum::conscientia::FindWindowPointer(std::string name)
{
	for (unsigned a = 0; a < virtualwindows.size(); a++) {
		if (virtualwindows[a].name == name) {
			return(a);
		}
	}
	return(0);
}

void pessum::conscientia::BindWindow(int pointer)
{
	boundwindow = pointer;
}

void pessum::conscientia::ToggleBorder(int pointer)
{
	if (virtualwindows[pointer].border == true) {
		virtualwindows[pointer].border = false;
	}
	else {
		virtualwindows[pointer].border = false;
	}
	DrawBorder(pointer);
}

void pessum::conscientia::ClearWindow(int pointer)
{
	std::string clear = "";
	int charCount, x, y;
	virtualwindows[pointer].cursorx = 0;
	virtualwindows[pointer].cursory = 0;
	if (virtualwindows[pointer].border == true || virtualwindows[pointer].title == true) {
		virtualwindows[pointer].cursory++;
	}
	if (virtualwindows[pointer].border == true) {
		virtualwindows[pointer].cursorx++;
	}
	x = virtualwindows[pointer].sizex;
	y = virtualwindows[pointer].sizey;
	if (virtualwindows[pointer].border == true) {
		x -= 2;
		y -= 2;
	}
	if (virtualwindows[pointer].title == true && virtualwindows[pointer].border == false) {
		x--;
	}
	charCount = x;
	for (int a = 0; a < charCount; a++) {
		clear = clear + char(0);
	}
	for (int a = 0; a < y; a++) {
		//PrintText(clear, pointer);
	}
	clear = "";
	virtualwindows[pointer].cursorx = 0;
	virtualwindows[pointer].cursory = 0;
	if (virtualwindows[pointer].border == true || virtualwindows[pointer].title == true) {
		virtualwindows[pointer].cursory++;
	}
	if (virtualwindows[pointer].border == true) {
		virtualwindows[pointer].cursorx++;
	}
}

void pessum::conscientia::ToggleTitle(int pointer)
{
	if (virtualwindows[pointer].title == true) {
		virtualwindows[pointer].title = false;
	}
	else {
		virtualwindows[pointer].title = false;
	}
	DrawTitle(pointer);
}

void pessum::conscientia::GetSize(int& x, int& y, int pointer)
{
	x = virtualwindows[pointer].sizex;
	y = virtualwindows[pointer].sizey;
}

void pessum::conscientia::DrawBorder(int pointer)
{
	int x = 0, y = 0;
	char TL = char(218), TR = char(191), BL = char(192), BR = char(217);
	char V = char(179), H = char(196);
	std::string characterH;
	std::stringstream sstreamH;
	COORD posH;
	DWORD dwBytesWrittenH = 0;
	sstreamH << H;
	sstreamH >> characterH;
	for (x = 0; x <= virtualwindows[pointer].sizex - 1; x++) {
		posH = { (short)x + (short)virtualwindows[pointer].posx, (short)virtualwindows[pointer].posy };
		WriteConsoleOutputCharacter(loadbuffer, characterH.c_str(), characterH.size(), posH, &dwBytesWrittenH);
		posH = { (short)x + (short)virtualwindows[pointer].posx, (short)(virtualwindows[pointer].posy + virtualwindows[pointer].sizey) - 1 };
		WriteConsoleOutputCharacter(loadbuffer, characterH.c_str(), characterH.size(), posH, &dwBytesWrittenH);
	}
	std::string characterV;
	std::stringstream sstreamV;
	COORD posV;
	DWORD dwBytesWrittenV = 0;
	sstreamV << V;
	sstreamV >> characterV;
	for (y = 0; y <= virtualwindows[pointer].sizey - 1; y++) {
		posV = { (short)virtualwindows[pointer].posx, (short)y + (short)virtualwindows[pointer].posy };
		WriteConsoleOutputCharacter(loadbuffer, characterV.c_str(), characterV.size(), posV, &dwBytesWrittenV);
		posV = { (short)(virtualwindows[pointer].posx + virtualwindows[pointer].sizex) - 1, (short)y + (short)virtualwindows[pointer].posy };
		WriteConsoleOutputCharacter(loadbuffer, characterV.c_str(), characterV.size(), posV, &dwBytesWrittenV);
	}
	std::string character1;
	std::stringstream sstream1;
	COORD pos1;
	DWORD dwBytesWritten1 = 0;
	sstream1 << TL;
	sstream1 >> character1;
	pos1 = { (short)virtualwindows[pointer].posx, (short)virtualwindows[pointer].posy };
	WriteConsoleOutputCharacter(loadbuffer, character1.c_str(), character1.size(), pos1, &dwBytesWritten1);
	std::string character2;
	std::stringstream sstream2;
	COORD pos2;
	DWORD dwBytesWritten2 = 0;
	sstream2 << TR;
	sstream2 >> character2;
	pos2 = { (short)(virtualwindows[pointer].posx + virtualwindows[pointer].sizex) - 1, (short)virtualwindows[pointer].posy };
	WriteConsoleOutputCharacter(loadbuffer, character2.c_str(), character2.size(), pos2, &dwBytesWritten2);
	std::string character3;
	std::stringstream sstream3;
	COORD pos3;
	DWORD dwBytesWritten3 = 0;
	sstream3 << BL;
	sstream3 >> character3;
	pos3 = { (short)virtualwindows[pointer].posx, (short)(virtualwindows[pointer].posy + virtualwindows[pointer].sizey) - 1 };
	WriteConsoleOutputCharacter(loadbuffer, character3.c_str(), character3.size(), pos3, &dwBytesWritten3);
	std::string character4;
	std::stringstream sstream4;
	COORD pos4;
	DWORD dwBytesWritten4 = 0;
	sstream4 << BR;
	sstream4 >> character4;
	pos4 = { (short)(virtualwindows[pointer].posx + virtualwindows[pointer].sizex) - 1, (short)(virtualwindows[pointer].posy + virtualwindows[pointer].sizey) - 1 };
	WriteConsoleOutputCharacter(loadbuffer, character4.c_str(), character4.size(), pos4, &dwBytesWritten4);
}

void pessum::conscientia::DrawTitle(int pointer)
{
	int titleSize = virtualwindows[pointer].name.size();
	int windowSize = virtualwindows[pointer].sizex;
	windowSize = windowSize / 2;
	titleSize = titleSize / 2;
	COORD pos = { (windowSize - titleSize) + virtualwindows[pointer].posx, virtualwindows[pointer].posy };
	DWORD dwBytesWritten;
	std::string title = virtualwindows[pointer].name;
	WriteConsoleOutputCharacter(loadbuffer, title.c_str(), title.size(), pos, &dwBytesWritten);
}

void pessum::conscientia::TerminateWindow(int pointer)
{
	if (virtualwindows[pointer].border == true) {
		ToggleBorder(pointer);
	}
	if (virtualwindows[pointer].title == true) {
		ToggleTitle(pointer);
	}
	ClearWindow(pointer);
	virtualwindows.erase(virtualwindows.begin() + pointer);
}

void pessum::conscientia::TerminateWindowAll()
{
	for (unsigned a = 0; a < virtualwindows.size(); a++) {
	}
}