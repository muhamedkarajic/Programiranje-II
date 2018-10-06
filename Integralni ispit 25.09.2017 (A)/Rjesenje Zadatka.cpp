#include<memory>
#include<iostream>
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEĆE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ĆETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/

using namespace std;
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum VrstaObaveze { Seminarski, Parcijalni1, Parcijalni2, Integralni, Prakticni };
const char* VrstaObavezeChar[] = { "Seminarski", "Parcijalni1", "Parcijalni2", "Integralni", "Prakticni" };
struct Datum {
	int * _dan, *_mjesec, *_godina;

	bool izmedjuDana(Datum &OD, Datum &DO)
	{
		return Dani() >= OD.Dani() && Dani() <= DO.Dani();
	}


	int Dani()
	{
		return *_dan + *_mjesec*31 + *_godina*31*12;
	}
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() {
		cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl;
	}
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; }
};
struct ObavezeNaPredmetu {
	shared_ptr<VrstaObaveze> _vrstaObaveze;
	Datum _datumIzvrsenja;
	char * _napomena;
	int _ocjena; // 5 - 10 
	void Unos(VrstaObaveze vrsta, Datum datum, int ocjena, const char * napomena) {
		_vrstaObaveze = make_shared<VrstaObaveze>(vrsta);
		_datumIzvrsenja.Unos(*datum._dan, *datum._mjesec, *datum._godina);
		_ocjena = ocjena;
		_napomena = new char[strlen(napomena) + 1];
		strcpy_s(_napomena, strlen(napomena) + 1, napomena);
	}
	void Ispis() {
		cout << VrstaObavezeChar[*_vrstaObaveze] << " " << _ocjena << " " << _napomena;
		_datumIzvrsenja.Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja.Dealociraj();
		delete[] _napomena; 
		_napomena = nullptr;
	}
};
/*da bi se odredjena obaveza na predmetu smatrala uspjesno izvrsenom, u napomeni (objekta ObavezeNaPredmetu) 
ne smije stajati niti jedna od rijeci koja se nalazi u nizu _kljucneRijeci*/

const char * _kljucneRijeci[15] = { NULL }; 

bool sadrziRijec(const char* napomena)
{
	for (int i = 0; i < 15; i++)
	{
		if (_kljucneRijeci[i] == nullptr)
			break;
		else if(strstr(napomena, _kljucneRijeci[i]) != nullptr)
				return true;
	}
	return false;
}

struct PolozeniPredmet {
	Datum _datumPolaganja;//datum koji ce se evidentirati kao datum kada je predmet polozen tj. kada je formirana konacna ocjena
	char * _nazivPredmeta;
	ObavezeNaPredmetu * _listaIzvrsenihObaveza;
	int _trenutnoIzvrsenihObaveza;
	int _minimalanBrojDana; //odnosi se na minimalan broj dana koji mora proci od izvrsenja pojedinih obaveza na predmetu (npr. 7 dana)
	int _konacnaOcjena; //formira se na osnovu ocjena izvrsenih obaveza

