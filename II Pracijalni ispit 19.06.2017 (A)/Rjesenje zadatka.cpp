#include <iostream>
#include <memory>
#include <tuple>
using namespace std;
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEĆE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ĆETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

const char *crt = "\n-------------------------------------------\n";

enum eNacinStudiranja { REDOVAN, DL };
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };



struct DatumVrijeme {
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
	char * DatumKaoNiz = nullptr;

	int Dani()
	{
		return *_dan + *_mjesec*30+*_godina*356;
	}


	//9.6.2017 10:15
	char *GetDatumKaoNizKaraktera()
	{
		delete[] DatumKaoNiz;
		DatumKaoNiz = nullptr;

		char dan[3], mjesec[3], godina[5], sati[3], minute[3];
		_itoa_s(*_dan, dan, 10);
		_itoa_s(*_mjesec, mjesec, 10);
		_itoa_s(*_godina, godina, 10);
		_itoa_s(*_sati, sati, 10);
		_itoa_s(*_minuti, minute, 10);

		int vel = 0;
		vel += strlen(dan);
		vel += strlen(mjesec);
		vel += strlen(godina);
		vel += strlen(sati);
		vel += strlen(minute);
		vel += 5;


		DatumKaoNiz = new char[vel];

		strcpy_s(DatumKaoNiz, vel, dan);
		strcat_s(DatumKaoNiz, vel, ".");
		strcat_s(DatumKaoNiz, vel, mjesec);
		strcat_s(DatumKaoNiz, vel, ".");
		strcat_s(DatumKaoNiz, vel, godina);
		strcat_s(DatumKaoNiz, vel, " ");
		strcat_s(DatumKaoNiz, vel, sati);
		strcat_s(DatumKaoNiz, vel, ":");
		strcat_s(DatumKaoNiz, vel, minute);


		return DatumKaoNiz;
	}


