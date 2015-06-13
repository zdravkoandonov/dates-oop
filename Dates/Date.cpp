#include "Date.h"
#include "Utility.h"
#include <algorithm>

using std::sort;

bool Date::areHolidaysInitialized = false;

vector<Date> Date::officialHolidays;
vector<Date> Date::holidays;
vector<Date> Date::workOffDays;

void Date::initializeHolidays()
{
	areHolidaysInitialized = true;

	// if adding new holidays here keep them in ascending order
	officialHolidays.push_back(Date(1, 1, 2015));
	officialHolidays.push_back(Date(3, 3, 2015));
	officialHolidays.push_back(Date(1, 5, 2015));
	officialHolidays.push_back(Date(6, 5, 2015));
	officialHolidays.push_back(Date(24, 5, 2015));
	officialHolidays.push_back(Date(6, 9, 2015));
	officialHolidays.push_back(Date(22, 9, 2015));
	officialHolidays.push_back(Date(24, 12, 2015));
	officialHolidays.push_back(Date(25, 12, 2015));
	officialHolidays.push_back(Date(26, 12, 2015));
	officialHolidays.push_back(Date(31, 12, 2015));
}

Date::Date(int day, int month, int year)
{
	printFormatCode = 0;
	workdaysOnly = false;
	workdaySearch = none;

	if (!areHolidaysInitialized)
	{
		initializeHolidays();
	}

	if (isDateCorrect(day, month, year))
	{
		this->day = day;
		this->month = month;
		this->year = year;
	}
	else
	{
		this->day = 0;
		this->month = 0;
		this->year = 0;
	}
}

Date::Date(char q, int dayCode, int month, int year)
	: month(month), year(year)
{
	printFormatCode = 0;
	workdaysOnly = false;
	workdaySearch = none;

	this->day = 1;
	int firstDayOfWeek = dayOfWeek(*this);
	int firstSeekedDay = (7 - firstDayOfWeek + 1 + dayCode) % 7;
	if (firstSeekedDay == 0)
	{
		firstSeekedDay = 7;
	}

	if (q != 'L')
	{
		int seekedDay = firstSeekedDay + 7 * (Utility::charToInt(q) - 1);
		if (seekedDay <= daysInMonthCount(*this)) // if such day exists in the month
		{
			this->day = seekedDay;
		}
	}
	else
	{
		this->day = firstSeekedDay + 7 * ((daysInMonthCount(*this) - firstSeekedDay) / 7);
	}
}

int Date::getDay() const
{
	return day;
}

int Date::getMonth() const
{
	return month;
}

int Date::getYear() const
{
	return year;
}

int Date::getWeekday() const
{
	return dayOfWeek(*this);
}

bool Date::isWorkday() const
{
	int dayCode = getWeekday();
	return ((dayCode != 6 && dayCode != 0) || dateExists(workOffDays, *this)) 
		&& !dateExists(holidays, *this) 
		&& !isOfficialHoliday(*this) 
		&& !(*this == (calcEaster(year) - 2)); // the friday before easter is a holiday
}

Date Date::getClosestWorkday()
{
	Date forw = getClosestWorkday(forward);
	Date backw = getClosestWorkday(backward);
	int daysForw = forw - (*this);
	int daysBackw = backw - (*this);

	return (daysForw > daysBackw) ? backw : forw;
}

Date Date::getClosestWorkday(workdaySearchType d)
{
	Date res(*this);
	if (d == backward)
	{
		while (!res.isWorkday())
			--res;
		return res;
	}
	else // d == forward
	{
		while (!res.isWorkday())
			++res;
		return res;
	}

}

void Date::fixDateWorkday()
{
	if (workdaySearch != none)
		if (workdaySearch == closest)
			(*this) = getClosestWorkday();
		else
			(*this) = getClosestWorkday(workdaySearch);
}

bool Date::getWorkdaysOnlyState()
{
	return workdaysOnly;
}