	/*Konacna ocjene predstavlja prosjecnu ocjenu na predmetu (prosjecnu ocjenu izvrsenih obaveza koje ispunjavaju uslove za 
	uspjesno polaganje predmeta),a za njeno formiranje student mora posjedovati polozen integralni ili dva parijcalna ispita. 
	
	Ukoliko je ispit polozen putem parcijalnih ispita, student takodjer mora imati pozitivno (ocjenom vecom od 5) 
	ocijenjena najmanje dva seminarska i jedan prakticni rad. Takodjer, datum polaganja drugog parcijalnog ispita ne smije biti 
	->raniji (manji) od datuma polaganja prvog parcijalnog ispita. 
	
	Konacna ocjena ne smije biti formirana u slucaju da napomena bilo koje odbaveze (koja je ucestvovala u formiranju 
	konacne ocjene) sadrzi neku od zabranjenih rijeci (niz _kljucneRijeci). 
	
	U slucaju da neki od navedenih uslova nije zadovoljen konacna ocjena treba biti postavljena na vrijednost 5. 
	Ukoliko su ispunjeni svi uslovi, osim formiranja konacne ocjene, datum polaganja je potrebno postaviti na vrijednost datuma 
	posljednje izvrsene obaveze sa najvecom ocjenom.
	*/
	int FormirajKonacnuOcjenu()
	{

		//enum VrstaObaveze { Seminarski, Parcijalni1, Parcijalni2, Integralni, Prakticni };
		
		
		int indeks = -1, indeksP1 = -1, indeksP2 = -1;
		int brojac[5] = { 0 };
		int suma[5] = { 0 };

		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
		{
			if (_listaIzvrsenihObaveza[i]._ocjena > 5 && !sadrziRijec(_listaIzvrsenihObaveza[i]._napomena))
			{
				if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Integralni)
				{
					_konacnaOcjena = _listaIzvrsenihObaveza[i]._ocjena;
					_datumPolaganja.Unos(*_listaIzvrsenihObaveza[i]._datumIzvrsenja._dan, *_listaIzvrsenihObaveza[i]._datumIzvrsenja._mjesec, *_listaIzvrsenihObaveza[i]._datumIzvrsenja._godina);
					return _konacnaOcjena;
				}
				else
				{
					if (indeks == -1)
						indeks = i;
					else if(_listaIzvrsenihObaveza[i]._ocjena > _listaIzvrsenihObaveza[indeks]._ocjena)
						indeks = i;

					if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Parcijalni1)
						indeksP1 = i;
					else if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Parcijalni2)
						indeksP2 = i;

					brojac[*_listaIzvrsenihObaveza[i]._vrstaObaveze]++;
					suma[*_listaIzvrsenihObaveza[i]._vrstaObaveze] += _listaIzvrsenihObaveza[i]._ocjena;
				}
			}
		}

		if (brojac[1] != 1 || brojac[2] != 1 || brojac[0] < 2 || brojac[4] < 1 || indeks == -1)
		{
			_konacnaOcjena = 5;
			return _konacnaOcjena;
		}
		
		if (_listaIzvrsenihObaveza[indeksP1]._datumIzvrsenja.Dani() > _listaIzvrsenihObaveza[indeksP2]._datumIzvrsenja.Dani())
			return false;

		int sumaUkupna = 0;
		int brojacUkupan = 0;
		
		for (int i = 0; i < 5; i++)
		{
			brojacUkupan += brojac[i];
			sumaUkupna += suma[i];
		}

		if (brojacUkupan == 0)//nije potrebno al eto jer će indeks biti -1 ako je nešto nula
		{
			_konacnaOcjena = 5; 
			return _konacnaOcjena;
		}

		_konacnaOcjena = (sumaUkupna / float(brojacUkupan))+0.5;
		_datumPolaganja.Unos(*_listaIzvrsenihObaveza[indeks]._datumIzvrsenja._dan, *_listaIzvrsenihObaveza[indeks]._datumIzvrsenja._mjesec, *_listaIzvrsenihObaveza[indeks]._datumIzvrsenja._godina);

		return _konacnaOcjena;
	}

	void ProsiriObavezu(VrstaObaveze vrsta, Datum &datum, int ocjena, const char* napomena)
	{
		ObavezeNaPredmetu * t = new ObavezeNaPredmetu[_trenutnoIzvrsenihObaveza+1];
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
		{
			t[i].Unos(*_listaIzvrsenihObaveza[i]._vrstaObaveze, _listaIzvrsenihObaveza[i]._datumIzvrsenja, _listaIzvrsenihObaveza[i]._ocjena, _listaIzvrsenihObaveza[i]._napomena);
			_listaIzvrsenihObaveza[i].Dealociraj();

		}
		t[_trenutnoIzvrsenihObaveza].Unos(vrsta, datum, ocjena, napomena);
		delete[] _listaIzvrsenihObaveza;
		_listaIzvrsenihObaveza = t;
		_trenutnoIzvrsenihObaveza++;
	}

	/*Na osnovu vrijednosti primljenih parametara osigurati dodavanje novoizvrsene obaveze na predmetu. 
	Potrebno je onemoguciti dodavanje identicnih obaveza, a izmedju izvrsenja pojedinih obaveza mora proci najmanje onoliko dana 
	koliko je definisano vrijednoscu atributa _minimalanBrojDana. 
	
	Identicna vrsta obaveze se moze dodati samo u slucaju da je prethodno dodana obaveza (identicne vrste) imala ocjenu 5 ili je 
	u napomeni sadrzavala neku od zabranjenih rijeci (definisanih nizom _kljucneRijeci).*/
	bool DodajIzvrsenuObavezu(VrstaObaveze vrsta, Datum &datum, int ocjena, const char* napomena)
	{
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == vrsta)
				if (_listaIzvrsenihObaveza[i]._ocjena != 5 && !sadrziRijec(_listaIzvrsenihObaveza[i]._napomena))
					if(vrsta != Seminarski && vrsta != Prakticni)
						return false;

		if (_trenutnoIzvrsenihObaveza > 0)
			if (_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza - 1]._datumIzvrsenja.Dani() + _minimalanBrojDana >= datum.Dani())
				return false;


		ProsiriObavezu(vrsta, datum, ocjena, napomena);
		return true;
	}



	void Unos(const char * naziv, int minimalanBrojDana) {
		_nazivPredmeta = new char[strlen(naziv) + 1];
		strcpy_s(_nazivPredmeta, strlen(naziv) + 1, naziv);
		_listaIzvrsenihObaveza = nullptr;
		_trenutnoIzvrsenihObaveza = 0;
		_minimalanBrojDana = minimalanBrojDana;
		_konacnaOcjena = 0;
	}
	void Dealociraj() {
		if(_konacnaOcjena > 5)
			_datumPolaganja.Dealociraj();
		delete[] _nazivPredmeta; _nazivPredmeta = nullptr;
		for (size_t i = 0; i < _trenutnoIzvrsenihObaveza; i++)
			_listaIzvrsenihObaveza[i].Dealociraj();
		delete[]_listaIzvrsenihObaveza;
	}
};

