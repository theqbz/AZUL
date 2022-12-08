/*
AZUL - beadand� feladat Programoz�s (GKLB_INTM021) t�rgyb�l
Borsodi Zolt�n (B7PK8Z)
borsodi.zoltan@gmail.com
*/

#include"iostream"
#include"iomanip"
#include"cmath"
#include"cstdlib"
#include"ctime"
#include"string"

using namespace std;

#define CSVARIACIOK 5		// Ennyi k�l�nb�z� csempefajta van a j�t�kban.
#define CSKEZDODARAB 20		// Egy adott fajta csemp�kb�l ennyi darab van �sszesen.
#define KMERET 4			// A korongokra helyezhet� csepmp�k maxim�lis sz�ma.
#define TABLAMERET 5		// A Mintasor illetve a Fal m�retei a j�t�kosok t�bl�j�n. (�ltal�ban megegyezik a csempefajt�k sz�m�val azaz CSVARIACIOK v�ltoz�val.)
#define PADLOMERET 7		// A Padlovonal m�rete a j�t�kosok t�bl�j�n.

struct Game												// A j�t�k f� adatai
{
	string*	 Korongok;									// A korongok t�mbje
	uint8_t  KorongSzam;								// A korongok sz�ma
	uint8_t  Zsak[CSVARIACIOK];							// A zs�k t�mbj�nek helye 
	uint8_t  Dobott[CSVARIACIOK];						// Az eldobott csemp�k t�mbj�nek helye
	uint8_t  AsztalKozep[CSVARIACIOK + 1];				// Az asztal k�zep�t t�rol� t�mb helye
	uint8_t	 numberOfPlayers;							// A j�t�kosok sz�ma
	uint8_t	 currentPlayer;								// Az aktu�lisan k�vetkez� j�t�kos
	bool	 jatekvege;									// A j�t�k v�ge
	bool	 fordulovege;								// A fordul� v�ge
	bool	 kilepes;									// A j�t�kos ki akar l�pni
	uint16_t fordulo;									// A fordul� sorsz�ma
	uint16_t lepes;										// A fordul�n bel�li l�p�sek sz�ma

	Game()
	{
		zsakokEsAsztalInicializalasa();
        numberOfPlayers = JatekosSzamBe();
        KorongSzam		= numberOfPlayers * 2 + 1;		// A korongok sz�ma egyel t�bb, mint a j�t�kosok sz�m�nak k�tszerese
        Korongok		= new string[KorongSzam];		// A Korongok t�mb elk�sz�t�se a korongok sz�ma alapj�n
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
            Zsak[i] = CSKEZDODARAB;						// A zs�k rekeszeinek felt�lt�se a csemp�k kezdeti darabsz�m�val
            Dobott[i] = 0;								// A dobottak zs�k null�z�sa
            AsztalKozep[i] = 0;							// Az asztal k�zep�nek null�z�sa
        }
        AsztalKozep[CSVARIACIOK] = 1;					// Itt tessz�k le az asztal k�zep�re a kezd�j�t�kos jelz�t
    }

	int JatekosSzamBe()
	{
		string jszbe = "";
		do {
			cout << "Add meg a j�t�kosok sz�m�t (2, 3 vagy 4 j�t�kos lehet!): ";
			cin >> jszbe;
		} while (jszbe != "2" && jszbe != "3" && jszbe != "4");
		return int(jszbe[0]) - 48;
	}

	void KezdoKepernyo()
	{
		//system("cls");
		cout << endl;
		cout << "   A   ZZZZZ U   U L            Beadand� feladat\n";;
		cout << "  A A      Z U   U L            Programoz�s (GKLB_INTM021)\n";
		cout << " A   A    Z  U   U L            t�rgyb�l.\n";
		cout << " A   A   Z   U   U L            \n";
		cout << " AAAAA  Z    U   U L            K�sz�tette:\n";
		cout << " A   A Z     U   U L            Borsodi Zolt�n\n";
		cout << " A   A ZZZZZ  UUU  LLLLL        (b7pk8z)\n";
		cout << endl;
		cout << "------------------------------------------------------------\n";
		cout << endl;
		cout << " Ez az AZUL c�m� t�rsasj�t�k konzolban j�tszhat� v�ltozata.\n";
		cout << " A sz�nes csemp�ket nagybet�k helyettes�tik: A, B, C, D �s E\n\n";
		cout << " A j�t�k addig tart, am�g legal�bb egy j�t�kos fel�p�t egy\n";
		cout << " k�l�nb�z� csemp�b�l �ll�, teljes sort a Fal�n.\n";
		cout << endl;
		cout << "------------------------------------------------------------\n";
		cout << endl;
	}

	void ZaroKepernyo()
	{
		//system("cls");
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
		cout << " K�sz�tette: Borsodi Zolt�n\n";
		cout << "             (SZE, m�rn�kinformatika, levelez�)\n";
		cout << endl;
		cout << " Neptun k�d: b7pk8z\n";
		cout << endl;
		cout << " E-mail:     borsodi.zoltan@gmail.com\n";
		cout << endl;
		cout << "-------------------------------------------------------------\n";
		cout << endl;
		cout << " K�sz�net az �tletel�s�rt �s a tesztel�s�rt\n";
		cout << " Eszenyi �kosnak, Tenk Norbertnek,\n";
		cout << " Pergel D�nielnek �s Varga L�szl�nak!\n";
		cout << endl;
		cout << "-------------------------------------------------------------\n";
	}

};

struct Player							// A j�t�kosokhoz tartoz� adatok
{
	string Nev;							// A j�t�kos neve
	int Pontok;							// A j�t�kos pontsz�ma
	string Mintasor[TABLAMERET];		// A mintasor, ahov� a korongokr�l vagy asztalr�l v�s�rolt csemp�ket gy�jti.
	string wall[TABLAMERET];				// A fal, ahov� felteszi a mintasoron �sszegy�lt csemp�ket.
	string Padlovonal;					// A padl�vonal ahol a b�ntet�pontok gy�lnek.
	bool kovetkezik;					// Azt mutatja, hogy a j�t�kos k�vetkezik-e
};

struct Score							// A v�geredm�ny kijelz�s�hez sz�ks�ges adatok
{
	string Nev;							// A j�t�kos neve
	int Pont;							// A j�t�kos pontsz�ma
	int SorokSzama;						// A j�t�kos �ltal bet�lt�tt sorok sz�ma
};


