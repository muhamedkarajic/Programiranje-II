#include<iostream>
#include<memory>
#include<tuple>

using namespace std;
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NECE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)


enum VrstaAktivnosti { Poligon, Gradska, Nocna, Parkiranje };
enum Kategorija { A, B, C, D, E };

const char* VrstaAktivnostiChar[] = { "Poligon", "Gradska", "Nocna", "Parkiranje" };
const char* KategorijaChar[] = { "A", "B", "C", "D", "E" };

const int maxKategorija = 5;

struct Datum {
	int * _dan, *_mjesec, *_godina;

	int Dani()
	{
		return *_dan + *_mjesec *31+ *_godina*31*12;
	}

	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { delete  _dan; delete  _mjesec; delete _godina; }
};
struct Aktivnost {
	unique_ptr<VrstaAktivnosti> _vrsta;
	Datum _datumIzvrsenja;
	char * _komentar;
	int _ocjena; // 1 - 5
	void Unos(VrstaAktivnosti vrsta, Datum * datum, int ocjena, const char * komentar) {
		_vrsta = make_unique<VrstaAktivnosti>(vrsta);
		_datumIzvrsenja.Unos(*datum->_dan, *datum->_mjesec, *datum->_godina);
		_ocjena = ocjena;
		int size = strlen(komentar) + 1;
		_komentar = new char[size];
		strcpy_s(_komentar, size, komentar);
	}
	void Ispis() {
		cout << VrstaAktivnostiChar[*_vrsta] << " " << _ocjena << " " << _komentar;
		_datumIzvrsenja.Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja.Dealociraj();
		delete[] _komentar; _komentar = nullptr;
	}
};
void swapAktivnosti(Aktivnost &a, Aktivnost &b)
{
	Aktivnost t;

	t.Unos(*a._vrsta, &a._datumIzvrsenja, a._ocjena, a._komentar);
	a.Dealociraj();

	a.Unos(*b._vrsta, &b._datumIzvrsenja, b._ocjena, b._komentar);
	b.Dealociraj();

	b.Unos(*t._vrsta, &t._datumIzvrsenja, t._ocjena, t._komentar);
	t.Dealociraj();
}

struct VozackaKategorija {
	Datum * _datumPolaganja;//datum koji ce se evidentirati kao datum polaganja kategorije tj. kada su ispunjene sve aktivnosti/obaveze
	Kategorija _kategorija;
	Aktivnost * _listaIzvrsenihAktivnosti;
	int _trenutnoIzvrsenihAktivnosti;
	
	/*Funkcija vraca prosjecnu ocjenu svih uspjesno realizovanih aktivnosti koja u nazivu ili opisu sadrze vrijednost primljenog 
	parametra. Ukoliko smatrate da je potrebno, mozete dodati i druge parametre za potrebe implementacije ove funkcije*/
	float PretragaRekrzivno(const char* opis, int brojac = 0, int uspjesni = 0, int suma = 0)
	{
		if (brojac == _trenutnoIzvrsenihAktivnosti)
			if (uspjesni == 0)
				return 0;
			else
				return suma / (float)uspjesni;
		else if (strstr(_listaIzvrsenihAktivnosti[brojac]._komentar, opis) != nullptr && _listaIzvrsenihAktivnosti[brojac]._ocjena >= 3)
		{
			uspjesni++;
			suma += _listaIzvrsenihAktivnosti[brojac]._ocjena;
		}

		return PretragaRekrzivno(opis, brojac + 1, uspjesni, suma);
	}


