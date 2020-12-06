#ifndef FILEDIALOG_H_
#define FILEDIALOG_H_

using namespace std;

class FileDialog
{
public:
	static string open_file(const char* filter);
	static string save_file(const char* filter);

};

#endif /* FILEDIALOG_H_ */
