#include "svpch.h"
#include "Time.h"

namespace Time {




double julian_day_now()
{
	//	wzięte z sterownia_unc09.h
	/// current UT -> JD

    int r,m,d;
	double aa, yy, mm, h;
    time_t rawtime;
    struct tm * ptm;

    time(&rawtime);
    ptm=gmtime(&rawtime);
    r=1900+(ptm->tm_year);
    m=1+(ptm->tm_mon);
    d=(ptm->tm_mday);
    h=1.0*(ptm->tm_hour) + (ptm->tm_min)/60.0 + (ptm->tm_sec)/3600.0;

	aa=floor((14.0-m)/12.0);
	yy=r+4800.0-aa;
	mm=m+12.0*aa-3.0;
	return (d + floor(0.2*(153.0*mm+2.0)) + 365.0*yy + floor(0.25*yy)
					- floor(0.01*yy) + floor(0.0025*yy) - 32045.5 + h/24.0);
}

double julian_day(int r, int m, int d, double h)
{
	double aa, yy, mm;
	aa=floor((14.0-m)/12.0);
	yy=r+4800.0-aa;
	mm=m+12.0*aa-3.0;
	return (d + floor(0.2*(153.0*mm+2.0)) + 365.0*yy + floor(0.25*yy)
					- floor(0.01*yy) + floor(0.0025*yy) - 32045.5 + h/24.0);
}

double julian_day(const CalendarDate &date)
{

	double aa, yy, mm;
	aa=floor((14.0 - date.month)/12.0);
	yy=date.year + 4800.0 - aa;
	mm=date.month + 12.0 * aa - 3.0;
	double h = date.hour + date.minute/60. + date.second/3600.;
	return (date.day + floor(0.2*(153.0*mm+2.0)) + 365.0*yy + floor(0.25*yy)
					- floor(0.01*yy) + floor(0.0025*yy) - 32045.5 + h/24.0);
}


double date_to_julian_day(const char *date, const char *format)
{
	/// format needs 6 fields: Y M D h m s

	int year, month, day, hour, minute;
	double sec;

	sscanf(date, format, &year, &month, &day, &hour, &minute, &sec);

	return julian_day(year, month, day, hour + minute/60. + sec/3600.);
}


CalendarDate jd_to_date(double input_JD)
{
	double JD, MJD, year, day, hour, minute, second, F, A, a, Z, B, C, D, E;
	int month;

	if (input_JD<2378496.5)
		JD = input_JD + 2400000.5;
	else
		JD = input_JD;


	Z = (int) (JD + 0.5);
	F = (JD + 0.5) - Z;

	if ( Z < 2299161)
		A = Z;
	else {
		a = (int) ( (Z-1867216.25)/36524.25 );
		A = Z + 1 + a - (int) (a/4.0);
	}

	B = A + 1524;
	C = (int)( (B-122.1)/365.25 );
	D = (int)( 365.25 * C);
	E = (int)( (B-D)/30.6001 );



	day = B - D  - (int) ( 30.6001*E) + F;

	if (E < 13.5)
		month = E-1;
	else
		month = E-13;


	if (month > 2.5)
		year=C-4716;
	else
		year=C-4715;

	hour = (int) ( (day - (int) day)*24.0 );
	minute = (int) (((day - (int) day)*24.0 - hour)*60.0);
	second = ((((day - (int) day)*24.0 - hour)*60.0) - minute)*60.0;

	day = (int) day;

// 	char buf[1000];
// 	sprintf(buf, "%04d-%02d-%02d %02d:%02d:%05.2lf",
// 			(int) year, month, (int) day, (int) hour, (int) minute, second);
// 	string s = buf;
	CalendarDate date((int)year, (int)month, (int)day, (int)hour, (int)minute, second);

	return date;
}



}
