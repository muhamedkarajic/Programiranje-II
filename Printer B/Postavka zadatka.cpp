/*
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
   
    ispitMAT.DodajSadrzaj("Matematika se razvila iz potrebe da se obavljaju proracuni u trgovini, vrse mjerenja zemljista i predvidjaju ","jasmin.azemovic", danas1);
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
    cout << "Prosjecan broj modifikacija odrejdne vrste printanih dokumenata je -> " << hp3200.GetProsjecanBrojModifikacijaByVrsta(DODAVANJE, /**/) << crt;
   
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
    int zamijenjeno = Zamijeni(' ', '-', "denis.music", danas1);
    cout << "Zamijenjeno -> " << zamijenjeno << " znakova" << endl;
    hp3200.Ispis();

    prije3Dana.Dealociraj(); danas1.Dealociraj(); danas2.Dealociraj(); za10Dana.Dealociraj();
    ispitMAT.Dealociraj(); ispitPRII.Dealociraj(); ispitUIT.Dealociraj(); ispitUITDrugiRok.Dealociraj();

    hp3200.Dealociraj();

    return 0;
}