	/*Stjecanje kategorije zahtijeva uspjesnu realizaciju svih planiranih aktivnosti, a one se za jedan povecavaju sa svakom novom 
	kategorijom, npr.
	A: 1 x poligon, 1 x gradska, 1 x nocna, 1 x parkiranje;
	B: 2 x poligon, 2 x gradska, 2 x nocna, 2 x parkiranje; i td...
	Funkcija vraca false u slucaju da: su kandidatu u listu aktivnosti evidentirane tri negativno ocijenjene identicne vrste aktivnosti 
	(npr. tri negativne ocjene iz gradske voznje), onda se ta kategorija ne moze smatrati uspjesno polozenom
	,te ukoliko nedostaje bilo koja od aktivnosti zahtijevanih u okviru te kategorije. Ukoliko je kandidat uspjesno realizovao sve 
	aktivnost, datum polaganja se postavlja na datum posljednje uspjesno realizovane aktivnosti*/
	bool DaLiJePolozena()
	{
		if (_trenutnoIzvrsenihAktivnosti == 0)
			return false;

		int brojacPolozenih[4] = { 0 }, brojacNepolozenih[4] = { 0 }, l = _trenutnoIzvrsenihAktivnosti-1;

		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			if (_listaIzvrsenihAktivnosti[i]._ocjena >= 3)
			{
				brojacPolozenih[*_listaIzvrsenihAktivnosti[i]._vrsta]++;
				if (_listaIzvrsenihAktivnosti[l]._datumIzvrsenja.Dani() < _listaIzvrsenihAktivnosti[i]._datumIzvrsenja.Dani())
					l = i;
			}
			else
				brojacNepolozenih[*_listaIzvrsenihAktivnosti[i]._vrsta]++;

		for (int i = 0; i < 4; i++)
			if (brojacNepolozenih[i] >= 3)
				return false;
			else if (brojacPolozenih[i] <= _kategorija)
				return false;

		if (_listaIzvrsenihAktivnosti[l]._ocjena < 3)
			return false;

		_datumPolaganja = new Datum;
		_datumPolaganja->Unos(*_listaIzvrsenihAktivnosti[l]._datumIzvrsenja._dan, *_listaIzvrsenihAktivnosti[l]._datumIzvrsenja._mjesec, *_listaIzvrsenihAktivnosti[l]._datumIzvrsenja._godina);
		
		return true;
	}



	//koristeci neki od obradjenih algoritama, po ocjeni sortirati aktivnosti u okviru odredjene kategorije
	void Sortiraj()
	{
		int l;

		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
		{
			l = i;
			for (int j = i+1; j < _trenutnoIzvrsenihAktivnosti; j++)
				if (_listaIzvrsenihAktivnosti[l]._ocjena > _listaIzvrsenihAktivnosti[j]._ocjena)
					l = j;
			if(l != i)
				swapAktivnosti(_listaIzvrsenihAktivnosti[i], _listaIzvrsenihAktivnosti[l]);
		}
	}



	/*Na osnovu vrijednosti primljenog parametra osigurati dodavanje novoizvrsene aktivnosti za potrebe stjecanja odredjene vozacke
		kategorije.Broj aktivnosti nije ogranicen.*/
	void ProsiriIzvrsenuAktivnost(VrstaAktivnosti v, Datum *datum, int ocjena, const char* opis)
	{
		Aktivnost *t = new Aktivnost[_trenutnoIzvrsenihAktivnosti + 1];
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
		{
			t[i].Unos(*_listaIzvrsenihAktivnosti[i]._vrsta, &_listaIzvrsenihAktivnosti[i]._datumIzvrsenja, _listaIzvrsenihAktivnosti[i]._ocjena, _listaIzvrsenihAktivnosti[i]._komentar);
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		}

		t[_trenutnoIzvrsenihAktivnosti].Unos(v, datum, ocjena, opis);
		delete[] _listaIzvrsenihAktivnosti;
		
		_listaIzvrsenihAktivnosti = t;

		_trenutnoIzvrsenihAktivnosti++;
	}



	/*
	Identicna aktivnost se moze dodati jedino u slucaju kada je prethodna (identivna aktivnost po vrsti i datumu izvrsenja) imala ocjenu
	manju od 3.
	
	Uspjesnom aktivnoscu se smatraju one aktivnosti koje imaju ocjenu vecu od 2, a svaka naredna identicna aktivnost, bez obzira da li 
	je uspjesna ili ne,
	moze biti dodana jedino ako je proslo najmanje 30 dana od izvrsenja prethodne. Onemoguciti dodavanje aktivnosti uspjesno polozenoj 
	kategoriji.*/
	bool DodajIzvrsenuAktivnost(VrstaAktivnosti v, Datum *datum, int ocjena, const char* opis)
	{
		if (_datumPolaganja != nullptr)
			return false;

		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == v)
				if (_listaIzvrsenihAktivnosti[i]._datumIzvrsenja.Dani() == datum->Dani())
					if (_listaIzvrsenihAktivnosti[i]._ocjena >= 3)
						return false;
		
		for (int i = _trenutnoIzvrsenihAktivnosti - 1; i >= 0; i--)
			if (*_listaIzvrsenihAktivnosti[i]._vrsta == v)
				if (_listaIzvrsenihAktivnosti[i]._datumIzvrsenja.Dani() + 30 > datum->Dani())
					return false;
				else
					break;

