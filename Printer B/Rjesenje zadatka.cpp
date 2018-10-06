/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA, U SUPROTNOM SE RAD NECE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VASIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJESENJA VASIH ZADATAKA. NE PREDAVATI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORISTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VASA RJESENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URADJENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
#include<iostream>
#include<memory>
#include<tuple>
using namespace std;

//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

const char *crt = "\n-------------------------------------------\n";

enum vrstaDokumenta { PDF, DOC, TXT, HTML };
const char * vrstaDokumentaChar[] = { "PDF", "DOC", "TXT", "HTML" };

enum vrstaAktivnosti { KREIRANJE, DODAVANJE, ZAMJENA, PRINTANJE };
const char * vrstaAktivnostiChar[] = { "KREIRANJE", "DODAVANJE", "ZAMJENA", "PRINTANJE" };

const int BROJ_ZNAKOVA_PO_STRANICI = 30;

struct DatumVrijeme {
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
	void Unos(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 1, int minuti = 1) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}



	int Dani()
	{
		return *_dan + *_mjesec*31 + *_godina*31*12;
	}

	bool izmedjuDatuma(DatumVrijeme &OD, DatumVrijeme &DO)
	{
		if (OD.Dani() < Dani() && Dani() < DO.Dani())
			return true;
		else if (OD.Dani() == Dani() && Dani() == DO.Dani())
		{
			if (*OD._sati < *_sati && *_sati < *DO._sati)
				return true;
			else if (*OD._sati == *_sati && *_sati == *DO._sati)
			{
				if (*OD._minuti <= *_minuti && *_minuti <= *DO._minuti)
					return true;
			}
		}
		return false;
	}


	void Dealociraj() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}
	void Ispis() {
		cout << *_dan << "." << *_mjesec << "." << *_godina << " " << *_sati << ":" << *_minuti << endl;
	}
};