// 0/a J�TSZHAT� CSEMP�K ELLEN�RZ�SE
bool ElfogyottACsempe(Game& game)
{
	bool ureszsak = false;							// Nincs csempe a Zs�kban
	bool uresdobott = false;						// Nincs csempe a Dobottakban
	int k = 0;
    // Addig n�zi am�g nem tal�l legal�bb egy nem �res rekeszt
	while (k < CSVARIACIOK && game.Zsak[k] == 0) {
		ureszsak = true;							// Akkor kell belemennem, ha �res rekeszt tal�lok, egy�bk�nt (azaz nem �res) mehet�nk tov�bb, mert van m�g csempe
		k++;
	}
	k = 0;
	while (k < CSVARIACIOK && game.Dobott[k] == 0) {
		uresdobott = true;
		k++;
	}
	if (ureszsak) {
        // Ha elfogyott minden csempe a Zs�kb�l �s a Dobottak k�z�l, akkor v�ge a j�t�knak
		if (uresdobott) {
			cout << "~ Elfogyott minden csempe => v�ge a j�t�knak.\n";
			return true;
		}
        // Ha csak a Zs�k �res, �tpakoljuk a Dobottakban t�rolt �rt�keket a Zs�kba
		else {
			cout << "~ A ki�r�lt Zs�k felt�lt�se...\n";
			for (int i = 0; i < CSVARIACIOK; i++) {
				game.Zsak[i] = game.Dobott[i];
				game.Dobott[i] = 0;
			}
		}
	}
	return false;
}

// 0/b A J�T�K V�G�NEK ELLEN�RZ�SE
bool JatekVege(Game& game, Player player[])
{
    // Amint valakin�l tal�l teljes sort a Falon, v�ge a j�t�knak
	cout << "~ J�t�k v�g�nek ellen�rz�se...\n";
	bool completedLine = false;
	int i = 0;
	while (!completedLine && i < game.numberOfPlayers) {
		int line = 0;
		while (!completedLine && line < TABLAMERET) {
			if (player[i].wall[line].find(".") == player[i].wall[line].npos) {
				cout << "~ " << player[i].Nev << " fal�nak " << line << ". sora teljes lett => v�ge a j�t�knak.\n";
				completedLine = true;
			}
			line++;
		}
		i++;
	}
	return completedLine || ElfogyottACsempe(game);
}

// 0/c FORDUL� V�G�NEK ELLEN�RZ�SE
bool ForduloVege(Game& game)
{
	bool kures = true;					// A Korongok ki�r�ltek-e
	bool aures = true;					// Az asztal �res-e
	int kk = 0;							// Korong-szkennel�
	int ak = 0;							// Asztal-szkennel�
	cout << "~ Fordul� v�g�nek ellen�rz�se...\n";
    // Amint tal�l egy nem �res Korongot, r�gt�n hamisra �ll�tja
	do {
		if (game.Korongok[kk] != "....") { kures = false; }
		kk++;
	} while (kures && kk < game.KorongSzam);
    // Amint tal�l csemp�t az Asztalon, r�gt�n hamisra �ll�tja
	do {
		if (game.AsztalKozep[ak] != 0) { aures = false; }
		ak++;
	} while (aures && ak < CSVARIACIOK);
	return kures && aures;
}

// 0/d ELD�NTEND� K�RD�S
bool Dontes(string szoveg)
{
	string valasz = "\0";
	do {
		cout << szoveg;
		cin >> valasz;
        // Nagybet�ss� alak�tjuk a bevitt sz�veget
		for (int n = 0; n < int(valasz.length()); n++) { valasz[n] = toupper(valasz[n]); }
	} while (valasz != "I" && valasz != "N");
	if (valasz == "I") { return true; }
	else { return false; }
}

// 1/b J�T�KOSOK NEV�NEK BEK�R�SE (OPCION�LIS)
void jatekosNevekMegadasa(const Game& game, Player player[])
{
	if (!Dontes("Megadod a j�t�kosok becenev�t? (i / n) ")) return;
    string kuka = "\0";														// A cin �r�t�se
    getline(cin, kuka, '\n');
    for (int i = 0; i < game.numberOfPlayers; i++) {
        cout << i + 1 << ". j�t�kos neve: ";
        getline(cin, player[i].Nev, '\n');
    }
}

// KEZD�J�T�KOS MEGAD�SA
void kezdoJatekosMegadasa(const Game& game, Player player[])
{
    for (int i = 0; i < game.numberOfPlayers; i++) player[i].kovetkezik = false;
	string kezd = "\0";
	bool jokezdo = false;
	while (!jokezdo) {
		cout << "Ki kezd? (A j�t�kos sorsz�m�t add meg!) ";
		cin >> kezd;
		if (kezd.length() != 1) { jokezdo = false; }
		else if (int(kezd[0]) < 49 || int(kezd[0]) - 48 > game.numberOfPlayers) {
			jokezdo = false;
			cout << "HIBA: Ilyen sorsz�m� j�t�kos nincs!\n";
		}
        else { jokezdo = true; }
	}
	player[int(kezd[0] - 49)].kovetkezik = true;
}

// 3/a CSEMPE EL�H�Z�SA A ZS�KB�L
int CsempeHuzas(Game& game)
{
    // A zs�kban l�v�, k�l�nb�z� sz�n� csemp�k sz�m�t �sszeadja a v�letlen sz�m gener�l�shoz
	int zsakbanLevoCsempekSzama = 0;
	for (int i = 0; i < CSVARIACIOK; i++) zsakbanLevoCsempekSzama += game.Zsak[i];
	int x = rand() % zsakbanLevoCsempekSzama + 1;
	// Ki kell keresni, hogy a v�letlen sz�m melyik sz�n� csemp�t jel�li:
    // ehhez addig b�v�ti a keres�vektort a zs�k elemeinek mennyis�g�vel,
	// am�g a keres�vektorn�l kisebb nem lesz a gener�lt v�letlen sz�mn�l.
	// �gy a keresett (kih�zand�) csempesz�nt az aktu�lis indexe jel�li
	int i = 0;
	int vektor = 0;
	while (vektor < x && i < CSVARIACIOK) vektor += game.Zsak[i++];
	return i;
}

// 3. KORONGOK FELT�LT�SE
bool KorongTolto(Game& game)
{
	cout << "~ Korongok felt�lt�se...\n";
    // Egyes�vel v�gigmegy a korongokon
	for (int i = 0; i < game.KorongSzam; i++) {
        // Annyi csemp�t h�z a zs�kb�l, ah�ny elf�r egy korongon
		for (int j = 0; j < KMERET; j++) {
            // Ha m�r nincs csempe a Zs�kban vagy a Dobottak k�z�tt, akkor v�ge a j�t�knak
			if (ElfogyottACsempe(game)) { return true; }
            // egy�bk�nt h�z egy csemp�t (ez a zs�k t�mb indexe), amib�l eggyel kevesebb marad a zs�kban �s ami r�ker�l az aktu�lis korongra
            int kihuzottCsempe = CsempeHuzas(game);
            game.Zsak[kihuzottCsempe]--;
            game.Korongok[i] += char(65 + kihuzottCsempe);
		}
	}
    // Kezd�j�t�kos jel�l� kihelyez�se az asztal k�zep�re
	game.AsztalKozep[CSVARIACIOK] = 1;
	return false;
}

// 4/a K�VETKEZ�-J�T�KOS JEL�L� RESETEL�SE
void KovJatekosReset(const Game& game, Player player[])
{
	for (int i = 0; i < game.numberOfPlayers; i++) { player[i].kovetkezik = false; }
}