		ProsiriIzvrsenuAktivnost(v, datum, ocjena, opis);
		return true;
	}

	void Unos(Kategorija kategorija) {
		_listaIzvrsenihAktivnosti = nullptr;
		_trenutnoIzvrsenihAktivnosti = 0;
		_datumPolaganja = nullptr;
		_kategorija = kategorija;
	}
	void Dealociraj() {
		if(_datumPolaganja != nullptr)
			_datumPolaganja->Dealociraj();
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Dealociraj();
		delete[] _listaIzvrsenihAktivnosti;
		_listaIzvrsenihAktivnosti = nullptr;
	}
	void Ispis() {
		if (_datumPolaganja != nullptr)
			_datumPolaganja->Ispis();
		cout << _kategorija << endl;
		for (size_t i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			_listaIzvrsenihAktivnosti[i].Ispis();
	}
};

struct Kandidat {
	char * _imePrezime;
	shared_ptr<VozackaKategorija> _kategorije[maxKategorija];
	int _trenutnoKategorija;
	
	//Funkcija GetNajboljuKategoriju  vraca par koji sadrzi oznaku i prosjecnu ocjenu (uspjesno okoncanih aktivnosti) kategorije sa 
	//najvecim prosjekom
	pair<Kategorija, float> GetNajboljuKategoriju()
	{
		pair<Kategorija, float> t;
		t = make_pair<Kategorija, float>(Kategorija(0), 0);

		float max;


		for (int i = 0; i < _trenutnoKategorija; i++)
		{
			max = _kategorije[i]->PretragaRekrzivno("");
			if (max > t.second)
			{
				t.second = max;
				t.first = _kategorije[i]->_kategorija;
			}
		}
		return t;
	}




	/*Pretpostavka je da se kategorije moraju polagati po redoslijedu tj. ne smije se dozvoliti dodavanje kategorije C ukoliko prethodno 
	nisu uspjesno polozene kategorije B i C. Za provjeru ispunjenosti preduslova dodavanja nove kategorije, unutar funkcije 
	DodajKategoriju, kreirati lambda funkciju.*/
	bool DodajKategoriju(VozackaKategorija &k)
	{
		auto ProvjeraIspravnostiUnosa = [&]()
		{
			if (_trenutnoKategorija == k._kategorija)
			{
				_kategorije[_trenutnoKategorija] = make_shared<VozackaKategorija>();
				_kategorije[_trenutnoKategorija]->Unos(k._kategorija);
				for (int x = 0; x < k._trenutnoIzvrsenihAktivnosti; x++)
					_kategorije[_trenutnoKategorija]->ProsiriIzvrsenuAktivnost(*k._listaIzvrsenihAktivnosti[x]._vrsta,&k._listaIzvrsenihAktivnosti[x]._datumIzvrsenja, k._listaIzvrsenihAktivnosti[x]._ocjena, k._listaIzvrsenihAktivnosti[x]._komentar);

				_trenutnoKategorija++;
				return true;
			}
			return false;
		};

		return ProvjeraIspravnostiUnosa();
	}



	void Unos(const char * imePrezime) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_trenutnoKategorija = 0;
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < _trenutnoKategorija; i++) {
			_kategorije[i]->Dealociraj();
			_kategorije[i].reset();
		}
	}
	void Ispis() {
		cout << _imePrezime << endl;
		for (size_t i = 0; i < _trenutnoKategorija; i++)
			_kategorije[i]->Ispis();
	}
};