char * AlocirajNizKaraktera(const char * sadrzaj) {
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

struct Modifikacija {
	char * _korisnicnkoIme;
	vrstaAktivnosti _aktivnost;
	DatumVrijeme _vrijemeModifikacije;
	void Unos(const char * sadrzaj, vrstaAktivnosti aktivnost, DatumVrijeme vm) {
		_korisnicnkoIme = AlocirajNizKaraktera(sadrzaj);
		_aktivnost = aktivnost;
		_vrijemeModifikacije.Unos(*vm._dan, *vm._mjesec, *vm._godina, *vm._dan, *vm._minuti);
	}
	void Dealociraj() {
		delete[] _korisnicnkoIme; _korisnicnkoIme = nullptr;
		_vrijemeModifikacije.Dealociraj();
	}
	void Ispis() { cout << _korisnicnkoIme << " (" << vrstaAktivnostiChar[_aktivnost] << ")"; _vrijemeModifikacije.Ispis(); }
};

struct Dokument {
	unique_ptr<vrstaDokumenta> _vrsta;
	char * _naziv;
	char * _sadrzaj;
	Modifikacija * _modifikacije;
	int _brojModifikacija;

	void prosiriModifikacije(const char* sadrzaj, vrstaAktivnosti vrsta, DatumVrijeme vrijemeModifikacije)
	{
		Modifikacija * t = new Modifikacija[_brojModifikacija + 1];

		for (int i = 0; i < _brojModifikacija; i++)
		{
			t[i].Unos(_modifikacije[i]._korisnicnkoIme, _modifikacije[i]._aktivnost, _modifikacije[i]._vrijemeModifikacije);
			_modifikacije[i].Dealociraj();

		}
		delete[] _modifikacije;
		t[_brojModifikacija].Unos(sadrzaj, vrsta, vrijemeModifikacije);
		_modifikacije = t;
		_brojModifikacija++;
	}

	/*DODAJE SADRZAJ U FAJL, ZADRZAVAJUCI POSTOJECI. SVAKO DODAVANJE EVIDENTIRATI KAO MODIFIKACIJU. 
	PARAMETRI:SADRZAJ, KORISNICKO IME, VRIJEME MODIFIKACIJE*/
	void DodajSadrzaj(const char* sadrzaj, const char* imePrezime, DatumVrijeme vrijemeModifikacije)
	{
		if (_sadrzaj == nullptr)
		{
			_sadrzaj = AlocirajNizKaraktera(sadrzaj);

		}
		else
		{
			int vel = strlen(_sadrzaj) + strlen(sadrzaj) + 1;
			char *temp = new char[vel];
			strcpy_s(temp, vel, _sadrzaj);
			strcat_s(temp, vel, sadrzaj);
			delete[] _sadrzaj;
			_sadrzaj = temp;
		}
		
		prosiriModifikacije(imePrezime, DODAVANJE, vrijemeModifikacije);
	}






	//vrijeme kreiranja dokumenta je ujedno i vrijeme posljednje modifikacije
	void Unos(vrstaDokumenta vrsta, const char * naziv, DatumVrijeme kreiran) {
		_naziv = AlocirajNizKaraktera(naziv);
		_vrsta = make_unique<vrstaDokumenta>(vrsta);
		_brojModifikacija = 0;
		_modifikacije = new Modifikacija[_brojModifikacija + 1];
		_modifikacije->Unos("", KREIRANJE, kreiran);
		_brojModifikacija++;
		_sadrzaj = nullptr;
	}
	void Dealociraj() {
		delete[] _naziv; _naziv = nullptr;
		delete[] _sadrzaj; _sadrzaj = nullptr;
		for (size_t i = 0; i < _brojModifikacija; i++)
			_modifikacije[i].Dealociraj();
		delete[] _modifikacije;
		_modifikacije = nullptr;
	}
	void Ispis() {
		cout << crt << _naziv << " " << vrstaDokumentaChar[*_vrsta] << " kreiran ";
		cout << crt << _sadrzaj << crt;
		for (size_t i = 0; i < _brojModifikacija; i++)
			_modifikacije[i].Ispis();
	}
};
struct Printer {
	char * _model;
	Dokument * _dokumenti;
	int _trenutnoDokumenata;

	//FUNKCIJA GetModifikacijeByDatum VRACA SVE MODIFIKACIJE KOJE SU NAPRAVLJENE NA DOKUMENTIMA U PERIODU OD - DO
	//(PROSLIJEDJENOM KAO PARAMETAR)
	pair<Modifikacija *, int> GetModifikacijeByDatum(DatumVrijeme &OD, DatumVrijeme &DO)
	{
		int brojac = 0;
		for (int i = 0; i < _trenutnoDokumenata; i++)
		{
			for (int j = 0; j < _dokumenti[i]._brojModifikacija; j++)
			{
				if (_dokumenti[i]._modifikacije[j]._vrijemeModifikacije.izmedjuDatuma(OD, DO))
					brojac++;
			}
		}

		if (brojac == 0)
			return make_pair(nullptr, 0);


		int index = 0;
		Modifikacija *t = new Modifikacija[brojac];
		for (int i = 0; i < _trenutnoDokumenata; i++)
		{
			for (int j = 0; j < _dokumenti[i]._brojModifikacija; j++)
			{
				if (_dokumenti[i]._modifikacije[j]._vrijemeModifikacije.izmedjuDatuma(OD, DO))
					t[index++].Unos(_dokumenti[i]._modifikacije[j]._korisnicnkoIme, _dokumenti[i]._modifikacije[j]._aktivnost, _dokumenti[i]._modifikacije[j]._vrijemeModifikacije);
			}
		}
		return make_pair(t, brojac);

	}


	//REKURZIVNA FUNKCIJA VRACA PROSJECAN BROJ MODIFIKACIJA ODREDJENE VRSTE KOJE SU VRSENE NAD PRINTANIM DOKUMENTIMA
	float GetProsjecanBrojModifikacijaByVrsta(vrstaAktivnosti vrsta, int brojac = 0, int uspjesni = 0, int ukupni = 0)
	{
		if (brojac == _trenutnoDokumenata)
		{
			if (uspjesni == 0)
				return 0;
			return uspjesni / ukupni;
		}

		bool printan = false;
		int brojacUspjesnih = 0;

		for (int i = 0; i < _dokumenti[brojac]._brojModifikacija; i++)
		{
			if (_dokumenti[brojac]._modifikacije[i]._aktivnost == PRINTANJE)
				printan = true;
			if (_dokumenti[brojac]._modifikacije[i]._aktivnost == vrsta)
				brojacUspjesnih++;
		}

		if (printan)
			return GetProsjecanBrojModifikacijaByVrsta(vrsta, brojac + 1, uspjesni + brojacUspjesnih, ukupni + _dokumenti[brojac]._brojModifikacija);
		return GetProsjecanBrojModifikacijaByVrsta(vrsta, brojac + 1, uspjesni, ukupni);
	}





	void prosiriDokumente(Dokument &dokument)
	{
		Dokument * t = new Dokument[_trenutnoDokumenata+1];

		for (int i = 0; i < _trenutnoDokumenata; i++)
		{
			t[i].Unos(*_dokumenti[i]._vrsta, _dokumenti[i]._naziv, _dokumenti[i]._modifikacije[0]._vrijemeModifikacije);
			t[i]._modifikacije = _dokumenti[i]._modifikacije;
			
			_dokumenti[i]._modifikacije = nullptr;
			//for (int j = 1; j < _dokumenti[i]._brojModifikacija; j++)
			//{
			//	t[i].prosiriModifikacije(_dokumenti[i]._modifikacije[j]._korisnicnkoIme, _dokumenti[i]._modifikacije[j]._aktivnost, _dokumenti[i]._modifikacije[j]._vrijemeModifikacije);
			//	_dokumenti[i]._modifikacije[j].Dealociraj();
			//}
			//delete[] _dokumenti[i]._modifikacije;
			//_dokumenti[i]._modifikacije = nullptr;
		}

		t[_trenutnoDokumenata].Unos(*dokument._vrsta, dokument._naziv, dokument._modifikacije[0]._vrijemeModifikacije);
		
		for (int j = 1; j < dokument._brojModifikacija; j++)
			t[_trenutnoDokumenata].prosiriModifikacije(dokument._modifikacije[j]._korisnicnkoIme, dokument._modifikacije[j]._aktivnost, dokument._modifikacije[j]._vrijemeModifikacije);

		_trenutnoDokumenata++;
	}


	/*
	DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDECA PRAVILA:
	
	*/
	bool usloviPrintanja(Dokument &dokument)
	{
		//1. NAZIV DOKUMENTA MOZE SADRZAVATI SAMO SLOVA, A EKSTENZIJA MORA BITI IDENTICNA ONOJ DEFINISANOJ VRIJEDNOSCU ATRIBUTA vrstaDokumenta
		int vel = strlen(dokument._naziv);
		for (int i = 0; i < vel; i++)
		{
			if (toupper(dokument._naziv[i]) < 'A' || toupper(dokument._naziv[i]) > 'Z')
				if (dokument._naziv[i] != '.')
					return false;
		}

		if (*dokument._vrsta < 0 || *dokument._vrsta > 3)
			return false;
		//2. NAD DOKUMENTOM MORAJU BITI IZVRSENE SLJEDECE MODIFIKACIJE :
		//	-1 KREIRANJE
		//	- 1 ILI VI�E DODAVANJA
		//	- 1 ILI VI�E ZAMJENA SADRZAJA
		int suma[4] = { 0 };
		for (int i = 0; i < dokument._brojModifikacija; i++)
		{
			suma[dokument._modifikacije[i]._aktivnost]++;
		}
		
		
		if (suma[0] != 1)
			return false;
		else if (suma[1] < 1)
			return false;
		if (suma[2] < 1)
			return false;

		return true;
	}




	/*
	SAMO DOKUMENTI KOJI ZADOVOLJE USLOVE PRINTANJA TREBAJU BITI SACUVANI U NIZ _dokumenti. 
	
	NA KONZOLU ISPISATI SADRZAJ DOKUMENTA KOJI SE PRINTA, A SA CRT LINIJOM ODVAJATI STRANICE DOKUMENTA.
	BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREDJUJE PRILIKOM  PRINTANJA, 
	
	A ZAVISI OD VRIJEDNOSTI BROJ_ZNAKOVA_PO_STRANICI UKLJUCUJUCI RAZMAKE I DRUGE VRSTE ZNAKOVA.

	ONEMOGUCITI PRINTANJE DOKUMENTA OSOBAMA KOJE NISU NAPRAVILE NAJMANJE JEDNU MODIFIKACIJU NA TOM DOKUMENTU. 
	NAKON PRINTANJA SADRZAJA DOKUMENTA, POTREBNO JE DOKUMENTU DODATI I MODIFIKACIJU "PRINTANJE".
	*/
	bool Printaj(Dokument &dokument, const char* imePrezime, DatumVrijeme datum)
	{
		if (usloviPrintanja(dokument) != true)
			return false;
		
		//LAMBDA
		auto provjeraOsobe = [&]()
		{
			for (int i = 0; i < dokument._brojModifikacija; i++)
			{
				if (strcmp(dokument._modifikacije[i]._korisnicnkoIme, imePrezime) == 0)
					return true;
			}
			return false;
		};

		if (provjeraOsobe() != true)
			return false;
		
		int vel = strlen(dokument._sadrzaj);
		for (int i = 0; i < vel; i++)
		{
			if (i%BROJ_ZNAKOVA_PO_STRANICI == 0)
				cout << crt;
			cout << dokument._sadrzaj[i];
		}

		prosiriDokumente(dokument);
		dokument.prosiriModifikacije(imePrezime, PRINTANJE, datum);
	}







	void Unos(const char * model) {
		_model = AlocirajNizKaraktera(model);
		_dokumenti = nullptr;
		_trenutnoDokumenata = 0;
	}
	void Dealociraj() {
		delete[] _model; _model = nullptr;
		for (size_t i = 0; i < _trenutnoDokumenata; i++)
			_dokumenti[i].Dealociraj();
		delete[]_dokumenti; _dokumenti = nullptr;
	}
	void Ispis() {
		cout << _model << crt;
		for (size_t i = 0; i < _trenutnoDokumenata; i++)
			_dokumenti[i].Ispis();
	}
};


int main() {
	DatumVrijeme prije3Dana; prije3Dana.Unos(3, 2, 2018, 10, 15);
	DatumVrijeme danas1; danas1.Unos(6, 2, 2018, 10, 15);//U KONTEKSTU MODIFIKACIJE, UNOS PODRAZUMIJEVA KREIRANJE
	DatumVrijeme danas2; danas2.Unos(6, 2, 2018, 10, 16);
	DatumVrijeme za10Dana; za10Dana.Unos(16, 2, 2018, 10, 15);

	Dokument ispitPRII, ispitMAT, ispitUIT, ispitUITDrugiRok;
	ispitPRII.Unos(DOC, "ispitPRII.doc", prije3Dana);
	ispitMAT.Unos(DOC, "ispitMAT.doc", danas1);
	ispitUIT.Unos(DOC, "ispitUIT.doc", danas2);
	ispitUITDrugiRok.Unos(PDF, "ispitUITDrugiRok.pdf", za10Dana);

	/*DODAJE SADRZAJ U FAJL, ZADRZAVAJUCI POSTOJECI. SVAKO DODAVANJE EVIDENTIRATI KAO MODIFIKACIJU. PARAMETRI:SADRZAJ, KORISNICKO IME, VRIJEME MODIFIKACIJE*/
	ispitPRII.DodajSadrzaj("Programiranje ili racunarsko programiranje (engl. programming) jeste vjestina pomocu koje ", "denis.music", danas1);
	ispitPRII.DodajSadrzaj("korisnik stvara i izvrsava algoritme koristeci odredjene programske jezike da bi ... ", "indira.hamulic", danas2);

	ispitPRII.Ispis();//ISPISUJE SVE PODATKE O DOKUMENTU

	ispitMAT.DodajSadrzaj("Matematika se razvila iz potrebe da se obavljaju proracuni u trgovini, vrse mjerenja zemljista i predvidjaju ", "jasmin.azemovic", danas1);
	ispitMAT.DodajSadrzaj("astronomski dogadjaji, i ove tri primjene se mogu dovesti u vezu sa grubom podjelom matematike ", "adel.handzic", danas2);

	Printer hp3200; hp3200.Unos("HP 3200");
	/*
	DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDECA PRAVILA:
	1. NAZIV DOKUMENTA MOZE SADRZAVATI SAMO SLOVA, A EKSTENZIJA MORA BITI IDENTICNA ONOJ DEFINISANOJ VRIJEDNOSCU ATRIBUTA vrstaDokumenta
	2. NAD DOKUMENTOM MORAJU BITI IZVRSENE SLJEDECE MODIFIKACIJE:
	- 1 KREIRANJE
	- 1 ILI VI�E DODAVANJA
	- 1 ILI VI�E ZAMJENA SADRZAJA
	SAMO DOKUMENTI KOJI ZADOVOLJE USLOVE PRINTANJA TREBAJU BITI SACUVANI U NIZ _dokumenti. NA KONZOLU ISPISATI SADRZAJ DOKUMENTA KOJI SE PRINTA, A SA CRT LINIJOM ODVAJATI STRANICE DOKUMENTA.
	BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREDJUJE PRILIKOM  PRINTANJA, A ZAVISI OD VRIJEDNOSTI BROJ_ZNAKOVA_PO_STRANICI UKLJUCUJUCI RAZMAKE I DRUGE VRSTE ZNAKOVA.
	ONEMOGUCITI PRINTANJE DOKUMENTA OSOBAMA KOJE NISU NAPRAVILE NAJMANJE JEDNU MODIFIKACIJU NA TOM DOKUMENTU. NAKON PRINTANJA SADRZAJA DOKUMENTA, POTREBNO JE DOKUMENTU DODATI I MODIFIKACIJU "PRINTANJE".
	*/
	if (hp3200.Printaj(ispitPRII, "denis.music", za10Dana))
		cout << "Printam -> " << ispitPRII._naziv << endl;
	if (hp3200.Printaj(ispitMAT, "denis.music", za10Dana))
		cout << "Printam -> " << ispitMAT._naziv << endl;
	if (hp3200.Printaj(ispitUIT, "iris.memic", za10Dana))
		cout << "Printam -> " << ispitUIT._naziv << endl;
	if (hp3200.Printaj(ispitUITDrugiRok, "goran.skondric", za10Dana))
		cout << "Printam -> " << ispitUITDrugiRok._naziv << endl;

	//REKURZIVNA FUNKCIJA VRACA PROSJECAN BROJ MODIFIKACIJA ODREDJENE VRSTE KOJE SU VRSENE NAD PRINTANIM DOKUMENTIMA
	cout << "Prosjecan broj modifikacija odrejdne vrste printanih dokumenata je -> " << hp3200.GetProsjecanBrojModifikacijaByVrsta(DODAVANJE/**/) << crt;

	Modifikacija * pok = nullptr;
	int brojac = 0;
	//FUNKCIJA GetModifikacijeByDatum VRACA SVE MODIFIKACIJE KOJE SU NAPRAVLJENE NA DOKUMENTIMA U PERIODU OD - DO (PROSLIJEDJENOM KAO PARAMETAR)
	tie(pok, brojac) = hp3200.GetModifikacijeByDatum(danas1, za10Dana);
	if (brojac > 0 && pok != nullptr)
		for (size_t i = 0; i < brojac; i++)
			pok[i].Ispis();

	//ISPISUJE INFORMACIJE O SVIM DOKUMENTIMA KOJI SU ISPRINTANI
	hp3200.Ispis();

	/*NAPISATI LAMBDA FUNKCIJU ZamijeniIPrintaj KOJA U SADRZAJU SVIH PRINTANIH DOKUMENATA MIJENJA PRVI POSLATI PARAMETAR SA VRIJEDNOSCU DRUGOG PARAMETRA, TE VRACA BROJ ZAMIJENJENIH ZNAKOVA*/
	//RAZMAK MIJENJA ZNAKOM CRTICA U SADRZAJU DOKUMENATA KOJI SU PRINTANI danas1. ZAMJENU REGISTROVATI KAO MODIFIKACIJU

	auto Zamijeni = [&hp3200](const char uslov, const char znak, const char* imePrezime, DatumVrijeme datum)
	{
		int brojac = 0;

		auto daLiJePrintan = [&](Dokument &dokument)
		{
			for (int i = dokument._brojModifikacija - 1; i >= 0; i--)
			{
				if (dokument._modifikacije[i]._aktivnost == PRINTANJE && datum.Dani() == dokument._modifikacije[i]._vrijemeModifikacije.Dani())
					return true;
			}
			return false;
		};

		for (int i = 0; i < hp3200._trenutnoDokumenata; i++)
		{
			if (daLiJePrintan(hp3200._dokumenti[i]))
			{
				int vel = strlen(hp3200._dokumenti[i]._sadrzaj);
				for (int j = 0; j < vel; j++)
				{
					if (hp3200._dokumenti[i]._sadrzaj[j] == ' ')
					{
						hp3200._dokumenti[i]._sadrzaj[j] = znak;
						brojac++;
					}
				}
			}
		}

		return brojac;
	};



	int zamijenjeno = Zamijeni(' ', '-', "denis.music", danas1);
	cout << "Zamijenjeno -> " << zamijenjeno << " znakova" << endl;
	hp3200.Ispis();

	prije3Dana.Dealociraj(); danas1.Dealociraj(); danas2.Dealociraj(); za10Dana.Dealociraj();
	ispitMAT.Dealociraj(); ispitPRII.Dealociraj(); ispitUIT.Dealociraj(); ispitUITDrugiRok.Dealociraj();

	hp3200.Dealociraj();

	system("PAUSE");
	return 0;
}
