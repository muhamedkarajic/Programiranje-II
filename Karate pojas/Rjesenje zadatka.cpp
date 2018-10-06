#include<iostream>
#include<memory>
#include<fstream>
#include<iomanip>
#include<tuple>
using namespace std;
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NECE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)


enum VrstaAktivnosti { Tehnike, Kata, Borba };
enum Pojas { Zuti, Narandzasti, Zeleni, Plavi, Smedji, Crni };

struct Datum {
	int * _dan, *_mjesec, *_godina;
	
	int Dani()
	{
		return *_dan + *_mjesec *31 + *_godina*12*31;
	}

	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; }
};
struct Aktivnost {
	unique_ptr<VrstaAktivnosti> _vrsta;
	Datum _datumIzvrsenja;
	char * _nazivOpis;
	int _ocjena; // 1 - 10 
	void Unos(VrstaAktivnosti vrsta, Datum * datum, int ocjena, const char * nazivOpis) {
		_vrsta = make_unique<VrstaAktivnosti>(vrsta);
		_datumIzvrsenja.Unos(*datum->_dan, *datum->_mjesec, *datum->_godina);
		_ocjena = ocjena;
		int size = strlen(nazivOpis) + 1;
		_nazivOpis = new char[size];
		strcpy_s(_nazivOpis, size, nazivOpis);
	}
	void Ispis() {
		cout << *_vrsta << " " << _ocjena << " " << _nazivOpis;
		_datumIzvrsenja.Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja.Dealociraj();
		delete[] _nazivOpis; _nazivOpis = nullptr;
	}
};

void SwapAktivnosti(Aktivnost &a, Aktivnost &b)
{
	Aktivnost t;
	t.Unos(*a._vrsta, &a._datumIzvrsenja, a._ocjena, a._nazivOpis);
	a.Dealociraj();

	a.Unos(*b._vrsta, &b._datumIzvrsenja, b._ocjena, b._nazivOpis);
	b.Dealociraj();

	b.Unos(*t._vrsta, &t._datumIzvrsenja, t._ocjena, t._nazivOpis);
	t.Dealociraj();
}

struct KaratePojas {
	Datum * _datumPolaganja;//datum koji ce se evidentirati kao datum polaganja pojasa tj. kada su ispunjene sve aktivnosti/obaveze
	Pojas _pojas;
	Aktivnost * _listaIzvrsenihAktivnosti;
	int _trenutnoIzvrsenihAktivnosti;

	/*Funkcija vraca prosjecnu ocjenu svih uspjesno realizovanih aktivnosti koja u nazivu ili opisu sadrze vrijednost primljenog 
	parametra. Ukoliko smatrate da je potrebno, mozete dodati i druge parametre za potrebe implementacije ove funkcije*/
	float PretragaRekrzivno(const char* opis, int brojac = 0, int uspjesni = 0, int suma = 0)
	{
		if (brojac == _trenutnoIzvrsenihAktivnosti)
			if (uspjesni == 0)
				return uspjesni;
			else
				return suma / (float)uspjesni;

		else if (strstr(_listaIzvrsenihAktivnosti[brojac]._nazivOpis, opis) != nullptr && _listaIzvrsenihAktivnosti[brojac]._ocjena >= 6)
		{
			uspjesni++;
			suma += _listaIzvrsenihAktivnosti[brojac]._ocjena;
		}
		return PretragaRekrzivno(opis, brojac + 1, uspjesni, suma);
	}



	/* Karate pojas zahtijeva uspjesnu realizaciju svih planiranih aktivnosti, a one se za jedan povecavaju sa svakim novim pojasom, 
	npr. zuti pojas: 1 x tehnika, 1 x kata, 1 x borba;	narandzasti pojas: 2 x tehnika, 2 x kata, 2 x borba; i td...
	Funkcija vraca false u slucaju da: su kandidatu u listu aktivnosti evidentirane tri negativno ocijenjene identicne vrste aktivnosti 
	(npr. tri negativne ocjene iz borbi), onda se taj pojas ne moze smatrati uspjesno stecenim i, te ukoliko nedostaje bilo koja od 
	aktivnosti zahtijevanih u okviru tog pojasa. Ukoliko je kandidat uspjesno realizovao sve aktivnost, datum polaganja se postavlja 
	na datum posljednje uspjesno realizovane aktivnosti*/
	bool DaLiJePolozen()
	{
		int brojacP[3] = { 0 }, brojacN[3] = { 0 }, l = -1;

		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			if (_listaIzvrsenihAktivnosti[i]._ocjena >= 6)
			{
				l = i;
				brojacP[*_listaIzvrsenihAktivnosti[i]._vrsta]++;
			}
			else
				brojacN[*_listaIzvrsenihAktivnosti[i]._vrsta]++;

		for (int i = 0; i < 3; i++)
			if (brojacN[i] >= 3)
				return false;
			else if (brojacP[i] - 1 < _pojas)
				return false;
		
		if (l == -1)
			return false;

		_datumPolaganja = new Datum;
		_datumPolaganja->Unos(*_listaIzvrsenihAktivnosti[l]._datumIzvrsenja._dan, *_listaIzvrsenihAktivnosti[l]._datumIzvrsenja._mjesec, *_listaIzvrsenihAktivnosti[l]._datumIzvrsenja._godina);
		return true;
	}