	void Unos(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	void Dealociraj() {
		delete[] DatumKaoNiz;
		DatumKaoNiz = nullptr;
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

DatumVrijeme rokZaPrijavu = { new int(5), new int(7), new int(2017), new int(12), new int(30) };


struct Predmet {
	char * _naziv;
	int _ocjena;
	DatumVrijeme * _datumUnosa;

	
	void Unos(const char * naziv, int ocjena, DatumVrijeme datumUnosa) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_ocjena = ocjena;
		_datumUnosa = new DatumVrijeme;
		_datumUnosa->Unos(*datumUnosa._dan, *datumUnosa._mjesec, *datumUnosa._godina, *datumUnosa._sati, *datumUnosa._minuti);
	}
	void Dealociraj() {
		delete[] _naziv; _naziv = nullptr;
		_datumUnosa->Dealociraj(); delete _datumUnosa;
	}

	void Ispis() {
		//kreirati funkciju GetDatumKaoNizKaraktera() koja vraca vrijednosti atributa strukture datum kao niz karaktera
		cout << _naziv << " (" << _ocjena << ") " << _datumUnosa->GetDatumKaoNizKaraktera() << endl;
	}
};

struct Uspjeh {
	eRazred _razred;
	Predmet * _predmeti;
	int _brojPredmeta;

	void DodajPredmet(const Predmet predmet)
	{
		Predmet * t = new Predmet[_brojPredmeta + 1];
		for (int i = 0; i < _brojPredmeta; i++)
		{
			t[i].Unos(_predmeti[i]._naziv, _predmeti[i]._ocjena, *_predmeti[i]._datumUnosa);
			_predmeti[i].Dealociraj();
		}
		delete[] _predmeti;
		_predmeti = t;
		t[_brojPredmeta].Unos(predmet._naziv, predmet._ocjena, *predmet._datumUnosa);
		_brojPredmeta++;
	}

	void Unos(eRazred razred) {
		_razred = razred;
		_predmeti = nullptr;
		_brojPredmeta = 0;
	}
	void Dealociraj() {
		for (size_t i = 0; i < _brojPredmeta; i++)
			_predmeti[i].Dealociraj();
		delete[] _predmeti; _predmeti = nullptr;
	}

	void Ispis() {
		cout << crt << "Razred -> " << _razred << crt;
		for (size_t i = 0; i < _brojPredmeta; i++)
			_predmeti[i].Ispis();
	}
};

struct Kandidat {
	eNacinStudiranja _nacinStudiranja;
	char * _imePrezime;
	shared_ptr<Uspjeh> _uspjeh[4];


	/*
	uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
	prilikom dodavanja onemoguciti:
	- dodavanje predmeta za razrede koji nisu definisani enumeracijom,jesam
	- dodavanje istoimenih predmeta na nivou jednog razreda,
	- dodavanje predmeta nakon dozvoljenog roka za prijavu (rokZaPrijavu).
	razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred,
	pa onda za I razred i sl.). Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	//ne bi trebao dodati jer je prosao postavljeni rok za dodavanje predmeta
	bool DodajPredmet(eRazred razred, const Predmet &predmet)
	{
		if (razred < 1 || razred > 4)
			return false;

		if (rokZaPrijavu.Dani() < predmet._datumUnosa->Dani())
			return false;
		else if (rokZaPrijavu.Dani() == predmet._datumUnosa->Dani())
		{
			if (rokZaPrijavu._sati < predmet._datumUnosa->_sati)
				return false;
			else if (rokZaPrijavu._sati == predmet._datumUnosa->_sati)
				if (rokZaPrijavu._minuti < predmet._datumUnosa->_minuti)
					return false;
		}

		razred = eRazred(razred - 1);
		if (_uspjeh[razred] != nullptr)
		{
			for (int i = 0; i < _uspjeh[razred]->_brojPredmeta; i++)
			{
				if (strcmp(_uspjeh[razred]->_predmeti[i]._naziv, predmet._naziv) == 0)
					return false;
			}
		}
		else
		{
			_uspjeh[razred] = make_shared<Uspjeh>();
			_uspjeh[razred]->Unos(razred);
		}
		_uspjeh[razred]->DodajPredmet(predmet);
		return true;
	}

	void Unos(eNacinStudiranja nacinStudiranja, const char * imePrezime) {
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);
		_nacinStudiranja = nacinStudiranja;
		for (size_t i = 0; i < 4; i++)
			_uspjeh[i] = nullptr;
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < 4; i++)
			if (_uspjeh[i] != nullptr)
				_uspjeh[i]->Dealociraj();
	}
	void Ispis() {
		cout << crt << _imePrezime << " " << _nacinStudiranja;
		for (size_t i = 0; i < 4; i++)
			if(_uspjeh[i] != nullptr)
			_uspjeh[i]->Ispis();
	}
};

/* napisati rekurzivnu funkciju koja ce vratiti pokazivac na kandidata sa najvecom ocjenom na predmetu koji je proslijedjen
kao parametar. ukoliko je vise kandidata ostvarilo istu ocjenu, funkcija treba da vrati onog kandidata koji je prvi
evidentirao tu ocjenu (ako je isto vrijeme evidentiranja, onda funkcija vraca kandidata koji je prvi u nizu).
U slucaju da niti jedan kandidat nije evidentirao trazeni predmet funkcija vraca nullptr. u nastavku je prikazan primjer
poziva rekurzivne funkcije, a ostale parametre dodajte po potrebi. */
Kandidat * rekNajboljaOcjena(
	
	Kandidat *prijave2017, int brojKandidata, const char* naziv, int ocjenaNajveceg = 0, 
	Kandidat *najveci = nullptr, int brojacPredmeta = 0,int brojacUspjeha = 0

)
{
	brojKandidata--;
	if (brojKandidata < 0)
	{
		return najveci;
	}
	else if (prijave2017[brojKandidata]._uspjeh[brojacUspjeha] == nullptr)
	{
		return rekNajboljaOcjena(prijave2017, brojKandidata, naziv, prijave2017[brojKandidata]._uspjeh[brojacUspjeha]->_predmeti[brojacPredmeta]._ocjena, &prijave2017[brojKandidata], 0, brojacUspjeha+1);
	}
	else if (brojacPredmeta == prijave2017[brojKandidata]._uspjeh[brojacUspjeha]->_brojPredmeta)
	{
		return rekNajboljaOcjena(prijave2017, brojKandidata, naziv, prijave2017[brojKandidata]._uspjeh[brojacUspjeha]->_predmeti[brojacPredmeta]._ocjena, &prijave2017[brojKandidata], 0, brojacUspjeha+1);
	}
	else if (brojacUspjeha == 4)
	{
		return rekNajboljaOcjena(prijave2017, brojKandidata, naziv, prijave2017[brojKandidata]._uspjeh[brojacUspjeha]->_predmeti[brojacPredmeta]._ocjena, &prijave2017[brojKandidata], 0, 0);
	}
	else if (strcmp(prijave2017[brojKandidata]._uspjeh[brojacUspjeha]->_predmeti[brojacPredmeta]._naziv, naziv) == 0)
	{
		if (ocjenaNajveceg < prijave2017[brojKandidata]._uspjeh[brojacUspjeha]->_predmeti[brojacPredmeta]._ocjena)
			return rekNajboljaOcjena(prijave2017, brojKandidata, naziv, prijave2017[brojKandidata]._uspjeh[brojacUspjeha]->_predmeti[brojacPredmeta]._ocjena, &prijave2017[brojKandidata], brojacPredmeta + 1, brojacUspjeha);
		else
			return rekNajboljaOcjena(prijave2017, brojKandidata, naziv, ocjenaNajveceg, najveci, brojacPredmeta + 1, brojacUspjeha);
	}
	else
		return rekNajboljaOcjena(prijave2017, brojKandidata, naziv, ocjenaNajveceg, najveci, brojacPredmeta + 1, brojacUspjeha);
}



void main()
{
	DatumVrijeme datum19062017_1015, datum20062017_1115, datum30062017_1215, datum05072017_1231;
	datum19062017_1015.Unos(19, 6, 2017, 10, 15);
	datum20062017_1115.Unos(20, 6, 2017, 11, 15);
	datum30062017_1215.Unos(30, 6, 2017, 12, 15);
	datum05072017_1231.Unos(5, 7, 2017, 12, 31);

	cout << datum19062017_1015.GetDatumKaoNizKaraktera() << endl;//9.6.2017 10:15

	Predmet Matematika, Fizika, Hemija, Engleski;
	//2 - ocjena na predmetu; datum - datum evidentiranja uspjeha na predmetu jer postoji krajnji rok za evidentiranje
	Matematika.Unos("Matematika", 2, datum19062017_1015);
	Fizika.Unos("Fizika", 5, datum20062017_1115);
	Hemija.Unos("Hemija", 2, datum20062017_1115);
	Engleski.Unos("Engleski", 5, datum05072017_1231);

	int brojKandidata = 2;

	Kandidat * prijave2017 = new Kandidat[brojKandidata];
	prijave2017[0].Unos(DL, "Jasmin Azemovic");
	prijave2017[1].Unos(REDOVAN, "Indira Hamulic");

	/*
	uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
	prilikom dodavanja onemoguciti:
	- dodavanje predmeta za razrede koji nisu definisani enumeracijom,
	- dodavanje istoimenih predmeta na nivou jednog razreda,
	- dodavanje predmeta nakon dozvoljenog roka za prijavu (rokZaPrijavu).
	razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.). Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	if (prijave2017[0].DodajPredmet(DRUGI, Engleski))//ne bi trebao dodati jer je prosao postavljeni rok za dodavanje predmeta
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(DRUGI, Matematika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(PRVI, Fizika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(PRVI, Hemija))
		cout << "Predmet uspjesno dodan!" << crt;

	Matematika._ocjena = 5;
	Hemija._ocjena = 3;

	if (prijave2017[1].DodajPredmet(PRVI, Matematika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(PRVI, Matematika))//ne bi trebalo ponovo dodati Matematiku!
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(TRECI, Hemija))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(DRUGI, Engleski))
		cout << "Predmet uspjesno dodan!" << crt;

	/*
	koristeci Lambda izraz kreirati funkciju koja ce vratiti uspjeh kandidata koji je ostvario najveci prosjek 
	(na nivou razreda, a ne ukupni prosjek). ukoliko vise kandidata ima isti prosjek funkcija vraca uspjeh 
	(najboljeg razreda) prvog pronadjenog kandidata
	*/
	auto najboljiUspjeh = [=]()
	{
		int razred;
		cout << "Unesi razred: ";
		cin >> razred;
		razred--;

		shared_ptr<Uspjeh> *najboljiUspjeh = nullptr;
		int suma = 0;
		int sumaPredhodna = 0;

		for (int i = 0; i < brojKandidata; i++)
		{
			if (prijave2017[i]._uspjeh[razred] != nullptr)
			{
				for (int j = 0; j < prijave2017[i]._uspjeh[razred]->_brojPredmeta; j++)
				{
					suma += prijave2017[i]._uspjeh[razred]->_predmeti[j]._ocjena;
				}
				if (suma > sumaPredhodna)
				{
					sumaPredhodna = suma;
					najboljiUspjeh = &prijave2017[i]._uspjeh[razred];
				}
				suma = 0;
			}
		}

		return *najboljiUspjeh;

	};
	shared_ptr<Uspjeh> najbolji = najboljiUspjeh();
	najbolji->Ispis();

	/* napisati rekurzivnu funkciju koja ce vratiti pokazivac na kandidata sa najvecom ocjenom na predmetu koji je proslijedjen 
	kao parametar. ukoliko je vise kandidata ostvarilo istu ocjenu, funkcija treba da vrati onog kandidata koji je prvi 
	evidentirao tu ocjenu (ako je isto vrijeme evidentiranja, onda funkcija vraca kandidata koji je prvi u nizu). 
	U slucaju da niti jedan kandidat nije evidentirao trazeni predmet funkcija vraca nullptr. u nastavku je prikazan primjer 
	poziva rekurzivne funkcije, a ostale parametre dodajte po potrebi. */
	Kandidat * kandidatSaNajboljomOcjenom = rekNajboljaOcjena(prijave2017, brojKandidata, "Matematika");
	
	cout << "\n///////////////////////////\n";
	if(kandidatSaNajboljomOcjenom != nullptr)
		kandidatSaNajboljomOcjenom->Ispis();
	cout << "\n///////////////////////////\n";



	for (size_t i = 0; i < brojKandidata; i++)
	{
		prijave2017[i].Ispis();
		prijave2017[i].Dealociraj();
	}
	delete[] prijave2017;
	prijave2017 = nullptr;
	kandidatSaNajboljomOcjenom = nullptr;

	system("pause>0");
}