#include"iostream"
#include"iomanip"
#include"cmath"
#include"cstdlib"
#include"ctime"
#include"string"

using namespace std;

#define CSVARIACIOK 5	// Ennyi különbözõ csempefajta van a játékban.
#define CSKEZDODARAB 20	// Egy adott fajta csempékbõl ennyi darab van összesen.
#define KMERET 4		// A korongokra helyezhetõ csepmpék maximális száma.
#define TABLAMERET 5	// A Mintasor illetve a Fal méretei a játékosok tábláján. (Általában megegyezik a csempefajták számával azaz CSVARIACIOK változóval.)
#define PADLOMERET 7	// A Padlovonal mérete a játékosok tábláján.

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

struct Score							// A végeredmény kijelzéséhez szükséges adatok
{
	string Nev;							// A játékos neve
	int Pont;							// A játékos pontszáma
	int SorokSzama;						// A játékos által betöltött sorok száma
};


// 0/a JÁTSZHATÓ CSEMPÉK ELLENÕRZÉSE
bool ElfogyottACsempe(int zsak[], int dobott[])
{
	bool ureszsak = false;							// Nincs csempe a Zsákban
	bool uresdobott = false;						// Nincs csempe a Dobottakban
	int k = 0;
	while (k < CSVARIACIOK && zsak[k] == 0)			// Addig nézi amíg nem talál legalább egy nem üres rekeszt
	{
		ureszsak = true;							// Akkor kell belemennem, ha üres rekeszt találok, egyébként (azaz nem üres) mehetünk tovább, mert van még csempe
		k++;
	}
	k = 0;
	while (k < CSVARIACIOK && dobott[k] == 0)
	{
		uresdobott = true;
		k++;
	}
	if (ureszsak)
	{
		if (uresdobott)								// Ha elfogyott minden csempe a Zsákból és a Dobottak közül, akkor vége a játéknak
		{
			cout << "~ Elfogyott minden csempe a Zsákból és a Dobottak közül. Vége a játéknak." << endl;
			return true;
		}
		else										// Ha csak a Zsák üres, átpakoljuk a Dobottakban tárolt értékeket a Zsákba
		{
			cout << "~ A kiürült Zsák feltöltése a Dobottakból." << endl;
			for (int i = 0; i < CSVARIACIOK; i++)
			{
				zsak[i] = dobott[i];
				dobott[i] = 0;
			}
		}
	}
	return false;
}

// 0/b A JÁTÉK VÉGÉNEK ELLENÕRZÉSE
bool JatekVege(Player j[], int jsz, int zsak[], int dobott[])
{
	bool vansor = false;										// Sor-detekror
	int i = 0;													// Játékosok indexe
	cout << "~ Játék végének ellenõrzése..." << endl;
	do
	{
		int s = 0;												// A Fal sorainak indexe
		do
		{
			if (j[i].Fal[s].find(".") == j[i].Fal[s].npos)		// Amint valakinél talál teljes sort a Falon, vége a játéknak
			{
				cout << "~ " << j[i].Nev << " falának " << s << ". sora teljes lett. Vége a játéknak." << endl;
				vansor = true;
			}
			s++;
		} while (!vansor && s < TABLAMERET);
		i++;
	} while (!vansor && i < jsz);
	return vansor || ElfogyottACsempe(zsak, dobott);
}

// 0/c FORDULÓ VÉGÉNEK ELLENÕRZÉSE
bool ForduloVege(string korongok[], int ksz, int asztal[])
{
	bool kures = true;					// A Korongok kiürültek-e
	bool aures = true;					// Az asztal üres-e
	int kk = 0;							// Korong-szkennelõ
	int ak = 0;							// Asztal-szkennelõ
	cout << "~ Forduló végének ellenõrzése..." << endl;
	do
	{
		if (korongok[kk] != "....")		// Amint talál egy nem üres Korongot, rögtön hamisra állítja
		{
			kures = false;
		}
		kk++;
	} while (kures && kk < ksz);
	do
	{
		if (asztal[ak] != 0)			// Amint talál csempét az Asztalon, rögtön hamisra állítja
		{
			aures = false;
		}
		ak++;
	} while (aures && ak < CSVARIACIOK);
	return kures && aures;
}

// 0/d ELDÖNTENDÕ KÉRDÉS
bool Dontes(string szoveg)
{
	string valasz = "\0";
	do
	{
		cout << szoveg;
		cin >> valasz;
	} while (valasz != "i" && valasz != "n");
	if (valasz == "i")
	{
		return true;
	}
	else
	{
		return false;
	}
}

// 1/b JÁTÉKOSOK NEVÉNEK BEKÉRÉSE (OPCIONÁLIS) ÉS KEZDÕ KIJELÖLÉSE
void JatekosNevek(Player j[], int jsz)
{
	string kezd = "\0";
	if (Dontes("Megadod a játékosok becenevét? (i / n) "))
	{
		string kuka = "\0";											// A cin ürítése
		getline(cin, kuka, '\n');
		for (int i = 0; i < jsz; i++)
		{
			cout << i + 1 << ". játékos neve: ";
			getline(cin, j[i].Nev, '\n');
		}
	}
	for (int i = 0; i < jsz; i++)									// A "következik" változó alapra állítása
	{
		j[i].kovetkezik = false;
	}
	bool jokezdo = false;
	do
	{
		cout << "Ki kezd? (A játékos sorszámát add meg!) ";
		cin >> kezd;
		if (kezd.length() != 1)
		{
			jokezdo = false;
		}
		else if (int(kezd[0]) < 49 ||								// Ha a megadott szám nagyobb, mint ahány játékos van, akkor nem jó
			int(kezd[0]) - 48 > jsz)
		{
			jokezdo = false;
			cout << "HIBA: Ilyen sorszámú játékos nincs!" << endl;
		}
		else
		{
			jokezdo = true;
		}
	} while (!jokezdo);
	j[int(kezd[0] - 49)].kovetkezik = true;
}

// 1. JÁTÉKOSOK SZÁMÁNAK BEKÉRÉSE
int JatekosSzamBe()
{
	string jszbe = "";
	do
	{
		cout << "Add meg a játékosok számát (2, 3 vagy 4 játékos lehet!): ";
		cin >> jszbe;
	} while (jszbe != "2" && jszbe != "3" && jszbe != "4");
	return int(jszbe[0]) - 48;
}

