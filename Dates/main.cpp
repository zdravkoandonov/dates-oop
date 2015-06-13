#include <iostream>
#include "Date.h"

using std::cin;
using std::cout;
using std::endl;

int main()
{
	Date d(10, 6, 2015);
	cout << d;
	d++;
	cout << d;
	d.setPrintFormatCode(3);
	cout << d;

	cin >> d;
	cout << d;
	
	Date p(31, 1, 2015);
	p &= 1; // add a year 
	p *= 1; // add a month
	cout << p;

	Date a('2', 6, 10, 2015); // second saturday of october
	cout << a;
	Date b('L', 1, 11, 2015); // last monday of november
	cout << b;
	b += 12;
	cout << b;
	b.toggleWorkdaysOnly(); // enable
	b += 9;
	cout << b;
	b.toggleWorkdaysOnly(); // disable
	b.setWorkdaySearch(Date::forward);
	b /= 1; // subtract months
	cout << b;

	Date d1(12, 6, 2015), d2(12, 6, 2016);
	cout << d2 - d1 << endl;
	cout << d1 - d2 << endl;
	cout << d2 / d1 << endl;
	cout << d1 / d2 << endl;
	cout << (d2 | d1) << endl;
	cout << d2 % d1 << endl;

	cout << Date::calcEaster(2014);
	cout << Date::calcEaster(2015);
	cout << Date::calcEaster(2016);

	Date q(30, 4, 2015);
	Date::addHoliday(Date(4, 5, 2015), Date(9, 5, 2015));
	q.toggleWorkdaysOnly(); // enable
	q += 5;
	cout << q;

	return 0;
}