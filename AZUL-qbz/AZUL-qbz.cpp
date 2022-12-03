/*
AZUL - beadandó feladat Programozás (GKLB_INTM021) tárgyból
Borsodi Zoltán (B7PK8Z)
borsodi.zoltan@gmail.com
*/

#include"iostream"
#include"iomanip"
#include"cmath"
#include"cstdlib"
#include"ctime"
#include"string"

using namespace std;

#define CSVARIACIOK 5		// Ennyi különbözõ csempefajta van a játékban.
#define CSKEZDODARAB 20		// Egy adott fajta csempékbõl ennyi darab van összesen.
#define KMERET 4			// A korongokra helyezhetõ csepmpék maximális száma.
#define TABLAMERET 5		// A Mintasor illetve a Fal méretei a játékosok tábláján. (Általában megegyezik a csempefajták számával azaz CSVARIACIOK változóval.)
#define PADLOMERET 7		// A Padlovonal mérete a játékosok tábláján.

struct Game												// A játék fõ adatai
{
	string*	 Korongok;									// A korongok tömbje
	uint8_t  KorongSzam;								// A korongok száma
	uint8_t  Zsak[CSVARIACIOK];							// A zsák tömbjének helye 
	uint8_t  Dobott[CSVARIACIOK];						// Az eldobott csempék tömbjének helye
	uint8_t  AsztalKozep[CSVARIACIOK + 1];				// Az asztal közepét tároló tömb helye
	uint8_t	 numberOfPlayers;							// A játékosok száma
	uint8_t	 currentPlayer;								// Az aktuálisan következõ játékos
	bool	 jatekvege;									// A játék vége
	bool	 fordulovege;								// A forduló vége
	bool	 kilepes;									// A játékos ki akar lépni
	uint16_t fordulo;									// A forduló sorszáma
	uint16_t lepes;										// A fordulón belüli lépések száma

	Game()
	{
		zsakokEsAsztalInicializalasa();
        numberOfPlayers = JatekosSzamBe();
        KorongSzam		= numberOfPlayers * 2 + 1;		// A korongok száma egyel több, mint a játékosok számának kétszerese
        Korongok		= new string[KorongSzam];		// A Korongok tömb elkészítése a korongok száma alapján
        jatekvege		= false;
        fordulovege		= false;
        kilepes			= false;
        fordulo			= 0;
        lepes			= 0;
        KezdoKepernyo();
	}

    void zsakokEsAsztalInicializalasa()
    {
        for (int i = 0; i < CSVARIACIOK; i++) {
            Zsak[i] = CSKEZDODARAB;						// A zsák rekeszeinek feltöltése a csempék kezdeti darabszámával
            Dobott[i] = 0;								// A dobottak zsák nullázása
            AsztalKozep[i] = 0;							// Az asztal közepének nullázása
        }
        AsztalKozep[CSVARIACIOK] = 1;					// Itt tesszük le az asztal közepére a kezdõjátékos jelzõt
    }

	void ujFordulo(Player players[])
	{
        fordulo++;																									// Növeljük a forduló sorszámát
        lepes = 0;																									// Visszaállítjuk a fordulón belüli lépésszámot
        fordulovege = false;																						// A forduló most kezdõdik, úgyhogy még nincs vége
        currentPlayer = 0;
        while (!players[currentPlayer].kovetkezik && currentPlayer < numberOfPlayers) currentPlayer++;				// Megkeresi melyik játékos következik
	}
};

