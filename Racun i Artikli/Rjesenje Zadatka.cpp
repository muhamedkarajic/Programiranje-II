#include <iostream>
#include <fstream>
using namespace std;


const char * crt = "\n----------------------------------------------------\n";
char generisanje[10] = "000000000";
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
	void Unos(const char * naziv, float cijena) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_cijena = cijena;
	}
	void Dealociraj() { delete[]_naziv; _naziv = nullptr; }
	void Ispis() { cout << _naziv << " " << _cijena << " KM" << endl; }
};
struct Racun {
	//format broja racuna: 006005491
	//osigurati automatsko generisanje broja racuna pocevsi od 000000001
	char _brojRacuna[10];
	Datum _datumKreiranja;
	Artikal * _artikli;
	int * _kolicine;//cuva informaciju o kolicini svakog artikla na racunu
	int _brojArtikala;

	// 5. BinarnaPretraga - koristeci binarnu pretragu, na osnovu naziva, pronalazi i vraca 
	// pokazivac na artikal. Ukoliko trazeni artikal ne postoji funkcija vraca nullptr.

	Artikal * BinarnaPretraga(const char* naziv)
	{
		int l;
		for (int i = 0; i < _brojArtikala-1; i++)
		{
			l = i;
			for (int j = i+1; j < _brojArtikala; j++)
				if (strcmp(_artikli[l]._naziv, _artikli[j]._naziv) > 0)
					l = j;
			if (l != i)
			{
				swap(_artikli[i], _artikli[l]);
				swap(_kolicine[i], _kolicine[l]);
			}
		}

		l = 0;
		int h = _brojArtikala - 1, m;
		while (l <= h)
		{
			m = (h + l) / 2;
			if (strcmp(_artikli[m]._naziv, naziv) == 0)
				return &_artikli[m];
			else if (strcmp(_artikli[m]._naziv, naziv) > 0)
				h = m - 1;
			else
				l = m + 1;
		}
		return nullptr;

	}



	// 4. Ispis - ispisuje racun u formatu prikazanom na slici (nije obavezno da bude 
	// identican, ali je svakako pozeljno). Prilikom svakog ispisa, racun
	// je potrebno spasiti u tekstualni fajl sa istim nazivom kao i broj racuna.
	void Ispis()
	{
		ofstream upis(_brojRacuna);
		upis << _brojRacuna << endl;
		upis << *_datumKreiranja._dan << "/" << *_datumKreiranja._mjesec << "/" << *_datumKreiranja._godina << endl;
		for (int i = 0; i < _brojArtikala; i++)
			upis << _artikli[i]._naziv << " = " << _artikli[i]._cijena* _kolicine[i] << "KM" << endl;
		upis << endl;
		upis.close();

		ifstream ispis(_brojRacuna);
		char znak;
		while (ispis.get(znak))
			cout << znak;
		ispis.close();
	}


	void ProsiriArtikal(Artikal &artikal, int kolicina)
	{
		Artikal * t = new Artikal[_brojArtikala + 1];
		int *k = new int[_brojArtikala + 1];
		for (int i = 0; i < _brojArtikala; i++)
		{
			t[i].Unos(_artikli[i]._naziv, _artikli[i]._cijena);
			k[i] = _kolicine[i];
		
			_artikli[i].Dealociraj();
		}

		t[_brojArtikala].Unos(artikal._naziv, artikal._cijena);
		k[_brojArtikala] = kolicina;

		delete[] _artikli;
		_artikli = t;
		delete[] _kolicine;
		_kolicine = k;

		_brojArtikala++;
	}

	// 3. DodajArtikal - dodaje novi artikal u listu artikala zajedno sa njegovom kolicinom. 
	// Onemoguciti ponavljanje artikala na nacin da se uvecava samo kolicina ukoliko
	// korisnik vise puta pokusa dodati isti artikal.
	
	void DodajArtikal(Artikal &artikal, int kolicina)
	{
		for (int i = 0; i < _brojArtikala; i++)
			if (strcmp(artikal._naziv, _artikli[i]._naziv) == 0 && artikal._cijena == _artikli[i]._cijena)
			{
				_kolicine[i] += kolicina;
				return;
			}

		ProsiriArtikal(artikal, kolicina);
	}

	void Dealociraj()
	{
		_datumKreiranja.Dealociraj();
		for (int i = 0; i < _brojArtikala; i++)
			_artikli[i].Dealociraj();
		delete[] _artikli;
		_artikli = nullptr;
		delete[] _kolicine;
		_kolicine = nullptr;
	}

	void Unos(const char* brojIndeksa, Datum datumKreiranja)
	{
		strcpy_s(_brojRacuna, brojIndeksa);
		_datumKreiranja.Unos(*datumKreiranja._dan, *datumKreiranja._mjesec, *datumKreiranja._godina);
		_artikli = nullptr;
		_kolicine = nullptr;
		_brojArtikala = 0;
	}
};
//1. PronadjiNajskupljiArtikal - rekurzivna funkcija koja vraca pokazivac na artikal sa najvecom ukupnom cijenom (cijena*kolicina)
Artikal * PronadjiNajskupljiArtikal(Racun &r, int brojac = 0, int iznos = 0, Artikal *najskuplji = nullptr)
{
	if (r._brojArtikala == brojac)
		return najskuplji;
	else if (iznos < r._artikli[brojac]._cijena * r._kolicine[brojac])
	{
		iznos = r._artikli[brojac]._cijena * r._kolicine[brojac];
		najskuplji = &r._artikli[brojac];
	}
	return PronadjiNajskupljiArtikal(r, brojac + 1, iznos, najskuplji);
}


//2. GenerisiSljedeciBrojRacuna - generise i vraca naredni broj racuna
char *GenerisiSljedeciBrojRacuna()
{
	bool test = false;
	if (generisanje[test] == '0')
		generisanje[test++] = '1';

	int broj = atoi(generisanje)+1;
	_itoa_s(broj, generisanje, 10);

	if (test)
		generisanje[0] = '0';

	return generisanje;
}

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
	cout << endl;
	racun.Ispis();
	Artikal * p2 = PronadjiNajskupljiArtikal(racun);
	cout << "Rekurzivno pronadjen najskuplji artikal -> ";
	p2->Ispis();
	cout << endl;
	//dealocirati zauzetu memoriju

	danas.Dealociraj();
	cigarete.Dealociraj(); cokolada.Dealociraj(); sok.Dealociraj();
	racun.Dealociraj();
	p1 = nullptr;
	p2 = nullptr;

	system("PAUSE>0");
}