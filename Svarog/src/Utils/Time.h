#ifndef TIME_H
#define TIME_H

#define DAY_TO_SEC 86400

namespace Time {

struct CalendarDate
{
	CalendarDate() {}
	CalendarDate(int y, int mo, int d, int h, int mi, float sec)
		: year(y), month(mo), day(d), hour(h), minute(mi), second(sec) {}

	int year, month, day, hour, minute;
	double second;

	void normalize()
	{
		while (month > 12)
			month -= 12;
		while (month <= 0)
			month += 12;

		while (day > 31)
			day -= 31;
		while (day <= 0 )
			day += 31;

		while (hour >= 24)
			hour -= 24;
		while (hour < 0)
			hour += 24;

		while (minute >= 60)
			minute -= 60;
		while (minute < 0)
			minute += 60;

		while (second >= 60)
			second -= 60;
		while (second < 0)
			second += 60;
	}
};

double julian_day_now();
double julian_day(int r, int m, int d, double h);
double julian_day(const CalendarDate&);
double date_to_julian_day(const char *date, const char *format);
CalendarDate jd_to_date(double);


}

#endif
