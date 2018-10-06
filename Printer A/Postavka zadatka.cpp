﻿/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA, U SUPROTNOM SE RAD NECE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VASIM BROJEM INDEKSA ( NPR. IB130030.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJESENJA VASIH ZADATAKA. NE PREDAVATI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORISTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VASA RJESENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URADJENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
#include<iostream>
using namespace std;

//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

const char *crt = "\n-------------------------------------------\n";

enum vrstaDokumenta { PDF, DOC, TXT, HTML };
const char * vrstaDokumentaChar[] = { "PDF", "DOC", "TXT", "HTML" };

enum Prioritet { VISOK, SREDNJI, NIZAK };
const char * PrioritetChar[] = { "VISOK", "SREDNJI", "NIZAK" };

char * AlocirajNizKaraktera(const char * sadrzaj) {
    int vel = strlen(sadrzaj) + 1;
    char * temp = new char[vel];
    strcpy_s(temp, vel, sadrzaj);
    return temp;
}

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

struct Filter {
    char * _sadrzaj;
    Prioritet _prioritet;
    void Unos(const char * sadrzaj, Prioritet prioritet) {
        _sadrzaj = AlocirajNizKaraktera(sadrzaj);
        _prioritet = prioritet;
    }
    void Dealociraj() {
        delete[] _sadrzaj; _sadrzaj = nullptr;
    }
    void Ispis() { cout << _sadrzaj << " (" << PrioritetChar[_prioritet] << ")" << endl; }
};

struct Dokument {
    vrstaDokumenta _vrsta;
    char * _naziv;
    char * _sadrzaj;
    unique_ptr<DatumVrijeme> _kreiran;
    int _brojStranica;

    void Unos(vrstaDokumenta vrsta, const char * naziv, DatumVrijeme kreiran) {
        _naziv = AlocirajNizKaraktera(naziv);
        _vrsta = vrsta;
        _kreiran = kreiran;
        _brojStranica = 0;//broj stranica se odredjuje prilikom svakog dodavanja novog sadrzaja dokumentu
        _sadrzaj = nullptr;
    }
    void Dealociraj() {
        delete[] _naziv; _naziv = nullptr;
        delete[] _sadrzaj; _sadrzaj = nullptr;
        _kreiran->Dealociraj();
    }
    void Ispis() {
        cout << crt << _naziv << " " << vrstaDokumentaChar[_vrsta] << " kreiran ";
        _kreiran->Ispis();
        cout << crt << _sadrzaj << crt << " br.stranica (" << _brojStranica << ")" << crt;
    }
};
struct Printer {
    char * _model;
    Dokument * _dokumenti;
    int _trenutnoDokumenata;
    Filter * _filteri;
    int _trenutnoFiltera;

    void Unos(const char * model) {
        _model = AlocirajNizKaraktera(model);
        _filteri = nullptr;
        _dokumenti = nullptr;
        _trenutnoDokumenata = 0;
        _trenutnoFiltera = 0;
    }
    void Dealociraj() {
        delete[] _model; _model = nullptr;
        for (size_t i = 0; i < _trenutnoDokumenata; i++)
            _dokumenti[i].Dealociraj();
        delete[]_dokumenti; _dokumenti = nullptr;
        for (size_t i = 0; i < _trenutnoFiltera; i++)
            _filteri[i].Dealociraj();
        delete[]_filteri; _filteri = nullptr;
    }
    void Ispis() {
        cout << _model << crt;
        for (size_t i = 0; i < _trenutnoDokumenata; i++)
            _dokumenti[i].Ispis();
    }    
};


int main() {
    DatumVrijeme prije3Dana; prije3Dana.Unos(3, 2, 2018, 10, 15);
    DatumVrijeme danas1; danas1.Unos(6, 2, 2018, 10, 15);
    DatumVrijeme danas2; danas2.Unos(6, 2, 2018, 10, 16);
    DatumVrijeme za10Dana; za10Dana.Unos(16, 2, 2018, 10, 15);

    Dokument ispitPRII, ispitMAT, ispitUIT, ispitUITDrugiRok;
    ispitPRII.Unos(DOC, "ispitPRII.doc", prije3Dana);
    ispitMAT.Unos(DOC, "ispitMAT.doc", danas1);
    ispitUIT.Unos(DOC, "ispitUIT.doc", danas2);
    ispitUITDrugiRok.Unos(PDF, "ispitUITDrugiRok.pdf", za10Dana);

    /*BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREDJUJE PRILIKOM DODAVANJA SADRZAJA. ZA POTREBE ISPITA PRETPOSTAVLJAMO DA NA JEDNU
    STRANICU MOZE STATI BROJ_ZNAKOVA_PO_STRANICI ZNAKOVA UKLJUCUJUCI RAZMAKE I DRUGE VRSTE ZNAKOVA*/
    ispitPRII.DodajSadrzaj("Programiranje ili racunarsko programiranje (engl. programming) jeste vjestina pomocu koje ");
    ispitPRII.DodajSadrzaj("korisnik stvara i izvrsava algoritme koristeci odredjene programske jezike da bi ... ");
    cout << "Broj stranica -> " << ispitPRII._brojStranica << endl;

    ispitPRII.Ispis();//ISPISUJE SVE PODATKE O DOKUMENTU

    ispitMAT.DodajSadrzaj("Matematika se razvila iz potrebe da se obavljaju proracuni u trgovini, vrse mjerenja zemljista i predvidjaju ");
    ispitMAT.DodajSadrzaj("astronomski dogadjaji, i ove tri primjene se mogu dovesti u vezu sa grubom podjelom matematike ");

    Printer hp3200; hp3200.Unos("HP 3200");
    /*PRINTER NECE DOZVOLITI PRINTANJE DOKUMENATA U CIJEM SADRZAJU SE NALAZI NEKA OD ZABRANJENIH RIJECI DEFINISANIH FILTERIMA*/
    if (hp3200.DodajFilter("RE", NIZAK)) //VISOK :)
        cout << "Filter dodan!" << endl;
    if (hp3200.DodajFilter("RAT", VISOK))
        cout << "Filter dodan!" << endl;
    if (hp3200.DodajFilter("UBITI", VISOK))
        cout << "Filter dodan!" << endl;
    if (hp3200.DodajFilter("MRZITI", SREDNJI))
        cout << "Filter dodan!" << endl;
    if (!hp3200.DodajFilter("RE", SREDNJI))/* ONEMOGUCITI PONAVLJANJE FILTERA, BEZ OBZIRA STO SU RAZLICITOG PRIORITETA*/
        cout << "Filter nije dodan!" << endl;

    /*
    DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDECA PRAVILA:
    1. NAZIV DOKUMENTA MOZE SADRZAVATI SAMO SLOVA, A EKSTENZIJA MOZE BITI IDENTICNA ONOJ DEFINISANOJ VRIJEDNOSCU ATRIBUTA vrstaDokumenta
    2. SADRZAJ DOKUMENTA MOZE POSJEDOVATI RIJECI DEFINISANE FILTERIMA, ALI PREMA SLJEDECIM PRAVILIMA:
    - NITI JEDNU RIJEC OZNACENU FILTEROM PRIORITETA VISOK
    - NAJVISE 2 RIJECI OZNACENE FILTEROM PRIORITETA SREDNJI
    - NAJVISE 3 RIJECI OZNACENE FILTEROM PRIORITETA NIZAK
    UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA PRINTAJ DOKUMENT TREBA SACUVATI U NIZ _dokumenti, ALI TAJ DOKUMENT NE TREBA PRINTATI I FUNKCIJA VRACA FALSE.
    UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI SADRZAJ DOKUMENTA KOJI SE PRINTA, A SA CRT LINIJOM ODVAJATI STRANICE DOKUMENTA
    */
    if (hp3200.Printaj(ispitPRII))
        cout << "Printam -> " << ispitPRII._naziv << endl;
    if (hp3200.Printaj(ispitMAT))
        cout << "Printam -> " << ispitMAT._naziv << endl;
    if (hp3200.Printaj(ispitUIT))
        cout << "Printam -> " << ispitUIT._naziv << endl;
    if (hp3200.Printaj(ispitUITDrugiRok))
        cout << "Printam -> " << ispitUITDrugiRok._naziv << endl;

    //PROSJECAN BROJ STRANICA = UKUPAN BROJ STRANICA / BROJ PRINTANIH DOKUMENATA
    //REKURZIVNA FUNKCIJA VRACA PROSJECAN BROJ ISPRINTANIH STRANICA (KOJE SU ZADOVOLJILE POSTAVLJENE FILTERE) U PERIODU OD (npr.prije3Dana) - DO (npr.danas2), UKLJUCUJUCI I TAJ PERIOD
    cout << "Prosjecan broj printanih stranica je -> " << hp3200.GetProsjecanBrojStranicaUPeriodu_Od_Do(/**/) << crt;
    //VRACA SADRZAJ FILTERA KOJI SE NAJCESCE KORISTIO U DOKUMENTIMA
    Filter * f = hp3200.GetNakoristenijiFilter();
    if(f!=nullptr && f->_sadrzaj!=nullptr)
        cout << "Najcesce koristeni sadrzaj filtera je -> " << f->_sadrzaj << crt;
    //PORED VRIJEDNOSTI SVIH OSTALIH ATRIBUTA, ISPISUJE SAMO ONE DOKUMENTE KOJI ZADOVOLJAVAJU SVA PRETHODNO DEFINISANA PRAVILA
    hp3200.Ispis();

    /*NAPISATI LAMBDA FUNKCIJU ZamijeniIPrintaj KOJA U SADRZAJU DOKUMENTA MIJENJA PRVI POSLATI PARAMETAR SA VRIJEDNOSCU DRUGOG PARAMETRA, TE VRACA BROJ ZAMIJENJENIH ZNAKOVA*/
    auto ZamijeniIPrintaj = /**/;
    //RAZMAK MIJENJA ZNAKOM CRTICA U SADRZAJU DOKUMENATA KOJI SU PRINTANI danas1
    int zamijenjeno = ZamijeniIPrintaj(' ','-', danas1);
    cout << "Zamijenjeno -> " << zamijenjeno << " znakova" << endl;
    hp3200.Ispis();    

    prije3Dana.Dealociraj(); danas1.Dealociraj(); danas2.Dealociraj(); za10Dana.Dealociraj();
    ispitMAT.Dealociraj(); ispitPRII.Dealociraj(); ispitUIT.Dealociraj(); ispitUITDrugiRok.Dealociraj();

    hp3200.Dealociraj();
    
    return 0;
}
