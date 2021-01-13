#ifndef STRING_H_
#define STRING_H_

using namespace std;

class File
{
public:
	static string get_file_extension(string filename);
	static bool is_extension(string filename, string ext);

};

#endif /* STRING_H_ */
