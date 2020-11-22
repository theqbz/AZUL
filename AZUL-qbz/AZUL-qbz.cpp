#include"iostream"
#include"iomanip"
#include"cmath"
#include"cstdlib"
#include"ctime"

using namespace std;


// HIBAKERESES
void ZsakListazas(int z[], int cv, string szoveg);
void KorongListazas(string k[], int ksz, string szoveg);
void GZsakListazas(int z[], int d[], int a[], int cv, string szoveg);
void GKorongListazas(string k[], int ksz, string szoveg);


// GLOBÁLIS VÁLTOZÓK
#define CSVARIACIOK 5	//Ennyi különbözõ csempefajta van a játékban.
#define CSKEZDODARAB 20	//Egy adott fajta csempékbõl ennyi darab van összesen.
#define KMERET 4		//A korongokra helyezhetõ csepmpék maximális száma.
#define TABLAMERET 5	//A Mintasor illetve a Fal méretei a játékosok tábláján. (Általában megegyezik a CSVARIACIOK változó értékével.
#define PADLOMERET 7	//A Padlovonal mérete a játékosok tábláján.
string* KTOMB = NULL;	//A csempéket tároló korongok


struct Game // A játék fõ adatai
{
	string* Korongok;				//A korongok tömbje
	int KorongSzam;					//A korongok száma
	int AsztalKozep[CSVARIACIOK+1];	//Az asztal közepét tároló tömb helye
	int Zsak[CSVARIACIOK];			//A zsák tömbjének helye 
	int Eldobott[CSVARIACIOK];		//Az eldobott csempék tömbjének helye
};


struct Player
{
	string Nev;						//A játékos neve
	int Pont;						//A játékos pontszáma
	string Mintasor[TABLAMERET];	//A játékos tábláján lévõ mintasor, ahová a korongokról vagy asztalról vásárolt csempéket gyûjti.
	string Fal[TABLAMERET];			//A játékos tábláján lévõ fal, ahová felteszi a mintasoron összegyûlt csempéket.
	string Padlovonal[PADLOMERET];	//A játékos tábláján lévõ padlóvonal ahol a büntetõpontok gyûlnek.
};


// 1. JÁTÉKOSOK SZÁMÁNAK BEKÉRÉSE
int JatekosSzamBe()
{
	string jszbe = "";
	do
	{
		cout << "Add meg a jatekosok szamat (2, 3 vagy 4 jatekos lehet!): ";
		cin >> jszbe;
	} while (jszbe != "2" && jszbe != "3" && jszbe != "4");
	return int(jszbe[0]) - 48;
}

// 2. KEZDETI ZSÁK, ELDOBOTT ÉS ASZTALKÖZÉP LÉTREHOZÁSA
void KezdoZsakok(int zs[], int d[], int a[]) {
	for (int i = 0; i < CSVARIACIOK; i++)
	{
		zs[i] = CSKEZDODARAB;	//A zsák rekeszeinek feltöltése a csempék kezdeti darabszámával
		d[i] = NULL;			//A dobottak zsák nullázása
		a[i] = NULL;			//Az asztal közepének nullázása
	}
	a[CSVARIACIOK] = 1;			//Itt tesszük le az asztal közepére a kezdõjátékos jelzõt
}

// 3. VÉLETLEN GENERÁLÁSA
int veletlen(int x)
{

	return rand() % x + 1;
}

// 4. CSEMPE ELÕHÚZÁSA A ZSÁKBÓL
int CsempeHuzas(const int hzs[])	//hzs[]: a struktúra zsák eleme, azaz a csempezsák
{
	srand((unsigned)time(NULL));	//véletlen-generátor inicializálása
	int csdb = 0;					//a zsákban maradt csempék száma
	for (int i = 0; i < CSVARIACIOK; i++)
	{
		csdb += hzs[i];				//A zsák[]-ban lévõ, különbözõ színû csempék számát (a tömb egyes rekeszeiben tárolt értékeket) összeadja
	}
	int v = rand() % csdb + 1;		//véletlen szám
	int cs = -1;					//kivalasztott csemperekesz azonosítója
	int cskeres = 0;				//Keresõváltozó
	do											//Azt keresi, hogy az elõbb generált véletlen szám melyik színû csempét jelöli:
	{
		cs++;									//A zsák[] tömb indexét növeli (azaz lépdel a különbözõ színû csempék között elõre)
		cskeres += hzs[cs];						//A zsák[] tömb adott indexû elemének értékét hozzáadja a keresõváltozóhoz.
	} while (cskeres < v && cs < CSVARIACIOK);	//Ha a véletlen szám a keresõváltozónál kisebb vagy egyenlõ, akkor a keresett csempeszínt a zsák[] aktuális indexe jelöli.
	return cs;
}