// 2. ZSÁK, ELDOBOTT ÉS ASZTALKÖZÉP INICIALIZÁLÁSA
void KezdoZsakok(int zs[], int d[], int a[])
{
	for (int i = 0; i < CSVARIACIOK; i++)
	{
		zs[i] = CSKEZDODARAB;				// A zsák rekeszeinek feltöltése a csempék kezdeti darabszámával
		d[i] = 0;							// A dobottak zsák nullázása
		a[i] = 0;							// Az asztal közepének nullázása
	}
	a[CSVARIACIOK] = 1;						// Itt tesszük le az asztal közepére a kezdõjátékos jelzõt
}

// 3/a CSEMPE ELÕHÚZÁSA A ZSÁKBÓL
int CsempeHuzas(int zsak[])
{
	srand((unsigned)time(NULL));				// véletlen-generátor inicializálása
	int csdb = 0;								// a Zsákban maradt csempék száma
	for (int i = 0; i < CSVARIACIOK; i++)
	{
		csdb += zsak[i];						// A zsák[]-ban lévõ, különbözõ színû csempék számát összeadja a véletlen szám generáláshoz
	}
	int cs = rand() % csdb + 1;					// Véletlen szám 1 és a csempék száma közötti értékben

	// Ki kell keresni, hogy a véletlen szám melyik csempét jelöli:

	int i = -1;									// Az aktuális csemperekesz, azaz a zsák[] tömb indexe
	int vektor = 0;								// Keresõvektor
	do
	{
		i++;									// A zsák[] tömb indexét növeli
		vektor += zsak[i];						// Bõvíti a keresõvektort a zsák[] tömb aktuális indexû elemének értékével
	} while (vektor < cs && i < CSVARIACIOK);	// Amikor a keresõvektornál kisebb lesz a véletlen szám, akkor a keresett csempeszínt a zsák[] aktuális indexe jelöli.
	return i;
}

// 3. KORONGOK FELTÖLTÉSE
bool KorongTolto(string korongok[], int zsak[], int dobott[], int asztal[], int korongszam)
{
	string csempek = "\0";							// Húzás közben ide gyûjti a kihúzott csempéket
	cout << "~ Koronok feltöltése..." << endl;
	for (int i = 0; i < korongszam; i++)			// Egyesével végigmegy a korongokon
	{
		for (int j = 0; j < KMERET; j++)			// Annyi csempét húz a zsákból, ahány elfér egy korongon
		{
			if (ElfogyottACsempe(zsak, dobott))		// Ha már nincs csempe a Zsákban vagy a Dobottak között, akkor vége a játéknak
			{
				return true;
			}
			else
			{
				int kihCs = CsempeHuzas(zsak);		// Egy kihúzott csempe, azaz a zsák[] tömb egyik rekeszének indexe
				csempek += char(65 + kihCs);		// A csempék változóba gyûjtjuk a karaktereket a kihúzott tömbindex alapján.
				zsak[kihCs]--;						// A kihúzott csempefajtából egyel kevesebb marad a zsákban.
			}
		}
		korongok[i] = csempek;						// A kihúzott csempék az éppen soron lévõ korongra mennek
		csempek = "\0";								// Nullázza gyûjtõ változót
	}
	asztal[CSVARIACIOK] = 1;						// Kezdõjátékos jelölõ kihelyezése az asztal közepére
	return false;
}

// 4/a KÖVETKEZÕ-JÁTÉKOS JELÖLÕ RESETELÉSE
void KovJatekosReset(Player j[], int jsz)
{
	for (int i = 0; i < jsz; i++)					// A játékosok tömb indexe
	{
		j[i].kovetkezik = false;
	}
}

// 4. A PLAYER STRUKTÚRÁJÚ TÖMB ELÕKÉSZÍTÉSE
int PlayerElokeszites(Player jatekos[], int jsz)
{
	for (int i = 0; i < jsz; i++)
	{
		jatekos[i].Nev = char(48 + i + 1);			// A név helyére egyelõre csak sorszám kerül
		jatekos[i].Pontok = 0;						// A pontok nullázódnak
		jatekos[i].Padlovonal = "\0";				// A padlóvonal üres legyen
		string minta = "\0";						// A Mintasor kirajzolsáshoz szükséges változó
		for (int j = 0; j < TABLAMERET; j++)
		{
			jatekos[i].Mintasor[j] = minta + ".";	// A Mintasor kirajzolása helyõrzõkkel
			jatekos[i].Fal[j] = ".....";			// A Fal kirajzolása helyõrzõkkel
			minta += ".";
		}
		for (int k = 0; k < PADLOMERET; k++)
		{
			jatekos[i].Padlovonal += ".";			// A Padlóvonal kirajzolása helyõrzõkkel
		}
	}
	KovJatekosReset(jatekos, jsz);					// A következõ-játékos jelölõt mindenhol hamisra állítja
	return 0;
}


// 5/a HELYÕRZÕ SZÓKÖZÖK
string Helyorzo(string ertek)
{
	if (ertek.length() == 1)
	{
		return ertek + " ";
	}
	else
	{
		return ertek;
	}
}

