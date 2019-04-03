#include "pch.h"
#include <iostream>
#include <time.h>
#include <fstream>
#include <Windows.h>
#include <iomanip>

using namespace std;

clock_t start;
clock_t stop;
float czas;
float liczba[100];
float liczba2[100];
float srednia;
float suma = 0;
float min;
float max;
ofstream pomiary;
ofstream pomiary2;
ofstream pomiary3;

bool sprawdzPorzadek(int *tablica, long int rozmiar) // funkcja sprawdzajaca poprawnosc sortowania
{
	int sprawdz = 0;
	for (int i = 0; i < rozmiar - 1; i++)
	{
		if (tablica[i + 1] < tablica[i])
		{
			sprawdz++;
		}
	}

	if (sprawdz == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void sortowanieShella(int *tablica, long int rozmiar)
{
	for (int przerwa = rozmiar / 2; przerwa > 0; przerwa /= 2) //wybor wartosci przerwy
	{
		for (int i = przerwa; i < rozmiar; i++) //sortowanie przez wstawianie z uzyciem wybranej wartosci przerwy
		{
			int bufor = tablica[i]; //element ktory bedzie wstawiany
			int j;
			for (j = i; j >= przerwa && tablica[j - przerwa] > bufor; j = j - przerwa)
			{
				tablica[j] = tablica[j - przerwa]; //przesuwanie elementow
			}
			tablica[j] = bufor; //wstawianie bufora w odpowiednie miejsce
		}
	}
}

void quick_sort(int *tablica, int lewy, int prawy)
{
	if (prawy <= lewy) return;

	int i = lewy - 1, j = prawy + 1,
		pivot = tablica[(lewy + prawy) / 2]; //wybor punktu odniesienia

	while (1)
	{
		while (pivot > tablica[++i]); //wyszukiwanie elementu niemniejszego od piwotu stojacego po prawej stronie wartosci piwot

		while (pivot < tablica[--j]); //wyszukiwanie elementu niewiekszego od piwotu stojacego po lewej stronie wartosci piwot

		if (i <= j)
			swap(tablica[i], tablica[j]); //zamiana elementow
		else
			break;
	}

	if (j > lewy)
		quick_sort(tablica, lewy, j);
	if (i < prawy)
		quick_sort(tablica, i, prawy);
}

void scalanie(int poczatek, int srodek, int koniec, int *tablica, int *tab)
{
	int i, j, q;
	for (i = poczatek; i <= koniec; i++) 
		tab[i] = tablica[i];  //skopiowanie danych do tablicy pomocniczej
	i = poczatek;
	j = srodek + 1;
	q = poczatek;
	while (i <= srodek && j <= koniec)  
	{        
		if (tab[i] < tab[j])
			tablica[q++] = tab[i++]; //przenoszenie danych z sortowaniem z tablicy pomocniczej do tablicy głównej
		else
			tablica[q++] = tab[j++]; //przenoszenie danych z sortowaniem z tablicy pomocniczej do tablicy głównej
	}
	while (i <= srodek) tablica[q++] = tab[i++]; //przeniesienie nieskopiowanych danych ze zbioru pierwszego
}

void sortowanieScalanie(int poczatek, int koniec, int *tablica, int *tab)
{
	int srodek;
	if (poczatek < koniec) {
		srodek = (poczatek + koniec) / 2;
		sortowanieScalanie(poczatek, srodek, tablica, tab); //podzial lewej części
		sortowanieScalanie(srodek + 1, koniec, tablica, tab); //podzial prawej części
		scalanie(poczatek, srodek, koniec, tablica, tab); //scalanie lewej i prawej czesci
	}
}


void sortowanieOdwrotne(int *tablica, long int rozmiar) //losowanie elementow tablicy
{
	for (int i = 0; i < rozmiar; i++)
	{
		tablica[i] = rozmiar - i;
	}
}

void sortowanieProcentowe(int *tablica, long int rozmiar, float procent) //uporzadkowanie czesci elementow w tablicy
{
	float p = (procent / 100) * rozmiar;
	for (int i = 0; i < rozmiar; i++)
	{
		if (i < p)
		{
			tablica[i] = i;
		}
		else
		{
			tablica[i] = (rand() % rozmiar) + p;
		}
	}
}

int main()
{
	srand(time(NULL));
	int liczba_tablic = 100;
	unsigned int rozmiarTablic[5] = { 10000, 50000, 100000, 500000, 1000000 };
	float losowoscTablicy[8] = { 0, 0.25, 0.5, 0.75, 0.95, 0.99, 0.997, -1 };
	int zakres = 1000000;
	float procent = 0;
	
	float min = czas;
	float max = czas;
	float aktualnyCalkowityCzas = 0;
	int* tablica = new int[1000000];
	int* tab = new int[1000000];

	pomiary.open("Pomiary.txt", ios::out);
	pomiary2.open("PomiaryMin.txt", ios::out);
	pomiary3.open("PomiaryMax.txt", ios::out);
	for (int metodaSortowania = 0; metodaSortowania < 3; metodaSortowania++) {
		for (int i = 0; i < sizeof(rozmiarTablic) / sizeof(rozmiarTablic[0]); i++) {
			for (int j = 0; j < sizeof(losowoscTablicy) / sizeof(losowoscTablicy[0]); j++) {
				aktualnyCalkowityCzas = 0;
				min = 9999;
				max = 0;
				for (int k = 0; k < liczba_tablic; k++) {

					if (losowoscTablicy[j] == -1)
						sortowanieOdwrotne(tablica, rozmiarTablic[i]);
					else
						sortowanieProcentowe(tablica, rozmiarTablic[i], losowoscTablicy[j]);

					/*cout << "Tablica przed sortowaniem: " << endl;

					for (int i = 0; i < rozmiar; i++)
					{
						cout << tablica[i] << " ";
					}*/

					start = clock(); //rozpoczecie odmierzania czasu
					switch (metodaSortowania)
					{
					case 0:
						sortowanieShella(tablica, rozmiarTablic[i]);
						break;
					case 1:
						quick_sort(tablica, 0, rozmiarTablic[i] - 1);
						break;
					case 2:
						sortowanieScalanie(0, rozmiarTablic[i] - 1, tablica, tab);
						break;
					}
					stop = clock(); //koniec pomiaru czasu


					if (sprawdzPorzadek(tablica, rozmiarTablic[i]) == false) //wywolanie funkcji sprawdzajacej poprawnosc sortowania
					{
						cout << "Sortowanie niepoprawne" << endl;
					}

					/*cout << endl << "Tablica po sortowaniu: " << endl;

					for (int i = 0; i < rozmiar; i++)
					{
						cout << tablica[i] << " ";
					}*/

					czas = (float)(stop - start) / CLOCKS_PER_SEC;

					if (min > czas)
						min = czas; //minimalna wartosci czasu
					if (max < czas)
						max = czas; //maksymalna wartosc czasu
					aktualnyCalkowityCzas += czas;
				}
				czas = aktualnyCalkowityCzas / (float)liczba_tablic; //srednia wartosc czasu
				
				pomiary << "Dla: " << metodaSortowania << " rozmiar tablicy: " << rozmiarTablic[i] << " losowosci: " << losowoscTablicy[j] << " czas:: " << czas << "\n";
				pomiary2 << "Dla: " << metodaSortowania << " rozmiar tablicy: " << rozmiarTablic[i] << " losowosci: " << losowoscTablicy[j] << " czas:: " << min << "\n";
				pomiary3 << "Dla: " << metodaSortowania << " rozmiar tablicy: " << rozmiarTablic[i] << " losowosci: " << losowoscTablicy[j] << " czas:: " << max << "\n";
			}
		}
	}

	pomiary.close();
	pomiary2.close();
	pomiary3.close();
	delete[] tablica;
	delete[] tab;
	return 0;
}