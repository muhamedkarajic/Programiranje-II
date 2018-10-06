#include <iostream>
#include <cstring>
#include <fstream>
using namespace std;

const char * crt = "\n----------------------------------------------------\n";
struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	int Dani() { return *_dan + *_mjesec*31 + *_godina*31*12; }
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() {
		delete _dan; delete _mjesec; delete _godina;
		_dan = _mjesec = _godina = nullptr;
	}

};
struct Izostanak {
	char * _razlogIzostanka;
	Datum _datumIzostanka;
	int _brojSati;
	bool _opravdano;//pravdanje zahtijeva da se evidentira i razlog izostanka	
	void Unos(Datum datumIzostanka, int brojSati) {
		_datumIzostanka.Unos(*datumIzostanka._dan, *datumIzostanka._mjesec, *datumIzostanka._godina);
		_brojSati = brojSati;
		_opravdano = false;
		_razlogIzostanka = new char;
		_razlogIzostanka[0] = '\0';
	}
	void Dealociraj() { delete[] _razlogIzostanka; _razlogIzostanka = nullptr; }
	void Ispis() { _datumIzostanka.Ispis(); cout << _razlogIzostanka << " " << _razlogIzostanka; }
	void Opravdaj(const char * razlogIzostanka) {
		if (_razlogIzostanka != nullptr)
			delete[] _razlogIzostanka;
		int vel = strlen(razlogIzostanka) + 1;
		_razlogIzostanka = new char[vel];
		strcpy_s(_razlogIzostanka, vel, razlogIzostanka);
		_opravdano = true;
	}
};

void swapIzostanke(Izostanak &a, Izostanak &b)
{
	Izostanak t;
	t.Unos(a._datumIzostanka, a._brojSati);
	if (a._opravdano)
		t.Opravdaj(a._razlogIzostanka);
	a.Dealociraj();
	
	a.Unos(b._datumIzostanka, b._brojSati);
	if (b._opravdano)
		a.Opravdaj(b._razlogIzostanka);
	b.Dealociraj();

	b.Unos(t._datumIzostanka, t._brojSati);
	if (t._opravdano)
		b.Opravdaj(t._razlogIzostanka);
	t.Dealociraj();
}

struct Student {
	//format broja indeksa: IB150051
	//osigurati automatsko generisanje broja indeksa pocevsi od IB150001
	char _brojIndeksa[9];
	char * _imePrezime;
	Izostanak * _izostanci;
	int _brojIzostanaka;
	//Potpisi funkcija trebaju odgovarati onima u main-u

	//5. BinarnaPretraga - koristeci binarnu pretragu, na osnovu datuma, pronalazi i vraca pokazivac na izostanak. 
	//Ukoliko trazeni izostanak ne postoji funkcija vraca nullptr.

	Izostanak * BinarnaPretraga(Datum &datum)
	{
		int lokacija;
		for (int i = 0; i < _brojIzostanaka; i++)
		{
			lokacija = i;
			for (int j = i+1; j < _brojIzostanaka; j++)
			{
				if (_izostanci[lokacija]._datumIzostanka.Dani() < _izostanci[j]._datumIzostanka.Dani())
					lokacija = j;
			}
			if (lokacija != i)
				swapIzostanke(_izostanci[lokacija], _izostanci[i]);
		}

		int l=0, h=_brojIzostanaka-1, m;
		while (l <= h)
		{
			m = (l + h) / 2;
			if (_izostanci[m]._datumIzostanka.Dani() == datum.Dani())
				return &_izostanci[m];
			else if (_izostanci[m]._datumIzostanka.Dani() > datum.Dani())
				l = m + 1;
			else
				h = m - 1;
		}

		return nullptr;
	}


	void ProsiriIzostanak(Izostanak &izostanak)
	{
		Izostanak * t = new Izostanak[_brojIzostanaka + 1];
		for (int i = 0; i < _brojIzostanaka; i++)
		{
			t[i].Unos(_izostanci[i]._datumIzostanka, _izostanci[i]._brojSati);
			if (_izostanci[i]._opravdano)
				t[i].Opravdaj(_izostanci[i]._razlogIzostanka);
			_izostanci[i].Dealociraj();
		}
		t[_brojIzostanaka].Unos(izostanak._datumIzostanka, izostanak._brojSati);
		if (izostanak._opravdano)
			t[_brojIzostanaka].Opravdaj(izostanak._razlogIzostanka);

		delete[] _izostanci;
		_izostanci = t;
		_brojIzostanaka++;
	}


	/*3. DodajIzostanak - dodaje novi izostanak u listu izostanaka.	
	Onemoguciti ponavljanje izostanaka na nacin da se uvecava samo broj sati ukoliko*/
	void DodajIzostanak(Izostanak &izostanak)
	{
		for (int i = 0; i < _brojIzostanaka; i++)
		{
			if (izostanak._datumIzostanka.Dani() == _izostanci[i]._datumIzostanka.Dani())
			{
				_izostanci[i]._brojSati += izostanak._brojSati;
				if (izostanak._opravdano)
					_izostanci[i].Opravdaj(izostanak._razlogIzostanka);
				return;
			}
		}
		ProsiriIzostanak(izostanak);
	}


