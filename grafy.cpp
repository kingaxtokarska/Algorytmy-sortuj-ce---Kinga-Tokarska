#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <string>
#include <math.h>

using namespace std;

int rozmiar;
int waga;
int V;
int E;

struct Element {
	int waga;
	int dest = -1;
	Element* nast = NULL;
};

struct Lista {
	int iloscElem = 0;
	Element* pierwszy = NULL;

	void dodajElem(int dest, int waga) {
		Element* element = new Element;

		element->waga = waga;
		element->dest = dest;


		if (iloscElem == 0) { //utworzenie pierwszego elementu
			pierwszy = element; //nadanie wskaznikowi pierwszy nowego elementu
		}
		else {
			element->nast = pierwszy->nast; //utworzenie kolejnego elementu
			pierwszy->nast = element; 
		}
		iloscElem++;
	}

	void usunElem(int n) {
		Element* element = pierwszy;

		if (n == 0) {
			Element* temp = pierwszy; //usuniecie pierwszego elementu
			element = pierwszy; 
			pierwszy = temp->nast;
		}
		else {
			Element* temp = pierwszy; //usuniecie n-tego elementu
			int nr = 0;

			while (temp != NULL) {
				if (nr == n - 1)
					break;
				nr++;
				temp = temp->nast;
			}

			element = temp->nast;

			if (element->nast == NULL) { //gdy wskaznik jest ostatni
				temp->nast = NULL;
			}
			else
				temp->nast = element->nast; //gdy wskaznik nie jest ostatni

		}

		delete element;
		iloscElem--;
	}

	Element* wezElem(int n) { //pobieranie n-tego elementu
		Element* element = pierwszy;
		int nr = 0;
		while (element != NULL) {
			if (nr == n)
				return element;
			element = element->nast;
			nr++;
		}
		return NULL;
	}

	~Lista() {
		Element* el = this->pierwszy;
		Element* el2 = this->pierwszy;
		while (el != NULL) { //usuniecie elementow od pierwszego do ostatniego
			el2 = el->nast;
			delete el;
			el = el2;
		}
	}
};


std::string zbudujNazwePliku(int indeksPliku) {
	std::string nazwaPliku = "plik";
	nazwaPliku.append(std::to_string(indeksPliku));
	nazwaPliku.append(".txt");

	return nazwaPliku;
}

void dijkstraLista(int wielkoscGrafu, Lista **krawedzie) {
	int minValue = 32767;
	int minNode = 0;

	int wierzcholki[wielkoscGrafu];
	for (int i = 0; i < wielkoscGrafu; i++) {
		wierzcholki[i] = i;
	}
	int index = 0;
	int odleglosci[wielkoscGrafu];
	int poprze[wielkoscGrafu];
	Element *el = krawedzie[0]->pierwszy; //wierzcholek startowy rowny 0
	while (el != NULL) { //petla dodaje wagi 
		odleglosci[el->dest] = el->waga;
		el = el->nast;
	}


	for (int i = 0; i < wielkoscGrafu; i++) { //petla sprawdza czy wierzcholki zostaly odwiedzone
		if (wierzcholki[i] == -1) { continue; }
		if (odleglosci[i] > 0 && odleglosci[i] < minValue) {
			minValue = odleglosci[i];
			minNode = i;
		}
	}
	for (int i = 0; i < wielkoscGrafu; i++) {
		if (wierzcholki[i] == -1) { continue; }
		if (odleglosci[i] > 0 && odleglosci[i] < minValue) {
			minValue = odleglosci[i];
			minNode = i;
		}
	}

	wierzcholki[minNode] = -1;


	el = krawedzie[minNode]->pierwszy;
	while (el != NULL) {
		if (el->waga < 0) { continue; }
		if (odleglosci[el->dest] < 0) {
			odleglosci[el->dest] = minValue + el->waga;
			continue;
		}

		if ((odleglosci[minNode] + el->waga) < odleglosci[el->dest]) {
			odleglosci[el->dest] = minValue + el->waga;
		}
		el = el->nast;
	}

	fstream plik;
	std::string nazwaPliku = zbudujNazwePliku(5);

	plik.open(nazwaPliku, ios::app);

	for (int i = 0; i < wielkoscGrafu; i++) {
		plik << odleglosci[i] << " ";
	}
	plik << endl << endl;

}


