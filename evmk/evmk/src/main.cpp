#include <iostream>
#include "proc.h"
#include "helper.h"

using namespace std;

void Menu::menu()
{
	processor_settings();
	system("pause");
}
void Menu::processor_settings()
{
	int Pn, Kr, M, K, N, flag = 1;
	cout << "������� ���-�� ������(N): ";
	cin >> N;
	cout << "������� % ������, ������������ � ����� ����� ������(Pn): ";
	cin >> Pn;
	cout << "������� % ������, �� ��������� ��������� � ������(Kr): ";
	cin >> Kr;
	cout << "������� ���-�� �������(M): ";
	cin >> M;
	cout << "������� ���-�� ������(K): ";
	cin >> K;
	Processor proc(Pn, Kr, M, K, N);
	proc.data_to_memory();
	if (flag == 1)
	{
		cout << "\n" << "Command name" << "\t\t" << "Command to" << "\t" <<
			"Processor" << "\n" << endl;
	}
	proc.commutator(flag);
	proc.bus(flag);
}

int main()
{
	setlocale(LC_ALL, "Russian");
	Menu start;
	start.menu();
		return 0;
}