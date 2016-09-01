#include "conscientia.h"
#include <string>
#include <sstream>
#include <vector>
#include <Windows.h>
#include <conio.h>

#include "../pessum_core.h"
#include "../logging.h"
#include "../lux_reader.h"
#include "conscientia_advanced.h"

namespace pessum {
	namespace conscientia {
		int logloc = 0;
		std::vector<Window> virtualwindows;
		int boundwindow = 0;
		int currentbuffer = 0;
		HANDLE displaybuffer1, displaybuffer2;
		bool seconedbuffer = true;
		_CONSOLE_SCREEN_BUFFER_INFO consoleinfo;
	}
}

void pessum::conscientia::InitializeConscientia()
{
	logloc = pessum::logging::AddLogLocation("pessum_files/conscientia_files/conscientia/");
	advanced::logloc = pessum::logging::AddLogLocation("pessum_files/conscientia_files/advanced");
	displaybuffer1 = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	displaybuffer2 = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	if (SetConsoleActiveScreenBuffer(displaybuffer1) == false) {
		pessum::logging::LogLoc(pessum::logging::LOG_ERROR, "Failed to set display buffer", logloc, "InitializeConscientia");
	}
	else {
		pessum::logging::LogLoc(pessum::logging::LOG_SUCCESS, "Set display buffer", logloc, "InitalizeConscientia");
	}
	if (GetConsoleScreenBufferInfo(displaybuffer1, &consoleinfo) == false) {
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
	sstreamH << H;
	sstreamH >> characterH;
	for (x = 0; x <= virtualwindows[pointer].sizex - 1; x++) {
		posH = { (short)x + (short)virtualwindows[pointer].posx, (short)virtualwindows[pointer].posy };
		WriteOutput(characterH, posH);
		posH = { (short)x + (short)virtualwindows[pointer].posx, (short)(virtualwindows[pointer].posy + virtualwindows[pointer].sizey) - 1 };
		WriteOutput(characterH, posH);
	}
	std::string characterV;
	std::stringstream sstreamV;
	COORD posV;
	sstreamV << V;
	sstreamV >> characterV;
	for (y = 0; y <= virtualwindows[pointer].sizey - 1; y++) {
		posV = { (short)virtualwindows[pointer].posx, (short)y + (short)virtualwindows[pointer].posy };
		WriteOutput(characterV, posV);
		posV = { (short)(virtualwindows[pointer].posx + virtualwindows[pointer].sizex) - 1, (short)y + (short)virtualwindows[pointer].posy };
		WriteOutput(characterV, posV);
	}
	std::string character1;
	std::stringstream sstream1;
	COORD pos1;
	sstream1 << TL;
	sstream1 >> character1;
	pos1 = { (short)virtualwindows[pointer].posx, (short)virtualwindows[pointer].posy };
	WriteOutput(character1, pos1);
	std::string character2;
	std::stringstream sstream2;
	COORD pos2;
	sstream2 << TR;
	sstream2 >> character2;
	pos2 = { (short)(virtualwindows[pointer].posx + virtualwindows[pointer].sizex) - 1, (short)virtualwindows[pointer].posy };
	WriteOutput(character2, pos2);
	std::string character3;
	std::stringstream sstream3;
	COORD pos3;
	sstream3 << BL;
	sstream3 >> character3;
	pos3 = { (short)virtualwindows[pointer].posx, (short)(virtualwindows[pointer].posy + virtualwindows[pointer].sizey) - 1 };
	WriteOutput(character3, pos3);
	std::string character4;
	std::stringstream sstream4;
	COORD pos4;
	sstream4 << BR;
	sstream4 >> character4;
	pos4 = { (short)(virtualwindows[pointer].posx + virtualwindows[pointer].sizex) - 1, (short)(virtualwindows[pointer].posy + virtualwindows[pointer].sizey) - 1 };
	WriteOutput(character4, pos4);
}

void pessum::conscientia::DrawTitle(int pointer)
{
	int titleSize = virtualwindows[pointer].name.size();
	int windowSize = virtualwindows[pointer].sizex;
	windowSize = windowSize / 2;
	titleSize = titleSize / 2;
	COORD pos = { (windowSize - titleSize) + virtualwindows[pointer].posx, virtualwindows[pointer].posy };
	std::string title = virtualwindows[pointer].name;
	WriteOutput(title, pos);
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

char pessum::conscientia::GetChar()
{
	char input;
	input = _getch();
	return(input);
}

int pessum::conscientia::CinInt()
{
	int input = 0;
	int rawin = 0;
	while (rawin != 13) {
		rawin = _getch();
		input = (input * 10) + rawin;
	}
	return(input);
}

std::string pessum::conscientia::CinString()
{
	std::string input;
	char inputchar;
	int rawint = 0;
	while (rawint != 13) {
		rawint = _getch();
		inputchar = char(rawint);
		input = input + inputchar;
	}
	return(input);
}

double pessum::conscientia::CinFloat()
{
	std::string rawstr = CinString();
	std::string::size_type sz;
	double input;
	input = stof(rawstr, &sz);
	return(input);
}

void pessum::conscientia::Print(std::string text, int cursorx, int cursory, int pointer)
{
	if (cursorx != -1) {
		virtualwindows[pointer].cursorx = cursorx;
	}
	if (cursory != -1) {
		virtualwindows[pointer].cursory = cursory;
	}
	for (int a = 0; a < text.size(); a++) {
		if (text[a] == '/') {
			int b = a + 1;
			if (text[b] == 'n') {
				virtualwindows[pointer].cursory++;
				if (virtualwindows[pointer].cursory > virtualwindows[pointer].sizey) {
					virtualwindows[pointer].cursory = 0;
					if (virtualwindows[pointer].border == true || virtualwindows[pointer].title == true) {
						virtualwindows[pointer].cursory++;
					}
				}
				ClearWindow(pointer);
				a = a + 2;
			}
		}
		else if (text[a] != '/') {
			if (virtualwindows[pointer].border == false) {
				if (virtualwindows[pointer].cursorx >= virtualwindows[pointer].sizex) {
					virtualwindows[pointer].cursorx = 0;
					virtualwindows[pointer].cursory++;
					if (virtualwindows[pointer].cursory >= virtualwindows[pointer].sizey) {
						virtualwindows[pointer].cursory = 0;
						if (virtualwindows[pointer].title == true) {
							virtualwindows[pointer].cursory++;
						}
						ClearWindow(pointer);
					}
				}
			}
			if (virtualwindows[pointer].border == true) {
				if (virtualwindows[pointer].cursorx >= virtualwindows[pointer].sizex - 1) {
					virtualwindows[pointer].cursorx = 1;
					virtualwindows[pointer].cursory++;
					if (virtualwindows[pointer].cursory >= virtualwindows[pointer].sizey - 1) {
						virtualwindows[pointer].cursory = 1;
						ClearWindow(pointer);
					}
				}
			}
			std::string character;
			std::stringstream sstream;
			sstream << text[a];
			sstream >> character;
			COORD pos = { virtualwindows[pointer].cursorx + virtualwindows[pointer].posx, virtualwindows[pointer].cursory + virtualwindows[pointer].posy };
			WriteOutput(character, pos);
			virtualwindows[pointer].cursorx++;
		}
	}
}

void pessum::conscientia::Update()
{
	if (seconedbuffer == true) {
		SetConsoleActiveScreenBuffer(displaybuffer2);
		seconedbuffer = false;
		displaybuffer2 = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	}
	else if (seconedbuffer == false) {
		SetConsoleActiveScreenBuffer(displaybuffer1);
		seconedbuffer = true;
		displaybuffer1 = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	}
	for (unsigned a = 0; a < virtualwindows.size(); a++) {
		if (virtualwindows[a].border == true) {
			DrawBorder(a);
		}
		if (virtualwindows[a].title == true) {
			DrawTitle(a);
		}
	}
}

void pessum::conscientia::SetConsoleName(std::string consolename)
{
	SetConsoleTitle(consolename.c_str());
}

void pessum::conscientia::WriteOutput(std::string text, COORD position)
{
	DWORD dwBytesWritten = 0;
	if (seconedbuffer == false) {
		WriteConsoleOutputCharacter(displaybuffer1, text.c_str(), text.size(), position, &dwBytesWritten);
	}
	else if (seconedbuffer == true) {
		WriteConsoleOutputCharacter(displaybuffer2, text.c_str(), text.size(), position, &dwBytesWritten);
	}
}

void pessum::conscientia::TerminateConscientia()
{
	TerminateWindowAll();
}

int pessum::conscientia::FindTextCenter(std::string text, int space)
{
	space = space / 2;
	int textsize = text.size();
	textsize = textsize / 2;
	int textstart = space - textsize;
	return(textstart);
}