// 4. A PLAYER STRUKT�R�J� T�MB EL�K�SZ�T�SE
int PlayerElokeszites(const Game& game, Player player[])
{
	for (int i = 0; i < game.numberOfPlayers; i++) {
		player[i].Nev = char(48 + i + 1);			// A n�v hely�re egyel�re csak sorsz�m ker�l
		player[i].Pontok = 0;						// A pontok null�z�dnak
		player[i].Padlovonal = "\0";				// A padl�vonal �res legyen
		string minta = "\0";						// A Mintasor kirajzols�shoz sz�ks�ges v�ltoz�
		for (int j = 0; j < TABLAMERET; j++) {
			player[i].Mintasor[j] = minta + ".";	// A Mintasor kirajzol�sa hely�rz�kkel
			player[i].wall[j] = ".....";			// A Fal kirajzol�sa hely�rz�kkel
			minta += ".";
		}
		for (int k = 0; k < PADLOMERET; k++) {
			player[i].Padlovonal += ".";			// A Padl�vonal kirajzol�sa hely�rz�kkel
		}
	}
	KovJatekosReset(game, player);					// A k�vetkez�-j�t�kos jel�l�t mindenhol hamisra �ll�tja
	return 0;
}


// 5/a HELY�RZ� SZ�K�Z�K
string Helyorzo(string ertek)
{
	if (ertek.length() == 1) { return ertek + " "; }
	else { return ertek; }
}

// 5. KORONGOKON �S ASZTAL K�ZEP�N L�V� CSEMP�K MEGJELEN�T�SE
void asztalMegj(const Game& game)
{
	const int sorokszama = 8;						// A ki�rand� sorok sz�ma �sszesen
	string sorok[sorokszama];						// Ide gy�jtj�k a ki�rand� sorok tartalm�t
	string elv = "|  ";								// Elv�laszt� a t�bl�zat r�szei k�z�tt
    // A T�BL�ZAT BAL OLDALA
	for (int i = 0; i < sorokszama; i++) { sorok[i] = elv; }
	// A KORONGOK
	int kszel = (game.KorongSzam / 2 + 1) * 5;										// Ilyen sz�les hely kell a korongok megjelen�t�s�hez
	sorok[0] += "Korongok:";											// A korongok r�sz c�msora
    // Kit�lti a sz�ks�ges mennyis�g� sz�k�zzel a korongok sz�m�nak f�ggv�ny�ben
	for (int k = 0; k < kszel - 9; k++) { sorok[0] += " "; }
	sorok[0] += elv;
    // A korongok sz�moz�sa: 1. sor
	for (int k = 0; k < ((game.KorongSzam / 2) + 1); k++) { sorok[1] += to_string(k + 1) + ".   "; }
	sorok[1] += elv;
    // A korongok tartalma: els� k�t sor
	for (int i = 0; i < 2; i++) {
        // Elsz�mol a korongok sz�m�nak fel�n�l egyel tov�bb...
		// ...�s az aktu�lis korongr�l kettes�vel ki�rja a csemp�ket (el�bb az els� kett�t azt�n a m�sodik kett�t)
		for (int k = 0; k < ((game.KorongSzam / 2) + 1); k++) { sorok[i + 2] += game.Korongok[k].substr((i * 2), 2) + "   "; }
		sorok[i + 2] += elv;
	}
    // Elv�laszt� �res sor
	for (int k = 0; k < kszel; k++) { sorok[4] += " "; }
	sorok[4] += elv;
    // A korongok sz�moz�sa: 2. sor
	for (int k = ((game.KorongSzam / 2) + 1); k < game.KorongSzam; k++) { sorok[5] += to_string(k + 1) + ".   "; }
	sorok[5] += "     " + elv;
    // A korongok tartalma: m�sdoik k�t sor
	for (int i = 0; i < 2; i++) {
        // A sz�mol�s a korongok sz�m�nak fel�n�l egyel t�bbr�l indul �s a korongok sz�m�ig tart...
        // ...�s az aktu�lis korongr�l kettes�vel ki�rja a csemp�ket
		for (int k2 = ((game.KorongSzam / 2) + 1); k2 < game.KorongSzam; k2++) { sorok[i + 6] += game.Korongok[k2].substr((i * 2), 2) + "   "; }
		sorok[i + 6] += "     " + elv;
	}
	// AZ ASZTAL
	// Az asztal r�sz c�msora
	sorok[0] += "Asztal:    " + elv;
    // A csemp�k sz�m�nak ki�r�sa
	for (int i = 0; i < CSVARIACIOK; i++) {
		sorok[i + 1] += char(65 + i);
		sorok[i + 1] += ": " + to_string(game.AsztalKozep[i]) + "       " + elv;
	}
    // �res sor
	sorok[6] += "           " + elv;
    // Ha m�g az asztalon van a kezd�j�t�kos-jel�l�...
    // ...fel�rja azt is...
	if (game.AsztalKozep[CSVARIACIOK] == 1) { sorok[7] += "Kezd�      " + elv; }
    // ...k�l�nben �res sort �r.
	else { sorok[7] += "           " + elv; }
	// A ZS�K �S DOBOTTAK
	sorok[0] += "Zs�k:           ";
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
    // SOROK KI�R�SA
	for (int ki = 0; ki < sorokszama; ki++) { cout << sorok[ki] << endl; }
	cout << endl;
}