void Date::setDay(int day)
{
	if (isDateCorrect(day, this->month, this->year))
	{
		this->day = day;
	}
}

void Date::setMonth(int month)
{
	if (isDateCorrect(this->day, month, this->year))
	{
		this->month = month;
	}
}

void Date::setYear(int year)
{
	if (isDateCorrect(this->day, this->month, year))
	{
		this->year = year;
	}
}

void Date::setPrintFormatCode(int code)
{
	printFormatCode = code;
}

void Date::toggleWorkdaysOnly()
{
	workdaysOnly = !workdaysOnly;
}

void Date::setWorkdaySearch(workdaySearchType t)
{
	workdaySearch = t;
}

void Date::addHoliday(Date &holiday, Date &workOff)
{
	holidays.push_back(holiday);
	workOffDays.push_back(workOff);

	sort(holidays.begin(), holidays.begin() + holidays.size(), isFirstDateEarlier);

	sort(workOffDays.begin(), workOffDays.begin() + workOffDays.size(), isFirstDateEarlier);
}

void Date::printDate(ostream& out) const
{
	switch (printFormatCode)
	{
		case 1:
			out << getMonthName(month) << " " << day << ", " << year << "\n";
			break;
		case 2:
			out << day << "/" << month << "/" << year << "\n";
			break;
		case 3:
			out << day << "." << month << "." << year << "\n";
			break;
		case 4:
			if (day < 10)
			{
				out << '0';
			}
			out << day << "/";
			if (month < 10)
			{
				out << '0';
			}
			out << month << "/" << year % 100 << "\n";
			break;
		case 5:
			if (day < 10)
			{
				out << '0';
			}
			out << day << "-";
			if (month < 10)
			{
				out << '0';
			}
			out << month << "-" << year % 100 << "\n";
			break;
		case 6:
			out << month << "/" << day << "/" << year << "\n";
			break;
		case 7:
			out << month << "." << day << "." << year << "\n";
			break;
		case 8:
			if (month < 10)
			{
				out << '0';
			}
			out << month << "/";
			if (day < 10)
			{
				out << '0';
			}
			out << day << "/" << year % 100 << "\n";
			break;
		case 9:
			if (month < 10)
			{
				out << '0';
			}
			out << month << "-";
			if (day < 10)
			{
				out << '0';
			}
			out << day << "-" << year % 100 << "\n";
			break;

		default:
			out << day << " " << getMonthName(month) << " " << year << "\n";
			break;
	}
}

int Date::daysInMonthCount(int month, int year)
{
	switch (month)
	{
		case 2:
			if (isLeapYear(year))
			{
				// leap year
				return 29;
			}
			else
			{
				return 28;
			}
			break;

		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
			break;

		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
			break;

		// if the month is invalid return 0
		default:
			return 0;
	}
}

int Date::daysInMonthCount(Date date)
{
	return daysInMonthCount(date.month, date.year);
}

int Date::daysInYearCount(int year)
{
	return 365 + isLeapYear(year);
}

int Date::daysUpToNowThisYear(Date date)
{
	int m = 1, daysCnt = 0;
	while (m < date.month)
	{
		daysCnt += daysInMonthCount(m, date.year);
		m++;
	}
	daysCnt += date.day;
	return daysCnt;
}

