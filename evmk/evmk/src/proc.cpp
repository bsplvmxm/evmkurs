#include <iostream>
#include <time.h>
#include "proc.h"

using namespace std;

enum
{
	DENIED, ALLOWED, ACCESSED // имена доступа
}; 

enum
{
	REQUEST, FREE // имена команд
}; 

int ACCESS = DENIED; //переменная доступа

Processor::Processor()
{
	this->Pn = 0;
	this->Kr = 0;
	this->M = 0;
	this->K = 0;
	this->N = 3; 
	lead_time = new int[N];
	memory_access = new int[N];
	data = new int* [N];

	for (int i = 0; i < N; i++)
	{
		memory_access[i] = ALLOWED;
		lead_time[i] = 0;
		data[i] = new int[K];
	}
}

Processor::Processor(int Pn, int Kr, int M, int K, int N)
{
	this->Pn = Pn;
	this->Kr = Kr;
	this->M = M;
	this->K = K;
	this->N = N;
	memory_access = new int[this->N];
	lead_time = new int[this->N];
	data = new int* [this->N];

	for (int i = 0; i < this->N; i++)
	{
		memory_access[i] = ALLOWED;
		lead_time[i] = 0;
		data[i] = new int[this->K];
	}
}

Processor::~Processor()
{
	for (int i = 0; i < N; i++)
	{
		delete[] data[i];
	}

	delete[] data;
	delete[] memory_access;
	delete[] lead_time;
}

void Processor::set_pn(int Pn)
{
	this->Pn = Pn;
}

void Processor::set_kr(int Kr)
{
	this->Kr = Kr;
}

void Processor::set_m(int M)
{
	this->M = M;
}

void Processor::set_k(int K)
{
	this->K = K;
}

void Processor::set_n(int N)
{
	this->N = N;
}

void Processor::data_to_memory()
{
	for (int id = 0; id < N; id++)
	{
		srand(time(NULL) - 100 * id);
		int raznK = K - K * Kr / 100; // кол-во команд, требующих обращение в память
		int raznK_block = raznK * Pn / 100; // кол-во команд, которые будут обращены в один кокнретный блок памяти
		int memory = 0;

		for (int i = 0; i < K; i++)
		{
			if (raznK && rand() % ((K - i) / raznK) == 0) // если raznK и рандом от 0 до (отношение декремента кол-ва команд на
			{												  // число команд, требующих обращение в память) = 0
				if (raznK - raznK_block != 0 && rand() % (raznK / (raznK - raznK_block)) == 0) // если разность raznK и raznK_block не равна нулю
				{																							   //и рандом от 0 до отношения raznK на разность raznK и
					while (true)																			   //raznK_block. С каждым шагом уменьшаем кол-во команд к 
					{																						   //конкретному блоку памяти
						memory = rand() % N + 1; // блок памяти от 1 до N

						if (memory != id + 1)
							break;
					}
				}
				else // если рандом не выпал, то идем к следующему блоку памяти, уменьшая кол-во команд до блока
				{
					memory = id + 1;
					raznK_block--;
				}

				data[id][i] = memory;
				raznK--;
			}
			else
			{
				data[id][i] = 0; // команда, не требующая обращения к блоку памяти
			}
		}
	}
}

void Processor::commutator(int flag)
{
	int access;
	int delay = 1;
	for (int i = 0; i < K; i++)
	{
		for (int id = 0; id < N; id++)
		{
			if (data[id][i])
			{
				if (flag == 1)
				{
					cout << "Data transfer request" << "\t" << data[id][i] << "\tProcessor " << id + 1 << endl;
				}
				while (true)
				{
					access = memory(REQUEST, data[id][i]);

					if (access == ACCESSED)
					{
						if (flag == 1)
						{
							cout << "Memory access" << "\t\t" << data[id][i] << "\tProcessor " << id + 1 << "\t" << endl;
						}
						lead_time[id] += M;
						break;
					}
					else
					{
						if (flag == 1)
						{
							cout << "Memory wait" << "\t\t" << data[id][i] << "\tProcessor " << id + 1 << "\t" << endl;
						}
						lead_time[id] += 1 * delay;
						delay++;
						memory(FREE, data[id][i]);
					}
				}
			}
			else
			{
				lead_time[id] += 1;
				if (flag == 1)
				{
					cout << "Without memory request" << "\t-" << "\tProcessor " << id + 1 << "\t" << endl;
				}
			}
		}

		for (int i = 0; i < N; i++)
		{
			memory_access[i] = ALLOWED;
		}
		delay = 1;
	}

	if (flag == 1)
	{
		cout << endl;
	}
	cout << "Worktime of commutator: " << worktime() << endl;
	if (flag == 1)
	{
		cout << endl;
	}
}

void Processor::bus(int flag)
{
	int access;
	int delay = 1;
	for (int i = 0; i < K; i++)
	{
		for (int id = 0; id < N; id++)
		{
			if (data[id][i])
			{
				if (flag == 1)
				{
					cout << "Data transfer request" << "\t" << data[id][i] << "\tProcessor " << id + 1 << "\t" << endl;
				}
				while (true)
				{
					access = memory(REQUEST);

					if (access == ACCESSED)
					{
						if (flag == 1)
						{
							cout << "Memory access" << "\t\t" << data[id][i] << "\tProcessor " << id + 1 << "\t" << endl;
						}
						lead_time[id] += M;
						break;
					}
					else
					{
						if (flag == 1)
						{
							cout << "Memory wait" << "\t\t" << data[id][i] << "\tProcessor " << id + 1 << "\t" << endl;
						}
						lead_time[id] += 1 * delay;
						delay++;
						memory(FREE);
					}
				}
			}
			else
			{
				lead_time[id] += 1;
				if (flag == 1)
				{
					cout << "Without memory request" << "\t-" << "\tProcessor " << id + 1 << "\t" << endl;
				}
			}
		}
		delay = 1;
	}
	if (flag == 1)
	{
		cout << endl;
	}
	cout << "Worktime of bus: " << worktime() << endl;
	if (flag == 1)
	{
		cout << endl;
	}
	//system("pause");
}

int Processor::memory(int req, int num) // функция доступа к памяти для коммутатора
{
	ACCESS = memory_access[num - 1]; // переменной аксес присваиваем команду из массива имен доступа
	if (ACCESS == ALLOWED && req == REQUEST)
	{
		ACCESS = DENIED;
		memory_access[num - 1] = DENIED;
		return ACCESSED;
	}
	else if (ACCESS == DENIED && req == REQUEST)
	{
		return DENIED;
	}
	else if (ACCESS == DENIED && req == FREE)
	{
		ACCESS = ALLOWED;
	}

	memory_access[num - 1] = ALLOWED;
	return ACCESS;
}

int Processor::memory(int req) // функция доступа к памяти для шины - не используется массив имен доступа
{
	if (ACCESS == ALLOWED && req == REQUEST)
	{
		ACCESS = DENIED;
		return ACCESSED;
	}
	else if (ACCESS == DENIED && req == FREE)
	{
		ACCESS = ALLOWED;
	}

	return ACCESS;
}

int Processor::worktime() // выбирает время самой медленной программы. Это и есть время работы коммуникационной сети
{				
	int max = 0;
	for (int i = 0; i < N; ++i)
	{
		if (lead_time[i] > max)
		{
			max = lead_time[i];
		}
	}

	for (int i = 0; i < N; i++)
	{
		lead_time[i] = 0;
	}
	return max;
}