// 6a PLAYER MEGJELEN�T�SE
void PlayerMegj(Game& game, Player player[])
{
	const int fs = 4;							// A fejl�cbe ki�rand� sorok sz�ma
	const int ps = 3;							// A padl�vonalra ki�rand� sorok sz�ma
	const int ssz = fs + TABLAMERET + ps;		// A ki�rand� sorok sz�ma �sszesen: a fejl�c, a t�bla �s a padl�ra ki�rand� sorok sz�ma
	const int tszel = (PADLOMERET * 3) + 2;		// Az t�bl�k sz�less�ge
	string sorok[ssz];							// Ebbe gy�jtj�k a j�t�kos el�tti t�bla ki�rand� tartalm�t soronk�nt
    // Sorok null�z�sa
	for (int i = 0; i < ssz; i++) { sorok[i] = "\0"; }
    // A j�t�kosok indexe
	for (int ji = 0; ji < game.numberOfPlayers; ji++) {
		// A ki�r�s el�k�sz�t�se: vonal �s t�ltel�k
		string vonal = "\0";							// Ez lesz a t�bl�zat v�zszintes vonala
		int sszel = tszel * (ji + 1);					// A gyarapod� sorok aktu�lis maximum sz�less�ge
        // Ha az adott j�t�kos k�vetkezik, akkor a v�zszintes vonal *-karakterb�l �ll
		if (ji == game.currentPlayer) { for (int i = 0; i < tszel - 1; i++) { vonal += "*"; } }
		else { for (int i = 0; i < tszel - 1; i++) {
				if (i == TABLAMERET + 5) { vonal += "+"; }
				else { vonal += "-"; }
			}
		}
		string toltelek = "\0";							// Helykit�lt� sz�k�z�k a Mintasor el�, hogy balra z�rt legyen
        // A t�ltel�kkarakterek elk�sz�t�se
		for (int i = 0; i < TABLAMERET - 1; i++) { toltelek += " "; }
		// A t�bla bal oldala
		for (int i = 0; i < ssz; i++) { sorok[i] += "|"; }
		// FEJL�C
		sorok[0] += vonal;
		sorok[1] += " " + to_string(ji + 1) + " - ";				// A j�t�kos sorsz�ma
		sorok[1] += player[ji].Nev.substr(0, tszel - 7);					// N�v (csonkolva, ha hossz�)
		sorok[2] += " " + to_string(player[ji].Pontok) + " pont";		// Pontsz�m
        // A 4. sor elej�t t�lti fel sz�k�zzel
		while (int(sorok[3].length()) < sszel - (TABLAMERET * 2)) { sorok[3] += " "; }
        // A 4. sor v�g�t felt�lti az oszlopok sorsz�m�val
		for (int i = 0; i < TABLAMERET; i++) { sorok[3] += to_string(i + 1) + " "; }
		// MINTASOR �S FAL
		for (int i = 0; i < TABLAMERET; i++) {
			sorok[i + fs] += "    " + toltelek + player[ji].Mintasor[i];	// T�ltel�k �s a Mintasor tartalma
			sorok[i + fs] += " " + to_string(i + 1) + " ";			// Sorsz�m a Mintasor �s a Fal k�z�tt
			for (int f = 0; f < TABLAMERET; f++) {
				sorok[i + fs] += player[ji].wall[i][f];					// A Fal tartalma sz�k�z�kkel elv�lasztva
				sorok[i + fs] += " ";
			}
			toltelek = toltelek.substr(0, toltelek.size() - 1);		// Egyel cs�kkenti a t�ltel�k hossz�s�g�t
		}
		// PADL�VONAL
		sorok[fs + TABLAMERET] += vonal;
		int p = 0;													// Padl�vonal aktu�lis karaktere
		while (p < PADLOMERET && int(sorok[fs + TABLAMERET + 1].length()) < sszel) {
			sorok[fs + TABLAMERET + 1] += "  ";
			sorok[fs + TABLAMERET + 1] += player[ji].Padlovonal[p];
			sorok[fs + TABLAMERET + 2] += " " + to_string(-(((p + 1) / 3) + 1));
			p++;
		}
		// V�G�L A MARAD�KOT MINDEN SORBAN FELT�LTI SZ�K�ZZEL
		for (int u = 0; u < ssz; u++) { while (int(sorok[u].length()) < sszel) { sorok[u] += " "; } }
	}
    // AZ ELK�SZ�LT SOROK KI�R�SA
	for (int ki = 0; ki < ssz; ki++) { cout << sorok[ki] << "|" << endl; }
	cout << endl;
}

// 7/a J�T�KOS KORONG- �S CSEMPEV�LASZT�SA V�S�RL�SHOZ
const string KCsValaszto(const Game& game, const Player player[])
{
	string kivalasztottCsempe = "\0";
	bool joValasztas = false;
	while (!joValasztas) {
		cout << player[game.currentPlayer].Nev << ", v�lassz csemp�ket egy korongr�l vagy az asztalr�l!\n(Pl. 1. korong A csempe: 1A | Az asztalt a 0 jel�li: 0D) ";
		cin >> kivalasztottCsempe;
        // El�bb nagybet�ss� alak�tjuk a bevitt sz�veget
		for (int n = 0; n < int(kivalasztottCsempe.length()); n++) { kivalasztottCsempe[n] = toupper(kivalasztottCsempe[n]); }
        // Ha ki szeretne l�pni r�gt�n megy is vissza
		if (kivalasztottCsempe == "X") { joValasztas = true; }
        // Ha nem k�t karaktert �rt be, m�r nem j�
		else if (kivalasztottCsempe.length() != 2) {
			joValasztas = false;
			cout << "HIBA: nem ket karakter!\n";
		}
        // Ha az els� karakter nem, egy a korongok sz�m�n�l kisebb sz�m
		else if (int(kivalasztottCsempe[0]) < 48 || int(kivalasztottCsempe[0]) - 48 > game.KorongSzam) {
			joValasztas = false;
			cout << "HIBA: Ilyen korong nincs a j�t�kban!\n";
		}
        // Ha m�sodik karakter nem olyan NAGYbet�, ami megfelel a csemp�k bet�jeleinek
		else if (!(int(kivalasztottCsempe[1]) > 64 && int(kivalasztottCsempe[1]) - 65 < CSVARIACIOK)) {
			joValasztas = false;
			cout << "HIBA: Ilyen bet�jel� csempe nincs a j�t�kban!\n";
		}
        // Ha korongot v�lasztott �s az �res, akkor nem j�
		else if (int(kivalasztottCsempe[0]) - 48 != 0 && game.Korongok[int(kivalasztottCsempe[0]) - 49] == "....") {
			joValasztas = false;
			cout << "HIBA: A v�lasztott korong �res!\n";
		}
        // Ha az asztalt v�lasztotta, �s egy sincs rajta a k�rt csemp�b�l, akkor nem j�
		else if (int(kivalasztottCsempe[0]) - 48 == 0 && game.AsztalKozep[int(kivalasztottCsempe[1]) - 65] == 0) {
			joValasztas = false;
			cout << "HIBA: A v�lasztott csemp�b�l nincs az asztalon!\n";
		}
        // Ha a korongot v�lasztott de nincs rajta a k�rt csempe, akkor nem j�
		else if (int(kivalasztottCsempe[0]) - 48 != 0 && game.Korongok[int(kivalasztottCsempe[0]) - 49].find(kivalasztottCsempe[1]) == game.Korongok[int(kivalasztottCsempe[0])].npos) {
			joValasztas = false;
			cout << "HIBA: A korongon nincs ilyen csempe!\n";
		}
		else { joValasztas = true; }
	}
	return kivalasztottCsempe;
}

