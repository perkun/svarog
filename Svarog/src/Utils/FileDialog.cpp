#include "FileDialog.h"


string FileDialog::save_file(const char* filter)
{
    char filename[1024];
	char command[1024];
	sprintf(command, "zenity --file-selection --save --confirm-overwrite --file-filter \"%s\"", filter);
    FILE *f = popen(command, "r");
    fscanf(f, "%s", filename);

	return string(filename);
}

string FileDialog::open_file(const char *filter)
{

	char filename[1024];
	char command[1024];
	sprintf(command, "zenity --file-selection --file-filter \"%s\"", filter);
	FILE *f = popen(command, "r");
	fscanf(f, "%s", filename);

	return string(filename);
}