void dijkstraMacierz(int wielkoscGrafu, int **macierz) {
	int minValue = 32767;
	int minNode = 0;

	// stworzenie tablicy z odwiedzanymi wierzcholkami
	// Odwiedzone wierzcholki będa mialy wartosc -1, wiec na poczatek inicjalizowane sa wartosciami != -1
	int wierzcholki[wielkoscGrafu];
	for (int i = 0; i < wielkoscGrafu; i++) {
		wierzcholki[i] = i;
	}

	// inicjalizacja listy odleglosci
	int odleglosci[wielkoscGrafu];
	for (int i = 0; i < wielkoscGrafu; i++) {
		odleglosci[i] = macierz[0][i];
	}


	for (int i = 0; i < wielkoscGrafu; i++) { //petla sprawdza czy wierzcholki zostaly odwiedzone
		if (wierzcholki[i] == -1) { continue; } //jesli nie zostaly odwiedzone, to sprawdza odleglosc miedzy nimi 
		if (odleglosci[i] > 0 && odleglosci[i] < minValue) //jesli odleglosc jest mniejsza niz ta ktora byla wczesniej
			minValue = odleglosci[i]; //zapisuje najkrotszy dystans
		minNode = i; //oznacza ten wezel jako aktualnie najblizszy
	}

	for (int i = 0; i < wielkoscGrafu; i++) {
		if (wierzcholki[i] == -1) { continue; }
		if (odleglosci[i] > 0 && odleglosci[i] < minValue) {
			minValue = odleglosci[i];
			minNode = i;
		}
	}

	wierzcholki[minNode] = -1;

	for (int i = 0; i < wielkoscGrafu; i++) {
		if (macierz[minNode][i] < 0) { continue; }
		if (odleglosci[i] < 0) {
			odleglosci[i] = minValue + macierz[minNode][i];
			continue;
		}
		if ((odleglosci[minNode] + macierz[minNode][i]) < odleglosci[i]) {
			odleglosci[i] = minValue + macierz[minNode][i];
		}
	}


	fstream plik;
	std::string nazwaPliku = zbudujNazwePliku(5);

	plik.open(nazwaPliku, ios::app);

	for (int i = 0; i < wielkoscGrafu; i++) {
		plik << odleglosci[i] << " ";
	}
	plik << endl << endl;

}

void zapisDoPliku()
{
	fstream plik;
	int i, j, k;
	float gestosci[4] = { 0.25, 0.5, 0.75, 1 };
	int iloscWierzcholkow = 10;

	for (int g = 0; g < 4; ++g) {
		// 4 pliki, dla kazdej gestosci osobny
		int iloscKrawedzi = gestosci[g] * (iloscWierzcholkow * (iloscWierzcholkow - 1)) / 2; //liczenie ilosci krawedzi na podstawie gestosci i wierzcholkow
		int indeksWierzcholkaStartowego = rand() % iloscWierzcholkow; //indeks wierzcholka startowego jest losowany z ilosci wierzcholkow
		std::string nazwaPliku = zbudujNazwePliku(g);

		plik.open(nazwaPliku, ios::out);
		plik << iloscKrawedzi << " " << iloscWierzcholkow << " " << indeksWierzcholkaStartowego << endl; //pierwsza linia kazdego pliku
		for (int h = 0; h < iloscWierzcholkow; ++h) { //w nastepnych liniach w petli zapisywane sa
			for (int j = 0; j < floor(iloscWierzcholkow * gestosci[g]); ++j) { //dane o wierzcholku poczatkowym, wierzcholku koncowym
				if ((j) != (h)) {
					int waga = (rand() % 100) + 1; //i wadze 
					plik << h << " " << j << " " << waga << endl;
				}
			}
		}

		plik.close();
	}
}