	//4. Ispis - ispisuje izostanke u formatu prikazanom na slici (nije obavezno da ispis bude identican, ali je svakako 
	//pozeljno). Prilikom svakog ispisa, izostanke je potrebno spasiti u tekstualni fajl sa istim nazivom kao i broj indeksa.
	void Ispis()
	{
		char naziv[13];
		strcpy_s(naziv, 13, _brojIndeksa);
		strcat_s(naziv, 13, ".txt");
		ofstream upis(naziv);

		upis << _brojIndeksa << crt;
		upis << "UKUPNO: " << _brojIzostanaka << endl;
		for (int i = 0; i < _brojIzostanaka; i++)
		{
			upis << " " <<  _izostanci[i]._brojSati;
			upis << " -> ";
			upis << *_izostanci[i]._datumIzostanka._dan << "/" << *_izostanci[i]._datumIzostanka._mjesec << "/" << *_izostanci[i]._datumIzostanka._godina;
			if (_izostanci[i]._opravdano)
				upis << " -> " << _izostanci[i]._razlogIzostanka;
			upis << crt;

		}
		upis.close();

		ifstream ispis(naziv);

		char znak;
		while (ispis.get(znak))
			cout << znak;
		ispis.close();
	}


	//1. Unos
	void Unos(const char* brojIndeksa, const char* imePrezime)
	{
		strcpy_s(_brojIndeksa, brojIndeksa);
		delete[] brojIndeksa;
		brojIndeksa = nullptr;

		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);

		_izostanci = nullptr;
		_brojIzostanaka = 0;
	}
	//2. Dealociraj
	void Dealociraj()
	{
		delete[] _imePrezime;
		for (int i = 0; i < _brojIzostanaka; i++)
			_izostanci->Dealociraj();
		delete[] _izostanci;
		_izostanci = nullptr;
		_brojIzostanaka = 0;
	}
	
	

};

//1. PronadjiNajveciNeopravdaniIzostanak - rekurzivna funkcija koja vraca pokazivac na neopravdani izostanak koji ima najveci broj 
Izostanak * PronadjiNajveciNeopravdaniIzostanak(Student *denis, int brojac = 0, Izostanak *najveci = nullptr)
{
	if (brojac == denis->_brojIzostanaka)
		return najveci;
	else if (najveci == nullptr)
		if (denis->_izostanci[brojac]._opravdano == false)
			return PronadjiNajveciNeopravdaniIzostanak(denis, brojac + 1, &denis->_izostanci[brojac]);
	
	return PronadjiNajveciNeopravdaniIzostanak(denis, brojac + 1, najveci);
}

//2. GenerisiSljedeciBrojIndeksa - generise i vraca naredni broj indeksa
int broj = 150000;
char *GenerisiSljedeciBrojIndeksa()
{
	char* temp = new char[9];
	strcpy_s(temp, 9, "IB");
	char brojIndeksa[7];
	broj++;
	_itoa_s(broj, brojIndeksa, 10);
	strcat_s(temp, 9, brojIndeksa);
	return temp;
}

void main() {
	Datum jucer, prije5Dana, prije10Dana;
	jucer.Unos(12, 6, 2016);
	prije5Dana.Unos(8, 6, 2016);
	prije10Dana.Unos(3, 6, 2016);

	Student denis;
	denis.Unos(GenerisiSljedeciBrojIndeksa(), "Denis Music");
	Izostanak izostanakJucer, izostanakPrije5Dana, izostanakPrije10Dana;
	
	izostanakJucer.Unos(jucer, 5);
	denis.DodajIzostanak(izostanakJucer);

	izostanakPrije5Dana.Unos(prije5Dana, 3);
	izostanakPrije5Dana.Opravdaj("Odsutan zbog bolesti - gripa");
	denis.DodajIzostanak(izostanakPrije5Dana);
	izostanakPrije5Dana.Unos(prije5Dana, 2);
	denis.DodajIzostanak(izostanakPrije5Dana);

	izostanakPrije10Dana.Unos(prije10Dana, 1);
	denis.DodajIzostanak(izostanakPrije10Dana);
	izostanakPrije10Dana.Unos(prije10Dana, 1);
	denis.DodajIzostanak(izostanakPrije10Dana);

	denis.Ispis();
	cout << crt;

	Izostanak * p = denis.BinarnaPretraga(jucer);
	cout << "Binarnom pretragom pronadjen izostanak -> ";
	if(p != nullptr)
		p->Ispis();

	cout << crt;

	denis.Ispis();
	cout << crt;
	Izostanak * p2 = PronadjiNajveciNeopravdaniIzostanak(&denis);
	cout << "Rekurzivno pronadjen najveci neopravdani izostanak -> ";
	if (p2 != nullptr)
		p2->Ispis();
	cout << crt;

	//dealocirati zauzetu memoriju
	p = nullptr;
	p2 = nullptr;
	
	jucer.Dealociraj();
	prije5Dana.Dealociraj();
	prije10Dana.Dealociraj();
	izostanakJucer.Dealociraj();
	izostanakPrije5Dana.Dealociraj();
	izostanakPrije10Dana.Dealociraj();
	denis.Dealociraj();

	system("PAUSE>0");
}