struct Player							// A játékosokhoz tartozó adatok
{
	string Nev;							// A játékos neve
	int Pontok;							// A játékos pontszáma
	string Mintasor[TABLAMERET];		// A mintasor, ahová a korongokról vagy asztalról vásárolt csempéket gyûjti.
	string wall[TABLAMERET];				// A fal, ahová felteszi a mintasoron összegyûlt csempéket.
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
bool ElfogyottACsempe(Game& game)
{
	bool ureszsak = false;							// Nincs csempe a Zsákban
	bool uresdobott = false;						// Nincs csempe a Dobottakban
	int k = 0;
    // Addig nézi amíg nem talál legalább egy nem üres rekeszt
	while (k < CSVARIACIOK && game.Zsak[k] == 0) {
		ureszsak = true;							// Akkor kell belemennem, ha üres rekeszt találok, egyébként (azaz nem üres) mehetünk tovább, mert van még csempe
		k++;
	}
	k = 0;
	while (k < CSVARIACIOK && game.Dobott[k] == 0) {
		uresdobott = true;
		k++;
	}
	if (ureszsak) {
        // Ha elfogyott minden csempe a Zsákból és a Dobottak közül, akkor vége a játéknak
		if (uresdobott) {
			cout << "~ Elfogyott minden csempe => vége a játéknak.\n";
			return true;
		}
        // Ha csak a Zsák üres, átpakoljuk a Dobottakban tárolt értékeket a Zsákba
		else {
			cout << "~ A kiürült Zsák feltöltése...\n";
			for (int i = 0; i < CSVARIACIOK; i++) {
				game.Zsak[i] = game.Dobott[i];
				game.Dobott[i] = 0;
			}
		}
	}
	return false;
}

// 0/b A JÁTÉK VÉGÉNEK ELLENÕRZÉSE
bool JatekVege(Player player[], int numberOfPlayers, int zsak[], int dobott[])
{
    // Amint valakinél talál teljes sort a Falon, vége a játéknak
	cout << "~ Játék végének ellenõrzése...\n";
	bool completedLine = false;
	int i = 0;
	while (!completedLine && i < numberOfPlayers) {
		int line = 0;
		while (!completedLine && line < TABLAMERET) {
			if (player[i].wall[line].find(".") == player[i].wall[line].npos) {
				cout << "~ " << player[i].Nev << " falának " << line << ". sora teljes lett => vége a játéknak.\n";
				completedLine = true;
			}
			line++;
		}
		i++;
	}
	return completedLine || ElfogyottACsempe(zsak, dobott);

	/*
	do {
		do {
			if (player[i].wall[line].find(".") == player[i].wall[line].npos) {
				completedLine = true;
			}
			line++;
		} while (!completedLine && line < TABLAMERET);
		i++;
	} while (!completedLine && i < numberOfPlayers);
	*/
}

// 0/c FORDULÓ VÉGÉNEK ELLENÕRZÉSE
bool ForduloVege(Game& game)
{
	bool kures = true;					// A Korongok kiürültek-e
	bool aures = true;					// Az asztal üres-e
	int kk = 0;							// Korong-szkennelõ
	int ak = 0;							// Asztal-szkennelõ
	cout << "~ Forduló végének ellenõrzése...\n";
    // Amint talál egy nem üres Korongot, rögtön hamisra állítja
	do {
		if (game.Korongok[kk] != "....") { kures = false; }
		kk++;
	} while (kures && kk < game.KorongSzam);
    // Amint talál csempét az Asztalon, rögtön hamisra állítja
	do {
		if (game.AsztalKozep[ak] != 0) { aures = false; }
		ak++;
	} while (aures && ak < CSVARIACIOK);
	return kures && aures;
}

// 0/d ELDÖNTENDÕ KÉRDÉS
bool Dontes(string szoveg)
{
	string valasz = "\0";
	do {
		cout << szoveg;
		cin >> valasz;
        // Nagybetûssé alakítjuk a bevitt szöveget
		for (int n = 0; n < valasz.length(); n++) { valasz[n] = toupper(valasz[n]); }
	} while (valasz != "I" && valasz != "N");
	if (valasz == "I") { return true; }
	else { return false; }
}

// 1/b JÁTÉKOSOK NEVÉNEK BEKÉRÉSE (OPCIONÁLIS)
void jatekosNevekMegadasa(const Game& game, Player player[])
{
	if (!Dontes("Megadod a játékosok becenevét? (i / n) ")) return;
    string kuka = "\0";														// A cin ürítése
    getline(cin, kuka, '\n');
    for (int i = 0; i < game.numberOfPlayers; i++) {
        cout << i + 1 << ". játékos neve: ";
        getline(cin, player[i].Nev, '\n');
    }
}

// KEZDÕJÁTÉKOS MEGADÁSA
void kezdoJatekosMegadasa(const Game& game, Player player[])
{
    for (int i = 0; i < game.numberOfPlayers; i++) player[i].kovetkezik = false;
	string kezd = "\0";
	bool jokezdo = false;
	while (!jokezdo) {
		cout << "Ki kezd? (A játékos sorszámát add meg!) ";
		cin >> kezd;
		if (kezd.length() != 1) { jokezdo = false; }
		else if (int(kezd[0]) < 49 || int(kezd[0]) - 48 > game.numberOfPlayers) {
			jokezdo = false;
			cout << "HIBA: Ilyen sorszámú játékos nincs!\n";
		}
        else { jokezdo = true; }
	}
	player[int(kezd[0] - 49)].kovetkezik = true;
}

// 1. JÁTÉKOSOK SZÁMÁNAK BEKÉRÉSE
int JatekosSzamBe()
{
	string jszbe = "";
	do {
		cout << "Add meg a játékosok számát (2, 3 vagy 4 játékos lehet!): ";
		cin >> jszbe;
	} while (jszbe != "2" && jszbe != "3" && jszbe != "4");
	return int(jszbe[0]) - 48;
}

// 3/a CSEMPE ELÕHÚZÁSA A ZSÁKBÓL
int CsempeHuzas(Game& game)
{
    // A zsákban lévõ, különbözõ színû csempék számát összeadja a véletlen szám generáláshoz
	int zsakbanLevoCsempekSzama = 0;
	for (int i = 0; i < CSVARIACIOK; i++) zsakbanLevoCsempekSzama += game.Zsak[i];
	int x = rand() % zsakbanLevoCsempekSzama + 1;
	// Ki kell keresni, hogy a véletlen szám melyik színû csempét jelöli:
    // ehhez addig bõvíti a keresõvektort a zsák elemeinek mennyiségével,
	// amíg a keresõvektornál kisebb nem lesz a generált véletlen számnál.
	// Így a keresett (kihúzandó) csempeszínt az aktuális indexe jelöli
	int i = 0;
	int vektor = 0;
	while (vektor < x && i < CSVARIACIOK) vektor += game.Zsak[i++];
	return i;
}

// 3. KORONGOK FELTÖLTÉSE
bool KorongTolto(Game& game)
{
	cout << "~ Korongok feltöltése...\n";
    // Egyesével végigmegy a korongokon
	for (int i = 0; i < game.KorongSzam; i++) {
        // Annyi csempét húz a zsákból, ahány elfér egy korongon
		for (int j = 0; j < KMERET; j++) {
            // Ha már nincs csempe a Zsákban vagy a Dobottak között, akkor vége a játéknak
			if (ElfogyottACsempe(game)) { return true; }
            // egyébként húz egy csempét (ez a zsák tömb indexe), amibõl eggyel kevesebb marad a zsákban és ami rákerül az aktuális korongra
            int kihuzottCsempe = CsempeHuzas(game);
            game.Zsak[kihuzottCsempe]--;
            game.Korongok[i] += char(65 + kihuzottCsempe);
		}
	}
    // Kezdõjátékos jelölõ kihelyezése az asztal közepére
	game.AsztalKozep[CSVARIACIOK] = 1;
	return false;
}

// 4/a KÖVETKEZÕ-JÁTÉKOS JELÖLÕ RESETELÉSE
void KovJatekosReset(const Game& game, Player player[])
{
	for (int i = 0; i < game.numberOfPlayers; i++) { player[i].kovetkezik = false; }
}

// 4. A PLAYER STRUKTÚRÁJÚ TÖMB ELÕKÉSZÍTÉSE
int PlayerElokeszites(const Game& game, Player player[])
{
	for (int i = 0; i < game.numberOfPlayers; i++) {
		player[i].Nev = char(48 + i + 1);			// A név helyére egyelõre csak sorszám kerül
		player[i].Pontok = 0;						// A pontok nullázódnak
		player[i].Padlovonal = "\0";				// A padlóvonal üres legyen
		string minta = "\0";						// A Mintasor kirajzolsáshoz szükséges változó
		for (int j = 0; j < TABLAMERET; j++) {
			player[i].Mintasor[j] = minta + ".";	// A Mintasor kirajzolása helyõrzõkkel
			player[i].wall[j] = ".....";			// A Fal kirajzolása helyõrzõkkel
			minta += ".";
		}
		for (int k = 0; k < PADLOMERET; k++) {
			player[i].Padlovonal += ".";			// A Padlóvonal kirajzolása helyõrzõkkel
		}
	}
	KovJatekosReset(game, player);					// A következõ-játékos jelölõt mindenhol hamisra állítja
	return 0;
}


// 5/a HELYÕRZÕ SZÓKÖZÖK
string Helyorzo(string ertek)
{
	if (ertek.length() == 1) { return ertek + " "; }
	else { return ertek; }
}

// 5. KORONGOKON ÉS ASZTAL KÖZEPÉN LÉVÕ CSEMPÉK MEGJELENÍTÉSE
void KorongAsztalMegj(const Game& game)
{
	const int sorokszama = 8;						// A kiírandó sorok száma összesen
	string sorok[sorokszama];						// Ide gyûjtjük a kiírandó sorok tartalmát
	string elv = "|  ";								// Elválasztó a táblázat részei között
    // A TÁBLÁZAT BAL OLDALA
	for (int i = 0; i < sorokszama; i++) { sorok[i] = elv; }
	// A KORONGOK
	int kszel = (game.KorongSzam / 2 + 1) * 5;										// Ilyen széles hely kell a korongok megjelenítéséhez
	sorok[0] += "Korongok:";											// A korongok rész címsora
    // Kitölti a szükséges mennyiségû szóközzel a korongok számának függvényében
	for (int k = 0; k < kszel - 9; k++) { sorok[0] += " "; }
	sorok[0] += elv;
    // A korongok számozása: 1. sor
	for (int k = 0; k < ((game.KorongSzam / 2) + 1); k++) { sorok[1] += to_string(k + 1) + ".   "; }
	sorok[1] += elv;
    // A korongok tartalma: elsõ két sor
	for (int i = 0; i < 2; i++) {
        // Elszámol a korongok számának felénél egyel tovább...
		// ...és az aktuális korongról kettesével kiírja a csempéket (elõbb az elsõ kettõt aztán a második kettõt)
		for (int k = 0; k < ((game.KorongSzam / 2) + 1); k++) { sorok[i + 2] += game.Korongok[k].substr((i * 2), 2) + "   "; }
		sorok[i + 2] += elv;
	}
    // Elválasztó üres sor
	for (int k = 0; k < kszel; k++) { sorok[4] += " "; }
	sorok[4] += elv;
    // A korongok számozása: 2. sor
	for (int k = ((game.KorongSzam / 2) + 1); k < game.KorongSzam; k++) { sorok[5] += to_string(k + 1) + ".   "; }
	sorok[5] += "     " + elv;
    // A korongok tartalma: másdoik két sor
	for (int i = 0; i < 2; i++) {
        // A számolás a korongok számának felénél egyel többrõl indul és a korongok számáig tart...
        // ...és az aktuális korongról kettesével kiírja a csempéket
		for (int k2 = ((game.KorongSzam / 2) + 1); k2 < game.KorongSzam; k2++) { sorok[i + 6] += game.Korongok[k2].substr((i * 2), 2) + "   "; }
		sorok[i + 6] += "     " + elv;
	}
	// AZ ASZTAL
	// Az asztal rész címsora
	sorok[0] += "Asztal:    " + elv;
    // A csempék számának kiírása
	for (int i = 0; i < CSVARIACIOK; i++) {
		sorok[i + 1] += char(65 + i);
		sorok[i + 1] += ": " + to_string(game.AsztalKozep[i]) + "       " + elv;
	}
    // Üres sor
	sorok[6] += "           " + elv;
    // Ha még az asztalon van a kezdõjátékosjelölõ...
    // ...felírja azt is...
	if (game.AsztalKozep[CSVARIACIOK] == 1) { sorok[7] += "Kezdõ      " + elv; }
    // ...különben üres sort ír.
	else { sorok[7] += "           " + elv; }
	// A ZSÁK ÉS DOBOTTAK
	sorok[0] += "Zsák:           ";
	for (int i = 0; i < CSVARIACIOK; i++) {
		sorok[1] += char(65 + i);
		sorok[1] += "  ";
	}
	for (int j = 0; j < CSVARIACIOK; j++) { sorok[2] += Helyorzo(to_string(game.Zsak[j])) + " "; }
	sorok[3] += "                ";
	sorok[4] += "Dobott:         ";
	for (int i = 0; i < CSVARIACIOK; i++) {
		sorok[5] += char(65 + i);
		sorok[5] += "  ";
	}
	for (int j = 0; j < CSVARIACIOK; j++) { sorok[6] += Helyorzo(to_string(game.Dobott[j])) + " "; }
    // SOROK KIÍRÁSA
	for (int ki = 0; ki < sorokszama; ki++) { cout << sorok[ki] << endl; }
	cout << endl;
}

// 6a PLAYER MEGJELENÍTÉSE
void PlayerMegj(Game& game, Player player[])
{
	const int fs = 4;							// A fejlécbe kiírandó sorok száma
	const int ps = 3;							// A padlóvonalra kiírandó sorok száma
	const int ssz = fs + TABLAMERET + ps;		// A kiíandó sorok száma összesen: a fejléc, a tábla és a padlóra kiírandó sorok száma
	const int tszel = (PADLOMERET * 3) + 2;		// Az táblák szélessége
	string sorok[ssz];							// Ebbe gyûjtjük a játékos elõtti tábla kiírandó tartalmát soronként
    // Sorok nullázása
	for (int i = 0; i < ssz; i++) { sorok[i] = "\0"; }
    // A játékosok indexe
	for (int ji = 0; ji < game.numberOfPlayers; ji++) {
		// A kiírás elõkészítése: vonal és töltelék
		string vonal = "\0";							// Ez lesz a táblázat vízszintes vonala
		int sszel = tszel * (ji + 1);					// A gyarapodó sorok aktuális maximum szélessége
        // Ha az adott játékos következik, akkor a vízszintes vonal *-karakterbõl áll
		if (ji == game.currentPlayer) { for (int i = 0; i < tszel - 1; i++) { vonal += "*"; } }
		else { for (int i = 0; i < tszel - 1; i++) {
				if (i == TABLAMERET + 5) { vonal += "+"; }
				else { vonal += "-"; }
			}
		}
		string toltelek = "\0";							// Helykitöltõ szóközök a Mintasor elé, hogy balra zárt legyen
        // A töltelékkarakterek elkészítése
		for (int i = 0; i < TABLAMERET - 1; i++) { toltelek += " "; }
		// A tábla bal oldala
		for (int i = 0; i < ssz; i++) { sorok[i] += "|"; }
		// FEJLÉC
		sorok[0] += vonal;
		sorok[1] += " " + to_string(ji + 1) + " - ";				// A játékos sorszáma
		sorok[1] += player[ji].Nev.substr(0, tszel - 7);					// Név (csonkolva, ha hosszú)
		sorok[2] += " " + to_string(player[ji].Pontok) + " pont";		// Pontszám
        // A 4. sor elejét tölti fel szóközzel
		while (sorok[3].length() < sszel - (TABLAMERET * 2)) { sorok[3] += " "; }
        // A 4. sor végét feltölti az oszlopok sorszámával
		for (int i = 0; i < TABLAMERET; i++) { sorok[3] += to_string(i + 1) + " "; }
		// MINTASOR ÉS FAL
		for (int i = 0; i < TABLAMERET; i++) {
			sorok[i + fs] += "    " + toltelek + player[ji].Mintasor[i];	// Töltelék és a Mintasor tartalma
			sorok[i + fs] += " " + to_string(i + 1) + " ";			// Sorszám a Mintasor és a Fal között
			for (int f = 0; f < TABLAMERET; f++) {
				sorok[i + fs] += player[ji].wall[i][f];					// A Fal tartalma szóközökkel elválasztva
				sorok[i + fs] += " ";
			}
			toltelek = toltelek.substr(0, toltelek.size() - 1);		// Egyel csökkenti a töltelék hosszúságát
		}
		// PADLÓVONAL
		sorok[fs + TABLAMERET] += vonal;
		int p = 0;													// Padlóvonal aktuális karaktere
		while (p < PADLOMERET && sorok[fs + TABLAMERET + 1].length() < sszel) {
			sorok[fs + TABLAMERET + 1] += "  ";
			sorok[fs + TABLAMERET + 1] += player[ji].Padlovonal[p];
			sorok[fs + TABLAMERET + 2] += " " + to_string(-(((p + 1) / 3) + 1));
			p++;
		}
		// VÉGÜL A MARADÉKOT MINDEN SORBAN FELTÖLTI SZÓKÖZZEL
		for (int u = 0; u < ssz; u++) { while (sorok[u].length() < sszel) { sorok[u] += " "; } }
	}
    // AZ ELKÉSZÜLT SOROK KIÍRÁSA
	for (int ki = 0; ki < ssz; ki++) { cout << sorok[ki] << "|" << endl; }
	cout << endl;
}

// 7/a JÁTÉKOS KORONG- ÉS CSEMPEVÁLASZTÁSA VÁSÁRLÁSHOZ
const string KCsValaszto(const Player player[], const Game& game)
{
	string kivalasztottCsempe = "\0";
	bool joValasztas = false;
	while (!joValasztas) {
		cout << player[game.currentPlayer].Nev << ", válassz csempéket egy korongról vagy az asztalról!\n(Pl. 1. korong A csempe: 1A | Az asztalt a 0 jelöli: 0D) ";
		cin >> kivalasztottCsempe;
        // Elõbb nagybetûssé alakítjuk a bevitt szöveget
		for (int n = 0; n < kivalasztottCsempe.length(); n++) { kivalasztottCsempe[n] = toupper(kivalasztottCsempe[n]); }
        // Ha ki szeretne lépni rögtön megy is vissza
		if (kivalasztottCsempe == "X") { joValasztas = true; }
        // Ha nem két karaktert írt be, már nem jó
		else if (kivalasztottCsempe.length() != 2) {
			joValasztas = false;
			cout << "HIBA: nem ket karakter!\n";
		}
        // Ha az elsõ karakter nem, egy a korongok számánál kisebb szám
		else if (int(kivalasztottCsempe[0]) < 48 || int(kivalasztottCsempe[0]) - 48 > game.KorongSzam) {
			joValasztas = false;
			cout << "HIBA: Ilyen korong nincs a játékban!\n";
		}
        // Ha második karakter nem olyan NAGYbetû, ami megfelel a csempék betújeleinek
		else if (!(int(kivalasztottCsempe[1]) > 64 && int(kivalasztottCsempe[1]) - 65 < CSVARIACIOK)) {
			joValasztas = false;
			cout << "HIBA: Ilyen betûjelû csempe nincs a játékban!\n";
		}
        // Ha korongot választott és az üres, akkor nem jó
		else if (int(kivalasztottCsempe[0]) - 48 != 0 && game.Korong[int(kivalasztottCsempe[0]) - 49] == "....") {
			joValasztas = false;
			cout << "HIBA: A választott korong üres!\n";
		}
        // Ha az asztalt választotta, és egy sincs rajta a kért csempébõl, akkor nem jó
		else if (int(kivalasztottCsempe[0]) - 48 == 0 && game.AsztalKozep[int(kivalasztottCsempe[1]) - 65] == 0) {
			joValasztas = false;
			cout << "HIBA: A választott csempébõl nincs az asztalon!\n";
		}
        // Ha a korongot választott de nincs rajta a kért csempe, akkor nem jó
		else if (int(kivalasztottCsempe[0]) - 48 != 0 && game.Korong[int(kivalasztottCsempe[0]) - 49].find(kivalasztottCsempe[1]) == game.Korong[int(kivalasztottCsempe[0])].npos) {
			joValasztas = false;
			cout << "HIBA: A korongon nincs ilyen csempe!\n";
		}
		else { joValasztas = true; }
	}
	return kivalasztottCsempe;
}

// 7/b A JÁTÉKOS MINTASOR VÁLASZTÁSA VÁRÁSLÁSHOZ
const char SorValaszto(const Player player[], const string csempe, const Game& game)
{
    // Van-e még egyáltalán olyan mintasor, ahová csempét lehet rakni?
	int i = 0;
	while (i < TABLAMERET && !(player[game.currentPlayer].Mintasor[i].find(".") == player[game.currentPlayer].Mintasor[i].npos)) i++;
	// Ha nincs -  tehát midnen Mintasor tele van - akkor a csempék mindenképpen a padlóra kerülnek
	if (i == TABLAMERET) return '0';
	// Egyébként meg kell adni, hogy melyik sorba kerüljenek a csempék
    string sor;
    bool josor = false;
	while (!josor) {
        cout << "Melyik Mintasorba szeretnéd tenni? (1-tõl " << CSVARIACIOK << "-ig | A Padlót a 0-val adhatod meg.) ";
        cin >> sor;
        // Ha a kilépést választja, már megy is vissza
        if (sor == "x" || sor == "X") { josor = true; }
        // Ha a megadott szám nagyobb, mint ahány mintasor van, akkor nem jó
        else if (int(sor[0]) - 48 > TABLAMERET) {
            cout << "HIBA: Ilyen sorszámú Mintasor nincs!\n";
            josor = false;
        }
        // Ha a játékos nem a padlót választotta...
        // ...és a kiválasztott Mintasor már nem üres...
        // ...és nincs benne a kiválasztott fajtájú csempe, akkor nem jó
        else if (sor != "0" && player[game.currentPlayer].Mintasor[int(sor[0]) - 49][0] != '.' && player[game.currentPlayer].Mintasor[int(sor[0]) - 49][0] != csempe[0]) {
            cout << "HIBA: Abban a sorban már másfajta csempék vannak!\n";
            josor = false;
        }
        // Ha nem a padlót választotta és a kiválasztott sor tele van...
		else if (sor != "0" && player[game.currentPlayer].Mintasor[int(sor[0]) - 1].find(".") == player[game.currentPlayer].Mintasor[int(sor[0]) - 1].npos) {
            // ...akkor, ha ez így jó neki, a csempék mennek a padlóra
            if (Dontes("Ez a sor tele van, ezért a csempék a Padlóra kerülnek. Biztosan ezt akarod? (i / n) ")) return '0';
            // egyébként választhat másik Mintasort
            josor = false;
		}
        else josor = true;
	}
	return sor[0];
}

// 7/c CSEMPE A PADLÓRA VAGY A DOBOTTAK KÖZÉ KERÜL CSEMPEVÁSÁRLÁSKOR
void CsempeAPadlora(Game& game, Player player[], string csempe)
{
    // Ha a Padló tele és nem a Kezdõ-jelölõt kell átrakni, akkor a Csempe a Dobottak közé kerül
	if (player[game.currentPlayer].Padlovonal.find(".") == player[game.currentPlayer].Padlovonal.npos && csempe != "K") {
		cout << "~ A Padló tele van, ezért egy " << csempe << " csempét el kell dobni.\n";
		dobott[int(csempe[0]) - 65]++;
	}
    // Ha van hely a Padlón akkor a Csempe vagy Kezdõ-jelölõ oda kerül
	else if (player[game.currentPlayer].Padlovonal.find(".") != player[game.currentPlayer].Padlovonal.npos) {
		player[game.currentPlayer].Padlovonal[player[game.currentPlayer].Padlovonal.find_first_of(".")] = csempe[0];
	}
	// Ha a Kezdõ-jelölõt kell átrakni de nincs hely a padlón, akkor nincs teendõ, mert a Kezdõt nem lehet a dobottak közé tenni!
}

// 7/d CSEMPÉK ÁTADÁSA A JÁTÉKOSNAK VAGY A PADLÓRA CSEMPEVÁSÁRLÁSKOR
void CsempeAtadas(Game& game, Player player[], string csempe, int mintasor)
{
    // A játékos a Padlót választotta vagy a teli sor miatt a Padlóra kerülnek a csempék
	if (mintasor == 0) {
		cout << "~ Egy " << csempe << " csempe a padlóra kerül.\n";
		CsempeAPadlora(game, player, csempe);
	}
	else {
        // Ha a mintasor már megtelt, akkor a csempe a padlóra kerül
		if (player[aj].Mintasor[mintasor - 1].find(".") == player[aj].Mintasor[mintasor - 1].npos) {
			cout << "~ Megtelt a(z) " << mintasor << ". sor, ezért egy " << csempe << " csempe a padlóra kerül.\n";
			CsempeAPadlora(game, player, csempe);
		}
        // A Csempe a kiválasztott Mintasorba kerül
		else {
			cout << "~ Egy " << csempe << " csempe a(z) " << mintasor << ". Mintasorba kerül.\n";
			player[aj].Mintasor[mintasor - 1][player[aj].Mintasor[mintasor - 1].find_first_of(".")] = csempe[0];
		}
	}
}

void csempePakolasAsztalrol(Game& game, Player player[], string csempe, uint8_t mintasor)
{
    // Ha még az asztalon van a kezdõjátékos jelölõ
    if (game.AsztalKozep[CSVARIACIOK] == 1) {
        cout << "~ A Kezdõjátékos-jelölõ áthelyezése a padlóvonalra...\n";
        game.AsztalKozep[CSVARIACIOK] = 0;													// Leveszi az Asztalról a kezdõjátékos jelölõt
        CsempeAPadlora(game, player, "K");													// majd átadja a padlóvonalra
        KovJatekosReset(game, player);														// Mindenkinél hamisra állítja a következõ-játékos jelölõt
        player[game.currentPlayer].kovetkezik = true;										// Az aktuális játékos kezdi majd a következõ kört
        cout << "~ A következõ fordulót " << player[game.currentPlayer].Nev << " kezdi.\n";
    }
    // Egesével kell átadni az asztalon lévõ csempéket
    for (int i = 0; i < game.AsztalKozep[int(csempe[0]) - 65]; i++) CsempeAtadas(game, player, csempe, mintasor);
    // Végül nullázza a csempék helyét az asztalon
    game.AsztalKozep[int(csempe[0]) - 65] = 0;
}

void csempePakolasKorongrol(Game& game, Player player[], string csempe, uint8_t korong, uint8_t mintasor)
{
    cout << "~ A kiválasztott csempék áthelyezése...\n";
    // Végigmegy a korongon tárolt csempéken
    for (int i = 0; i < KMERET; i++) {
        // Ha az aktuális csempe a kért csempe, akkor átadja a játékosnak
        if (game.Korongok[korong - 1].substr(i, 1) == csempe) CsempeAtadas(game, player, csempe, mintasor);
        // A nem kívánt csempéket az asztalra teszi
        else { game.AsztalKozep[int(game.Korongok[korong - 1][i]) - 65]++; }
    }
    // Végül a korong ürítése
    game.Korongok[korong - 1] = "\0";
    for (int i = 0; i < KMERET; i++) { game.Korongok[korong - 1] += "."; }
}

// 7. CSEMPÉK VÁSÁRLÁSA KORONGOKRÓL VAGY ASZTALRÓL
bool CsempeVasar(Game& game, Player player[])
{
	string korongEsCsempe = KCsValaszto(player, game);						// A kiválasztott korong és csempe
	if (korongEsCsempe == "X") return true;									// Ha ki akar lépni, rögtön megy is vissza
	uint8_t	korong	 = int(korongEsCsempe[0]) - 48;							// A kiválasztott korong
	string	csempe	 = korongEsCsempe.substr(1, 1);							// A kért csempe
	uint8_t	mintasor = int(SorValaszto(player, csempe, game)) - 48;			// A kiválasztott Mintasor ahová a csempék kerülnek
	if (mintasor == 40 || mintasor == 72) return true;						// Ha ki akar lépni, máris megy vissza
    // Ha forrásnak az asztalt választotta
	if (korong == 0) csempePakolasAsztalrol(game, player, csempe, mintasor);
	// Ha forrásnak valamelyik korongot választotta
	else csempePakolasKorongrol(game, player, csempe, korong, mintasor);
	return false;
}

// 8/a MINTASOROK NULLÁZÁSA
void MintasorNullazas(Player j[], int aktj, int akts)
{
	string minta = "\0";								// A Mintasor kirajzolsáshoz szükséges változó
    // A soron belüli pozíció
	for (int i = 0; i < akts + 1; i++) {
		j[aktj].Mintasor[akts] = minta + ".";
		minta += ".";
	}
}

// 8/b PADLÓ NULLÁZÁSA
void PadloNullazas(Player j[], int aj)
{
	j[aj].Padlovonal = "\0";							// A padlóvonal üres legyen
    // Padlóvonalon belüli pozíció
    // A Padlóvonal kirajzolása helyõrzõkkel
	for (int k = 0; k < PADLOMERET; k++) { j[aj].Padlovonal += "."; }
}

// 8/c PONTSZÁMÍTÁS FALAZÁSKOR
int FalPontok(Player j[], int aj, int akts, int kivo)
{
	int pont = 1;										// A játékos által kapott pontok (egyet biztosan kap, mert felfalazta a csempét)
	int k = 0;											// Keresõ index
	bool vansor = false;								// Találat vízszintes keresésénél
	bool vanoszlop = false;								// Találat függõleges keresésénél
    // Csak akkor néz el balra, ha a lerakott csempe nem a bal szélsõ oszlopba került
	if (kivo > 0) {
		k = kivo;
        // Akkor jár pont, ha az aktuális pozíciótól balra is van csempe
		while (k > 0 && j[aj].wall[akts][k - 1] != '.') {
			pont++;
			k--;
			vansor = true;
		}
	}
    // Csak akkor néz el jobbra, ha a lerakott csempe nem a jobb szélsõ oszlopba került
	if (kivo < TABLAMERET - 1) {
		k = kivo;
        // Akkor jár pont, ha az aktuális pozíciótól jobbra is van csempe
		while (k < TABLAMERET - 1 && j[aj].wall[akts][k + 1] != '.') {
			pont++;
			k++;
			vansor = true;
		}
	}
    // Csak akkor néz felfelé, ha a lerakott csempe nem a legfelsõ oszlopba került
	if (akts > 0) {
		k = akts;
        // Akkor jár pont, ha az aktuális pozíció felett is van csempe
		while (k > 0 && j[aj].wall[k - 1][kivo] != '.') {
			pont++;
			k--;
			vanoszlop = true;
		}
	}
    // Csak akkor néz lefelé, ha a lerakott csempe nem a legalsó oszlopba került
	if (akts < TABLAMERET - 1) {
		k = akts;
        // Akkor jár pont, ha az aktuális pozíció alatt is van csempe
		while (k < TABLAMERET - 1 && j[aj].wall[k + 1][kivo] != '.') {
			pont++;
			k++;
			vanoszlop = true;
		}
	}
    // Ha volt sor és oszlop is, akkor még egy pont jár, mert az éppen lerakott csempét egyikbe se számolta bele
	if (vansor && vanoszlop) { pont++; }
	cout << j[aj].Nev << ", " << pont << " pontot kaptál a csempe felrakásáért.\n";
	return pont;
}

// 8/d PONTLEVONÁS A PADLÓ MIATT
int PadloPontok(Player j[], int aj)
{
	int buntipontok = 0;								// A padlóra esett csempékért járó büntetõpontok
    // A padlón belüli pozíció
	for (int p = 0; p < PADLOMERET; p++) { if (j[aj].Padlovonal[p] != '.') { buntipontok += -(((p + 1) / 3) + 1); } }
	cout << j[aj].Nev << ", " << buntipontok << " büntetõpontot kaptál a padlóra esett csempék miatt.\n";
	return buntipontok;
}

// 8/e FAL FÜGGÕLEGES (OSZLOP) VIZSGÁLATA FALAZÁSHOZ
bool JoOszlop(Player j[], int aj, int ao, string csempe)
{
	bool joo = true;									// Az oszlop alkalmassága
	int i = 0;											// Oszlop-szkennelõ
	do {
        // Amint talál egyet a megfelelõ csempébõl hamisra állítja
		if (j[aj].wall[i][ao] == csempe[0]) { joo = false; }
		i++;
	} while (joo && i < TABLAMERET);
	return joo;
}

// 8/f CSEMPÉK ELDOBÁSA FALAZÁSKOR
void CsempeEldobas(Player j[], int dobott[], int aj, int msor)
{
	int i = 0;													// A Padlóvonal egy karakterének indexe
    // Ha a Padlóról kell eldobni csempét
	if (msor == 0) {
		while (i < PADLOMERET && j[aj].Padlovonal[i] != '.') {
            // Ha az eldobandó csempe a Kezdõjátékos-jelölõ, akkor nem kerül a Dobottak közé
			if (j[aj].Padlovonal[i] == 'K') { i++; }
			else {
				dobott[int(j[aj].Padlovonal[i]) - 65]++;
				i++;
			}
		}
	}
    // Ha a Mintasorból kell eldobni csempét
	else {
		while (i < j[aj].Mintasor[msor - 1].length() - 1) {
			dobott[int(j[aj].Mintasor[msor - 1][i]) - 65]++;
			i++;
		}
	}
}

// 8. FALAZÁS
bool Falazas(Game& game, Player player[])
{
	bool   jovalasz		 = false;				// A megfelelõ oszlop kivalasztasa
	string oszlop		 = "\0";				// A kiválasztott oszlop
	string csempe		 = "\0";				// Az éppen manipulált csempe
	int    osszespluszp	 = 0;					// A csempék falazásáért járó pontok
	int    buntetopontok = 0;					// A padlóra esett cempék miatt kapott büntetõpontok
	cout << "* * * Kezdõdik a falazás! * * *\n";;
    // Aktuális játékos sorszáma
	for (int i = 0; i < game.numberOfPlayers; i++) {
        // Mintasor szorszáma
		for (int sor = 0; sor < TABLAMERET; sor++) {
            // Ha az aktuális Mintasor nincs tele
			if (player[i].Mintasor[sor].find(".") == player[i].Mintasor[sor].npos) {
                // Ha a Falon abban a sorban még nincs a Mintasorban lévõkkel azonos csempe
				if (player[i].wall[sor].find(player[i].Mintasor[sor][0]) == player[i].wall[sor].npos) {
					KorongAsztalMegj(game);																// Megjeleníti a Korongok, az Asztal, a Zsák és a Dobott tartalmát
					PlayerMegj(game, player);															// Megjeleníti a játékosok tábláit
					csempe = player[i].Mintasor[sor][0];
					cout << player[i].Nev << ", a " << sor + 1 << ". sorból egy " << csempe << " csempét felfalazhatsz.\n";
					do {
						cout << "Melyik oszlopba szeretnéd teni? (1-tõl " << TABLAMERET << "-ig, a padlót a 0-val adhatod meg) ";
						cin >> oszlop;
                        // A játékos ki szeretne lépni, ezért már megyünk is vissza
						if (oszlop == "x" || oszlop == "X") { return true; }
                        // Ha a megadott oszlop nagyobb, mint az oszlopok száma (vagy nem szám)
						else if (int(oszlop[0]) - 48 > TABLAMERET) {
							jovalasz = false;
							cout << "HIBA: Ilyen sorszámú oszlop nincs a falon!\n";
						}
                        // Ha nem a Padlót választotta és a kiválasztott oszlopban lévõ hely nem üres
						else if (oszlop != "0" && player[i].wall[sor][int(oszlop[0]) - 49] != '.') {
							jovalasz = false;
							cout << "HIBA: A kiválasztott hely foglalt!\n";
						}
                        // Ha nem a Padlót választotta de a megadott oszlopban valahol már van ilyen csempe
						else if (oszlop != "0" && !JoOszlop(player, i, int(oszlop[0]) - 49, csempe)) {
							jovalasz = false;
							cout << "HIBA: Ebben az oszlopban már van ilyen csempe!\n";
						}
						else { jovalasz = true; }
					} while (!jovalasz);
                    // Ha a játékos a Padlót választotta
					if (oszlop == "0") {
                        // Annyiszor dobok csempét a padlóra, ahány darab a Mintasorban volt (azaz a Mintasor hossza)
						for (int p = 0; p < player[i].Mintasor[sor].length(); p++) {
							cout << "~ Egy " << csempe << " a(z) " << sor + 1 << ". sorból a Padlóra kerül.\n";
                            // Egyesével a Padlóra kerülnek a csempék
							CsempeAPadlora(game, player, csempe, i);
						}
					}
					else {
						cout << "~ Egy " << csempe << " a(z) " << sor + 1 << ". sorból a Fal " << oszlop[0] - 48 << ". oszlopába kerül.\n";
						player[i].wall[sor][int(oszlop[0]) - 49] = csempe[0];				// A falra kerül a csempe
						player[i].Pontok += FalPontok(player, i, sor, int(oszlop[0]) - 49);		// A falazásért pontot kap a játékos
						cout << "~ A Mintasorban maradt csempék eldobása...\n";
						CsempeEldobas(player, game.Dobott, i, sor + 1);						// Eldobjuk a maradék csempét a Mintasorból
					}
					cout << "~ Mintasor ürítése...\n";
					MintasorNullazas(player, i, sor);										// A Mintasor ürítése
				}
                // Ha a Falon már van a Mintasorban lévõkkel azonos csempe
				else {
					csempe = player[i].Mintasor[sor][0];
					cout << "~ Van már " << csempe << " csempe a Fal " << sor << ". sorában, ezért a Mintasorból az összes a Padlóra kerül.\n";
                    // Annyiszor dobok csempét a padlóra, ahány a Mintasorban volt
					for (int p = 0; p < player[i].Mintasor[sor].length(); p++) {
                        // Egyesével a Padlóra kerülnek a csempék
						CsempeAPadlora(game, player, csempe, i);
					}
					cout << "~ Mintasor ürítése...\n";
					MintasorNullazas(player, i, sor);								// A Mintasor ürítése
				}
			}
		}
		cout << "~ Büntetõpontok számítása...\n";
		buntetopontok = PadloPontok(player, i);				// A padlón lévõ csempék után járó büntetõpontok kiszámítása
        // Ha a büntetõpontok miatt negatív lenne a játékos pontszáma, akkor nulla pontja lesz
		if (player[i].Pontok + buntetopontok < 0) { player[i].Pontok = 0; }
		else { player[i].Pontok += buntetopontok; }
		CsempeEldobas(player, game.Dobott, i, 0);														// Eldobjuk a csempéket a Padlóról
		PadloNullazas(player, i);																	// Üríti a Padlót
		cout << player[i].Nev << ", a falazás után " << player[i].Pontok << " pontod van.\n";
	}
	return false;
}

// 9. BÓNUSZPONTOK SZÁMÍTÁSA
void BonuszPontok(Player j[], Score e[], int jsz)
{
	int bonuszsor = 0;										// A teljes sorokért járó bónuszpontok
	int bonuszoszlop = 0;									// A teljes oszlopokért járó bónuszpontok
	int bonuszszin = 0;										// A teljes színekért járó bónuszpontok
	bool teljesoszlop = true;								// Van teljes oszlop
	bool teljesszin = true;									// Van teljes szín
	int o = 0;												// Oszlopkeresõ
	int sz = 0;												// Színkeresõ
	cout << "\n* * * Bónuszpontok * * *\n\n";
    // A játékosok indexe
	for (int i = 0; i < jsz; i++) {
        // Nullázzuk a sorok számát az adott játékos eredményjelzõjénél
		e[i].SorokSzama = 0; 
		for (int s = 0; s < TABLAMERET; s++) {
            // Ha a Fal adott sorában nincs üres hely, akkor jár a 2 pont
			if (j[i].wall[s].find(".") == j[i].wall[s].npos) {
				bonuszsor += 2;
				e[i].SorokSzama++;							// Ha találtunk nála egy sort, akkor az eredményjelzõn is kap érte 1 pontot
			}
			teljesoszlop = true;
			o = 0;
			do {
                // Amint talál az oszlopban egy üres helyet, már nem teljes az osztlop
				if (j[i].wall[o][s] == '.') { teljesoszlop = false; }
				o++;
			} while (teljesoszlop && o < TABLAMERET);
            // Ha a Fal adott oszlopában nincs üres hely, akkor jár a 7 pont
			if (teljesoszlop) { bonuszoszlop += 7; }
		}
        // A csempefajták
		for (int c = 0; c < CSVARIACIOK; c++) {
			teljesszin = true;
			sz = 0;
			do {
                // Amint talál egy olyan sort, amiben nincs az adott cseme, már nem teljes a szín
				if (j[i].wall[sz].find(char(c + 65)) == j[i].wall[sz].npos) { teljesszin = false; }
				sz++;
			} while (teljesszin && sz < TABLAMERET);
            // Ha nincs olyan sor a Falon, amiben ne szerepelne az adott Csempe, akkor jár a 10 pont
			if (teljesszin) { bonuszszin += 10; }
		}
		j[i].Pontok += bonuszsor + bonuszoszlop + bonuszszin;
		cout << j[i].Nev << " bónuszpontjai:\n\t+"
			<< bonuszsor << " pont a sorokért\n\t+"
			<< bonuszoszlop << " pont az oszlopokért\n\t+" 
			<< bonuszszin << " pont a teljes színekért.\n";
	}
}

// 10. EREDMÉNYEK KIÍRÁSA
void Vegeredmeny(Player j[], Score e[], int jsz)
{
	string n = "\0";									// Neveket tároló segédváltozó
	int p = 0;											// Pontokat tároló segédváltozó
	int s = 0;											// A Sorok számát tároló segédváltozó

    // Átmásoljuk az eredményjelzõ tömbbe a játékosok neveit és pontszámait
	for (int m = 0; m < jsz; m++) {
		e[m].Nev = j[m].Nev;
		e[m].Pont = j[m].Pontok;
	}

    // A kisebb indexû elem
	for (int i = 0; i < jsz - 1; i++) {
        // Az egyel nagyobb indexû elem
		for (int k = i + 1; k < jsz; k++) {
            // Ha a kisebbik indexû kisebb, mint a nagyobb indexû, akkor csere (mert a helyezéseket az index határozza meg)
			if (e[i].Pont < e[k].Pont) {
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
            // Ha a két egymás melletti elem egyenlõ, akkor a sorok száma dönt
			else if (e[i].Pont == e[k].Pont && e[i].SorokSzama < e[k].SorokSzama) {
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
	cout << "\n\n* * * VÉGEREDMÉNY * * *\n\n";
	for (int l = 0; l < jsz; l++) { cout << l + 1 << ". helyezett: " << e[l].Nev << ", " << e[l].Pont << " ponttal, " << e[l].SorokSzama << " teljes sorral\n"; }
}

// 11. KEZDÕ KÉPERNYÕ
void KezdoKepernyo()
{
	system("cls");
	cout << endl;
	cout << "   A   ZZZZZ U   U L            Beadandó feladat\n";;
	cout << "  A A      Z U   U L            Programozás (GKLB_INTM021)\n";
	cout << " A   A    Z  U   U L            tárgyból.\n";
	cout << " A   A   Z   U   U L            \n";
	cout << " AAAAA  Z    U   U L            Készítette:\n";
	cout << " A   A Z     U   U L            Borsodi Zoltán\n";
	cout << " A   A ZZZZZ  UUU  LLLLL        (b7pk8z)\n";
	cout << endl;
	cout << "------------------------------------------------------------\n";
	cout << endl;
	cout << " Ez az AZUL címû társasjáték konzolban játszható változata.\n";
	cout << " A színes csempéket nagybetûk helyettesítik: A, B, C, D és E\n\n";
	cout << " A játék addig tart, amíg legalább egy játékos felépít egy\n";
	cout << " különbözõ csempébõl álló, teljes sort a Falán.\n";
	cout << endl;
	cout << "------------------------------------------------------------\n";
	cout << endl;
}
// 11. ZÁRÓ KÉPERNYÕ
void ZaroKepernyo()
{
	system("cls");
	cout << endl;
	cout << "   A   ZZZZZ U   U L        b               QQQ  BBBB  ZZZZZ\n";
	cout << "  A A      Z U   U L        b     y   y    Q   Q B   B     Z\n";
	cout << " A   A    Z  U   U L        b bb  y   y    Q   Q B   B    Z \n";
	cout << " A   A   Z   U   U L        bb  b y   y    Q   Q BBBB    Z  \n";
	cout << " AAAAA  Z    U   U L        b   b  yyyy    Q   Q B   B  Z   \n";
	cout << " A   A Z     U   U L        bbbb      y     QQQ  B   B Z    \n";
	cout << " A   A ZZZZZ  UUU  LLLLL           yyy     Q     BBBB  ZZZZZ\n";
	cout << endl;
	cout << "-------------------------------------------------------------\n";
	cout << endl;
	cout << " Készítette: Borsodi Zoltán\n";
	cout << "             (SZE, mérnökinformatika, levelezõ)\n";
	cout << endl;
	cout << " Neptun kód: b7pk8z\n";
	cout << endl;
	cout << " E-mail:     borsodi.zoltan@gmail.com\n";
	cout << endl;
	cout << "-------------------------------------------------------------\n";
	cout << endl;
	cout << " Köszönet az ötletelésért és a tesztelésért\n";
	cout << " Eszenyi Ákosnak, Tenk Norbertnek,\n";
	cout << " Pergel Dánielnek és Varga Lászlónak!\n";
	cout << endl;
	cout << "-------------------------------------------------------------\n";
}

int main()
{
	setlocale(LC_ALL, "hun");								// Magyarra állítja a nyelvet
	srand((unsigned)time(NULL));							// véletlen-generátor inicializálása

	Game	game;											// Változó a Game struktúrával
	Player* players = new Player[game.numberOfPlayers];		// A játékosok adatait tartalmazó Player típusú tömb létrehozása a játékosszám alapján
	Score*	score	= new Score[game.numberOfPlayers];		// Az eredmények kijelzéséhez szükséges tömb létrehozása a játékosszám alapján

	PlayerElokeszites(game, players);						// A Player típusú tömb elõkészítése a játékosszám alapján
	jatekosNevekMegadasa(game, players);					// Opcionálisan megadható a játékosok beceneve
	kezdoJatekosMegadasa(game, players);					// A kezdõjátékos megadása

	while (!game.jatekvege) {
		game.jatekvege = KorongTolto(game);																					// Feltölti a Korongokat, az Asztalta teszi a kezdõjátékos jelölõt
		game.ujFordulo(players);
		while (!game.fordulovege && !game.jatekvege) {
			game.lepes++;
//			system("cls");
			cout << "* * *  CSEMPEVÁSÁR " << game.fordulo << ". forduló " << game.lepes << ". lépés  * * *\n\n";
			KorongAsztalMegj(game);																					// Megjeleníti a Korongok, az Asztal, a Zsák és a Dobott tartalmát
			PlayerMegj(game, players);																				// Megjeleníti a játékosok tábláit
			game.kilepes = CsempeVasar(game, players);																// A játékos csempéket vásárolhat a Korongokról vagy az Asztalról
			if (game.currentPlayer == game.numberOfPlayers - 1) { game.currentPlayer = 0; }							// Egyel léptei az játékosokat
			else { game.currentPlayer++; }
			game.fordulovege = ForduloVege(game) || game.kilepes;													// A forduló végének leellenõrzése
		}
		if (!game.kilepes) {
			game.kilepes = Falazas(players, game.Korongok, game.AsztalKozep, game.Zsak, game.Dobott, game.KorongSzam, game.numberOfPlayers);		// Falazás + pontok
		}
		game.jatekvege = JatekVege(players, game.numberOfPlayers, game.Zsak, game.Dobott) || game.kilepes;										// A játék végének ellenõrzése
	}
	BonuszPontok(players, score, game.numberOfPlayers);
	Vegeredmeny(players, score, game.numberOfPlayers);
	cout << "\nGRATULÁLOK!\n";
	cout << "Köszönöm, hogy játszottatok!\n\n\n";
	system("pause");
	ZaroKepernyo();
	system("pause");
	delete[]game.Korongok;
	delete[]players;
	return 0;
}