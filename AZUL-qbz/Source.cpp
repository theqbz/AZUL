#include"iostream"
#include"iomanip"
#include"cmath"
#include"cstdlib"
#include"ctime"
#include"string"

using namespace std;


#define CSVARIACIOK 5	// Ennyi különbözõ csempefajta van a játékban.
#define TABLAMERET 5	// A Mintasor illetve a Fal méretei a játékosok tábláján. (Általában megegyezik a csempefajták számával azaz CSVARIACIOK változóval.)


struct Game								// A játék fõ adatai
{
	string* Korongok;					// A korongok tömbje
	int KorongSzam;						// A korongok száma
	int Zsak[CSVARIACIOK];				// A zsák tömbjének helye 
	int Dobott[CSVARIACIOK];			// Az eldobott csempék tömbjének helye
	int AsztalKozep[CSVARIACIOK + 1];	// Az asztal közepét tároló tömb helye
};

struct Player							// A játékosokhoz tartozó adatok
{
	string Nev;							// A játékos neve
	int Pontok;							// A játékos pontszáma
	string Mintasor[TABLAMERET];		// A mintasor, ahová a korongokról vagy asztalról vásárolt csempéket gyûjti.
	string Fal[TABLAMERET];				// A fal, ahová felteszi a mintasoron összegyûlt csempéket.
	string Padlovonal;					// A padlóvonal ahol a büntetõpontok gyûlnek.
	bool kovetkezik;					// Azt mutatja, hogy a játékos következik-e
};



// KORONGOK ÉS ASZTAL KÖZEPÉN LÉVÕ CSEMPÉK MEGJELENÍTÉSE
// A függvényhívásba be kell küldeni a Game struktúrával létrehozott változó részeit:
// KorongAsztalMegjelenites(g.Korongok, g.AsztalKozep, g.Zsak, g.Dobott, g.KorongSzam);
//
// A Zsák és a Dobottak megjelenítését nem kéri a feladat és az igazi játékban se látjuk,
// hogy mi van bennük, úgyhogy azt csak azért tettem hozzá, hogy látszódjon a program mûködése, 
// de ki lehet venni természetesen.

void KorongAsztalMegjelenites(string korong[], int asztal[], int zsak[], int dobott[], int ksz)
{
	cout << "Korongok:\t\tAsztal:\t\tZsák:\t\tDobott:" << endl;	// A táblázat fejléce
	if (asztal[CSVARIACIOK] == 1)								// Ha a kezdõjátékos jelölõ az asztalon van, akkor kiírja
	{
		cout << "\t\t\tKezdõ" << endl;
	}
	for (int i = 0; i < CSVARIACIOK; i++)						// Elõszor csak addig számolunk el, amíg minden tömb biztosan tartalmaz elemeket azaz, addig ahány csempefajta van (CSVARIACIOK)
	{
		cout << i + 1 << ".: " << korong[i] << "\t\t"
			<< char(65 + i) << ": " << asztal[i] << "\t\t"
			<< char(65 + i) << ": " << zsak[i] << "\t\t"	//!! Ezt a sort húzd ki, ha nem akarod megjeleníteni a zsákot
			<< char(65 + i) << ": " << dobott[i] << endl;	//!! Ezt a sort húzd ki, ha a dobottakat nem akarod megjeleníteni
	}
	if (ksz > CSVARIACIOK)										// Ha több a korong, mint ahány csempefajta van, akkor már csak a korongok tartalmát írjuk ki
	{
		for (int i = CSVARIACIOK; i < ksz; i++)
		{
			cout << i + 1 << ".: " << korong[i] << endl;
		}
	}
	cout << endl;
}



// PLAYER MEGJELENÍTÉSE
// Híváskor ebbe be kell küldeni a Player struktúrával létrehozott tömböt és a játékosok számát:
// PlayerMegjelenites(j, jsz);

void PlayerMegjelenites(Player j[], int jsz)
{
	const int fs = 7;							// A fejlécbe kiírandó sorok száma
	const int ps = 3;							// A padlóvonalra kiírandó sorok száma
	const int soroksz = fs + TABLAMERET + ps;	// A kiíandó sorok száma összesen: a fejléc, a tábla és a padlóra kiírandó sorok száma
	string kiiras[soroksz];						// Ebbe gyûjtjük a játékos elõtti tábla kiírandó tartalmát soronként
	string toltelek;							// Helykitöltõ szóközök a Mintasor elé, hogy balra zárt legyen
	string elv = " | ";							// Ezzel választja el egymástól a játékosok tábláit
	string vonal = "----------------";			// Ez lesz a táblázat alsó és felsõ vonala

	for (int i = 0; i < soroksz; i++)			// A táblázat bal oldala
	{
		kiiras[i] = "| ";
	}

	// A játékosok tábláinak kiírása egymás mellé

	for (int kisz = 0; kisz < jsz; kisz++)
	{
		toltelek = "\0";

		for (int i = 0; i < TABLAMERET - 1; i++)					// A töltelékkarakterek elkészítése
		{
			toltelek += " ";
		}

		// itt kerül bele a fejléc tartalma

		kiiras[0] += vonal + elv;
		kiiras[1] += to_string(kisz + 1) + ". JÁTÉKOS:     " + elv;
		kiiras[2] += j[kisz].Nev.substr(0, 16);						// A névnek legfeljebb az elsõ 16 karaktere lesz kiírva
		int k2h = 16 - j[kisz].Nev.length();						// A [2]-hez szükséges szóközök száma
		for (int i = 0; i < k2h; i++)								// Feltöltjük a szükséges mennyiségû szóközzel
		{
			kiiras[2] += " ";
		}
		kiiras[2] += elv;
		kiiras[3] += "(" + to_string(j[kisz].Pontok) + " pont)";
		int k3h = 16 - to_string(j[kisz].Pontok).length() - 7;		// A [3]-hoz szükséges szóközök száma
		for (int i = 0; i < k3h; i++)								// Feltöltjük a szükséges mennyiségû szóközzel
		{
			kiiras[3] += " ";
		}
		kiiras[3] += elv;
		kiiras[4] += "---------+------" + elv;
		kiiras[5] += "         | 12345" + elv;
		kiiras[6] += "---------+------" + elv;

		// itt kerül bele a Mintasor és a Fal tartalma

		for (int i = 0; i < TABLAMERET; i++)
		{
			kiiras[i + fs] += to_string(i + 1) + ". " + toltelek + j[kisz].Mintasor[i] + elv + j[kisz].Fal[i] + elv;
			toltelek = toltelek.substr(0, toltelek.size() - 1);
		}

		// itt kerül be a Padlóvonal tartalma

		kiiras[fs + TABLAMERET] += "---------+------" + elv;
		kiiras[fs + TABLAMERET + 1] += "Padlo: " + j[kisz].Padlovonal;
		int kp1h = 16 - j[kisz].Padlovonal.length() - 7;
		for (int i = 0; i < kp1h; i++)								// Feltöltjük a szükséges mennyiségû szóközzel
		{
			kiiras[fs + TABLAMERET + 1] += " ";
		}
		kiiras[fs + TABLAMERET + 1] += elv;
		kiiras[fs + TABLAMERET + 2] += vonal + elv;
	}

	for (int ki = 0; ki < soroksz; ki++)							// Az elkészült sorok kiírása
	{
		cout << kiiras[ki] << endl;
	}
}