/*Rekurzivna funkcija treba vratiti prosjecnu ocjenu uspjesno izvrsenih obaveze na predmetu odredjene vrste u definisanom periodu. 
Izbjegavati koristenje globalnih varijabli*/
float PretragaRekurzivno(PolozeniPredmet &p, VrstaObaveze vrsta, Datum &OD, Datum &DO, int uspjesni, int brojac, int suma=0)
{
	if (p._trenutnoIzvrsenihObaveza == brojac)
	{
		if (uspjesni == 0)
			return uspjesni;
		return suma / (float)uspjesni;
	}
	else if (p._listaIzvrsenihObaveza[brojac]._ocjena > 5 && 
		!sadrziRijec(p._listaIzvrsenihObaveza[brojac]._napomena) && 
		*p._listaIzvrsenihObaveza[brojac]._vrstaObaveze == vrsta && 
		p._listaIzvrsenihObaveza[brojac]._datumIzvrsenja.izmedjuDana(OD, DO))
	{
		return PretragaRekurzivno(p, vrsta, OD, DO, uspjesni+1, brojac+1, p._listaIzvrsenihObaveza[brojac]._ocjena+suma);
	}
	return PretragaRekurzivno(p, vrsta, OD, DO, uspjesni, brojac+1, suma);

}