// 5. KORONGOKON ÉS ASZTAL KÖZEPÉN LÉVÕ CSEMPÉK MEGJELENÍTÉSE
void KorongAsztalMegj(string korong[], int asztal[], int zsak[], int dobott[], int ksz)
{
	const int sorokszama = 8;				// A kiírandó sorok száma összesen
	string sorok[sorokszama];				// Ide gyûjtjük a kiírandó sorok tartalmát
	string elv = "|  ";						// Elválasztó a táblázat részei között

	for (int i = 0; i < sorokszama; i++)	// A táblázat bal oldala
	{
		sorok[i] = elv;
	}

	// A korongok

	int kszel = (ksz / 2 + 1) * 5;										// Ilyen széles hely kell a korongok megjelenítéséhez
	sorok[0] += "Korongok:";											// A korongok rész címsora
	for (int k = 0; k < kszel - 9; k++)									// Kitölti a szükséges mennyiségû szóközzel a korongok számának függvényében
	{
		sorok[0] += " ";
	}
	sorok[0] += elv;
	for (int k = 0; k < ((ksz / 2) + 1); k++)							//A korongok számozása: 1. sor
	{
		sorok[1] += to_string(k + 1) + ".   ";
	}
	sorok[1] += elv;
	for (int i = 0; i < 2; i++)											// A korongok tartalma: elsõ két sor
	{
		for (int k = 0; k < ((ksz / 2) + 1); k++)						// Elszámol a korongok számának felénél egyel tovább...
		{
			sorok[i + 2] += korong[k].substr(i * 2, 2) + "   ";	// ...és az aktuális korongról kettesével kiírja a csempéket (elõbb az elsõ kettõt aztán a második kettõt)
		}
		sorok[i + 2] += elv;
	}
	for (int k = 0; k < kszel; k++)										// Elválasztó üres sor
	{
		sorok[4] += " ";
	}
	sorok[4] += elv;
	for (int k = ((ksz / 2) + 1); k < ksz; k++)							//A korongok számozása: 2. sor
	{
		sorok[5] += to_string(k + 1) + ".   ";
	}
	sorok[5] += "     " + elv;
	for (int i = 0; i < 2; i++)											// A korongok tartalma: másdoik két sor
	{
		for (int k2 = ((ksz / 2) + 1); k2 < ksz; k2++)					// A számolás a korongok számának felénél egyel többrõl indul és a korongok számáig tart...
		{
			sorok[i + 6] += korong[k2].substr(i * 2, 2) + "   ";		// ...és az aktuális korongról kettesével kiírja a csempéket
		}
		sorok[i + 6] += "     " + elv;
	}

	// Az Asztal

	sorok[0] += "Asztal:    " + elv;									// Az asztal rész címsora
	for (int i = 0; i < CSVARIACIOK; i++)								// A csempék számának kiírása
	{
		sorok[i + 1] += char(65 + i);
		sorok[i + 1] += ": " + to_string(asztal[i]) + "       " + elv;
	}
	sorok[6] += "           " + elv;									// Üres sor
	if (asztal[CSVARIACIOK] == 1)										// Ha még az asztalon van a kezdõjátékosjelölõ...
	{
		sorok[7] += "- Kezdõ     " + elv;								// ...felírja azt is...
	}
	else
	{
		sorok[7] += "           " + elv;								// ...különben üres sort ír.
	}

	// A Zsák és Dobottak

	sorok[0] += "Zsák:           ";
	for (int i = 0; i < CSVARIACIOK; i++)
	{
		sorok[1] += char(65 + i);
		sorok[1] += "  ";
	}
	for (int j = 0; j < CSVARIACIOK; j++)
	{
		sorok[2] += Helyorzo(to_string(zsak[j])) + " ";
	}
	sorok[3] += "                ";
	sorok[4] += "Dobott:         ";
	for (int i = 0; i < CSVARIACIOK; i++)
	{
		sorok[5] += char(65 + i);
		sorok[5] += "  ";
	}
	for (int j = 0; j < CSVARIACIOK; j++)
	{
		sorok[6] += Helyorzo(to_string(dobott[j])) + " ";
	}


	for (int ki = 0; ki < sorokszama; ki++)					// Sorok kiírása
	{
		cout << sorok[ki] << endl;
	}
	cout << endl;
}

// 6a PLAYER MEGJELENÍTÉSE
void PlayerMegj(Player j[], int jsz, int aj)
{
	const int fs = 4;							// A fejlécbe kiírandó sorok száma
	const int ps = 3;							// A padlóvonalra kiírandó sorok száma
	const int ssz = fs + TABLAMERET + ps;		// A kiíandó sorok száma összesen: a fejléc, a tábla és a padlóra kiírandó sorok száma
	const int tszel = (PADLOMERET * 3) + 2;		// Az táblák szélessége
	string sorok[ssz];							// Ebbe gyûjtjük a játékos elõtti tábla kiírandó tartalmát soronként

	for (int i = 0; i < ssz; i++)				// Sorok nullázása
	{
		sorok[i] = "\0";
	}

	for (int ji = 0; ji < jsz; ji++)			// A játékosok indexe
	{
		// A kiírás elõkészítése: vonal és töltelék

		string vonal = "\0";							// Ez lesz a táblázat vízszintes vonala
		int sszel = tszel * (ji + 1);					// A gyarapodó sorok aktuális maximum szélessége
		if (ji == aj)									// Ha az adott játékos következik, akkor a vízszintes vonal *-karakterbõl áll
		{
			for (int i = 0; i < tszel - 1; i++) { vonal += "*"; }
		}
		else
		{
			for (int i = 0; i < tszel - 1; i++)
			{
				if (i == TABLAMERET + 5) { vonal += "+"; }
				else { vonal += "-"; }
			}
		}
		string toltelek = "\0";							// Helykitöltõ szóközök a Mintasor elé, hogy balra zárt legyen
		for (int i = 0; i < TABLAMERET - 1; i++)		// A töltelékkarakterek elkészítése
		{
			toltelek += " ";
		}

		// A tábla bal oldala

		for (int i = 0; i < ssz; i++)
		{
			sorok[i] += "|";
		}

		// Fejléc
		sorok[0] += vonal;
		sorok[1] += " " + to_string(ji + 1) + " - ";				// A játékos sorszáma
		sorok[1] += j[ji].Nev.substr(0, tszel - 7);					// Név (csonkolva, ha hosszú)
		sorok[2] += " " + to_string(j[ji].Pontok) + " pont";		// Pontszám
		while (sorok[3].length() < sszel - (TABLAMERET * 2))		// A 4. sor elejét tölti fel szóközzel
		{
			sorok[3] += " ";
		}
		for (int i = 0; i < TABLAMERET; i++)						// A 4. sor végét feltölti az oszlopok sorszámával
		{
			sorok[3] += to_string(i + 1) + " ";
		}

		// Mintasor és Fal

		for (int i = 0; i < TABLAMERET; i++)
		{
			sorok[i + fs] += "    " + toltelek + j[ji].Mintasor[i];	// Töltelék és a Mintasor tartalma
			sorok[i + fs] += " " + to_string(i + 1) + " ";			// Sorszám a Mintasor és a Fal között
			for (int f = 0; f < TABLAMERET; f++)
			{
				sorok[i + fs] += j[ji].Fal[i][f];					// A Fal tartalma szóközökkel elválasztva
				sorok[i + fs] += " ";
			}
			toltelek = toltelek.substr(0, toltelek.size() - 1);		// Egyel csökkenti a töltelék hosszúságát
		}

		// Padlóvonal

		sorok[fs + TABLAMERET] += vonal;
		int p = 0;													// Padlóvonal aktuális karaktere
		while (p < PADLOMERET
			&& sorok[fs + TABLAMERET + 1].length() < sszel)
		{
			sorok[fs + TABLAMERET + 1] += "  ";
			sorok[fs + TABLAMERET + 1] += j[ji].Padlovonal[p];
			sorok[fs + TABLAMERET + 2] += " " + to_string(-(((p + 1) / 3) + 1));
			p++;
		}

		// Végül a maradékot minden sorban feltölti szóközzel

		for (int u = 0; u < ssz; u++)
		{
			while (sorok[u].length() < sszel)
			{
				sorok[u] += " ";
			}
		}
	}

	for (int ki = 0; ki < ssz; ki++)			// Az elkészült sorok kiírása
	{
		cout << sorok[ki] << "|" << endl;
	}
	cout << endl;
}