bool Date::isLeapYear(int year)
{
	return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

int Date::pastLeapYearsCount(int year)
{
	return year / 4 - year / 100 + year / 400;
}

int Date::dayOfWeek(Date date)
{
	Date baseDate(1, 1, 2000);
	int base = 6, res;

	if (compareDates(baseDate, date) < 0)
	{
		res = (base + calcDurationDays(baseDate, date)) % 7;
	}
	else
	{
		res = (base - calcDurationDays(baseDate, date)) % 7;
		if (res < 0)
		{
			res += 7;
		}
	}

	// Sunday to Saturday -> 0 to 6
	return res;
}

// returns <0 if date1 is before date2
// 			0 if date1 is the same as date2
// 		   >0 if date1 is after date2
int Date::compareDates(Date date1, Date date2)
{
	if (date1.year == date2.year)
	{
		if (date1.month == date2.month)
		{
			if (date1.day == date2.day)
			{
				return 0;
			}
			else
			{
				return date1.day - date2.day;
			}
		}
		else
		{
			return date1.month - date2.month;
		}
	}
	else
	{
		return date1.year - date2.year;
	}
}

// returns <0 if date1 is before date2
// 			0 if date1 is the same as date2
// 		   >0 if date1 is after date2
int Date::compareDatesNoYear(Date date1, Date date2)
{
	if (date1.month == date2.month)
	{
		if (date1.day == date2.day)
		{
			return 0;
		}
		else
		{
			return date1.day - date2.day;
		}
	}
	else
	{
		return date1.month - date2.month;
	}
}

bool Date::isFirstDateEarlier(Date date1, Date date2)
{
	return compareDates(date1, date2) < 0;
}

const char* Date::getWeekdayName(int dayCode)
{
	switch (dayCode)
	{
		case 0:
			return "Sunday";
		case 1:
			return "Monday";
		case 2:
			return "Tuesday";
		case 3:
			return "Wednesday";
		case 4:
			return "Thursday";
		case 5:
			return "Friday";
		case 6:
			return "Saturday";
		default:
			return "No such day of the week";
	}
}

const char* Date::getMonthName(int month)
{
	switch (month)
	{
		case 1:
			return "January";
			break;
		case 2:
			return "February";
			break;
		case 3:
			return "March";
			break;
		case 4:
			return "April";
			break;
		case 5:
			return "May";
			break;
		case 6:
			return "June";
			break;
		case 7:
			return "July";
			break;
		case 8:
			return "August";
			break;
		case 9:
			return "September";
			break;
		case 10:
			return "October";
			break;
		case 11:
			return "November";
			break;
		case 12:
			return "December";
			break;

		default:
			return "No such month";
			break;
	}
}

int Date::calcDurationDays(Date date1, Date date2)
{
	// if date2 is before date1
	if (compareDates(date1, date2) > 0)
	{
		swapDates(date1, date2);
	}

	int days = 0;

	// years
	days += (365 * (date2.year - 1) + pastLeapYearsCount(date2.year - 1));
	days += daysUpToNowThisYear(date2);
	days -= (365 * (date1.year - 1) + pastLeapYearsCount(date1.year - 1));
	days -= daysUpToNowThisYear(date1);

	return days;
}

void Date::swapDates(Date& date1, Date& date2)
{
	Date temp = date1;
	date1 = date2;
	date2 = temp;
}

bool Date::isDateCorrect(int day, int month, int year)
{
	if (day < 1 || month < 1 || month > 12)
		return false;

	return day <= daysInMonthCount(month, year);
}

Date Date::calcEaster(int year)
{
	int a, b, c, d, e;
	a = year % 4;
	b = year % 7;
	c = year % 19;
	d = (19 * c + 16) % 30;
	e = (2 * a + 4 * b + 6 * d) % 7;

	Date easter = Date(21, 3, year) + (d + e);

	// convert to new style
	if (year >= 1583 && year < 1700)
		easter += 10;
	else if (year >= 1700 && year < 1800)
		easter += 11;
	else if (year >= 1800 && year < 1900)
		easter += 12;
	else if (year >= 1900 && year < 2100)
		easter += 13;
	else if (year >= 2100)
		easter += 14;

	return easter;
}

bool Date::operator==(const Date& other) const
{
	return (day == other.day)
		&& (month == other.month)
		&& (year == other.year);
}

Date& Date::operator+(int days)
{
	return (*this) += days;
}

Date& Date::operator+=(int days)
{
	if (workdaysOnly)
	{
		while (days > 0)
		{
			++(*this);
			if (isWorkday())
				days--;
		}
	}
	else
	{
		if (days <= 0)
		{
			return *this;
		}

		if (days >= daysInYearCount(year) - daysUpToNowThisYear(*this) + 1)
		{
			days -= (daysInYearCount(year) - daysUpToNowThisYear(*this) + 1);
			day = 1;
			month = 1;
			year++;
			while (days >= daysInYearCount(year))
			{
				days -= daysInYearCount(year);
				day = 1;
				month = 1;
				year++;
			}
		}
		else if (days >= daysInMonthCount(*this) - day + 1)
		{
			days -= (daysInMonthCount(*this) - day + 1);
			day = 1;
			month++;
			if (month > 12)
			{
				year = month / 12;
				month %= 12;
			}
		}
		while (days >= daysInMonthCount(*this))
		{
			days -= daysInMonthCount(*this);
			day = 1;
			month++;
			if (month > 12)
			{
				year += month / 12;
				month %= 12;
			}
		}
		day += days;
	}

	fixDateWorkday();

	return *this;
}

Date& Date::operator++()
{
	day++;
	if (day > daysInMonthCount(*this))
	{
		day = 1;
		month++;
		if (month == 13)
		{
			month = 1;
			year++;
		}
	}

	fixDateWorkday();

	return *this;
}

Date Date::operator++(int)
{
	Date prev = (*this);
	++(*this);
	return prev;
}

Date& Date::operator-(int days)
{
	return (*this) -= days;
}

Date& Date::operator-=(int days)
{
	if (workdaysOnly)
	{
		while (days > 0)
		{
			--(*this);
			if (isWorkday())
				days--;
		}
	}
	else
	{
		if (days <= 0)
		{
			return *this;
		}

		if (days >= daysUpToNowThisYear(*this))
		{
			days -= daysUpToNowThisYear(*this);
			day = 31;
			month = 12;
			year--;
			while (days >= daysInYearCount(year))
			{
				days -= daysInYearCount(year);
				day = 31;
				month = 12;
				year--;
			}
		}
		else if (days >= day)
		{
			days -= day;
			month--;
			if (month == 0)
			{
				year--;
				month = 12;
			}
			day = daysInMonthCount(*this);
		}
		while (days >= daysInMonthCount(*this))
		{
			days -= daysInMonthCount(*this);
			month--;
			if (month == 0)
			{
				year--;
				month = 12;
			}
			day = daysInMonthCount(*this);
		}
		day -= days;
	}

	fixDateWorkday();

	return *this;
}

Date& Date::operator--()
{
	day--;
	if (day == 0)
	{
		month--;
		if (month == 0)
		{
			month = 12;
			year--;
		}
		day = daysInMonthCount(*this);
	}

	fixDateWorkday();

	return *this;
}

Date Date::operator--(int)
{
	Date prev = (*this);
	--(*this);
	return prev;
}

Date& Date::operator*(int months)
{
	return (*this) *= months;
}

Date& Date::operator*=(int months)
{
	year += months / 12;
	months %= 12;
	month += months;
	if (month > 12)
	{
		year++;
		month -= 12;
	}
	int daysInMonth = daysInMonthCount(month, year);
	if (day > daysInMonth)
	{
		day = daysInMonth;
	}

	fixDateWorkday();

	return *this;
}
	
Date& Date::operator/(int months)
{
	return (*this) /= months;
}

Date& Date::operator/=(int months)
{
	year -= months / 12;
	months %= 12;
	month -= months;
	if (month < 1)
	{
		year--;
		month += 12;
	}
	int daysInMonth = daysInMonthCount(month, year);
	if (day > daysInMonth)
	{
		day = daysInMonth;
	}

	fixDateWorkday();

	return *this;
}
	
Date& Date::operator&(int years)
{
	return (*this) &= years;
}

Date& Date::operator&=(int years)
{
	year += years;
	int daysInMonth = daysInMonthCount(month, year);
	if (day > daysInMonth)
	{
		day = daysInMonth;
	}

	fixDateWorkday();

	return *this;
}
	
Date& Date::operator|(int years)
{
	return (*this) |= years;
}

Date& Date::operator|=(int years)
{
	year -= years;
	int daysInMonth = daysInMonthCount(month, year);
	if (day > daysInMonth)
	{
		day = daysInMonth;
	}

	fixDateWorkday();

	return *this;
}
	
Date& Date::operator^(int weeks)
{
	return (*this) ^= weeks;
}

Date& Date::operator^=(int weeks)
{
	(*this) += (weeks * 7);
	return *this;
}
	
Date& Date::operator%(int weeks)
{
	return (*this) %= weeks;
}

Date& Date::operator%=(int weeks)
{
	(*this) -= (weeks * 7);
	return *this;
}

int Date::operator-(Date &date2)
{
	int days = 0;

	// if date2 is before date1
	if (compareDates(*this, date2) > 0)
	{
		// years
		days += (365 * (year - 1) + pastLeapYearsCount(year - 1));
		days += daysUpToNowThisYear(*this);
		days -= (365 * (date2.year - 1) + pastLeapYearsCount(date2.year - 1));
		days -= daysUpToNowThisYear(date2);
	}
	else
	{
		// years
		days += (365 * (date2.year - 1) + pastLeapYearsCount(date2.year - 1));
		days += daysUpToNowThisYear(date2);
		days -= (365 * (year - 1) + pastLeapYearsCount(year - 1));
		days -= daysUpToNowThisYear(*this);
	}

	return days;
}

int Date::operator/(Date &date2)
{
	// calculates full months
	
	if (compareDates(*this, date2) > 0)
	{
		int months = (year - date2.year) * 12;
		if (month < date2.month)
		{
			months += month - date2.month;
		}
		else
		{
			months += month - date2.month;
		}

		if (day < date2.day)
		{
			months--;
		}
		return months;
	}
	else
	{
		int months = (date2.year - year) * 12;
		if (date2.month < month)
		{
			months += date2.month - month;
		}
		else
		{
			months += date2.month - month;
		}

		if (date2.day < day)
		{
			months--;
		}
		return months;
	}
}

int Date::operator|(Date &date2)
{
	// returns full years
	return ((*this) / date2) / 12;
}

int Date::operator%(Date &date2)
{
	// returns full weeks
	return ((*this) - date2) / 7;
}

bool Date::dateExists(vector<Date> &dates, Date date)
{
	int cnt = dates.size();
	int l = 0, r = cnt - 1, mid;
	while (l <= r)
	{
		mid = (l + r) / 2;
		if (compareDates(date, dates[mid]) == 0)
		{
			return true;
		}
		else if (compareDates(date, dates[mid]) < 0)
		{
			r = mid - 1;
		}
		else
		{
			l = mid + 1;
		}
	}

	return false;
}

bool Date::isOfficialHoliday(const Date date)
{
	int cnt = officialHolidays.size();
	int l = 0, r = cnt - 1, mid;
	while (l <= r)
	{
		mid = (l + r) / 2;
		if (compareDatesNoYear(date, officialHolidays[mid]) == 0)
		{
			return true;
		}
		else if (compareDatesNoYear(date, officialHolidays[mid]) < 0)
		{
			r = mid - 1;
		}
		else
		{
			l = mid + 1;
		}
	}

	return false;
}

ostream& operator<<(ostream& out, const Date& date)
{
	date.printDate(out);		
	return out;
}

istream& operator>>(istream& in, Date& date)
{
	int day, month, year;
	in >> day >> month >> year;
	if (Date::isDateCorrect(day, month, year))
	{
		// removes print format settings
		date = Date(day, month, year);
	}
	return in;
}
