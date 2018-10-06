#include <iostream>
#include<tuple>
#include<iomanip>
const char * crt = "\n-------------------------------------\n";
using namespace std;

const int brojRedova = 10; // broj redova je isti u svim salama

struct Posjetitelj {
	char * _imePrezime;
	//pretpostavka je da ce se uvijek unijeti validna oznaka sjedista u formatu A-XX gdje je XX dvocifreni broj, a A predstavlja veliko slovo. Jednocifrene oznake ce imati prefiks 0
	char _ozankaSjedista[5]; //npr. A-01, B-17, C-23, -> slovo predstavlja oznaku reda, a broj oznaku kolone
};

struct  Projekcija {
	char * _film;//Leo da Vinci: Mission Mona Lisa
				 //svaki red ce cuvati informacije o posjetiteljima u tom redu
	Posjetitelj * _posjetitelji[brojRedova];
	int _brojPosjetiteljaURedu[brojRedova];//cuva informaciju koliko svaki red ima dodatih posjetitelja
										   /*ako zelite, nacin cuvanja informacije o broju elemenata u pojedinom redu mozete implementirati i na neki drugi nacin npr. postavljajuci neke defaultne vrijednosti atributa objekta*/
	int _brojKolonaUSali;//broj kolona mora biti paran >=5 i <=10
};

void Dealociraj(Projekcija & projekcija) {
	delete[] projekcija._film; projekcija._film = nullptr;
	for (size_t i = 0; i < brojRedova; i++) {
		for (size_t j = 0; j < projekcija._brojPosjetiteljaURedu[i]; j++) {
			delete[] projekcija._posjetitelji[i][j]._imePrezime;
			projekcija._posjetitelji[i][j]._imePrezime = nullptr;
		}
		delete[]  projekcija._posjetitelji[i];
		projekcija._posjetitelji[i] = nullptr;
	}
}

void Unos(Projekcija &p, const char* film, int brojKolona)
{
	if (brojKolona < 5 || brojKolona > 10 && brojKolona % 2 != 0)
		return;

	int vel = strlen(film) + 1;
	p._film = new char[vel];
	strcpy_s(p._film, vel, film);

	for (int i = 0; i < brojRedova; i++)
	{
		p._posjetitelji[i] = nullptr;
		p._brojPosjetiteljaURedu[i] = 0;
	}
	p._brojKolonaUSali = brojKolona;
}

//GetPozicijuUReduIKoloni - na osnovu oznake sjedista vraca pair red i kolona tj. poziciju u matrici
pair<int, int> GetPozicijuUReduIKoloni(const char* oznkaSjedista)
{
	//npr. A-01, B-17, C-23, -> slovo predstavlja oznaku reda, a broj oznaku kolone
	int brojKolone;
	int brojreda = oznkaSjedista[0] - 'A';


	char temp[3];
	for (int i = 0; i < 2; i++)
		temp[i] = oznkaSjedista[2 + i];
	temp[2] = '\0';


	brojKolone = atoi(temp)-1;

	return make_pair(brojreda, brojKolone);
}
//GetNovogPosjetitelja - vraca objekat (ne pokazivac) tipa Posjetitelj koji je inicijalizovan vrijednostima proslijedjenih parametara
Posjetitelj GetNovogPosjetitelja(const char* oznaka, const char*imePrezime)
{
	Posjetitelj t;
	int vel = strlen(imePrezime) + 1;
	t._imePrezime = new char[vel];
	strcpy_s(t._imePrezime, vel, imePrezime);

	strcpy_s(t._ozankaSjedista, oznaka);
	return t;
}

/*
DodajPosjetitelja - podatke o novom posjetitelju dodaje u listu (niz) posjetitelja za odredjenu projekciju.
Onemoguciti dodavanje posjetitelja sa istim imenom i prezimenom, nepostojecom lokacijom sjedista ili u slucaju da su sva mjesta 
popunjena
*/
bool DodajPosjetitelja(Projekcija &p, Posjetitelj posjetitelj)
{
	int r, k;
	tie(r, k) = GetPozicijuUReduIKoloni(posjetitelj._ozankaSjedista);

	if (r*k > p._brojKolonaUSali * brojRedova)
		return false;

	for (int i = 0; i < brojRedova; i++)
		for (int j = 0; j < p._brojPosjetiteljaURedu[i]; j++)
			if (strcmp(p._posjetitelji[i][j]._imePrezime, posjetitelj._imePrezime) == 0)
				return false;

			else if (r == i)
			{
				int R, K;
				tie(R, K) = GetPozicijuUReduIKoloni(p._posjetitelji[i][j]._ozankaSjedista);
				if (K == k && r == R)
					return false;
			}

	if (p._brojPosjetiteljaURedu[r] == brojRedova)
		return false;

	Posjetitelj * t = new Posjetitelj[p._brojPosjetiteljaURedu[r]+1];
	for (int i = 0; i < p._brojPosjetiteljaURedu[r]; i++)
	{
		int vel = strlen(p._posjetitelji[r][i]._imePrezime);
		t[i]._imePrezime = new char[vel];
		strcpy_s(t[i]._imePrezime, vel, p._posjetitelji[r][i]._imePrezime);

		delete[] p._posjetitelji[r][i]._imePrezime;
		p._posjetitelji[r][i]._imePrezime = nullptr;

		strcpy_s(t[i]._ozankaSjedista, p._posjetitelji[r][i]._ozankaSjedista);
	}
	delete[] p._posjetitelji[r];
	p._posjetitelji[r] = t;

	int vel = strlen(posjetitelj._imePrezime)+1;
	t[p._brojPosjetiteljaURedu[r]]._imePrezime = new char[vel];
	strcpy_s(t[p._brojPosjetiteljaURedu[r]]._imePrezime, vel, posjetitelj._imePrezime);

	strcpy_s(t[p._brojPosjetiteljaURedu[r]]._ozankaSjedista, posjetitelj._ozankaSjedista);

	delete[] posjetitelj._imePrezime;
	posjetitelj._imePrezime = nullptr;

	p._brojPosjetiteljaURedu[r]++;

	return true;
}