// 7/a JÁTÉKOS KORONG- ÉS CSEMPEVÁLASZTÁSA VÁSÁRLÁSHOZ
string KCsValaszto(Player j[], string korong[], int korsz, int asztal[], int aj)
{
	string kivcs;				// A kiválasztott csempe
	bool jocsvalasztas = false;	// A játékos csempeválasztó bevitelének ellenõrzese

	do
	{
		cout << j[aj].Nev << ", válassz csempéket egy korongról vagy az asztalról!\n(Pl. 1. korong A csempe: 1A | Az asztalt a 0 jelöli: 0D) ";
		cin >> kivcs;

		if (kivcs == "x" || kivcs == "X")						// Ha ki szeretne lépni rögtön megy is vissza
		{
			jocsvalasztas = true;
		}
		else if (kivcs.length() != 2)							// Ha nem két karaktert írt be, már nem jó
		{
			jocsvalasztas = false;
			cout << "HIBA: nem ket karakter!" << endl;
		}
		else if (int(kivcs[0]) < 48 ||
			int(kivcs[0]) - 48 > korsz)							// Ha az elsõ karakter nem, egy a korongok számánál kisebb szám
		{
			jocsvalasztas = false;
			cout << "HIBA: Ilyen korong nincs a játékban!" << endl;
		}
		else if (int(kivcs[1]) < 65
			|| int(kivcs[1]) - 65 > CSVARIACIOK)				// Ha második karakter nem egy olyan NAGYBETÛ, ami megfelel a csempék betújeleinek
		{
			jocsvalasztas = false;
			cout << "HIBA: Ilyen betûjelû csempe nincs a játékban!" << endl;
		}
		else if (int(kivcs[0]) - 48 != 0						// Ha korongot választott és az üres, akkor nem jó
			&& korong[int(kivcs[0]) - 49] == "....")
		{
			jocsvalasztas = false;
			cout << "HIBA: A választott korong üres!" << endl;
		}
		else if (int(kivcs[0]) - 48 == 0						// Ha az asztalt választotta, és egy sincs rajta a kért csempébõl, akkor nem jó
			&& asztal[int(kivcs[1]) - 65] == 0)
		{
			jocsvalasztas = false;
			cout << "HIBA: A választott csempébõl nincs az asztalon!" << endl;
		}
		else if (int(kivcs[0]) - 48 != 0						// Ha a korongot választott de nincs rajta a kért csempe, akkor nem jó
			&& korong[int(kivcs[0]) - 49].find(kivcs[1])
			== korong[int(kivcs[0])].npos)
		{
			jocsvalasztas = false;
			cout << "HIBA: A korongon nincs ilyen csempe!" << endl;
		}
		else
		{
			jocsvalasztas = true;
		}
	} while (!jocsvalasztas);

	return kivcs;
}

// 7/b A JÁTÉKOS MINTASOR VÁLASZTÁSA VÁRÁSLÁSHOZ
char SorValaszto(Player j[], string cs, int aj)
{
	string kivs;			// A kiválasztott sor
	bool josor = false;		// A játékos Mintasor választásának ellenõrzése
	bool vanhely = false;	// A mintasorok telítettségének ellenõrzése
	int szk = 0;			// Változó a szabad hely kereséséhez

	do																	// Van-e még egyáltalán olyan mintasor, ahová csempét lehet rakni?
	{
		if (j[aj].Mintasor[szk].find(".") == j[aj].Mintasor[szk].npos)	// Ha a helykitöltõ '.' karaktert nem találja az aktuális Mintasorban...
		{
			vanhely = false;											// ...akkor ott már nincs szabad hely...
		}
		else
		{
			vanhely = true;												// ...különben még van szabad hely a Mintasorok valamelyikében
		}
		szk++;
	} while (!vanhely && szk < TABLAMERET);

	if (vanhely)
	{
		do
		{
			cout << "Melyik Mintasorba szeretnéd tenni? (1-tõl " << CSVARIACIOK << "-ig | A Padlót a 0-val adhatod meg.) ";
			cin >> kivs;
			if (kivs == "x" || kivs == "X")								// Ha a kilépést választja, már megy is vissza
			{
				josor = true;
			}
			else if (int(kivs[0]) - 48 > TABLAMERET)					// Ha a megadott szám nagyobb, mint ahány mintasor van, akkor nem jó
			{
				josor = false;
				cout << "HIBA: Ilyen sorszámú Mintasor nincs!" << endl;
			}
			else if (kivs != "0" &&										// Ha a játékos nem a padlót választotta...
				j[aj].Mintasor[int(kivs[0]) - 49][0] != '.'				// ...ésa kiválasztott Mintasor már nem üres...
				&& j[aj].Mintasor[int(kivs[0]) - 49][0] != cs[0])		// ...és nincs benne a kiválasztott fajtájú csempe, akkor nem jó
			{
				josor = false;
				cout << "HIBA: Abban a sorban már másfajta csempék vannak!" << endl;
			}
			else
			{
				josor = true;
			}
		} while (!josor);
	}
	else
	{
		kivs = "0";			// Ha már minden Mintasor tele van, akkor a csempék mindenképpen a Padlóvonalra kerülnek
	}
	return kivs[0];
}

