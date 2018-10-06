#include<iostream>
#pragma warning(disable:4996)
using namespace std;

enum enumKursevi { HtmlCSSJavaScript, SoftwareEngeneeringFundamentals, MasteringSQL, WindowsSecurity };
const char* enumKurseviChar[] { "HtmlCSSJavaScript", "SoftwareEngeneeringFundamentals", "MasteringSQL", "WindowsSecurity" };
const char *crt = "\n--------------------------------------------\n";

struct Datum {
	int * _dan, *_mjesec, *_godina;

	int Dani() { return *_dan + *_mjesec * 30 + *_godina * 365; }

	bool IzmedjuDatum(Datum OD, Datum DO) { return Dani() > OD.Dani() && Dani() < DO.Dani(); }

	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; }
};

struct Kurs {
	enumKursevi _kurs;
	Datum _pocetak;
	Datum _kraj;
	char * _imePredavaca;
	bool _aktivan;//SVAKI KURS JE NA POCETKU AKTIVAN
	
	void Ispis()
	{
		cout << enumKurseviChar[_kurs] << endl;
		_pocetak.Ispis();
		_kraj.Ispis();
		cout << "Predavac: " << _imePredavaca << endl;
		if (_aktivan)
			cout << "AKTIVAN" << endl;
	}


	void Unos(enumKursevi kurs, Datum pocetak, Datum kraj, const char* imePredavaca)
	{
		int vel = strlen(imePredavaca) + 1;
		_imePredavaca = new char[vel];
		strcpy_s(_imePredavaca, vel, imePredavaca);

		_pocetak.Unos(*pocetak._dan, *pocetak._mjesec, *pocetak._godina);
		_kraj.Unos(*kraj._dan, *kraj._mjesec, *kraj._godina);
		_aktivan = true;

		_kurs = kurs;
	}



	void Dealociraj() {
		delete[] _imePredavaca; _imePredavaca = nullptr;
	}
};
struct Polaznik {
	int _polaznikID;
	char * _imePrezime;
	void Unos(int polaznikID, const char * imePrezime) {
		_polaznikID = polaznikID;
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
	}
	void Ispis() { cout << _polaznikID << " " << _imePrezime << endl; }
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}
};
struct Polaganja {
	Polaznik _polaznik;
	Kurs _kurs;
	Datum _datumPolaganja;
	float _ostvareniUspjeh;

	void Ispis()
	{
		_polaznik.Ispis();
		_kurs.Ispis();
		_datumPolaganja.Ispis();
		cout << "Uspjeh kandidata: " << _ostvareniUspjeh  << "%"<< endl;
	}

	void Dealociraj()
	{
		_polaznik.Dealociraj();
		_kurs.Dealociraj();
		_datumPolaganja.Dealociraj();
	}

	//61 predstavlja ostvareni uspjeh/procenat, a uspjesno polozenim se smatra svaki kurs na kome je polaznik ostvari vise od 55%
	void Unos(Polaznik &polaznik, Kurs &kurs, Datum &datum, float ostvareniUspjeh)
	{
		_polaznik.Unos(polaznik._polaznikID, polaznik._imePrezime);
		_kurs.Unos(kurs._kurs, kurs._pocetak, kurs._kraj, kurs._imePredavaca);
		_datumPolaganja.Unos(*datum._dan, *datum._mjesec, *datum._godina);
		_ostvareniUspjeh = ostvareniUspjeh;
	}


};
struct SkillsCentar {
	char * _nazivCentra;
	Kurs * _kursevi[50];//JEDAN EDUKACIJSKI CENTAR MOZE NUDITI NAJVISE 50 KURSEVA
	int _trenutnoKurseva;
	Polaganja * _polaganja;
	int _trenutnoPolaganja;