// 7/b A J�T�KOS MINTASOR V�LASZT�SA V�S�RL�SHOZ
const char SorValaszto(const Game& game, const Player player[], const uint16_t csempe)
{
    cout << "SOR VALASZTO FUGGVENY\n";
	// Van-e m�g egy�ltal�n olyan mintasor, ahov� csemp�t lehet rakni?
	int i = 0;
	while (i < TABLAMERET && !(player[game.currentPlayer].Mintasor[i].find(".") == player[game.currentPlayer].Mintasor[i].npos)) i++;
	// Ha nincs -  teh�t minden Mintasor tele van - akkor a csemp�k mindenk�ppen a padl�ra ker�lnek
	if (i == TABLAMERET) return '0';
	// Egy�bk�nt meg kell adni, hogy melyik sorba ker�ljenek a csemp�k
    string sor;
    bool josor = false;
	while (!josor) {
        cout << "Melyik Mintasorba szeretn�d tenni? (1-t�l " << CSVARIACIOK << "-ig | A Padl�t a 0-val adhatod meg.) ";
        cin >> sor;
        // Ha a kil�p�st v�lasztja, m�r megy is vissza
        if (sor == "x" || sor == "X") { josor = true; }
        // Ha a megadott sz�m nagyobb, mint ah�ny mintasor van, akkor nem j�
        else if (int(sor[0]) - 48 > TABLAMERET) {
            cout << "HIBA: Ilyen sorsz�m� Mintasor nincs!\n";
            josor = false;
        }
        // Ha a j�t�kos nem a padl�t v�lasztotta...
        // ...�s a kiv�lasztott Mintasor m�r nem �res...
        // ...�s nincs benne a kiv�lasztott fajt�j� csempe, akkor nem j�
        else if (sor != "0" && player[game.currentPlayer].Mintasor[int(sor[0]) - 49][0] != '.' && player[game.currentPlayer].Mintasor[int(sor[0]) - 49][0] != csempe) {
            cout << "HIBA: Abban a sorban m�r m�sfajta csemp�k vannak!\n";
            josor = false;
        }
        // Ha nem a padl�t v�lasztotta �s a kiv�lasztott sor tele van...
		else if (sor != "0" && player[game.currentPlayer].Mintasor[int(sor[0]) - 1].find(".") == player[game.currentPlayer].Mintasor[int(sor[0]) - 1].npos) {
            // ...akkor, ha ez �gy j� neki, a csemp�k mennek a padl�ra
            if (Dontes("Ez a sor tele van, ez�rt a csemp�k a Padl�ra ker�lnek. Biztosan ezt akarod? (i / n) ")) return '0';
            // egy�bk�nt v�laszthat m�sik Mintasort
            josor = false;
		}
        else josor = true;
	}
	return sor[0];
}

// 7/c CSEMPE A PADL�RA VAGY A DOBOTTAK K�Z� KER�L CSEMPEV�S�RL�SKOR
void CsempeAPadlora(Game& game, Player player[], uint8_t playerId, uint8_t csempe)
{
    // Ha a Padl� tele �s nem a Kezd�-jel�l�t kell �trakni, akkor a Csempe a Dobottak k�z� ker�l
	if (player[playerId].Padlovonal.find(".") == player[playerId].Padlovonal.npos && csempe != 'K') {
		cout << "~ A Padl� tele van, ez�rt egy " << csempe << " csemp�t el kell dobni.\n";
		game.Dobott[csempe - 65]++;
	}
    // Ha van hely a Padl�n akkor a Csempe vagy Kezd�-jel�l� oda ker�l
	else if (player[playerId].Padlovonal.find(".") != player[playerId].Padlovonal.npos) {
		player[playerId].Padlovonal[player[playerId].Padlovonal.find_first_of(".")] = csempe;
	}
	// Ha a Kezd�-jel�l�t kell �trakni de nincs hely a padl�n, akkor nincs teend�, mert a Kezd�t nem lehet a dobottak k�z� tenni!
}

// 7/d CSEMP�K �TAD�SA A J�T�KOSNAK VAGY A PADL�RA CSEMPEV�S�RL�SKOR
void CsempeAtadas(Game& game, Player player[], uint8_t csempe, int mintasor)
{
    // A j�t�kos a Padl�t v�lasztotta vagy a teli sor miatt a Padl�ra ker�lnek a csemp�k
	if (mintasor == 0) {
		cout << "~ Egy " << char(csempe) << " csempe a padl�ra ker�l.\n";
		CsempeAPadlora(game, player, game.currentPlayer, csempe);
	}
	else {
        // Ha a mintasor m�r megtelt, akkor a csempe a padl�ra ker�l
		if (player[game.currentPlayer].Mintasor[mintasor - 1].find(".") == player[game.currentPlayer].Mintasor[mintasor - 1].npos) {
			cout << "~ Megtelt a(z) " << mintasor << ". sor, ez�rt egy " << csempe << " csempe a padl�ra ker�l.\n";
			CsempeAPadlora(game, player, game.currentPlayer, csempe);
		}
        // A Csempe a kiv�lasztott Mintasorba ker�l
		else {
			cout << "~ Egy " << char(csempe) << " csempe a(z) " << mintasor << ". Mintasorba ker�l.\n";
			player[game.currentPlayer].Mintasor[mintasor - 1][player[game.currentPlayer].Mintasor[mintasor - 1].find_first_of(".")] = csempe;
		}
	}
}

void csempePakolasAsztalrol(Game& game, Player player[], uint8_t csempe, uint8_t mintasor)
{
    // Ha m�g az asztalon van a kezd�j�t�kos jel�l�
    if (game.AsztalKozep[CSVARIACIOK] == 1) {
        cout << "~ A Kezd�j�t�kos-jel�l� �thelyez�se a padl�vonalra...\n";
        game.AsztalKozep[CSVARIACIOK] = 0;													// Leveszi az Asztalr�l a kezd�j�t�kos jel�l�t
        CsempeAPadlora(game, player, game.currentPlayer, 'K');													// majd �tadja a padl�vonalra
        KovJatekosReset(game, player);														// Mindenkin�l hamisra �ll�tja a k�vetkez�-j�t�kos jel�l�t
        player[game.currentPlayer].kovetkezik = true;										// Az aktu�lis j�t�kos kezdi majd a k�vetkez� k�rt
        cout << "~ A k�vetkez� fordul�t " << player[game.currentPlayer].Nev << " kezdi.\n";
    }
    // Eges�vel kell �tadni az asztalon l�v� csemp�ket
    for (int i = 0; i < game.AsztalKozep[csempe - 65]; i++) CsempeAtadas(game, player, csempe, mintasor);
    // V�g�l null�zza a csemp�k hely�t az asztalon
    game.AsztalKozep[csempe - 65] = 0;
}

void csempePakolasKorongrol(Game& game, Player player[], uint8_t csempe, uint8_t korong, uint8_t mintasor)
{
    cout << "~ A kiv�lasztott csemp�k �thelyez�se...\n";
    // V�gigmegy a korongon t�rolt csemp�ken
    for (int i = 0; i < KMERET; i++) {
        // Ha az aktu�lis csempe a k�rt csempe, akkor �tadja a j�t�kosnak
        if (game.Korongok[korong - 1].substr(i, 1) == to_string(char(csempe))) CsempeAtadas(game, player, csempe, mintasor);
        // A nem k�v�nt csemp�ket az asztalra teszi
        else { game.AsztalKozep[int(game.Korongok[korong - 1][i]) - 65]++; }
    }
    // V�g�l a korong �r�t�se
    game.Korongok[korong - 1] = "\0";
    for (int i = 0; i < KMERET; i++) { game.Korongok[korong - 1] += "."; }
}