	//Koristeci neki od obradjenih algoritama, po ocjeni sortirati aktivnosti u okviru odredjenog pojasa
	void Sortiraj()
	{
		int l;
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti-1; i++)
		{
			l = i;
			for (int j = i+1; j < _trenutnoIzvrsenihAktivnosti; j++)
				if (_listaIzvrsenihAktivnosti[l]._ocjena > _listaIzvrsenihAktivnosti[j]._ocjena)
					l = j;
			if(l != i)
				SwapAktivnosti(_listaIzvrsenihAktivnosti[i], _listaIzvrsenihAktivnosti[l]);
		}
	}



	/*
	Identicna aktivnost se moze dodati jedino u slucaju kada je prethodna (identivna aktivnost po vrsti i datumu izvrsenja) imala ocjenu 
	manju od 6. Uspjesnom aktivnoscu se smatraju one aktivnosti koje imaju ocjenu vecu od 5, a svaka naredna identicna aktivnost, 
	bez obzira da li je uspjesna ili ne, moze biti dodana jedino ako je proslo najmanje 15 dana od izvrsenja prethodne. 
	*/
	bool DodajIzvrsenuAktivnost(Aktivnost *a)
	{
		if (_datumPolaganja != nullptr)
			return false;

		for (int i = _trenutnoIzvrsenihAktivnosti - 1; i >= 0; i--)
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == *a->_vrsta)
				if (_listaIzvrsenihAktivnosti[i]._datumIzvrsenja.Dani() == a->_datumIzvrsenja.Dani())
					if (_listaIzvrsenihAktivnosti[i]._ocjena < 6)
						return false;
				
		auto ProvjeriAktivnosti = [&]()
		{
			for (int i = _trenutnoIzvrsenihAktivnosti - 1; i >= 0; i--)
				if (*_listaIzvrsenihAktivnosti[i]._vrsta == *a->_vrsta)
					if (_listaIzvrsenihAktivnosti[i]._datumIzvrsenja.Dani() + 15 > a->_datumIzvrsenja.Dani())
						return false;
					else
						break;
			return true;
		};



		ProsiriIzvrsenuAktivnost(*a);
		return true;
	}
		


	void ProsiriIzvrsenuAktivnost(Aktivnost &a)
	{
		Aktivnost * t = new Aktivnost[_trenutnoIzvrsenihAktivnosti + 1];
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
		{
			t[i].Unos(*_listaIzvrsenihAktivnosti[i]._vrsta, &_listaIzvrsenihAktivnosti[i]._datumIzvrsenja, _listaIzvrsenihAktivnosti[i]._ocjena, _listaIzvrsenihAktivnosti[i]._nazivOpis);
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		}
		t[_trenutnoIzvrsenihAktivnosti].Unos(*a._vrsta, &a._datumIzvrsenja, a._ocjena, a._nazivOpis);
		delete[] _listaIzvrsenihAktivnosti;

		_listaIzvrsenihAktivnosti = t;

		_trenutnoIzvrsenihAktivnosti++;
	}


	void Unos(Pojas pojas) {
		_listaIzvrsenihAktivnosti = nullptr;
		_trenutnoIzvrsenihAktivnosti = 0;
		_datumPolaganja = nullptr;
		_pojas = pojas;
	}
	void Dealociraj() {
		if (_datumPolaganja != nullptr)
		{
			_datumPolaganja->Dealociraj(); _datumPolaganja = nullptr;
		}
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		delete[] _listaIzvrsenihAktivnosti;
		_listaIzvrsenihAktivnosti = nullptr;
	}
	void Ispis() {
		if (_datumPolaganja != nullptr)
			_datumPolaganja->Ispis();
		cout << _pojas << endl;
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Ispis();
	}
};