	void Dealociraj()
	{
		delete[] _nazivCentra;
		_nazivCentra = nullptr;
		for (int i = 0; i < _trenutnoKurseva; i++)
		{
			_kursevi[i]->Dealociraj();
			delete _kursevi[i];
			_kursevi[i] = nullptr;
		}
		for (int i = 0; i < _trenutnoPolaganja; i++)
			_polaganja[i].Dealociraj();
		delete[] _polaganja;
		_polaganja = nullptr;
	}

	//Ispis - ispisuje sve informacije o edukacijskom centru. prije ispisa sortirati sve kurseve na osnovu pocetka odrzavanja kursa
	void Ispis()
	{
		SortirajKurseve();
		cout << "Naziv centra: " << _nazivCentra << endl;
		for (int i = 0; i < _trenutnoKurseva; i++)
			_kursevi[i]->Ispis();
		for (int i = 0; i < _trenutnoPolaganja; i++)
			_polaganja[i].Ispis();
	}


	void SortirajKurseve()
	{
		int l;
		for (int i = 0; i < _trenutnoKurseva-1; i++)
		{
			l = i;
			for (int j = i+1; j < _trenutnoKurseva; j++)
				if (_kursevi[l]->_pocetak.Dani() > _kursevi[j]->_pocetak.Dani())
					l = j;
			if(i != l)
				swap(_kursevi[i], _kursevi[l]);
		}
	}




	int PolaganjaByDatumBrojac(Datum OD, Datum DO, int brojac = 0, int brojPolaganja = 0)
	{
		if (brojac == _trenutnoPolaganja)
			return brojPolaganja;
		else if (_polaganja[brojac]._datumPolaganja.IzmedjuDatum(OD, DO))
			brojPolaganja++;
		return PolaganjaByDatumBrojac(OD, DO, brojac+1, brojPolaganja);
	}

	//PolaganjaByDatum – vraca niz polaganja koja su uspjesno realizovana u periodu OD-DO
	Polaganja *PolaganjaByDatum(Datum OD, Datum DO, int &brojPolaganja)
	{
		brojPolaganja = PolaganjaByDatumBrojac(OD, DO);
		if (brojPolaganja == 0)
			return nullptr;

		Polaganja *t = new Polaganja[brojPolaganja];
		brojPolaganja = 0;

		for (int i = 0; i < _trenutnoPolaganja; i++)
			if (_polaganja[i]._datumPolaganja.IzmedjuDatum(OD, DO))
				t[brojPolaganja++].Unos(_polaganja[i]._polaznik, _polaganja[i]._kurs, _polaganja[i]._datumPolaganja, _polaganja[i]._ostvareniUspjeh);

		return t;
	}

	void ProsiriPolaganje(Polaganja &polaganje)
	{
		Polaganja * t = new Polaganja[_trenutnoPolaganja + 1];
		for (int i = 0; i < _trenutnoPolaganja; i++)
		{
			t[i].Unos(_polaganja[i]._polaznik, _polaganja[i]._kurs, _polaganja[i]._datumPolaganja, _polaganja[i]._ostvareniUspjeh);
			_polaganja[i].Dealociraj();
		}	
		
		t[_trenutnoPolaganja].Unos(polaganje._polaznik, polaganje._kurs, polaganje._datumPolaganja, polaganje._ostvareniUspjeh);

		delete[] _polaganja;
		_polaganja = t;
		
		_trenutnoPolaganja++;
	}

	//nije moguce dodati polaganje onog kursa koji nije evidentiran (registrovan, ponudjen) u tom edukacijskom centru
	//moguce je polagati samo aktivne kurseve i to najkasnije 5 dana od dana zavrsetka (kraja) posljednje dodanog (aktivnog) kursa iste 
	//vrste (npr. MasteringSQL)
	//jedan polaznik moze vise puta polagati isti kurs, ali novi procenat mora biti veci od prethodno dodanog (na istom kursu)
	bool DodajPolaganje(Polaganja &polaganje)
	{
		auto PostojanjeKursa = [&]()
		{
			for (int i = _trenutnoKurseva - 1; i >= 0; i--)
				if (polaganje._kurs._kurs == _kursevi[i]->_kurs && _kursevi[i]->_aktivan)
					if(_kursevi[i]->_kraj.Dani() + 5 > polaganje._datumPolaganja.Dani())
						return true;
			return false;
		};
		if (!PostojanjeKursa())
			return false;

		for (int i = 0; i < _trenutnoPolaganja; i++)
			if (_polaganja[i]._kurs._kurs == polaganje._kurs._kurs && _polaganja[i]._ostvareniUspjeh > polaganje._ostvareniUspjeh)
				return false;

		ProsiriPolaganje(polaganje);
		return true;
	}