// 7. CSEMP�K V�S�RL�SA KORONGOKR�L VAGY ASZTALR�L
bool CsempeVasar(Game& game, Player player[])
{
	string korongEsCsempe = KCsValaszto(game, player);						// A kiv�lasztott korong �s csempe
	cout << "korongEsCsempe: " << korongEsCsempe << "\n";
	if (korongEsCsempe == "X") return true;									// Ha ki akar l�pni, r�gt�n megy is vissza
	uint16_t	korong	 = int(korongEsCsempe[0]) - 48;							// A kiv�lasztott korong
	uint8_t	csempe	 = int(korongEsCsempe[1]);							// A k�rt csempe

	cout << "korong: " << korong << "\tcsempe: " << csempe << "\n";
	uint16_t	mintasor = int(SorValaszto(game, player, csempe)) - 48;			// A kiv�lasztott Mintasor ahov� a csemp�k ker�lnek
	if (mintasor == 40 || mintasor == 72) return true;						// Ha ki akar l�pni, m�ris megy vissza
    // Ha forr�snak az asztalt v�lasztotta
	if (korong == 0) csempePakolasAsztalrol(game, player, csempe, mintasor);
	// Ha forr�snak valamelyik korongot v�lasztotta
	else csempePakolasKorongrol(game, player, csempe, korong, mintasor);
	return false;
}

// 8/a MINTASOROK NULL�Z�SA
void MintasorNullazas(Player player[], int playerId, int sor)
{
	string minta = "\0";								// A Mintasor kirajzols�shoz sz�ks�ges v�ltoz�
    // A soron bel�li poz�ci�
	for (int i = 0; i < sor + 1; i++) {
		player[playerId].Mintasor[sor] = minta + ".";
		minta += ".";
	}
}

// 8/b PADL� NULL�Z�SA
void PadloNullazas(Player player[], int playerId)
{
	player[playerId].Padlovonal = "\0";							// A padl�vonal �res legyen
    // Padl�vonalon bel�li poz�ci�
    // A Padl�vonal kirajzol�sa hely�rz�kkel
	for (int k = 0; k < PADLOMERET; k++) { player[playerId].Padlovonal += "."; }
}

// 8/c PONTSZ�M�T�S FALAZ�SKOR
int FalPontok(Player player[], uint8_t playerId, uint8_t sor, uint8_t oszlop)
{
	int pont = 1;										// A j�t�kos �ltal kapott pontok (egyet biztosan kap, mert felfalazta a csemp�t)
	int k = 0;											// Keres� index
	bool vansor = false;								// Tal�lat v�zszintes keres�s�n�l
	bool vanoszlop = false;								// Tal�lat f�gg�leges keres�s�n�l
    // Csak akkor n�z el balra, ha a lerakott csempe nem a bal sz�ls� oszlopba ker�lt
	if (oszlop > 0) {
		k = oszlop;
        // Akkor j�r pont, ha az aktu�lis poz�ci�t�l balra is van csempe
		while (k > 0 && player[playerId].wall[sor][k - 1] != '.') {
			pont++;
			k--;
			vansor = true;
		}
	}
    // Csak akkor n�z el jobbra, ha a lerakott csempe nem a jobb sz�ls� oszlopba ker�lt
	if (oszlop < TABLAMERET - 1) {
		k = oszlop;
        // Akkor j�r pont, ha az aktu�lis poz�ci�t�l jobbra is van csempe
		while (k < TABLAMERET - 1 && player[playerId].wall[sor][k + 1] != '.') {
			pont++;
			k++;
			vansor = true;
		}
	}
    // Csak akkor n�z felfel�, ha a lerakott csempe nem a legfels� oszlopba ker�lt
	if (sor > 0) {
		k = sor;
        // Akkor j�r pont, ha az aktu�lis poz�ci� felett is van csempe
		while (k > 0 && player[playerId].wall[k - 1][oszlop] != '.') {
			pont++;
			k--;
			vanoszlop = true;
		}
	}
    // Csak akkor n�z lefel�, ha a lerakott csempe nem a legals� oszlopba ker�lt
	if (sor < TABLAMERET - 1) {
		k = sor;
        // Akkor j�r pont, ha az aktu�lis poz�ci� alatt is van csempe
		while (k < TABLAMERET - 1 && player[playerId].wall[k + 1][oszlop] != '.') {
			pont++;
			k++;
			vanoszlop = true;
		}
	}
    // Ha volt sor �s oszlop is, akkor m�g egy pont j�r, mert az �ppen lerakott csemp�t egyikbe se sz�molta bele
	if (vansor && vanoszlop) { pont++; }
	cout << player[playerId].Nev << ", " << pont << " pontot kapt�l a csempe felrak�s��rt.\n";
	return pont;
}

// 8/d PONTLEVON�S A PADL� MIATT
int PadloPontok(Player player[], int playerId)
{
	int buntipontok = 0;								// A padl�ra esett csemp�k�rt j�r� b�ntet�pontok
    // A padl�n bel�li poz�ci�
	for (int p = 0; p < PADLOMERET; p++) { if (player[playerId].Padlovonal[p] != '.') { buntipontok += -(((p + 1) / 3) + 1); } }
	cout << player[playerId].Nev << ", " << buntipontok << " b�ntet�pontot kapt�l a padl�ra esett csemp�k miatt.\n";
	return buntipontok;
}

// 8/e FAL F�GG�LEGES (OSZLOP) VIZSG�LATA FALAZ�SHOZ
bool JoOszlop(Player player[], int playerId, int oszlop, uint8_t csempe)
{
	bool joOszlop = true;									// Az oszlop alkalmass�ga
	uint8_t sor = 0;											// Oszlop-szkennel�
	do {
        // Amint tal�l egyet a megfelel� csemp�b�l hamisra �ll�tja
		if (player[playerId].wall[sor][oszlop] == csempe) { joOszlop = false; }
		sor++;
	} while (joOszlop && sor < TABLAMERET);
	return joOszlop;
}

// 8/f CSEMP�K ELDOB�SA FALAZ�SKOR
void CsempeEldobas(Game& game, Player player[], int playerId, int sor)
{
	int i = 0;													// A Padl�vonal egy karakter�nek indexe
    // Ha a Padl�r�l kell eldobni csemp�t
	if (sor == 0) {
		while (i < PADLOMERET && player[playerId].Padlovonal[i] != '.') {
            // Ha az eldoband� csempe a Kezd�j�t�kos-jel�l�, akkor nem ker�l a Dobottak k�z�
			if (player[playerId].Padlovonal[i] == 'K') { i++; }
			else {
				game.Dobott[int(player[playerId].Padlovonal[i]) - 65]++;
				i++;
			}
		}
	}
    // Ha a Mintasorb�l kell eldobni csemp�t
	else {
		while (i < int(player[playerId].Mintasor[sor - 1].length()) - 1) {
			game.Dobott[int(player[playerId].Mintasor[sor - 1][i]) - 65]++;
			i++;
		}
	}
}

