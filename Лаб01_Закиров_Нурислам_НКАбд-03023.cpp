#include <iostream>
#include <cmath>

float minusp(float a, float b);
float plusp(float a, float b);
float multiply(float a, float b);
float divide(float a, float b);
float degree(float a, float b);
float take_second();
using namespace std;

int main() {

	setlocale(LC_ALL, "Russian");
	float result;
	float first;
	float second;
	char operation;
	cout << "Калькулятор" << endl;
	cout << "Операция(| + | - | =(конец операций) | ^ | / | * | #(удалить) |) " << endl;
	cout << "Приступайте!\nНапример: 1 + 2" << endl;
	cin >> first;
	cin >> operation;
	while (operation != '=') {
		switch (operation) {
		case '#':
			cout << "История очищена \n 0" << endl;
			cout << "Приступайте!\nНапример: 1 + 2" << endl;
			cin >> first;
			break;
		case '+':
			second = take_second();
			result = plusp(first, second);
				cout << first << " " << operation << " " << second << " = " << result << endl;
			first = result;
			cout << first << " ";
			break;
		case '-':
			second = take_second();
			result = minusp(first, second);
				cout << first << " " << operation << " " << second << " = " << result << endl;
			first = result;
			cout << first << " ";
			break;
		case '*':
			second = take_second();
			result = multiply(first, second);
				cout << first << " " << operation << " " << second << " = "  << result << endl;
			first = result;
			cout << first << " ";
			break;
		case '/':
			second = take_second();
			if (second == 0) {
				cout << "У вас ошибка! На ноль делить нельзя!!!! Перезапустите программу))";
				return 0;
			}
			result = divide(first, second);
				cout << first << " " << operation << " " << second << " = " << result << endl;
			first = result;
			cout << first << " ";
			break;
		case '^':
			second = take_second();
			result = degree(first, second);
				cout << first << " в степени " << second << " = " << result << endl;
			first = result;
			cout << first << " ";
			break;
		default:
			cout << "У вас ошибка! Перезапустите программу))";
			return 0;
		}
		cin >> operation;

	}
	cout << "Ваш конечный результат: " << first << endl;
	cout << "Благодарим вас за использование нашего калькулятора!";

}
float plusp(float a, float b) {
	return (a + b);
}
float minusp(float a, float b) {
	return (a - b);
}
float multiply(float a, float b) {
	return (a * b);
}
float divide(float a, float b) {
	return (a / b);
}
float degree(float a, float b) {
	return (pow(a, b));
}
float take_second() {
	float second;
	cin >> second;
	if (cin.fail()) {
		cout << "У вас ошибка! Перезапустите программу))";
		exit(EXIT_SUCCESS);
	}
	return second;
}