	//jedan edukacijski centar moze nuditi vise istih kurseva (npr. MasteringSQL), ali se oni moraju realizovati u razlictim periodima 
	//(pocetak-kraj) tj. ne smiju se preklapati novi kurs istog tipa (npr. MasteringSQL) se mora realizovati nakon prethodno dodanog npr. 
	//ako prethodni kurs MasteringSQL traje 01.03.2016 - 01.04.2016, onda se novi kurs MasteringSQL moze dodati jedino ako pocinje nakon 
	//01.04.2016
	bool DodajKurs(Kurs &kurs)
	{
		for (int i = _trenutnoKurseva - 1; i >= 0; i--)
			if (_kursevi[i]->_kraj.Dani() >= kurs._pocetak.Dani())
				return false;
			else
				break;

		ProsiriKurs(kurs);
	}

	void ProsiriKurs(Kurs &kurs)
	{
		_kursevi[_trenutnoKurseva] = new Kurs;
		_kursevi[_trenutnoKurseva]->Unos(kurs._kurs, kurs._pocetak, kurs._kraj, kurs._imePredavaca);
		_trenutnoKurseva++;
	}

	void Unos(const char *nazivCentra)
	{
		int vel = strlen(nazivCentra) + 1;
		_nazivCentra = new char[vel];
		strcpy_s(_nazivCentra, vel, nazivCentra);

		_trenutnoKurseva = 0;
		_trenutnoPolaganja = 0;
		_polaganja = nullptr;
	}


};

