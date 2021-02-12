#include "svpch.h"
#include "File.h"


string File::get_file_extension(string filename)
{
	int dot_pos = filename.rfind(".");

	if (dot_pos == string::npos)
		return string("");

	return filename.substr(dot_pos + 1, filename.size() - dot_pos);
}

string File::get_file_base(string filename)
{
	int dot_pos = filename.rfind(".");
	return filename.substr(0, dot_pos - 1);
}

bool File::is_extension(string filename, string ext)
{
	return ext.compare(File::get_file_extension(filename)) == 0;
}



