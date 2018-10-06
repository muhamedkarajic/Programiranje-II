#include <iostream>
#include <fstream>
using namespace std;


char * crt = "\n----------------------------------------------------\n";
char generisanje[10] = "080658999";
struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() {
		delete _dan; delete _mjesec; delete _godina;
		_dan = _mjesec = _godina = nullptr;
	}
};
struct Artikal {
	char * _naziv;
	float _cijena;
	void Unos(char * naziv, float cijena) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_cijena = cijena;
	}
	void Dealociraj() { delete[]_naziv; _naziv = nullptr; }
	void Ispis() { cout << _naziv << " " << _cijena << " KM"; }
};
struct Racun {
	//format broja racuna: 006005491
	//osigurati automatsko generisanje broja racuna pocevsi od 000000001
	char _brojRacuna[10];
	Datum _datumKreiranja;
	Artikal * _artikli;
	int * _kolicine;//cuva informaciju o kolicini svakog artikla na racunu
	int _brojArtikala;
	//Potpisi funkcija trebaju odgovarati onima u main-u
	//1. Unos
	
	//2. Dealociraj
	
	// 3. DodajArtikal - dodaje novi artikal u listu artikala zajedno sa njegovom kolicinom. 
	// Onemoguciti ponavljanje artikala na nacin da se uvecava samo kolicina ukoliko
	// korisnik vise puta pokusa dodati isti artikal.
	
	// 4. Ispis - ispisuje racun u formatu prikazanom na slici (nije obavezno da bude 
	// identican, ali je svakako pozeljno). Prilikom svakog ispisa, racun
	// je potrebno spasiti u tekstualni fajl sa istim nazivom kao i broj racuna.
	
	// 5. BinarnaPretraga - koristeci binarnu pretragu, na osnovu naziva, pronalazi i vraca 
	// pokazivac na artikal. Ukoliko trazeni artikal ne postoji funkcija vraca nullptr.
	
};
//1. PronadjiNajskupljiArtikal - rekurzivna funkcija koja vraca pokazivac na artikal sa najvecom ukupnom cijenom (cijena*kolicina)
//2. GenerisiSljedeciBrojRacuna - generise i vraca naredni broj racuna

void main() {
	//za eventualne nejasnoce analizirati testni program i ispise na slici
	Datum danas; danas.Unos(13, 6, 2016);
	Artikal cigarete, cokolada, sok;
	cigarete.Unos("Cigarete", 2.23);
	cokolada.Unos("Cokolada", 1.23);
	sok.Unos("Cappy", 2.10);
	Racun racun;
	racun.Unos(GenerisiSljedeciBrojRacuna(), danas);
	racun.DodajArtikal(cokolada, 5);
	racun.DodajArtikal(sok, 10);
	racun.DodajArtikal(cigarete, 2);
	racun.DodajArtikal(cigarete, 5);
	racun.Ispis();
	Artikal * p1 = racun.BinarnaPretraga("Cokolada");
	cout << "Binarnom pretragom pronadjen artikal -> ";
	p1->Ispis();
	racun.Ispis();
	Artikal * p2 = PronadjiNajskupljiArtikal(/*Potrebna lista parametara*/);
	cout << "Rekurzivno pronadjen najskuplji artikal -> ";
	p2->Ispis();
	cout << endl;
	//dealocirati zauzetu memoriju
}