// 7/c CSEMPE A PADLÓRA VAGY A DOBOTTAK KÖZÉ KERÜL CSEMPEVÁSÁRLÁSKOR
void CsempeAPadlora(Player j[], string csempe, int dobott[], int aj)
{
	if (j[aj].Padlovonal.find(".") == j[aj].Padlovonal.npos
		&& csempe != "K")
	{
		cout << "~ A Padló tele van, ezért egy " << csempe << " csempét el kell dobni." << endl;
		dobott[int(csempe[0]) - 65]++;										// Ha a Padló tele és nem a Kezdõ-jelölõt kell átrakni, akkor a Csempe a Dobottak közé kerül
	}
	else if (j[aj].Padlovonal.find(".") != j[aj].Padlovonal.npos)
	{
		j[aj].Padlovonal[j[aj].Padlovonal.find_first_of(".")] = csempe[0];	// Ha van hely a Padlón akkor a Csempe vagy Kezdõ-jelölõ oda kerül
		cout << "~ Egy " << csempe << " cseme a Padlóra került." << endl;
	}
	// Ha a Kezdõ-jelölõt kell átrakni de nincs hely a padlón, akkor nincs teendõ, mert a Kezdõt nem lehet a dobottak közé tenni!
}

// 7/d CSEMPÉK ÁTADÁSA A JÁTÉKOSNAK VAGY A PADLÓRA CSEMPEVÁSÁRLÁSKOR
void CsempeAtadas(Player j[], string csempe, int msor, int dobott[], int aj)
{
	if (msor == 0)																				// A játékos a Padlót választotta vagy a teli sor miatt a Padlóra kerülnek a csempék
	{
		cout << "~ A(z) " << csempe << " csempe a padlóra kerül." << endl;
		CsempeAPadlora(j, csempe, dobott, aj);
	}
	else
	{
		if (j[aj].Mintasor[msor - 1].find(".") == j[aj].Mintasor[msor - 1].npos)				// Ha a mintasor már megtelt, akkor a csempe a padlóra kerül
		{
			cout << "~ A(z) " << msor << " már tele van, ezért a(z) " << csempe << " csempe a padlóra kerül." << endl;
			CsempeAPadlora(j, csempe, dobott, aj);
		}
		else
		{
			cout << "~ A(z) " << csempe << " csempe a(z) " << msor << ". Mintasorba kerül." << endl;
			j[aj].Mintasor[msor - 1][j[aj].Mintasor[msor - 1].find_first_of(".")] = csempe[0];	// A Csempe a kiválasztott Mintasorba kerül
		}
	}
}

// 7. CSEMPÉK VÁSÁRLÁSA KORONGOKRÓL VAGY ASZTALRÓL
bool CsempeVasar(Player j[], string korongok[], int ksz, int asztal[], int zsak[], int dobott[], int aj, int jsz)
{
	string kivkcs = KCsValaszto(j, korongok, ksz, asztal, aj);	// A kiválasztott korong és csempe
	if (kivkcs == "x" || kivkcs == "X") { return true; }		// Ha ki akar lépni, rögtön megy is vissza
	int korong = int(kivkcs[0]) - 48;							// A kiválasztott korong
	string csempe = kivkcs.substr(1, 1);						// A kért csempe
	int msor = int(SorValaszto(j, csempe, aj)) - 48;			// A kiválasztott Mintasor ahová a csempék kerülnek
	if (msor == 40 || msor == 72) { return true; }				// Ha ki akar lépni, máris megy vissza

	while (msor != 0
		&& j[aj].Mintasor[msor - 1].find(".") == j[aj].Mintasor[msor - 1].npos)	// a nem a padlót választotta a játékos és a kiválasztott sor tele van...
	{
		if (Dontes("Ebben a sorban már nincs szabad hely ezért a csempék a Padlóra kerülnek. Biztosan ezt akarod? (i / n) "))
		{
			msor = 0;															// Ha így jó neki, akkor a csempék mennek a padlóra
		}
		else
		{
			msor = SorValaszto(j, csempe, aj) - 48;								// Ha nem jó, akkor választhat másik Mintasort
		}
	}

	if (korong == 0)											// Ha az asztalt választotta
	{
		if (asztal[CSVARIACIOK] == 1)							// Ha még az asztalon van a kezdõjátékos jelölõ
		{
			cout << "~ A kezdõjátékos-jelölõ áthelyezése a padlóvonalra." << endl;
			CsempeAPadlora(j, "K", dobott, aj);					// Elõször átadja a padlóvonalra
			asztal[CSVARIACIOK] = 0;							// Leveszi az Asztalról a kezdõjátékos jelölõt
			KovJatekosReset(j, jsz);							// Mindenkinél hamisra állítja a következõ-játékos jelölõt
			j[aj].kovetkezik = true;							// Az aktuális játékos kezdi majd a következõ kört
			cout << "~ A következõ fordulót " << j[aj].Nev << " kezdi." << endl;
		}
		for (int i = 0; i < asztal[int(csempe[0]) - 65]; i++)
		{
			CsempeAtadas(j, csempe, msor, dobott, aj);			// Egesével kell átadni az asztalon lévõ csempéket
		}
		asztal[int(csempe[0]) - 65] = 0;						// Végül nullázza a csempék helyét az asztalon
		cout << "~ A kiválasztott csempék áthelyezése" << endl;
	}
	else														// Ha valamelyik korongot választotta
	{
		for (int i = 0; i < KMERET; i++)						// Végigmegy a korongon tárolt csempéken
		{
			if (korongok[korong - 1].substr(i, 1) == csempe)
			{
				CsempeAtadas(j, csempe, msor, dobott, aj);		// Ha az aktuális csempe a kért csempe, akkor átadja a játékosnak vagy a padlóra
			}
			else
			{
				asztal[int(korongok[korong - 1][i]) - 65]++;	// A nem kívánt csempéket az asztalra teszi
			}
		}
		korongok[korong - 1] = "....";							// Végül a korong ürítése
	}
	return false;
}

