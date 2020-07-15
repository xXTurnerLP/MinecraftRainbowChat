// VENDOR
#include "color/color.hpp"

// STL
#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>

#include <Windows.h>

#define MC_MAX_NAME_LENGTH 32 + 1 // 1 for null termination

int main() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY); // LIGHT YELLOW
	puts("Minecrart rainbow text generator v1.0 - by: turner#6040\n\n");
	
	char szNameBuffer[MC_MAX_NAME_LENGTH] = {};
	std::string szTextBuffer;

	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	puts("Minecraft Username (Spaces are ignored, 32 max characters): ");

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // WHITE
	unsigned char counter = 0;
	unsigned char temp_char = 0;
	while ((temp_char = fgetc(stdin)) != '\n') {
		if (counter < 32 && temp_char != ' ') {
			szNameBuffer[counter] = temp_char;
			++counter;
		}
	}

	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	puts("Minecraft Message: ");

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // WHITE
	std::getline(std::cin, szTextBuffer);

	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE); // AQUA-ISH
	puts("Generating command ..");

	if (strlen(szNameBuffer) == 0 || szTextBuffer.size() == 0) {
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED); // RED
		puts("Generation failed: Username or Message is empty! Exiting..");
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // WHITE
		system("pause");
		return 1;
	}

	std::string mcCommand = std::string("tellraw @a [{\"text\":\"<\"},{\"selector\":\"") + szNameBuffer + "\"},{\"text\":\"> \"},";

	for (unsigned int i = 0; i < szTextBuffer.size(); ++i) {
		color::hsl<double> color({360.0 / szTextBuffer.size() * (i + 1), 100.0, 50.0});
		color::rgb<double> color_converted; color_converted = color;

		std::stringstream ss;

		unsigned int red = static_cast<int>(round(color::get::red(color_converted) * 255));
		unsigned int green = static_cast<int>(round(color::get::green(color_converted) * 255));
		unsigned int blue = static_cast<int>(round(color::get::blue(color_converted) * 255));

		if (red < 16)
			ss << std::hex << 0 << red;
		else
			ss << std::hex << red;

		if (green < 16)
			ss << std::hex << 0 << green;
		else
			ss << std::hex << green;

		if (blue < 16)
			ss << std::hex << 0 << blue;
		else
			ss << std::hex << blue;

		if (i == szTextBuffer.size() - 1) {
			mcCommand += "{\"text\":\"";
			mcCommand += szTextBuffer.c_str()[i];
			mcCommand += "\",\"color\":\"#";

			mcCommand += ss.str();

			mcCommand += "\"}]";
		} else {
			mcCommand += "{\"text\":\"";
			mcCommand += szTextBuffer.c_str()[i];
			mcCommand += "\",\"color\":\"#";

			mcCommand += ss.str();

			mcCommand += "\"},";
		}
	}

	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, mcCommand.size() + 1);
	memcpy(GlobalLock(hMem), mcCommand.c_str(), mcCommand.size() + 1);
	GlobalUnlock(hMem);

	if (!OpenClipboard(GetActiveWindow())) {
		printf("Windows Error: Could not copy to clipboard. Code: %u\n", GetLastError());
		printf("Command:\n", mcCommand.c_str());
	} else {
		EmptyClipboard();
		if (!SetClipboardData(CF_TEXT, hMem)) {
			printf("Windows Error: Could not copy to clipboard. Code: %u\n", GetLastError());
			printf("Command:\n", mcCommand.c_str());
		} else {
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY); // WHITE
			puts("Copied the command to the clipboard!");
			CloseClipboard();
		}
	}

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // WHITE
	system("pause");
}