struct Kandidat {
	char * _imePrezime;
	shared_ptr<KaratePojas> _pojasevi[6];

	//Funkcija GetNajbolji vraca par koji sadrzi oznaku i prosjecnu ocjenu (uspjesno okoncanih aktivnosti) pojasa sa najvecim prosjekom
	pair<Pojas, float> GetNajbolji()
	{
		pair<Pojas, float> t;
		t = make_pair<Pojas, float>(Pojas(0), 0);
		float max = 0;

		for (int i = 0; i < 6; i++)
		{
			if (_pojasevi[i] == nullptr)
				break;
			max = _pojasevi[i]->PretragaRekrzivno("");
			if (max > t.second)
			{
				t.second = max;
				t.first = _pojasevi[i]->_pojas;
			}
		}

		return t;
	}


	/* Karate pojasevi se moraju dodavati po redoslijedu tj. ne smije se dozvoliti dodavanje zelenog pojasa ukoliko prethodno nije dodan 
	zuti i narandzasti. Za provjeru lokacije (unutar funkcije DodajPojas) na koju ce se dodati novi karate pojas, te da li su nizi 
	pojasevi prethodno dodani koristiti lambda funkciju.*/
	bool DodajPojas(KaratePojas &k)
	{
		auto ProvjeriPojas = [&]()
		{
			for (int i = 0; i < 6; i++)
				if (_pojasevi[i] == nullptr)
					if (k._pojas == i)
					{
						_pojasevi[i] = make_shared<KaratePojas>();
						_pojasevi[i]->Unos(k._pojas);
						if (k._datumPolaganja != nullptr)
						{
							_pojasevi[i]->_datumPolaganja = new Datum;
							_pojasevi[i]->_datumPolaganja->Unos(*k._datumPolaganja->_dan, *k._datumPolaganja->_mjesec, *k._datumPolaganja->_godina);
						}

						for (int x = 0; x < k._trenutnoIzvrsenihAktivnosti; x++)
							_pojasevi[i]->ProsiriIzvrsenuAktivnost(k._listaIzvrsenihAktivnosti[x]);

						return true;
					}
					else
						return false;
			return false;
		};


		return ProvjeriPojas();
	}


	void Unos(const char * imePrezime) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		for (size_t i = 0; i < 6; i++)
			_pojasevi[i] = nullptr;
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr) {
				_pojasevi[i]->Dealociraj();
				_pojasevi[i].reset();
			}
	}
	void Ispis() {
		cout << _imePrezime << endl;
		for (size_t i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr)
				_pojasevi[i]->Ispis();
	}
};