// 8/a MINTASOROK NULLÁZÁSA
void MintasorNullazas(Player j[], int aktj, int akts)
{
	string minta = "\0";							// A Mintasor kirajzolsáshoz szükséges változó
	for (int i = 0; i < akts + 1; i++)				// A soron belüli pozíció
	{
		j[aktj].Mintasor[akts] = minta + ".";
		minta += ".";
	}
}

// 8/b PADLÓ NULLÁZÁSA
void PadloNullazas(Player j[], int aj)
{
	j[aj].Padlovonal = "\0";						// A padlóvonal üres legyen
	for (int k = 0; k < PADLOMERET; k++)			// Padlóvonalon belüli pozíció
	{
		j[aj].Padlovonal += ".";					// A Padlóvonal kirajzolása helyõrzõkkel
	}
}

// 8/c PONTSZÁMÍTÁS FALAZÁSKOR
int FalPontok(Player j[], int aj, int akts, int kivo)
{
	int pont = 1;								// A játékos által kapott pontok (egyet már biztosan kap, mert felfalazta a csempét)
	bool vansor = false;						// Találat vízszintes keresésénél
	bool vanoszlop = false;						// Találat függõleges keresésénél

	if (kivo > 0)
	{
		int ki = kivo;							// Keresõ index
		while (ki > 0 
			&& j[aj].Fal[akts][ki - 1] != '.')
		{
			pont++;
			ki--;
			vansor = true;
		}
	}
	if (kivo < TABLAMERET - 1)
	{
		int ki = kivo;
		while (ki < TABLAMERET - 1 
			&& j[aj].Fal[akts][ki + 1] != '.')
		{
			pont++;
			ki++;
			vansor = true;
		}
	}
	if (akts > 0)
	{
		int ki = akts;
		while (ki > 0 
			&& j[aj].Fal[ki - 1][kivo] != '.')
		{
			pont++;
			ki--;
			vanoszlop = true;
		}
	}
	if (akts < TABLAMERET - 1)
	{
		int ki = akts;
		while (ki < TABLAMERET - 1 
			&& j[aj].Fal[ki + 1][kivo] != '.')
		{
			pont++;
			ki++;
			vanoszlop = true;
		}
	}
	if (vansor && vanoszlop)
	{
		pont++;
	}
	cout << j[aj].Nev << ", " << pont << " pontot kaptál a csempe felrakásáért." << endl;
	return pont;
}

// 8/d PONTLEVONÁS A PADLÓ MIATT
int PadloPontok(Player j[], int aj)
{
	int buntipontok = 0;								// A padlóra esett csempékért járó büntetõpontok

	for (int p = 0; p < PADLOMERET; p++)				// A padlón belüli pozíció
	{
		if (j[aj].Padlovonal[p] != '.')
		{
			buntipontok += -(((p + 1) / 3) + 1);
		}
	}
	cout << j[aj].Nev << ", " << buntipontok << " büntetõpontot kaptál a padlóra esett csempék miatt." << endl;
	return buntipontok;
}

// 8/e FAL FÜGGÕLEGES (OSZLOP) VIZSGÁLATA FALAZÁSHOZ
bool JoOszlop(Player j[], int aj, int ao, string csempe)
{
	bool joo = true;							// Az oszlop alkalmassága
	int i = 0;									// Oszlop-szkennelõ

	do
	{
		if (j[aj].Fal[i][ao] == csempe[0])		// Amint talál egyet a megfelelõ csempébõl hamisra állítja
		{
			joo = false;
		}
		i++;
	} while (joo && i < TABLAMERET);

	return joo;
}

// 8/f CSEMPÉK ELDOBÁSA FALAZÁSKOR
void CsempeEldobas(Player j[], int dobott[], int aj, int msor)
{
	int i = 0;
	if (msor == 0)												// Ha a Padlóról kell eldobni csempét
	{
		while (i < PADLOMERET && j[aj].Padlovonal[i] != '.')
		{
			if (j[aj].Padlovonal[i] == 'K')						// Ha az eldobandó csempe a Kezdõjátékos-jelölõ, akkor nem kerül a Dobottak közé
			{
				i++;
			}
			else
			{
				dobott[int(j[aj].Padlovonal[i]) - 65]++;
				i++;
			}
		}
	}
	else														// Ha a Mintasorból kell eldobni csempét
	{
		while (i < j[aj].Mintasor[msor - 1].length() - 1)
		{
			dobott[int(j[aj].Mintasor[msor - 1][i]) - 65]++;
			i++;
		}
	}
}

