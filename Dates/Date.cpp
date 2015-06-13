#include "Date.h"
#include "Utility.h"

bool Date::areHolidaysInitialized = false;

vector<Date> Date::holidays;
vector<Date> Date::workOffDays;

void Date::initializeHolidays()
{
	areHolidaysInitialized = true;

	holidays.push_back(Date(1, 1, 2015));
	holidays.push_back(Date(2, 3, 2015));
	holidays.push_back(Date(3, 3, 2015));
	holidays.push_back(Date(10, 4, 2015));
	holidays.push_back(Date(11, 4, 2015));
	holidays.push_back(Date(12, 4, 2015));
	holidays.push_back(Date(13, 4, 2015));
	holidays.push_back(Date(1, 5, 2015));
	holidays.push_back(Date(6, 5, 2015));
	holidays.push_back(Date(21, 9, 2015));
	holidays.push_back(Date(22, 9, 2015));
	holidays.push_back(Date(24, 12, 2015));
	holidays.push_back(Date(25, 12, 2015));
	holidays.push_back(Date(26, 12, 2015));
	holidays.push_back(Date(31, 12, 2015));
	holidays.push_back(Date(1, 1, 2016));
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

	if (q != 'l')
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

Date::~Date()
{
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
	return !dateExists(holidays, *this) && ((dayCode != 6 && dayCode != 0) || dateExists(workOffDays, *this));
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
	int a, b, c, k, p, q, m, n, d, e;
	a = year % 19;
	b = year % 4;
	c = year % 7;
	k = year / 100;
	p = (13 + 8*k) / 25;
	q = k / 4;
	m = (15 - p + k - q) % 30;
	n = (4 + k - q) % 7;
	d = (19*a + m) % 30;
	e = (2*b + 4*c + 6*d + n) % 7;

	int day, month;
	int dayOfMarch = 22 + d + e;
	if (dayOfMarch > 31)
	{
		if (d == 29 && e == 6)
		{
			day = 19;
		}
		else if (d == 28 && e == 6 && (11*m+11) % 30 < 19)
		{
			day = 18;
		}
		else
		{
			day = dayOfMarch - 31;
		}
		month = 4;
	}
	else
	{
		day = dayOfMarch;
		month = 3;
	}

	return Date(day, month, year);
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

int Date::datesInIntervalCount(vector<Date> &dates, Date date1, Date date2)
{
	int cnt = dates.size();
	if (cnt == 0)
	{
		return 0;
	}

	int leftLimit, rightLimit;
	int l = -1, r = cnt - 1, mid;
	while (r - l > 1)
	{
		mid = (l + r) / 2;
		if (compareDates(date1, dates[mid]) == 0)
		{
			r = mid;
			break;
		}
		else if (compareDates(date1, dates[mid]) < 0)
		{
			r = mid;
		}
		else
		{
			l = mid;
		}
	}
	leftLimit = r;

	l = 0;
	r = cnt;
	while (r - l > 1)
	{
		mid = (l + r) / 2;
		if (compareDates(date2, dates[mid]) == 0)
		{
			l = mid;
			break;
		}
		else if (compareDates(date2, dates[mid]) < 0)
		{
			r = mid;
		}
		else
		{
			l = mid + 1;
		}
	}
	rightLimit = l;

	return rightLimit - leftLimit + 1;
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
		date = Date(day, month, year);
	}
	return in;
}