//PretragaRekurzivno - rekurzivna funkcija pronalazi prosjecni uspjeh koji su polaznici tokom godine (npr.2016) ostvarili na odredjenom 
//kursu (npr. MasteringSQL)
float PretragaRekurzivno(SkillsCentar &sc, int godina, enumKursevi kurs, int brojac = 0, int uspjesni = 0, int suma = 0)
{
	if (sc._trenutnoPolaganja == brojac)
		if (uspjesni == 0)
			return uspjesni;
		else
			return suma / (float)uspjesni;
	else if (*sc._polaganja[brojac]._datumPolaganja._godina == godina && sc._polaganja[brojac]._kurs._kurs == kurs)
	{
		uspjesni++;
		suma += sc._polaganja[brojac]._ostvareniUspjeh;
	}
	return PretragaRekurzivno(sc, godina, kurs, brojac+1, uspjesni, suma);
}
void main() {

	Datum datum1, datum2, datum3, datum4, datum5, datum6;
	datum1.Unos(26, 11, 2015);
	datum2.Unos(29, 5, 2016);
	datum3.Unos(5, 6, 2016);
	datum4.Unos(15, 8, 2016);
	datum5.Unos(13, 7, 2016);
	datum6.Unos(22, 9, 2016);

	Kurs softverski, html, sql, security;
	softverski.Unos(SoftwareEngeneeringFundamentals, datum1, datum2, "Emina Junuz");
	html.Unos(HtmlCSSJavaScript, datum2, datum3, "Larisa Tipura");
	sql.Unos(MasteringSQL, datum3, datum4, "Jasmin Azemovic");
	security.Unos(WindowsSecurity, datum3, datum4, "Adel Handzic");

	Polaznik denis, zanin, indira;
	denis.Unos(1, "Denis Music");
	zanin.Unos(2, "Zanin Vejzovic");
	indira.Unos(3, "Indira Hamulic");

	Polaganja denisHtml, zaninHtml, indiraSql;
	denisHtml.Unos(denis, html, datum5, 61);
	//61 predstavlja ostvareni uspjeh/procenat, a uspjesno polozenim se smatra svaki kurs na kome je polaznik ostvari vise od 55%
	zaninHtml.Unos(zanin, html, datum6, 93);

	SkillsCentar mostar;
	mostar.Unos("Skills Center Mostar");
	//jedan edukacijski centar moze nuditi vise istih kurseva (npr. MasteringSQL), ali se oni moraju realizovati u razlictim periodima (pocetak-kraj) tj. ne smiju se preklapati
	//novi kurs istog tipa (npr. MasteringSQL) se mora realizovati nakon prethodno dodanog npr. ako prethodni kurs MasteringSQL traje 01.03.2016 - 01.04.2016, onda se novi kurs MasteringSQL moze dodati jedino ako pocinje nakon 01.04.2016
	if (mostar.DodajKurs(softverski)) {
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		softverski.Ispis();
		cout << crt;
	}
	//nije moguce dodati polaganje onog kursa koji nije evidentiran (registrovan, ponudjen) u tom edukacijskom centru
	//moguce je polagati samo aktivne kurseve i to najkasnije 5 dana od dana zavrsetka (kraja) posljednje dodanog (aktivnog) kursa iste vrste (npr. MasteringSQL)
	//jedan polaznik moze vise puta polagati isti kurs, ali novi procenat mora biti veci od prethodno dodanog (na istom kursu)
	if (mostar.DodajPolaganje(denisHtml)) {
		cout << crt << "POLAGANJE EVIDENTIRANO" << crt;
		denisHtml.Ispis();
	}
	Datum OD, DO;
	int brojPolaganja = 0;
	OD.Unos(1, 6, 2016);
	DO.Unos(1, 8, 2016);
	//PolaganjaByDatum – vraca niz polaganja koja su uspjesno realizovana u periodu OD-DO
	Polaganja * polaganjaByDatum = mostar.PolaganjaByDatum(OD, DO, brojPolaganja);
	cout << "U periodu od ";
	OD.Ispis();
	cout << " do ";
	DO.Ispis();
	cout << " uspjesno je realizovano " << brojPolaganja << " polaganja--->>>";
	for (size_t i = 0; i < brojPolaganja; i++)
		polaganjaByDatum[i].Ispis();

	cout << "\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-\n";

	//PretragaRekurzivno - rekurzivna funkcija pronalazi prosjecni uspjeh koji su polaznici tokom godine (npr.2016) ostvarili na odredjenom kursu (npr. MasteringSQL)
	cout << "Prosjecan uspjeh na kursu MasteringSQL u 2016 godini je " << PretragaRekurzivno(mostar,2016,MasteringSQL) << crt;
	//Ispis - ispisuje sve informacije o edukacijskom centru. prije ispisa sortirati sve kurseve na osnovu pocetka odrzavanja kursa
	mostar.Ispis();

	for (int i = 0; i < brojPolaganja; i++)
		polaganjaByDatum[i].Dealociraj();
	delete[] polaganjaByDatum;
	polaganjaByDatum = nullptr;


	//izvrsiti potrebne dealokacije
	
	denisHtml.Dealociraj(); zaninHtml.Dealociraj(); 
	mostar.Dealociraj();
	OD.Dealociraj(); DO.Dealociraj();
	denis.Dealociraj(); zanin.Dealociraj(); indira.Dealociraj();
	datum1.Dealociraj(); datum2.Dealociraj(); datum3.Dealociraj(); datum4.Dealociraj(); datum5.Dealociraj(); datum6.Dealociraj();
	softverski.Dealociraj(); html.Dealociraj(); sql.Dealociraj(); security.Dealociraj();

	//indiraSql.Dealociraj();

	system("pause>0");
}
