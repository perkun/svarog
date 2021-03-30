#ifndef FITSHEADER_H_
#define FITSHEADER_H_


using namespace std;

class FitsHeaderEntry
{
public:
	FitsHeaderEntry() {}
	~FitsHeaderEntry() {}

	string key, comment;
	virtual void write(fitsfile *file) {}
	virtual shared_ptr<FitsHeaderEntry> clone() = 0;
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

	virtual shared_ptr<FitsHeaderEntry> clone() override
	{
		return make_shared<FitsHeaderEntryInt>(key, value, comment);
	}

// private:
// 	FitsHeaderEntryInt(const FitsHeaderEntryInt &obj)
// 	{
// 	}
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

	virtual shared_ptr<FitsHeaderEntry> clone() override
	{
		return make_shared<FitsHeaderEntryFloat>(key, value, comment);
	}
// private:
// 	FitsHeaderEntryFloat(const FitsHeaderEntryFloat &obj)
// 	{
// 		return make_shared<FitsHeaderEntryFloat>(key, value, comment);
// 	}
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
	virtual shared_ptr<FitsHeaderEntry> clone() override
	{
		return make_shared<FitsHeaderEntryDouble>(key, value, comment);
	}
// private:
// 	FitsHeaderEntryDouble(const FitsHeaderEntryDouble &obj)
// 	{
// 		return make_shared<FitsHeaderEntryDouble>(key, value, comment);
// 	}
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

	virtual shared_ptr<FitsHeaderEntry> clone() override
	{
		return make_shared<FitsHeaderEntryString>(key, value, comment);
	}
// private:
// 	FitsHeaderEntryString(const FitsHeaderEntryString &obj)
// 	{
// 		return make_shared<FitsHeaderEntryString>(key, value, comment);
// 	}
};

class FitsHeader
{
public:
	FitsHeader() {}
	~FitsHeader() {}
	FitsHeader (const FitsHeader &obj)
	{
		entries.clear();
		for (auto entry : obj.entries)
		{
			entries.push_back(entry->clone());
		}
	}

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

	int read_int_key(fitsfile *file, string key) {
		char comment[256];
		int value, status = 0;
		fits_read_key(file, TLONG, key.c_str(), &value, comment, &status);
		push(key, value, string(comment));
		return value;
	}

	float read_float_key(fitsfile *file, string key) {
		char comment[256];
		float value;
		int status = 0;
		fits_read_key(file, TFLOAT, key.c_str(), &value, comment, &status);
		push(key, value, string(comment));
		return value;
	}

	double read_double_key(fitsfile *file, string key) {
		char comment[256];
		double value;
		int status = 0;

		fits_read_key(file, TDOUBLE, key.c_str(), &value, comment, &status);
		push(key, value, string(comment));
		return value;
	}

	string read_string_key(fitsfile *file, string key) {
		char comment[256];
		char value[256];
		int status = 0;
		fits_read_key(file, TSTRING, key.c_str(), &value, comment, &status);
		push(key, value, string(comment));
		return string(value);
	}

private:
	vector< shared_ptr<FitsHeaderEntry> > entries;
};

#endif /* FITSHEADER_H_ */
