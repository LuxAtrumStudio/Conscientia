#ifndef _PESSUM_FILES_CONSCIENTIA_FILES_CONSCIENTIA_H_
#define _PESSUM_FILES_CONSCIENTIA_FILES_CONSCIENTIA_H_
#include <string>
#include <vector>
#include <Windows.h>
#include "../lux_reader.h"
#include "../pessum_core.h"
namespace pessum {
	namespace conscientia {
		namespace advanced {}
		//Structure for storing virtual window data
		struct Window {
			std::string name;
			int posx, posy, sizex, sizey;
			int cursorx, cursory;
			bool border, title;
		};
		//Log location integer index
		extern int logloc;
		//Vector of all currently active windows
		extern std::vector<Window> virtualwindows;
		//Pointer to currently active window from virtualwindows
		extern int boundwindow;
		//Declarations for two buffers, so one is a load, and the other a display
		extern HANDLE displaybuffer1, displaybuffer2;
		//Indication which buffer is the current load buffer
		extern bool seconedbuffer;
		//Windows console information for conversion to internal methods of data management
		extern _CONSOLE_SCREEN_BUFFER_INFO consoleinfo;
		//Loaded all neccesary data, and creates a default virtual window, and sets log locations
		void InitializeConscientia();
		//Creates a new window given parameters, and binds it as current virtual window
		void GenorateWindow(std::string name = "scr", int sizex = 10, int sizey = 10, int posx = 0, int posy = 0, bool border = true, bool title = false);
		//Clears the content of all windows in the vitralwindows vector
		void ClearWindowAll();
		//Determins the int value of a pointer to a window given the virtual window name
		int FindWindowPointer(std::string name = "stdscr");
		//Sets currently active virtual window
		void BindWindow(int pointer = 0);
		//Toggles the virtual windows border
		void ToggleBorder(int pointer = boundwindow);
		//Clears the current virtual window
		void ClearWindow(int pointer = boundwindow);
		//Toggles the virtual windows title display
		void ToggleTitle(int pointer = boundwindow);
		//Returns the size of the current virtual window
		void GetSize(int& x, int& y, int pointer = boundwindow);
		//Draws the border of the virtual window
		void DrawBorder(int pointer = boundwindow);
		//Draws the title of the virtual window
		void DrawTitle(int pointer = boundwindow);
		//Erases the virtual window and removes it from virtualwindows vector
		void TerminateWindow(int pointer = boundwindow);
		//Termiantes all windows
		void TerminateWindowAll();
		//Returns char value of user key press
		char GetChar();
		//Returns int value entered
		int CinInt();
		//Returns string value entered
		std::string CinString();
		//Returns double/float falue entered
		double CinFloat();
		//Outputs text to the console based on the vitrual window, and the cursor coords
		void Print(std::string text = "NULL", int cursorx = -1, int cursory = -1, int pointer = boundwindow);
		//Swiches the two buffers
		void Update();
		//Changes the console to a new name
		void SetConsoleName(std::string consolename = "NULL");
		//Functinal to determin which buffer to output to
		void WriteOutput(std::string text = "NULL", COORD position = { 0, 0 });
		//Termiantes all windows, and unloades any data
		void TerminateConscientia();
		//Locates the text starting position to center text given the space avalible
		int FindTextCenter(std::string text = "NULL", int space = 0);
	}
}
#endif // !_PESSUM_FILES_CONSCIENTIA_FILES_CONSCIENTIA_H_