// 5. KORONGOK FELTÖLTÉSE
int KorongTolto(string korongok[], int zsak[], int dobott[], int korongszam)
{
	//Ha a Zsák üres, fel kell tölteni a Dobottakból, ha ott sincs semmi, akkor elfogytak a csempék és vége a játéknak.

	int zscsdb = 0;							//a zsákban maradt csempék száma
	int dcsdb = 0;							//a dobott zsákban lévõ csempék száma
	bool jatekvege = false;					//Ha elfogott minden csempe a zákból és a dobottak közül, akkor vége a játéknak
	for (int i = 0; i < CSVARIACIOK; i++)
	{
		zscsdb += zsak[i];					//A zsák[]-ban lévõ, különbözõ színû csempék számát (a tömb egyes rekeszeiben tárolt értékeket) összeadja
		dcsdb += dobott[i];					//A dobott zsákban lévõ csempék számát adja össze
	}
	if (zscsdb == 0)
	{						//A Zsák üres...
		if (dcsdb == 0)
		{					//...és a Dobottak is üres...
			jatekvege = true;
		} 
		else
		{
			for (int i = 0; i < CSVARIACIOK; i++)
			{
				zsak[i] = dobott[i];				//Átpakoljuk a dobottakban tárolt értékeket a zsákba.
				dobott[i] = 0;
			}
		}
	}
	if (jatekvege)
	{
		return 1;
	}
	else
	{

	//A Zsák még nem üres, úgyhogy lehet húzni a csempéket a korongokra:

		string csempek="\0";					//Húzás közben ide gyûjti a kihúzott csempéket
		for (int i = 0; i < korongszam; i++)	//Egyesével végigmegy a korongokon
		{
			for (int j = 0; j < KMERET; j++)	//Annyi csempét húz a zsákból, ahány elfér egy korongon
			{
				int kihCs = CsempeHuzas(zsak);	//egy kihúzott csempe, azaz a zsák[] tömb egyik rekeszének indexsze
				csempek += char(65 + kihCs);	//A csempék változóba gyûjtjuk a karaktereket a kihúzott tömbindex alapján.
				zsak[kihCs]--;					//A kihúzott csempefajtából egyel kevesebb marad a zsákban.
			}
			korongok[i] = csempek;				//A kihúzott csempék az éppen soron lévõ korongra mennek
			csempek = "\0";						//Nullázza gyûjtõ változót
		}
		GKorongListazas(korongok, korongszam, "Korongok tartalma a feltoltes utan:");
		return 0;
	}
}

// 6. KORONGOKON ÉS ASZTAL KÖZEPÉN LÉVÕ CSEMPÉK MEGJELENÍTÉSE
void KorongAsztalMegj(string korong[], int asztal[], int ksz)
{
	cout << "A KORONGOK TARTALMA:\n";
	for (int i = 0; i < ksz; i++)
	{
		cout << i + 1 << ". korong: " << korong[i] << endl;
	}
	cout << "\nAZ ASZTAL TARTALMA:\n";
	for (int i = 0; i < CSVARIACIOK; i++)
	{
		cout << char(65 + i) << " csempe: " << asztal[i] << "db" << endl;
	}
	if (asztal[CSVARIACIOK] == 1)											 //Ha ott van még a kezdõjátékos jelzõ (asztal[5]==1), akkor azt is kiírja
	{
		cout << "+ a Kezdojatekos jelzo (K)" << endl;
	}
}




int main ()
{

	Game g;						//Változó a Game struktúrával

	int zsak[CSVARIACIOK];		//A zsák, amiben a csempék vannak.
	int dobott[CSVARIACIOK];	//Az eldobott csempék ide kerülnek.
	int kozep[CSVARIACIOK + 1];	//Az asztal közepén lévõ csempék + a kezdõjátékosjelölõ
	int jsz = JatekosSzamBe();	//A játékosok száma.

	g.KorongSzam = jsz * 2 + 1;							//A korongok száma (a g változóban) egyel több, mint a játékosok számának kétszerese
	KezdoZsakok(g.Zsak, g.Eldobott, g.AsztalKozep);		//A Zsák, Eldobottak zsákja és az Asztal közepének elõkészítése
	g.Korongok = new string[g.KorongSzam];				//A Korongok tömb elkészítése a játékosszám alapján

	int vege = KorongTolto(g.Korongok, g.Zsak, g.Eldobott, g.KorongSzam);
	if (vege == 1)
	{
		cout << "\n\nJATEK VEGE\n\nNincs tovabb, mert elfogyott minden korong!" << endl;
		return 0;
	}
	KorongAsztalMegj(g.Korongok, g.AsztalKozep, g.KorongSzam);
	GZsakListazas(g.Zsak, g.Eldobott, g.AsztalKozep, CSVARIACIOK, "Korongok, Dobottak es Asztal tartalma a KorongTolto utan.");


	//GKorongListazas(g.Korongok, g.KorongSzam, "A strukturaban levo korongok tartalma letrehozas utan: ");

	//KorongKeszito(g.Korongok, jsz * 2 + 1);


		
		
	//KezdoZsak(zsak); 


	//KorongKeszito(jsz); // itt majd vissza kell adnia a tömb címét!
	//KorongTolto(zsak, jsz);

	/*
	for (int i = 0; i < g.KorongSzam; i++)
	{
		g.Korongok[i] = "teszt";
	}
	KorongAsztalMegj(g.Korongok, g.AsztalKozep, g.KorongSzam);
	*/



	delete[]KTOMB;
	return 0;
}









// 5. KORONGOK ELKÉSZÍTÉSE
int KorongKeszito(string korong[], int ksz)
{
	korong = new string[ksz]; //A korongok tömb készítése a játékosszámnak megfelelõen
	for (int i = 0; i < ksz; i++)
	{
		korong[i] = "456";
	}
	return 0;



}