void odczytZPliku_1()
{
	fstream plik;
	int iloscKrawedzi = -1;
	int iloscWierzcholkow = -1;
	int indeksWierzcholkaStartowego = -1;
	int pomiarCzasu[4];

	for (int g = 0; g < 4; ++g) {
		// 4 pliki, dla kazdej gestosci osobny plik
		std::string nazwaPliku = zbudujNazwePliku(g);

		plik.open(nazwaPliku, ios::in);
		// odczyt 1. linii z pliku
		plik >> iloscKrawedzi >> iloscWierzcholkow >> indeksWierzcholkaStartowego;
		//inicjalziacja macierzy sasiedztwa (tablica 2 wymiarowa)
		int **macierz;
		macierz = new int *[iloscWierzcholkow];
		for (int i = 0; i < iloscWierzcholkow; ++i) {
			macierz[i] = new int[iloscWierzcholkow];
		}
		// odczyt kolejnych linii z pliku, aby odtworzyc macierz sasiedztwa
		while (plik) {
			std::string wierzcholekPoczatkowy;
			std::string wierzcholekKoncowy;
			std::string waga; //zczytujemy z pliku 3 wartosci wiercholka poczatkowego, konocwego, wagi
			plik >> wierzcholekPoczatkowy >> wierzcholekKoncowy >> waga;
			int wierzcholekPoczatkowyInt = std::atoi(wierzcholekPoczatkowy.c_str()); // konwersja string na int wartosci odczytanych z pliku 
			int wierzcholekKoncowyInt = std::atoi(wierzcholekKoncowy.c_str());
			int wagaInt = std::atoi(waga.c_str());
			if (wierzcholekPoczatkowyInt != -1 && wierzcholekKoncowyInt != -1 && wagaInt != -1) { //jesli warunek jest spelkony i wierzcholki i wagi nie sa ujemne
				macierz[wierzcholekPoczatkowyInt][wierzcholekPoczatkowyInt] = 0;
				macierz[wierzcholekPoczatkowyInt][wierzcholekKoncowyInt] = wagaInt; //to zapisujemy wagi od wierzcholka poczatkowego do koncowego, i od koncowego dopoczatkowego
				macierz[wierzcholekKoncowyInt][wierzcholekPoczatkowyInt] = wagaInt;
			}
		}
		plik.close();

		// uzupelnienie brakujacych wag (ktorych nie ma w pliku) wartoscia -1 (brak krawedzi)
		for (int i = 0; i < iloscWierzcholkow; ++i) {
			for (int h = 0; h < iloscWierzcholkow; ++h) {
				if (!(macierz[i][h] >= 0)) {
					macierz[i][h] = -1;
				}
			}
		}
		cout << "macierz gotowa" << endl;
		// pomiar czasu w sekundach
		double time;
		for (int i = 0; i < 100; ++i) { //petla w ktorej dla stu intacji mierzymy czas wykonania algorytmu
			clock_t begin = clock();
			dijkstraMacierz(iloscWierzcholkow, macierz);
			clock_t end = clock();

			time += (double(end - begin) / CLOCKS_PER_SEC);
		}
		pomiarCzasu[g] = time / 100;
		cout << "czas wykonania dla gestosci " << (g + 1)*0.25 << ": " << pomiarCzasu[g] << " " << time << endl;  // dla kazdej poszczegolnej gestosci

	}
}

void odczytZPliku_2()
{
	fstream plik;
	int iloscKrawedzi = -1;
	int iloscWierzcholkow = -1;
	int indeksWierzcholkaStartowego = -1;
	int pomiarCzasu[4];

	for (int g = 0; g < 4; ++g) {
		// 4 pliki, dla kazdej gestosci osobny plik
		std::string nazwaPliku2 = zbudujNazwePliku(g);

		plik.open(nazwaPliku2, ios::in);
		plik >> iloscKrawedzi >> iloscWierzcholkow >> indeksWierzcholkaStartowego;


		Lista** krawedzie = new Lista*[iloscWierzcholkow];
		for (int i = 0; i < iloscWierzcholkow; i++) {
			krawedzie[i] = new Lista;
		}

		while (plik) {
			std::string wierzcholekPoczatkowy;
			std::string wierzcholekKoncowy;
			std::string waga; // zczytujemy z pliku 3 wartosci wiercholka poczatkowego, konocwego, wagi
			plik >> wierzcholekPoczatkowy >> wierzcholekKoncowy >> waga;
			int wierzcholekPoczatkowyInt = std::atoi(wierzcholekPoczatkowy.c_str()); //wartosci te konwertujemy ze stringa do intigera
			int wierzcholekKoncowyInt = std::atoi(wierzcholekKoncowy.c_str());
			int wagaInt = std::atoi(waga.c_str());
			if (wierzcholekPoczatkowyInt != -1 && wierzcholekKoncowyInt != -1 && wagaInt != -1) { //jesli warunek jest spelkony i wierzcholki i wagi nie sa ujemne
				krawedzie[wierzcholekPoczatkowyInt]->dodajElem(wierzcholekKoncowyInt, wagaInt); //to zapisujemy wagi od wierzcholka poczatkowego do koncowego, i od koncowego dopoczatkowego
				krawedzie[wierzcholekKoncowyInt]->dodajElem(wierzcholekPoczatkowyInt, wagaInt);
			}
		}
		plik.close();


		cout << "macierz gotowa" << endl;
		double time; //pomiar czasu w sekundach
		for (int i = 0; i < 100; ++i) { //petla w ktorej dla stu intacji mierzymy czas wykonania algorytmu
			clock_t begin = clock();
			dijkstraLista(iloscWierzcholkow, krawedzie);
			clock_t end = clock();

			time += (double(end - begin) / CLOCKS_PER_SEC);
		}
		pomiarCzasu[g] = time / 100;
		cout << "czas wykonania dla gestosci " << (g + 1)*0.25 << ": " << pomiarCzasu[g] << " " << time << endl; // dla kazdej poszczegolnej gestosci

	}
}


int main()
{
	srand(time(NULL));


	zapisDoPliku();
	odczytZPliku_2();
}