void Ispis(const char* znak, const int &n)
{
	for (int i = 0; i < n; i++)
		cout << znak;
}


int RekurzivnoBrojKaraktera(Projekcija &p, int brojac = 0, int suma = 0)
{
	if (brojac == brojRedova)
		return suma;

	for (int i = 0; i < p._brojPosjetiteljaURedu[brojac]; i++)
		if (strlen(p._posjetitelji[brojac][i]._imePrezime) > suma)
			suma = strlen(p._posjetitelji[brojac][i]._imePrezime);

	return RekurzivnoBrojKaraktera(p, brojac + 1, suma);
}

void PrikaziRasporedSjedenja(Projekcija &p)
{
	int n = RekurzivnoBrojKaraktera(p);
	int r, k;
	for (int i = 0; i < brojRedova; i++)
	{
		cout << "|";
		for (int j = 0; j < p._brojKolonaUSali; j++)
		{
			Ispis("+-", n/2);
			cout << "|";
		}
		cout << endl;



		bool provjera;



		cout << "|";
		for (int j = 0; j < p._brojKolonaUSali; j++)
		{
			provjera = false;
			for (int x = 0; x < p._brojPosjetiteljaURedu[i]; x++)
			{
				tie(r, k) = GetPozicijuUReduIKoloni(p._posjetitelji[i][x]._ozankaSjedista);
				if (r == i && k == j)
				{
					provjera = true;
					cout << setw(n) << p._posjetitelji[i][x]._imePrezime;
					break;
				}
			}

			if(provjera == false)
				Ispis("  ", n / 2);
			cout << "|";
		}
		cout << endl;
	}
	cout << "|";
	for (int j = 0; j < p._brojKolonaUSali; j++)
	{
		Ispis("+-", n / 2);
		cout << "|";
	}
	cout << endl;

}



int main() {

	int oznakaR = 0, oznakaK = 0;
	Projekcija Leo; //Leo da Vinci: Mission Mona Lisa
					//inicijalizuje vrijednosti atributa objekta osvetnici
	Unos(Leo, "Leo da Vinci: Mission Mona Lisa", 6);//film, broj_kolona

	//GetPozicijuUReduIKoloni - na osnovu oznake sjedista vraca pair red i kolona tj. poziciju u matrici
	tie(oznakaR, oznakaK) = GetPozicijuUReduIKoloni("B-15");
	cout << crt << "GetPozicijuUReduIKoloni(B-15) ->" << oznakaR << "/" << oznakaK << endl;//ispisuje 1/14 
	tie(oznakaR, oznakaK) = GetPozicijuUReduIKoloni("C-01");
	cout << "GetPozicijuUReduIKoloni(C-01) ->" << oznakaR << "/" << oznakaK << crt;//ispisuje 2/0
	/*
	GetNovogPosjetitelja - vraca objekat (ne pokazivac) tipa Posjetitelj koji je inicijalizovan vrijednostima proslijedjenih parametara
	DodajPosjetitelja - podatke o novom posjetitelju dodaje u listu (niz) posjetitelja za odredjenu projekciju.
	Onemoguciti dodavanje posjetitelja sa istim imenom i prezimenom, nepostojecom lokacijom sjedista ili u slucaju da su sva mjesta popunjena
	*/
	if (DodajPosjetitelja(Leo, GetNovogPosjetitelja("A-02", "Denis Music")))
		cout << crt << "Posjetitelj uspjesno dodan!" << crt;
	if (DodajPosjetitelja(Leo, GetNovogPosjetitelja("C-03", "Zanin Vejzovic")))
		cout << crt << "Posjetitelj uspjesno dodan!" << crt;
	if (DodajPosjetitelja(Leo, GetNovogPosjetitelja("D-05", "Adel Handzic")))
		cout << crt << "Posjetitelj uspjesno dodan!" << crt;

	//PrikaziRasporedSjedenja - na osnovu oznake sjedista prikazuje raspored sjedenja posjetitelja za vrijeme projekcije
	PrikaziRasporedSjedenja(Leo);

	/*
	DODATNO:
	1. prilikom ispisa, sirinu kolone prilagoditi maksimalnom broju karaktera u imenu i prezimenu posjetioca
	2. automatski generisati oznaku sjedista na osnovu prvog slobodnog mjesta na projekciji. ovu funkciju nije potrebno integrisati u postojece rjesenje, vec se moze i zasebno testirati
	*/

	Dealociraj(Leo);

	system("pause>0");
	return 0;
}