void main() {
	_kljucneRijeci[0] = "prepisivao";
	_kljucneRijeci[1] = "ometao";
	_kljucneRijeci[2] = "nije dostavio";

	Datum datumSeminarski1, datumSeminarski2, datumParcijalni1, datumParcijalni2, datumIntegralni;
	datumSeminarski1.Unos(1, 6, 2017);
	datumSeminarski2.Unos(9, 6, 2017);
	datumParcijalni1.Unos(22, 6, 2017);
	datumParcijalni2.Unos(30, 6, 2017);

	PolozeniPredmet prII;
	//naziv predmeta, minimalan broj dana
	prII.Unos("Programiranje II", 7);

	/*Na osnovu vrijednosti primljenih parametara osigurati dodavanje novoizvrsene obaveze na predmetu. Potrebno je onemoguciti dodavanje identicnih obaveza, a izmedju izvrsenja pojedinih obaveza mora proci najmanje onoliko dana koliko je definisano vrijednoscu atributa _minimalanBrojDana. Identicna vrsta obaveze se moze dodati samo u slucaju da je prethodno dodana obaveza (identicne vrste) imala ocjenu 5 ili je u napomeni sadrzavala neku od zabranjenih rijeci (definisanih nizom _kljucneRijeci).*/
	
	if (prII.DodajIzvrsenuObavezu(Parcijalni1, datumParcijalni1, 6, "uslovno polozen ispit"))
		cout << "Parcijalni 1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Parcijalni2, datumParcijalni2, 5, "previse gresaka, a tokom ispita ometao druge studente"))
		cout << "Parcijalni 2...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski1, 8, "implementirani svi algoritmi"))
		cout << "Seminarski1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski2, 6, "rad slican kao kod studenta IB150388"))
		cout << "Seminarski2...dodan" << endl;

	/*Konacna ocjene predstavlja prosjecnu ocjenu na predmetu (prosjecnu ocjenu izvrsenih obaveza koje ispunjavaju uslove za uspjesno polaganje predmeta),a za njeno formiranje student mora posjedovati polozen integralni ili dva parijcalna ispita. Ukoliko je ispit polozen putem parcijalnih ispita, student takodjer mora imati pozitivno (ocjenom vecom od 5) ocijenjena najmanje dva seminarska i jedan prakticni rad. Takodjer, datum polaganja drugog parcijalnog ispita ne smije biti raniji (manji) od datuma polaganja prvog parcijalnog ispita. Konacna ocjena ne smije biti formirana u slucaju da napomena bilo koje odbaveze (koja je ucestvovala u formiranju konacne ocjene) sadrzi neku od zabranjenih rijeci (niz _kljucneRijeci). U slucaju da neki od navedenih uslova nije zadovoljen konacna ocjena treba biti postavljena na vrijednost 5. Ukoliko su ispunjeni svi uslovi, osim formiranja konacne ocjene, datum polaganja je potrebno postaviti na vrijednost datuma posljednje izvrsene obaveze sa najvecom ocjenom.
	*/
	cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;

	datumParcijalni2.Unos(22, 7, 2017);
	if (prII.DodajIzvrsenuObavezu(Parcijalni2, datumParcijalni2, 6, ""))
		cout << "Parcijalni 2...dodan" << endl;
	cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;
	Datum OD, DO; OD.Unos(1, 1, 2017); DO.Unos(1, 7, 2017);

	/*Rekurzivna funkcija treba vratiti prosjecnu ocjenu uspjesno izvrsenih obaveze na predmetu odredjene vrste u definisanom 
	periodu. Izbjegavati koristenje globalnih varijabli*/
	float prosjek = PretragaRekurzivno(prII, Parcijalni1, OD, DO, 0, 0);
	cout << "Prosjecna ocjena izvrsenih obaveza je: " << prosjek << endl;

	/*Koristeci lambda-u, unutar funkcije GetBrojObavezaIznadProsjeka pronaci broj izvrsenih obaveza koje imaju istu ili vecu 
	ocjenu od prosjecne*/
	auto GetBrojObavezaIznadProsjeka = [](PolozeniPredmet &p, float &prosjek) -> int
	{
		int brojac = 0;

		for (int i = 0; i < p._trenutnoIzvrsenihObaveza; i++)
			if (prosjek <= (float)p._listaIzvrsenihObaveza[i]._ocjena && !sadrziRijec(p._listaIzvrsenihObaveza[i]._napomena))
				brojac++;

		return brojac;
	};
	
	
	int iznadProsjeka = GetBrojObavezaIznadProsjeka(prII, prosjek);
	cout << "Broj izvrsenih obaveza iznad prosjeka je: " << iznadProsjeka << endl;

	datumParcijalni1.Dealociraj();
	datumParcijalni2.Dealociraj();
	datumSeminarski1.Dealociraj();
	datumSeminarski2.Dealociraj();
	//datumIntegralni.Dealociraj(); -> nije koristen
	prII.Dealociraj();
	system("PAUSE");
}