void main() {

	//BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA
	//STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	Datum datumPolaganja1, datumPolaganja2, datumPolaganja3, datumPolaganja4;
	datumPolaganja1.Unos(10, 6, 2018);
	datumPolaganja2.Unos(18, 6, 2018);
	datumPolaganja3.Unos(22, 3, 2018);
	datumPolaganja4.Unos(28, 7, 2018);

	VozackaKategorija kategorijaA, kategorijaB, kategorijaC;
	kategorijaA.Unos(A);
	kategorijaB.Unos(B);
	kategorijaC.Unos(C);

	/*Na osnovu vrijednosti primljenog parametra osigurati dodavanje novoizvrsene aktivnosti za potrebe stjecanja odredjene vozacke kategorije. Broj aktivnosti nije ogranicen.
	Identicna aktivnost se moze dodati jedino u slucaju kada je prethodna (identivna aktivnost po vrsti i datumu izvrsenja) imala ocjenu manju od 3.
	Uspjesnom aktivnoscu se smatraju one aktivnosti koje imaju ocjenu vecu od 2, a svaka naredna identicna aktivnost, bez obzira da li je uspjesna ili ne,
	moze biti dodana jedino ako je proslo najmanje 30 dana od izvrsenja prethodne. Onemoguciti dodavanje aktivnosti uspjesno polozenoj kategoriji.*/
	if (kategorijaA.DodajIzvrsenuAktivnost(Poligon, &datumPolaganja1, 3, "izuzetno dobra orijentacija"))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (kategorijaA.DodajIzvrsenuAktivnost(Gradska, &datumPolaganja1, 2, "nesigurnost u raskrsnicama"))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (kategorijaA.DodajIzvrsenuAktivnost(Gradska, &datumPolaganja2, 4, "raskrsnice savladane"))//datum?
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (kategorijaA.DodajIzvrsenuAktivnost(Gradska, &datumPolaganja4, 4, "raskrsnice savladane"))
		cout << "Aktivnost uspjesno dodana!" << endl;


	//koristeci neki od obradjenih algoritama, po ocjeni sortirati aktivnosti u okviru odredjene kategorije
	kategorijaA.Sortiraj();
	//for (int i = 0; i < kategorijaA._trenutnoIzvrsenihAktivnosti; i++)
	//	cout << kategorijaA._listaIzvrsenihAktivnosti[i]._ocjena << endl;

	/*Stjecanje kategorije zahtijeva uspjesnu realizaciju svih planiranih aktivnosti, a one se za jedan povecavaju sa svakom novom kategorijom, npr.
	A: 1 x poligon, 1 x gradska, 1 x nocna, 1 x parkiranje;
	B: 2 x poligon, 2 x gradska, 2 x nocna, 2 x parkiranje; i td...
	Funkcija vraca false u slucaju da: su kandidatu u listu aktivnosti evidentirane tri negativno ocijenjene identicne vrste aktivnosti (npr. tri negativne ocjene iz gradske voznje), onda se ta kategorija ne moze smatrati uspjesno polozenom
	,te ukoliko nedostaje bilo koja od aktivnosti zahtijevanih u okviru te kategorije. Ukoliko je kandidat uspjesno realizovao sve aktivnost, datum polaganja se postavlja na datum posljednje uspjesno realizovane aktivnosti*/
	if (kategorijaA.DaLiJePolozena())
		kategorijaA.Ispis();

	/*Funkcija vraca prosjecnu ocjenu svih uspjesno realizovanih aktivnosti koja u nazivu ili opisu sadrze vrijednost primljenog parametra. Ukoliko smatrate da je potrebno, mozete dodati i druge parametre za potrebe implementacije ove funkcije*/
	cout << "Prosjecna ocjena za kategoriju A -> " << kategorijaA.PretragaRekrzivno("nesigurno") << endl;

	//ispisuje sve dostupne podatke o kategoriji
	kategorijaA.Ispis();

	Kandidat jasmin;
	jasmin.Unos("Jasmin Azemovic");

	/*
	Pretpostavka je da se kategorije moraju polagati po redoslijedu tj. ne smije se dozvoliti dodavanje kategorije C ukoliko prethodno nisu uspjesno polozene kategorije B i C. Za provjeru ispunjenosti preduslova dodavanja nove kategorije, unutar funkcije DodajKategoriju, kreirati lambda funkciju.
	*/
	if (jasmin.DodajKategoriju(kategorijaA))
		cout << "Kategorija uspjesno dodan!" << endl;
	if (jasmin.DodajKategoriju(kategorijaC))
		cout << "Kategorija uspjesno dodan!" << endl;
	if (jasmin.DodajKategoriju(kategorijaB))
		cout << "Kategorija uspjesno dodan!" << endl;

	float prosjek = 0;
	Kategorija kategorija;
	//Funkcija GetNajboljuKategoriju  vraca par koji sadrzi oznaku i prosjecnu ocjenu (uspjesno okoncanih aktivnosti) kategorije sa najvecim prosjekom
	tie(kategorija, prosjek) = jasmin.GetNajboljuKategoriju();
	if(prosjek != 0)
		cout << "Najbolji rezultat od " << prosjek << " je ostvaren tokom polaganja kategorije " << kategorija << endl;


	datumPolaganja1.Dealociraj(), datumPolaganja2.Dealociraj(), datumPolaganja3.Dealociraj(), datumPolaganja4.Dealociraj();
	kategorijaA.Dealociraj(), kategorijaB.Dealociraj(), kategorijaC.Dealociraj();
	jasmin.Dealociraj();

	system("pause>0");
}

