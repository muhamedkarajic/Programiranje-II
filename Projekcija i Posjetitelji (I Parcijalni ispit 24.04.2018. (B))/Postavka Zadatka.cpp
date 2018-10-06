#include <iostream>
#include<tuple>
const char * crt = "\n-------------------------------------\n";
using namespace std;

const int brojRedova = 10; // broj redova je isti u svim salama

struct Posjetitelj {
	char * _imePrezime;
	//pretpostavka je da ce se uvijek unijeti validna oznaka sjedista u formatu A-XX gdje je XX dvocifreni broj, a A predstavlja veliko slovo. Jednocifrene oznake ce imati prefiks 0
	char _ozankaSjedista[5]; //npr. A-01, B-17, C-23, -> slovo predstavlja oznaku reda, a broj oznaku kolone
};

struct  Projekcija{
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
		projekcija._posjetitelji [i] = nullptr;
	}
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