// 8. FALAZ�S
bool Falazas(Game& game, Player player[])
{
	bool   jovalasz		 = false;				// A megfelel� oszlop kivalasztasa
	string oszlop		 = "\0";				// A kiv�lasztott oszlop
	cout << "* * * Kezd�dik a falaz�s! * * *\n";;
    // Aktu�lis j�t�kos sorsz�ma
	for (int playerId = 0; playerId < game.numberOfPlayers; playerId++) {
        // Mintasor szorsz�ma
		for (int sor = 0; sor < TABLAMERET; sor++) {
            // Ha az aktu�lis Mintasor nincs tele
			if (player[playerId].Mintasor[sor].find(".") == player[playerId].Mintasor[sor].npos) {
                // Ha a Falon abban a sorban m�g nincs a Mintasorban l�v�kkel azonos csempe
				if (player[playerId].wall[sor].find(player[playerId].Mintasor[sor][0]) == player[playerId].wall[sor].npos) {
					// Az asztal �s a j�t�kosok rajzol�sa
					asztalMegj(game);
					PlayerMegj(game, player);
					uint8_t csempe = player[playerId].Mintasor[sor][0];				// Az �ppen manipul�lt csempe
					cout << player[playerId].Nev << ", a " << sor + 1 << ". sorb�l egy " << char(csempe) << " csemp�t felfalazhatsz.\n";
					do {
						cout << "Melyik oszlopba szeretn�d teni? (1-t�l " << TABLAMERET << "-ig, a padl�t a 0-val adhatod meg) ";
						cin >> oszlop;
                        // A j�t�kos ki szeretne l�pni, ez�rt m�r megy�nk is vissza
						if (oszlop == "x" || oszlop == "X") { return true; }
                        // Ha a megadott oszlop nagyobb, mint az oszlopok sz�ma (vagy nem sz�m)
						else if (int(oszlop[0]) - 48 > TABLAMERET) {
							jovalasz = false;
							cout << "HIBA: Ilyen sorsz�m� oszlop nincs a falon!\n";
						}
                        // Ha nem a Padl�t v�lasztotta �s a kiv�lasztott oszlopban l�v� hely nem �res
						else if (oszlop != "0" && player[playerId].wall[sor][int(oszlop[0]) - 49] != '.') {
							jovalasz = false;
							cout << "HIBA: A kiv�lasztott hely foglalt!\n";
						}
                        // Ha nem a Padl�t v�lasztotta de a megadott oszlopban valahol m�r van ilyen csempe
						else if (oszlop != "0" && !JoOszlop(player, playerId, int(oszlop[0]) - 49, csempe)) {
							jovalasz = false;
							cout << "HIBA: Ebben az oszlopban m�r van ilyen csempe!\n";
						}
						else { jovalasz = true; }
					} while (!jovalasz);
                    // Ha a j�t�kos a Padl�t v�lasztotta
					if (oszlop == "0") {
                        // Annyiszor dobok csemp�t a padl�ra, ah�ny darab a Mintasorban volt (azaz a Mintasor hossza)
						for (int p = 0; p < int(player[playerId].Mintasor[sor].length()); p++) {
							cout << "~ Egy " << char(csempe) << " a(z) " << sor + 1 << ". sorb�l a Padl�ra ker�l.\n";
                            // Egyes�vel a Padl�ra ker�lnek a csemp�k
							CsempeAPadlora(game, player, playerId, csempe);
						}
					}
					else {
						cout << "~ Egy " << char(csempe) << " a(z) " << sor + 1 << ". sorb�l a Fal " << oszlop[0] - 48 << ". oszlop�ba ker�l.\n";
						player[playerId].wall[sor][int(oszlop[0]) - 49] = csempe;					// A falra ker�l a csempe
						player[playerId].Pontok += FalPontok(player, playerId, sor, int(oszlop[0]) - 49);		// A falaz�s�rt pontot kap a j�t�kos
						cout << "~ A Mintasorban maradt csemp�k eldob�sa...\n";
						CsempeEldobas(game, player, playerId, sor + 1);							// Eldobjuk a marad�k csemp�t a Mintasorb�l
					}
					cout << "~ Mintasor �r�t�se...\n";
					MintasorNullazas(player, playerId, sor);										// A Mintasor �r�t�se
				}
                // Ha a Falon m�r van a Mintasorban l�v�kkel azonos csempe
				else {
					uint8_t csempe = player[playerId].Mintasor[sor][0];
					cout << "~ Van m�r " << char(csempe) << " csempe a Fal " << sor << ". sor�ban, ez�rt a Mintasorb�l az �sszes a Padl�ra ker�l.\n";
                    // Annyiszor dobok csemp�t a padl�ra, ah�ny a Mintasorban volt
					for (int p = 0; p < int(player[playerId].Mintasor[sor].length()); p++) {
                        // Egyes�vel a Padl�ra ker�lnek a csemp�k
						CsempeAPadlora(game, player, playerId, csempe);
					}
					cout << "~ Mintasor �r�t�se...\n";
					MintasorNullazas(player, playerId, sor);								// A Mintasor �r�t�se
				}
			}
		}
		cout << "~ B�ntet�pontok sz�m�t�sa...\n";
		int buntetopontok = PadloPontok(player, playerId);					// A padl�ra esett cemp�k miatt kapott b�ntet�pontok
		// Ha a b�ntet�pontok miatt negat�v lenne a j�t�kos pontsz�ma, akkor nulla pontja lesz
		if (player[playerId].Pontok + buntetopontok < 0) { player[playerId].Pontok = 0; }
		else { player[playerId].Pontok += buntetopontok; }
		CsempeEldobas(game, player, playerId, 0);														// Eldobjuk a csemp�ket a Padl�r�l
		PadloNullazas(player, playerId);																	// �r�ti a Padl�t
		cout << player[playerId].Nev << ", a falaz�s ut�n " << player[playerId].Pontok << " pontod van.\n";
	}
	return false;
}

// 9. B�NUSZPONTOK SZ�M�T�SA
void BonuszPontok(Game& game, Player player[], Score score[])
{
	int bonuszsor = 0;										// A teljes sorok�rt j�r� b�nuszpontok
	int bonuszoszlop = 0;									// A teljes oszlopok�rt j�r� b�nuszpontok
	int bonuszszin = 0;										// A teljes sz�nek�rt j�r� b�nuszpontok
	bool teljesoszlop = true;								// Van teljes oszlop
	bool teljesszin = true;									// Van teljes sz�n
	int o = 0;												// Oszlopkeres�
	int sz = 0;												// Sz�nkeres�
	cout << "\n* * * B�nuszpontok * * *\n\n";
    // A j�t�kosok indexe
	for (int i = 0; i < game.numberOfPlayers; i++) {
        // Null�zzuk a sorok sz�m�t az adott j�t�kos eredm�nyjelz�j�n�l
		score[i].SorokSzama = 0; 
		for (int s = 0; s < TABLAMERET; s++) {
            // Ha a Fal adott sor�ban nincs �res hely, akkor j�r a 2 pont
			if (player[i].wall[s].find(".") == player[i].wall[s].npos) {
				bonuszsor += 2;
				score[i].SorokSzama++;							// Ha tal�ltunk n�la egy sort, akkor az eredm�nyjelz�n is kap �rte 1 pontot
			}
			teljesoszlop = true;
			o = 0;
			do {
                // Amint tal�l az oszlopban egy �res helyet, m�r nem teljes az osztlop
				if (player[i].wall[o][s] == '.') { teljesoszlop = false; }
				o++;
			} while (teljesoszlop && o < TABLAMERET);
            // Ha a Fal adott oszlop�ban nincs �res hely, akkor j�r a 7 pont
			if (teljesoszlop) { bonuszoszlop += 7; }
		}
        // A csempefajt�k
		for (int c = 0; c < CSVARIACIOK; c++) {
			teljesszin = true;
			sz = 0;
			do {
                // Amint tal�l egy olyan sort, amiben nincs az adott cseme, m�r nem teljes a sz�n
				if (player[i].wall[sz].find(char(c + 65)) == player[i].wall[sz].npos) { teljesszin = false; }
				sz++;
			} while (teljesszin && sz < TABLAMERET);
            // Ha nincs olyan sor a Falon, amiben ne szerepelne az adott Csempe, akkor j�r a 10 pont
			if (teljesszin) { bonuszszin += 10; }
		}
		player[i].Pontok += bonuszsor + bonuszoszlop + bonuszszin;
		cout << player[i].Nev << " b�nuszpontjai:\n\t+"
			<< bonuszsor << " pont a sorok�rt\n\t+"
			<< bonuszoszlop << " pont az oszlopok�rt\n\t+" 
			<< bonuszszin << " pont a teljes sz�nek�rt.\n";
	}
}