void main() {

	//BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA
	//STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	Datum datumPolaganja1, datumPolaganja2, datumPolaganja3, datumPolaganja4;
	datumPolaganja1.Unos(10, 6, 2018);
	datumPolaganja2.Unos(18, 6, 2018);
	datumPolaganja3.Unos(22, 3, 2018);
	datumPolaganja4.Unos(22, 7, 2018);

	Aktivnost aktivnost1, aktivnost2, aktivnost3, aktivnost4, aktivnost5;
	aktivnost1.Unos(Tehnike, &datumPolaganja1, 6, "Tehnike za zuti pojas");
	aktivnost2.Unos(Kata, &datumPolaganja1, 8, "Taiki joko shodan - zuti pojas");
	aktivnost3.Unos(Borba, &datumPolaganja1, 2, "Jednostavne borbene tehnike sa partnerom");
	aktivnost4.Unos(Borba, &datumPolaganja1, 6, "Jednostavne borbene tehnike sa partnerom");
	aktivnost5.Unos(Borba, &datumPolaganja4, 6, "Jednostavne borbene tehnike sa partnerom");

	KaratePojas pojasZuti, pojasNarandzasti, pojasZeleni;
	pojasZuti.Unos(Zuti);
	pojasNarandzasti.Unos(Narandzasti);
	pojasZeleni.Unos(Zeleni);

	/*Na osnovu vrijednosti primljenog parametra osigurati dodavanje novoizvrsene aktivnosti za potrebe stjecanja odredjenog pojasa. Broj aktivnosti nije ogranicen.
	Identicna aktivnost se moze dodati jedino u slucaju kada je prethodna (identivna aktivnost po vrsti i datumu izvrsenja) imala ocjenu manju od 6.
	Uspjesnom aktivnoscu se smatraju one aktivnosti koje imaju ocjenu vecu od 5, a svaka naredna identicna aktivnost, bez obzira da li je uspjesna ili ne,
	moze biti dodana jedino ako je proslo najmanje 15 dana od izvrsenja prethodne. Onemoguciti dodavanje aktivnosti uspjesno polozenom pojasu.*/
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost1))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost2))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost3))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost4))//15 dana...
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost5))
		cout << "Aktivnost uspjesno dodana!" << endl;

	//Koristeci neki od obradjenih algoritama, po ocjeni sortirati aktivnosti u okviru odredjenog pojasa
	pojasZuti.Sortiraj();
	//for (int i = 0; i < pojasZuti._trenutnoIzvrsenihAktivnosti; i++)
	//	cout << pojasZuti._listaIzvrsenihAktivnosti[i]._ocjena << endl;

	/* Karate pojas zahtijeva uspjesnu realizaciju svih planiranih aktivnosti, a one se za jedan povecavaju sa svakim novim pojasom, npr.
	zuti pojas: 1 x tehnika, 1 x kata, 1 x borba;	narandzasti pojas: 2 x tehnika, 2 x kata, 2 x borba; i td...
	Funkcija vraca false u slucaju da: su kandidatu u listu aktivnosti evidentirane tri negativno ocijenjene identicne vrste aktivnosti (npr. tri negativne ocjene iz borbi), onda se taj pojas ne moze smatrati uspjesno stecenim
	i, te ukoliko nedostaje bilo koja od aktivnosti zahtijevanih u okviru tog pojasa. Ukoliko je kandidat uspjesno realizovao sve aktivnost, datum polaganja se postavlja na datum posljednje uspjesno realizovane aktivnosti*/
	if (pojasZuti.DaLiJePolozen())
		pojasZuti.Ispis();

	/*Funkcija vraca prosjecnu ocjenu svih uspjesno realizovanih aktivnosti koja u nazivu ili opisu sadrze vrijednost primljenog parametra. Ukoliko smatrate da je potrebno, mozete dodati i druge parametre za potrebe implementacije ove funkcije*/
	cout << "Prosjecna ocjena za zuti pojas -> " << pojasZuti.PretragaRekrzivno("pojas") << endl;

	//ispisuje sve dostupne podatke o pojasu
	pojasZuti.Ispis();

	Kandidat jasmin;
	jasmin.Unos("Jasmin Azemovic");
	/*
	Karate pojasevi se moraju dodavati po redoslijedu tj. ne smije se dozvoliti dodavanje zelenog pojasa ukoliko prethodno nije dodan zuti i narandzasti. Za provjeru lokacije (unutar funkcije DodajPojas) na koju ce se dodati novi karate pojas, te da li su nizi pojasevi prethodno dodani koristiti lambda funkciju.
	*/
	if (jasmin.DodajPojas(pojasZuti))
		cout << "Pojas uspjesno dodan!" << endl;
	if (jasmin.DodajPojas(pojasZeleni))//prethodno je trebao biti dodan narandzasti pojas
		cout << "Pojas uspjesno dodan!" << endl;
	if (jasmin.DodajPojas(pojasNarandzasti))
		cout << "Pojas uspjesno dodan!" << endl;

	float prosjek = 0;
	Pojas pojas;
	//Funkcija GetNajbolji vraca par koji sadrzi oznaku i prosjecnu ocjenu (uspjesno okoncanih aktivnosti) pojasa sa najvecim prosjekom
	tie(pojas, prosjek) = jasmin.GetNajbolji();
	cout << "Najbolji rezultat od " << prosjek << " je ostvaren tokom stjecanja pojasa " << pojas << endl;


	datumPolaganja1.Dealociraj(), datumPolaganja2.Dealociraj(), datumPolaganja3.Dealociraj(), datumPolaganja4.Dealociraj();
	aktivnost1.Dealociraj(), aktivnost2.Dealociraj(), aktivnost3.Dealociraj(), aktivnost4.Dealociraj(), aktivnost5.Dealociraj();
	pojasZuti.Dealociraj(), pojasNarandzasti.Dealociraj(), pojasZeleni.Dealociraj();
	jasmin.Dealociraj();

	system("pause");
}