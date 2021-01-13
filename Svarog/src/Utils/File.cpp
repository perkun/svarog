#include "svpch.h"
#include "File.h"


string File::get_file_extension(string filename)
{
	int dot_pos = filename.rfind(".");
	return filename.substr(dot_pos + 1, filename.size() - dot_pos);
}

bool File::is_extension(string filename, string ext)
{
	return ext.compare(File::get_file_extension(filename)) == 0;
}