// 8. FALAZÁS
bool Falazas(Player j[], string korong[], int asztal[], int zsak[], int dobott[], int ksz, int jsz)	//j[]: A játékosok tömbje, g: Game változó, dobott[]: a dobott csempék tömbje, jsz: játékosok száma
{
	bool jovalasz = false;					// A megfelelõ oszlop kivalasztasa
	string kivo = "\0";						// A kiválasztott oszlop
	string csempe = "\0";					// Az éppen manipulált csempe
	int osszespluszp = 0;					// A csempék falazásáért járó pontok
	int buntetopontok = 0;					// A padlóra esett cempék miatt kapott büntetõpontok

	cout << "* * * Kezdõdik a falazás! * * *" << endl;

	for (int aj = 0; aj < jsz; aj++)														// Aktuális játékos sorszáma
	{
		for (int s = 0; s < TABLAMERET; s++)												// Mintasor szorszáma
		{
			if (j[aj].Mintasor[s].find(".") == j[aj].Mintasor[s].npos)						// Ha az aktuális Mintasor nincs tele
			{
				if (j[aj].Fal[s].find(j[aj].Mintasor[s][0]) == j[aj].Fal[s].npos)			// Ha a Falon abban a sorban még nincs a Mintasorban lévõkkel azonos csempe
				{
					KorongAsztalMegj(korong, asztal, zsak, dobott, ksz);					// Megjeleníti a Korongok, az Asztal, a Zsák és a Dobott tartalmát
					PlayerMegj(j, jsz, aj);													// Megjeleníti a játékosok tábláit

					csempe = j[aj].Mintasor[s][0];
					cout << "Most " << j[aj].Nev << " falazhat..." << endl;
					cout << "A " << s + 1 << ". mintasorból egy " << csempe << " csempét a falra tehetsz." << endl;
					do
					{
						cout << "Melyik oszlopba szeretnéd teni? (1-tõl 5-ig, a padlót a 0-val adhatod meg) ";
						cin >> kivo;
						if (kivo == "x" || kivo == "X")												// A játékos ki szeretne lépni, ezért már megyünk is vissza
						{
							return true;
						}
						else if (int(kivo[0]) - 48 > TABLAMERET)									// Ha a megadott oszlop nagyobb, mint az oszlopok száma (vagy nem szám)
						{
							jovalasz = false;
							cout << "HIBA: Ilyen sorszámú oszlop nincs a falon!" << endl;
						}
						else if (kivo != "0" && j[aj].Fal[s][int(kivo[0]) - 49] != '.') 			// Ha nem a Padlót választotta és a kiválasztott oszlopban lévõ hely nem üres
						{
							jovalasz = false;
							cout << "HIBA: A kiválasztott hely foglalt!" << endl;
						}
						else if (kivo != "0" && !JoOszlop(j, aj, int(kivo[0]) - 49, csempe))		// Ha nem a Padlót választotta de a megadott oszlopban valahol már van ilyen csempe
						{
							jovalasz = false;
							cout << "HIBA: Ebben az oszlopban már van ilyen csempe!" << endl;
						}
						else
						{
							jovalasz = true;
						}
					} while (!jovalasz);
					if (kivo == "0")												// Ha a játékos a Padlót választotta
					{
						for (int p = 0; p < j[aj].Mintasor[s].length(); p++)		// Annyiszor dobok csempét a padlóra, ahány darab a Mintasorban volt (azaz a Mintasor hossza)
						{
							cout << "~ Egy " << csempe << " a(z) " << s << ". sorból a Padlóra kerül." << endl;
							CsempeAPadlora(j, csempe, dobott, aj);					// Egyesével a Padlóra kerülnek a csempék
						}
					}
					else
					{
						cout << "~ Egy " << csempe << " a(z) " << s << ". sorból a Fal " << kivo[0] - 48 << ". oszlopába kerül." << endl;
						j[aj].Fal[s][int(kivo[0]) - 49] = csempe[0];				// A falra kerül a csempe
						j[aj].Pontok += FalPontok(j, aj, s, int(kivo[0]) - 49);		// A falazásért pontot kap a játékos
						cout << "~ A maradék csempét eldobjuk a Mintasorból..." << endl;
						CsempeEldobas(j, dobott, aj, s + 1);						// Eldobjuk a maradék csempét a Mintasorból
					}
					cout << "~ Mintasor ürítése..." << endl;
					MintasorNullazas(j, aj, s);										// A Mintasor ürítése
				}
				else														// Ha a Falon már van a Mintasorban lévõkkel azonos csempe
				{
					csempe = j[aj].Mintasor[s][0];
					cout << "~ Mivel a Fal " << s << ". sorában már van " << csempe << " csempe ezért a Mintasorból az összes a Padlóra kerül." << endl;
					for (int p = 0; p < j[aj].Mintasor[s].length(); p++)	// Annyiszor dobok csempét a padlóra, ahány a Mintasorban volt
					{
						CsempeAPadlora(j, csempe, dobott, aj);				// Egyesével a Padlóra kerülnek a csempék
					}
					cout << "~ Mintasor ürítése..." << endl;
					MintasorNullazas(j, aj, s);								// A Mintasor ürítése
				}
			}
		}
		cout << "A Mintasorok végére értünk, most a Padlón lévõ büntetõpontok számítása következik..." << endl;
		buntetopontok = PadloPontok(j, aj);				// A padlón lévõ csempék után járó büntetõpontok kiszámítása
		if (j[aj].Pontok + buntetopontok < 0)			// Ha a büntetõpontok miatt negatív lenne a játékos pontszáma, akkor nulla pontja lesz
		{
			j[aj].Pontok = 0;
		}
		else
		{
			j[aj].Pontok += buntetopontok;
		}
		CsempeEldobas(j, dobott, aj, 0);														// Eldobjuk a csempéket a Padlóról
		PadloNullazas(j, aj);																	// Üríti a Padlót
		cout << j[aj].Nev << ", a falazás után " << j[aj].Pontok << " pontod van." << endl;
	}
	return false;
}

// 9. BÓNUSZPONTOK SZÁMÍTÁSA
void BonuszPontok(Player j[], Score e[], int jsz)
{
	cout << "* * * Bónuszpontok számítása * * *" << endl;
	for (int i = 0; i < jsz; i++)							// A játékosok indexe
	{
		int bonuszsor = 0;									// A teljes sorokért járó bónuszpontok
		int bonuszoszlop = 0;								// A teljes oszlopokért járó bónuszpontok
		int bonuszszin = 0;									// A teljes színekért járó bónuszpontok

		e[i].SorokSzama = 0;								// Nullázzuk a sorok számát az adott játékos eredményjelzõjénél

		for (int s = 0; s < TABLAMERET; s++)
		{
			if (j[i].Fal[s].find(".") == j[i].Fal[s].npos)	// Ha a Fal adott sorában nincs üres hely, akkor jár a pont
			{
				bonuszsor += 2;
			}
			int teljesoszlop = true;						// Van teljes oszlop
			int o = 0;										// Oszlopkeresõ
			do
			{
				if (j[i].Fal[o][s] == '.')					// Ha a Fal adott oszlopában nincs üres hely, akkor jár a pont
				{
					teljesoszlop = false;
				}
				o++;
			} while (teljesoszlop && o < TABLAMERET);
			if (teljesoszlop)
			{
				bonuszoszlop += 7;
			}
		}
		for (int c = 0; c < CSVARIACIOK; c++)
		{
			bool teljesszin = true;											// Van teljes szín
			int sz = 0;														// Színkeresõ
			do
			{
				if (j[i].Fal[sz].find(char(c + 65)) == j[i].Fal[sz].npos)	// Ha nincs olyan sor a Falon, amiben ne szerepelne az adott Csempe, akkor jár a pont
				{
					teljesszin = false;
				}
				sz++;
			} while (teljesszin && sz < TABLAMERET);
			if (teljesszin)
			{
				bonuszszin += 10;
			}
		}
		j[i].Pontok += bonuszsor + bonuszoszlop + bonuszszin;
		cout << j[i].Nev << ": " << bonuszsor << " pont a sorokért + " << bonuszoszlop << " pont az oszlopokért + " << bonuszszin << " pont a teljes színekért." << endl;
		cout << "Így összesen: " << j[i].Pontok << " pont." << endl;
	}
}

