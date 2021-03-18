#include "svpch.h"
#include "File.h"


string File::file_extension(string filename)
{
	int dot_pos = filename.rfind(".");

	if (dot_pos == string::npos)
		return string("");

	return filename.substr(dot_pos + 1, filename.size() - dot_pos);
}

string File::file_base(string filename)
{
	int slash_pos = filename.rfind("/");
	return filename.substr(slash_pos + 1, filename.size() - slash_pos);
}

string File::remove_extension(string filename)
{
	int dot_pos = filename.rfind(".");
	int slash_pos = filename.rfind("/");
	if (slash_pos > dot_pos)
		return filename;
	else
		return filename.substr(0, dot_pos);
}

bool File::is_extension(string filename, string ext)
{
	return ext.compare(File::file_extension(filename)) == 0;
}



