#ifndef FITSHEADER_H_
#define FITSHEADER_H_

#include <iostream>
#include <stdio.h>

using namespace std;

class FitsHeaderEntry
{
public:
	FitsHeaderEntry() {}
	~FitsHeaderEntry() {}

	string key, comment;
	virtual void write(fitsfile *file) {}
};


class FitsHeaderEntryInt : public FitsHeaderEntry
{
public:
	FitsHeaderEntryInt(string k, int v, string c)
	{
		key = k;
		value = v;
		comment = c;
	}

	int value;
	virtual void write(fitsfile *file) override {
		int status = 0;
		fits_update_key_lng(file, key.c_str(), value, comment.c_str(), &status);
	}
};


class FitsHeaderEntryFloat : public FitsHeaderEntry
{
public:
	FitsHeaderEntryFloat(string k, float v, string c)
	{
		key = k;
		value = v;
		comment = c;
	}

	float value;
	virtual void write(fitsfile *file) override {
		int status = 0;
		fits_update_key_fixflt(file, key.c_str(),
				value, 6, comment.c_str(), &status);
	}
};


class FitsHeaderEntryDouble : public FitsHeaderEntry
{
public:
	FitsHeaderEntryDouble(string k, double v, string c)
	{
		key = k;
		value = v;
		comment = c;
	}

	double value;

	virtual void write(fitsfile *file) override {
		int status = 0;
		fits_update_key_fixdbl(file, key.c_str(),
				value, 15, comment.c_str(), &status);
	}
};


class FitsHeaderEntryString : public FitsHeaderEntry
{
public:
	FitsHeaderEntryString(string k, string v, string c)
	{
		key = k;
		value = v;
		comment = c;
	}

	string value;

	virtual void write(fitsfile *file) override {
		int status = 0;
		fits_update_key_str(file, key.c_str(),
				value.c_str(), comment.c_str(), &status);
	}
};

class FitsHeader
{
public:
	void push(string key, float value, string comment)
	{
		auto hef = make_shared<FitsHeaderEntryFloat>(key, value, comment);
		entries.emplace_back(hef);
	}

	void push(string key, double value, string comment)
	{
		auto hef = make_shared<FitsHeaderEntryDouble>(key, value, comment);
		entries.emplace_back(hef);
	}

	void push(string key, string value, string comment)
	{
		auto hef = make_shared<FitsHeaderEntryString>(key, value, comment);
		entries.emplace_back(hef);
	}

	void push(string key, int value, string comment)
	{
		auto hef = make_shared<FitsHeaderEntryInt>(key, value, comment);
		entries.emplace_back(hef);
	}

	void write(fitsfile *file) const
	{
		for (auto he : entries)
			he->write(file);
	}

private:
	vector< shared_ptr<FitsHeaderEntry> > entries;
};

#endif /* FITSHEADER_H_ */