// 10. EREDMÉNYEK KIÍRÁSA
void Vegeredmeny(Player j[], Score e[], int jsz)
{
	int p = 0;											// Pontokat tároló segédváltozó
	string n = "\0";									// Neveket tároló segédváltozó
	int s = 0;											// A Sorok számát tároló segédváltozó

	for (int m = 0; m < jsz; m++)						// Átmásoljuk az eredményjelzõ tömbbe a játékosok neveit és pontszámait
	{
		e[m].Nev = j[m].Nev;
		e[m].Pont = j[m].Pontok;
	}

	for (int i = 0; i < jsz - 1; i++)					// A kisebb indexû elem
	{
		for (int k = i + 1; k < jsz; k++)				// Az egyel nagyobb indexû elem
		{
			if (e[i].Pont < e[k].Pont)					// Ha a kisebbik indexû kisebb, mint a nagyobb indexû, akkor csere (mert a helyezéseket az index határozza meg)
			{
				n = e[i].Nev;
				p = e[i].Pont;
				s = e[i].SorokSzama;
				e[i].Nev = e[k].Nev;
				e[i].Pont = e[k].Pont;
				e[i].SorokSzama = e[k].SorokSzama;
				e[k].Nev = n;
				e[k].Pont = p;
				e[k].SorokSzama = s;
			}
			else if (e[i].Pont == e[k].Pont				// Ha a két egymás melletti elem egyenlõ, akkor a sorok száma dönt
				&& e[i].SorokSzama < e[k].SorokSzama)
			{
				n = e[i].Nev;
				p = e[i].Pont;
				s = e[i].SorokSzama;
				e[i].Nev = e[k].Nev;
				e[i].Pont = e[k].Pont;
				e[i].SorokSzama = e[k].SorokSzama;
				e[k].Nev = n;
				e[k].Pont = p;
				e[k].SorokSzama = s;
			}
		}
	}
	cout << "* * *\n\nA JÁTÉK VÉGEREDMÉNYE:\n\n";
	for (int l = 0; l < jsz; l++)
	{
		cout << l + 1 << ". helyezett: " << e[l].Nev << " - " << e[l].Pont << " ponttal." << endl;
	}
}

int main()
{
	setlocale(LC_ALL, "hun");		// Magyarra állítja a nyelvet

	Game g;							// Változó a Game struktúrával
	Player* p;						// Tömb a Palyer stuktúrával
	Score* e;						// Tömb a Score struktúrával

	bool jatekvege = false;			// A játék vége
	bool kilepes = false;			// A játékos ki akar lépni
	int fordulo = 0;				// A forduló sorszáma
	int lepes = 0;					// A fordulón belüli lépések száma

	int jsz = JatekosSzamBe();							// A játékosok száma
	p = new Player[jsz];								// A játékosok adatait tartalmazó Player típusú tömb létrehozása a játékosszám alapján
	e = new Score[jsz];									// Az eredmények kijelzéséhez szükséges tömb létrehozása a játékosszám alapján
	g.KorongSzam = jsz * 2 + 1;							// A korongok száma (a g változóban) egyel több, mint a játékosok számának kétszerese
	g.Korongok = new string[g.KorongSzam];				// A Korongok tömb elkészítése a korongok száma alapján

	KezdoZsakok(g.Zsak, g.Dobott, g.AsztalKozep);		// A Zsák, Eldobottak zsákja és az Asztal közepének elõkészítése
	PlayerElokeszites(p, jsz);							// A Player típusú tömb elõkészítése a játékosszám alapján
	JatekosNevek(p, jsz);								// Opcionálisan megadható a játékosok beceneve

	while (!jatekvege)
	{
		jatekvege = KorongTolto(g.Korongok, g.Zsak, g.Dobott, g.AsztalKozep, g.KorongSzam);			// Feltölti a Korongokat, az Asztalta teszi a kezdõjátékos jelölõt
		int aj = 0;																					// Aktuális játékos
		while (!p[aj].kovetkezik && aj < jsz) { aj++; }												// Megkeresi melyik játékos következik
		fordulo++;																					// Növeljük a forduló sorszámát
		lepes = 0;																					// Visszaállítjuk a fordulón belüli lépésszámot
		bool fordulovege = false;																	// A forduló vége

		while (!fordulovege && !jatekvege)
		{
			lepes++;
			cout << "\n* * *  CSEMPEVÁSÁR " << fordulo << ". forduló " << lepes << ". lépés  * * *" << endl;	// Kírja hol tartunk
			KorongAsztalMegj(g.Korongok, g.AsztalKozep, g.Zsak, g.Dobott, g.KorongSzam);						// Megjeleníti a Korongok, az Asztal, a Zsák és a Dobott tartalmát
			PlayerMegj(p, jsz, aj);																				// Megjeleníti a játékosok tábláit
			kilepes = CsempeVasar(p, g.Korongok, g.KorongSzam, g.AsztalKozep, g.Zsak, g.Dobott, aj, jsz);		// A játékos csempéket vásárolhat a Korongokról vagy az Asztalról
			if (aj == jsz - 1) { aj = 0; }																		// Egyel léptei az játékosokat
			else { aj++; }
			fordulovege = ForduloVege(g.Korongok, g.KorongSzam, g.AsztalKozep) || kilepes;						// A forduló végének leellenõrzése
		}

		if (!kilepes)
		{
			kilepes = Falazas(p, g.Korongok, g.AsztalKozep, g.Zsak, g.Dobott, g.KorongSzam, jsz);		// Falazás + pontok
		}
		jatekvege = JatekVege(p, jsz, g.Zsak, g.Dobott) || kilepes;										// A játék végének ellenõrzése
	}

	BonuszPontok(p, e, jsz);											// Bónusz pontok kiszámítása
	Vegeredmeny(p, e, jsz);												// A végeredmény kiírása

	cout << "\nGRATULÁLOK A GYÕZTESNEK!" << endl;
	cout << "Köszönöm, hogy játszottál!" << endl;

	delete[]g.Korongok;
	delete[]p;
	return 0;
}