#ifndef OBJHEADER_H_
#define OBJHEADER_H_

#include <regex>

using namespace std;

class ObjHeader
{
public:
	ObjHeader() = default;
	ObjHeader(string filename);
	~ObjHeader();

	template<typename T>
	T get_item(string key);
	bool loaded = false;

private:
	void parse(string filename);
	void trim(string &);

	map<string, string> items;

};

template<typename T>
T ObjHeader::get_item(string key)
{
    stringstream ss;
	if (items.find(key) == items.end()) // not found
		ss.str("0.0");
	else
		ss.str(items[key]);

    T x;
    ss >> x;
    return x;
}


#endif /* OBJHEADER_H_ */
