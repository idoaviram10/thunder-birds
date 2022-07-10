#include <iostream>
#include "Menu.h"

/*---------------------------------------------------------*/
/*                                                         */
/*                  Ori Avitan 206589855                   */
/*                                                         */
/*                  Ido Aviram 318599834                   */
/*                                                         */
/*---------------------------------------------------------*/


void wrongCommandLineInput();

int main(int argc, char** argv) {
	hide_cursor();
	int mode = simple;
	bool loadON = false;
	bool saveON = false;
	Menu* menu = new Menu();

	if (argc == 1)
		mode = simple;
	else if (strcmp(argv[1], "-load") == 0) {
		mode = load;
		loadON = true;
	}
	else if (strcmp(argv[1], "-save") == 0) {
		mode = save;
		saveON = true;
	}
	else {
		wrongCommandLineInput();
		return 0;
	}

	if (loadON) {
		if (argv[2]) {
			if (strcmp(argv[2], "[-silent]") == 0)
				mode = silent;
			else {
				wrongCommandLineInput();
				return 0;
			}
		}
	}

	if (saveON) {
		if (argv[2] && (strcmp(argv[2], "[-silent]") != 0)) {
			wrongCommandLineInput();
			return 0;
		}
	}

	menu->menuManager(mode);

	delete menu;
	return 0;
}


void wrongCommandLineInput() {
	cout << "Wrong input!" << endl << "Please write: -load | -save | -load [-silent] \n\n\n\n";
}