// 10. EREDM�NYEK KI�R�SA
void Vegeredmeny(Game& game, Player player[], Score score[])
{
	string n = "\0";									// Neveket t�rol� seg�dv�ltoz�
	int p = 0;											// Pontokat t�rol� seg�dv�ltoz�
	int s = 0;											// A Sorok sz�m�t t�rol� seg�dv�ltoz�

    // �tm�soljuk az eredm�nyjelz� t�mbbe a j�t�kosok neveit �s pontsz�mait
	for (int i = 0; i < game.numberOfPlayers; i++) {
		score[i].Nev = player[i].Nev;
		score[i].Pont = player[i].Pontok;
	}

    // A kisebb index� elem
	for (int i = 0; i < game.numberOfPlayers - 1; i++) {
        // Az egyel nagyobb index� elem
		for (int k = i + 1; k < game.numberOfPlayers; k++) {
            // Ha a kisebbik index� kisebb, mint a nagyobb index�, akkor csere (mert a helyez�seket az index hat�rozza meg)
			if (score[i].Pont < score[k].Pont) {
				n = score[i].Nev;
				p = score[i].Pont;
				s = score[i].SorokSzama;
				score[i].Nev = score[k].Nev;
				score[i].Pont = score[k].Pont;
				score[i].SorokSzama = score[k].SorokSzama;
				score[k].Nev = n;
				score[k].Pont = p;
				score[k].SorokSzama = s;
			}
            // Ha a k�t egym�s melletti elem egyenl�, akkor a sorok sz�ma d�nt
			else if (score[i].Pont == score[k].Pont && score[i].SorokSzama < score[k].SorokSzama) {
				n = score[i].Nev;
				p = score[i].Pont;
				s = score[i].SorokSzama;
				score[i].Nev = score[k].Nev;
				score[i].Pont = score[k].Pont;
				score[i].SorokSzama = score[k].SorokSzama;
				score[k].Nev = n;
				score[k].Pont = p;
				score[k].SorokSzama = s;
			}
		}
	}
	cout << "\n\n* * * V�GEREDM�NY * * *\n\n";
	for (int l = 0; l < game.numberOfPlayers; l++) { cout << l + 1 << ". helyezett: " << score[l].Nev << ", " << score[l].Pont << " ponttal, " << score[l].SorokSzama << " teljes sorral\n"; }
}

void ujFordulo(Game& game, Player players[])
{
	game.fordulo++;															// N�velj�k a fordul� sorsz�m�t
	game.lepes = 0;															// Vissza�ll�tjuk a fordul�n bel�li l�p�ssz�mot
	game.fordulovege = false;												// A fordul� most kezd�dik, �gyhogy m�g nincs v�ge
	game.currentPlayer = 0;
	// Megkeresi melyik j�t�kos k�vetkezik
	while (!players[game.currentPlayer].kovetkezik && game.currentPlayer < game.numberOfPlayers) game.currentPlayer++;
}


int main()
{
	setlocale(LC_ALL, "hun");								// Magyarra �ll�tja a nyelvet
	srand((unsigned)time(NULL));							// v�letlen-gener�tor inicializ�l�sa

	Game	game;											// V�ltoz� a Game strukt�r�val
	Player* players = new Player[game.numberOfPlayers];		// A j�t�kosok adatait tartalmaz� Player t�pus� t�mb l�trehoz�sa a j�t�kossz�m alapj�n
	Score*	score	= new Score[game.numberOfPlayers];		// Az eredm�nyek kijelz�s�hez sz�ks�ges t�mb l�trehoz�sa a j�t�kossz�m alapj�n

	PlayerElokeszites(game, players);						// A Player t�pus� t�mb el�k�sz�t�se a j�t�kossz�m alapj�n
	jatekosNevekMegadasa(game, players);					// Opcion�lisan megadhat� a j�t�kosok beceneve
	kezdoJatekosMegadasa(game, players);					// A kezd�j�t�kos megad�sa

	while (!game.jatekvege) {
		game.jatekvege = KorongTolto(game);																			// Felt�lti a Korongokat, az Asztalta teszi a kezd�j�t�kos jel�l�t
		ujFordulo(game, players);
		while (!game.fordulovege && !game.jatekvege) {
			game.lepes++;
//			system("cls");
			cout << "* * *  CSEMPEV�S�R " << game.fordulo << ". fordul� " << game.lepes << ". l�p�s  * * *\n\n";
			asztalMegj(game);																						// Megjelen�ti a Korongok, az Asztal, a Zs�k �s a Dobott tartalm�t
			PlayerMegj(game, players);																				// Megjelen�ti a j�t�kosok t�bl�it
			game.kilepes = CsempeVasar(game, players);																// A j�t�kos csemp�ket v�s�rolhat a Korongokr�l vagy az Asztalr�l
			if (game.currentPlayer == game.numberOfPlayers - 1) { game.currentPlayer = 0; }							// Egyel l�ptei az j�t�kosokat
			else { game.currentPlayer++; }
			game.fordulovege = ForduloVege(game) || game.kilepes;													// A fordul� v�g�nek leellen�rz�se
		}
		if (!game.kilepes) game.kilepes = Falazas(game, players);													// Falaz�s + pontok
		game.jatekvege = JatekVege(game, players) || game.kilepes;													// A j�t�k v�g�nek ellen�rz�se
	}
	BonuszPontok(game, players, score);
	//Vegeredmeny(game, players, score);
	cout << "\nGRATUL�LOK!\n";
	cout << "K�sz�n�m, hogy j�tszottatok!\n\n\n";
	system("pause");
	//game.ZaroKepernyo();
	system("pause");
	delete[]game.Korongok;
	delete[]players;
	delete[]score;
	return 0;
}
