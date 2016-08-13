#include "Interface.h"
#include <iostream>
#include <conio.h>  // _getch()

using namespace std;

void menu();

int main()
{
	system("mode 650");
	previous_load();

	while (!cin.fail())
		menu();

	getchar();
	return 0;
}

void menu()
{
	cout << endl << "a - add file,  r - reduce file, o - mem dump, f - FAT16 output" << endl;

	switch (_getch())
	{
	case 'a':
	{
				char * name = new char[11];
				unsigned int size;

				cout << "File name: ";
				cin >> name;

				cout << "File size: ";
				cin >> dec >> size;

				add_file(name, size);
				break;
	}
	case 'r':
	{
				char * name = new char[11];
				unsigned int size;

				cout << "File name: ";
				cin >> name;

				cout << "New size: ";
				cin >> dec >> size;

				red_file(name, size);
				break;
	}
	case 'o':
	{
				unsigned int from, to;

				cout << "From: [0..FFF0]     ";
				cin >> hex >> from;

				cout << "To: [0..FFF0]     ";
				cin >> hex >> to;

				mem_dump(from, to);
				break;
	}
	case 'f':
		output_fat();
		break;

	case 'q':
		exit(0);

	default:
		cout << "Wrong choice :(" << endl << endl;
		break;
	}
}