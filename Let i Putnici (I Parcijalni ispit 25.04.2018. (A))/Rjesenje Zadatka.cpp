#include <iostream>
#include<tuple>
#include<iomanip>
const char * crt = "\n-------------------------------------\n";

using namespace std;

struct Putnik {
	char * _imePrezime;
	//pretpostavka je da ce se uvijek unijeti validna oznaka sjedista u formatu XXA gdje je XX dvocifreni broj, a A predstavlja veliko slovo. Jednocifrene oznake ce imati prefiks 0
	char _ozankaSjedista[4]; //npr. 02A, 12B, 14B, 28C -> broj predstavlja oznaku reda, a slovo oznaku kolone
};
//GetNovogPutnika - vraca objekat (ne pokazivac) tipa Putnik koji je inicijalizovan vrijednsotima proslijedjenih parametara

Putnik GetNovogPutnika(const char* oznaka, const char* imePrezime)
{
	Putnik t;
	t._imePrezime = new char[strlen(imePrezime) + 1];
	strcpy_s(t._imePrezime, strlen(imePrezime) + 1, imePrezime);

	strcpy_s(t._ozankaSjedista, oznaka);

	return t;
}


//GetPozicijuUReduIKoloni - na osnovu oznake sjedista vraca pair red i kolona tj. poziciju u matrici
pair <int, int> GetPozicijuUReduIKoloni(const char* opis)
{
	//npr. 02A, 12B, 14B, 28C -> broj predstavlja oznaku reda, a slovo oznaku kolone

	pair <int, int> t;
	t = make_pair<int, int>(0, 0);

	t.second = opis[2] - 'A';

	char temp[3];
	for (int i = 0; i < 2; i++)
		temp[i] = opis[i];
	temp[2] = '\0';

	t.first = atoi(temp) - 1;

	return t;
}
struct Let {
	char * _relacija;//Mostar -> Sarajevo
	Putnik * _putnici;
	int _trenutnoPutnika;
	int _brojRedovaUAvionu;
	int _brojKolonaUAvionu;//broj kolona mora biti paran >=4 i <=10
};

//PrikaziRasporedSjedenja - na osnovu oznake sjedista prikazuje raspored sjedenja u avionu za let koji je proslijedjen kao parametar

void printaj(const char* znak, int n)
{
	for (int i = 0; i < n; i++)
		cout << znak;
}

void PrikaziRasporedSjedenja(Let let, int n)
{
	int r, k;
	for (int i = 0; i < let._brojRedovaUAvionu; i++)
	{
		cout << "|";
		for (int j = 0; j < let._brojKolonaUAvionu; j++)
		{
			printaj("+-", n/2);
			cout << "|";
		}
		cout << endl;


		bool test;
		cout << "|";
		for (int j = 0; j < let._brojKolonaUAvionu; j++)
		{

			test = true;
			for (int x = 0; x < let._trenutnoPutnika; x++)
			{
				tie(r, k) = GetPozicijuUReduIKoloni(let._putnici[x]._ozankaSjedista);

				if (r == i && k == j)
				{
					cout << setw(n) << let._putnici[x]._imePrezime;
					test = false;
					break;
				}
			}

			if (test)
				printaj("  ", n/2);

			cout << "|";
		}
		cout << endl;

	}

	cout << "|";
	for (int j = 0; j < let._brojKolonaUAvionu; j++)
	{
		printaj("+-", n/2);
		cout << "|";
	}
	cout << endl;

}




//DodajPutnika - podatke o novom putniku dodaje u listu (niz) putnika na proslijedjenom letu. 

//Onemoguciti dodavanje putnika sa istim imenom i prezimenom,
//nepostojecom lokacijom sjedista ili u slucaju da su sva mjesta popunjena

bool DodajPutnika(Let &let, Putnik p)
{
	int r, k;
	tie(r, k) = GetPozicijuUReduIKoloni(p._ozankaSjedista);
	
	if (r < 0 || k < 0 || k >= 10 || r >= 'Z' - 65)
		return false;

	else if (let._trenutnoPutnika >= let._brojKolonaUAvionu*let._brojRedovaUAvionu)
		return false;

	for (int i = 0; i < let._trenutnoPutnika; i++)
		if (strcmp(let._putnici[i]._imePrezime, p._imePrezime) == 0)
			return false;

	Putnik * t = new Putnik[let._trenutnoPutnika + 1];
	for (int i = 0; i < let._trenutnoPutnika; i++)
	{
		t[i]._imePrezime = new char[strlen(let._putnici[i]._imePrezime) + 1];
		strcpy_s(t[i]._imePrezime, strlen(let._putnici[i]._imePrezime) + 1, let._putnici[i]._imePrezime);

		strcpy_s(t[i]._ozankaSjedista, let._putnici[i]._ozankaSjedista);

		delete[] let._putnici[i]._imePrezime;
		let._putnici[i]._imePrezime = nullptr;
	}
	
	delete[] let._putnici;
	let._putnici = t;
	t[let._trenutnoPutnika]._imePrezime = new char[strlen(p._imePrezime) + 1];

	strcpy_s(t[let._trenutnoPutnika]._imePrezime, strlen(p._imePrezime) + 1, p._imePrezime);
	strcpy_s(t[let._trenutnoPutnika]._ozankaSjedista, p._ozankaSjedista);

	let._trenutnoPutnika++;

	delete[] p._imePrezime;
	p._imePrezime = nullptr;

	return true;
}



