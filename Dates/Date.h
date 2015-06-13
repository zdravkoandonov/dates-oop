#pragma once

#include <iostream>
#include <vector>

using std::vector;
using std::ostream;
using std::istream;

class Date
{
public:
	enum workdaySearchType { none, closest, forward, backward };

private:
	int day;
	int month;
	int year;
	
	int printFormatCode;

	bool workdaysOnly;
	workdaySearchType workdaySearch;

	static bool areHolidaysInitialized;
	static vector<Date> holidays;
	static vector<Date> workOffDays;
	static void initializeHolidays();

public:
	Date(int day, int month, int year);
	Date(char q, int dayCode, int month, int year);
	~Date();

	int getDay() const;
	int getMonth() const;
	int getYear() const;
	int getWeekday() const;

	bool isWorkday() const;
	
	Date getClosestWorkday();
	Date getClosestWorkday(workdaySearchType);

	void fixDateWorkday();

	bool getWorkdaysOnlyState();

	void setDay(int day);
	void setMonth(int month);
	void setYear(int year);

	void setPrintFormatCode(int code);

	void toggleWorkdaysOnly();
	void setWorkdaySearch(workdaySearchType);

	void addHoliday(Date &holiday, Date &workOff);

	Date& operator+(int days);
	Date& operator+=(int days);
	Date& operator++();
	Date operator++(int);

	Date& operator-(int days);
	Date& operator-=(int days);
	Date& operator--();
	Date operator--(int);
	
	Date& operator*(int months);
	Date& operator*=(int months);
	
	Date& operator/(int months);
	Date& operator/=(int months);
	
	Date& operator&(int years);
	Date& operator&=(int years);
	
	Date& operator|(int years);
	Date& operator|=(int years);
	
	Date& operator^(int weeks);
	Date& operator^=(int weeks);
	
	Date& operator%(int weeks);
	Date& operator%=(int weeks);
	
	int operator-(Date &date2);
	int operator/(Date &date2);
	int operator|(Date &date2);
	int operator%(Date &date2);

private:
	void printDate(ostream& out) const;
	static int daysInMonthCount(int month, int year);
	static int daysInMonthCount(Date date);
	static int daysInYearCount(int year);
	static int daysUpToNowThisYear(Date date);
	static bool isLeapYear(int year);
	static int pastLeapYearsCount(int year);
	static int dayOfWeek(Date date);
	static const char* getWeekdayName(int dayCode);
	static const char* getMonthName(int month);
	static int compareDates(Date date1, Date date2);
	static int calcDurationDays(Date date1, Date date2);
	static void swapDates(Date& date1, Date& date2);
	static bool isDateCorrect(int day, int month, int year);
	static Date calcEaster(int year);

	static bool dateExists(vector<Date>&, Date);
	static int datesInIntervalCount(vector<Date>&, Date, Date);

public:
	friend ostream& operator<<(ostream& out, const Date& date);
	friend istream& operator>>(istream& in, Date& date);
};

ostream& operator<<(ostream& out, const Date& date);
istream& operator>>(istream& in, Date& date);