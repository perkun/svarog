#include "svpch.h"
#include "ObjHeader.h"


ObjHeader::ObjHeader(string filename)
{
    parse(filename);
}


ObjHeader::~ObjHeader()
{
}


void ObjHeader::parse(string filename)
{
    FILE *f = fopen(filename.c_str(), "r");
	if (f == NULL)
	{
		loaded = false;
		return;
	}

    char buf[1000];
    while (fgets(buf, 999, f) != NULL)
    {
        if (buf[0] != '#')
            continue;

        string line = buf;

        size_t semicolon_pos = line.find_first_of(":");
        if (semicolon_pos == string::npos)
            continue;

        // szuka spacji lub # od pozycji : wstecz
        size_t non_white_pos = line.find_last_of("#", semicolon_pos) + 1;
        string key =
            line.substr(non_white_pos, (semicolon_pos - non_white_pos));

        string value = line.substr(semicolon_pos + 1);

        trim(key);
        trim(value);

		if (value != "")
			items[key] = value;
    }
    fclose(f);
	loaded = true;
}


void ObjHeader::trim(string &s)
{
    size_t p = s.find_first_not_of(" \t\n");
    s.erase(0, p);

    p = s.find_last_not_of(" \t\n");
    if (string::npos != p)
        s.erase(p + 1);
}