void Unos(Let &let, const char* relacija, int redovi, int kolone)
{
	//broj kolona mora biti paran >= 4 i <= 10
	if (kolone % 2 != 0 || kolone > 10 || kolone < 4)
	{
		cout << "Broj kolona ne zadovoljava uslove... Program treba prekinuti..." << endl;
		return;
	}

	let._putnici = nullptr;
	let._trenutnoPutnika = 0;
	let._brojKolonaUAvionu = kolone;
	let._brojRedovaUAvionu = redovi;
	int vel = strlen(relacija) + 1;
	let._relacija = new char[vel];
	strcpy_s(let._relacija, vel, relacija);
}


void Dealociraj(Let & let) {
	delete[] let._relacija; let._relacija = nullptr;
	for (size_t i = 0; i < let._trenutnoPutnika; i++) {
		delete[] let._putnici[i]._imePrezime;
		let._putnici[i]._imePrezime = nullptr;
	}
	delete[] let._putnici;
	let._putnici = nullptr;
}

//rekBrojacKaraktera - rekurzivna funkcija koja vraca maksimalan broj karaktera u imenu i prezimenu putnika na odredjenom letu
int rekBrojacKaraktera(Let &let, int brojac, int suma)
{
	if (--brojac == 0)
		return suma;
	else if (strlen(let._putnici[brojac]._imePrezime) > suma)
		suma = strlen(let._putnici[brojac]._imePrezime);
	return rekBrojacKaraktera(let, brojac, suma);
}

//2. automatski generisati oznaku sjedista na osnovu narednog slobodnog mjesta na letu (povratnom vrijednoscu sugerisati na slucaj u kome su 
//sva mjesta na letu vec zauzeta). ovu funkciju nije potrebno integrisati u postojece rjesenje, vec se moze i zasebno testirati

const char* GenerisatiOznakuSjedista(Let &let)
{
	//oznakaR 15 -> 14
	//oznakaK B -> 1



	int r, k;
	tie(r, k) = GetPozicijuUReduIKoloni(let._putnici[let._trenutnoPutnika - 1]._ozankaSjedista);
	r++;

	//npr. 02A, 12B, 14B, 28C -> broj predstavlja oznaku reda, a slovo oznaku kolone
	if (k+1 >= let._brojKolonaUAvionu)
	{
		if (r + 1 >= let._brojRedovaUAvionu)
			return nullptr;
		k = 0;
		r++;
	}
	else
		k++;

	char * temp = new char[4];

	char red[3];
	bool test = false;
	if (r < 10)
	{
		r += 10;
		test = true;
	}
	_itoa_s(r, red, 10);

	
	for (int i = 0; i < 2; i++)
		temp[i] = red[i];

	temp[2] = k + 'A';
	temp[3] = '\0';


	if (test)
		temp[0] = '0';

	return temp;
}


int main() {
	int oznakaR = 0, oznakaK = 0;
	Let mostar_sarajevo;
	//inicijalizuje vrijednosti atributa objekta mostar_sarajevo
	Unos(mostar_sarajevo, "Mostar -> Sarajevo", 10, 4);//relacija, broj_redova, broj_kolona

	//GetPozicijuUReduIKoloni - na osnovu oznake sjedista vraca pair red i kolona tj. poziciju u matrici
	tie(oznakaR, oznakaK) = GetPozicijuUReduIKoloni("15B");
	cout << crt << "GetPozicijuUReduIKoloni(15B) ->" << oznakaR << "/" << oznakaK << endl;//ispisuje 14/1 
	tie(oznakaR, oznakaK) = GetPozicijuUReduIKoloni("01B");
	cout << "GetPozicijuUReduIKoloni(01B) ->" << oznakaR << "/" << oznakaK << crt;//ispisuje 0/1

	//GetNovogPutnika - vraca objekat (ne pokazivac) tipa Putnik koji je inicijalizovan vrijednsotima proslijedjenih parametara
	//DodajPutnika - podatke o novom putniku dodaje u listu (niz) putnika na proslijedjenom letu. Onemoguciti dodavanje putnika sa istim imenom i prezimenom,
	//nepostojecom lokacijom sjedista ili u slucaju da su sva mjesta popunjena

	if (DodajPutnika(mostar_sarajevo, GetNovogPutnika("01A", "Denis Music")))
		cout << crt << "Putnik uspjesno dodan!" << crt;
	cout << GenerisatiOznakuSjedista(mostar_sarajevo) << endl;

	if (DodajPutnika(mostar_sarajevo, GetNovogPutnika("07D", "Zanin Vejzovic")))
		cout << crt << "Putnik uspjesno dodan!" << crt;

	if (DodajPutnika(mostar_sarajevo, GetNovogPutnika("10C", "Adel Handzic")))
		cout << crt << "Putnik uspjesno dodan!" << crt;

	//for (int i = 0; i < mostar_sarajevo._trenutnoPutnika; i++)
	//	cout << mostar_sarajevo._putnici[i]._imePrezime << endl;

	int brojKaraktera = rekBrojacKaraktera(mostar_sarajevo, mostar_sarajevo._trenutnoPutnika, 0);
	//rekBrojacKaraktera - rekurzivna funkcija koja vraca maksimalan broj karaktera u imenu i prezimenu putnika na odredjenom letu
	cout << crt << "Maksimalna broj karaktera u imenu i prezimenu putnika je -> " << brojKaraktera << crt; //broj 0 je pocetno stanje prilikom brojanja max karaktera

	
	//PrikaziRasporedSjedenja - na osnovu oznake sjedista prikazuje raspored sjedenja u avionu za let koji je proslijedjen kao parametar
	PrikaziRasporedSjedenja(mostar_sarajevo, brojKaraktera);

	//DODATNO:
	//1. prilikom ispisa, sirinu kolone prilagoditi maksimalnom broju karaktera u imenu i prezimenu
	

	Dealociraj(mostar_sarajevo);
	system("pause>0");
	return 0;
}
