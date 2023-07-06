#include "Headers/Order.h"

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

int main() {
	Order order1(123, "Buy", 100.0, 10.0);
	order1.print_order();

	cout << "Press any key to exit";
	int key;
	cin >> key;

	return 0;
	
}