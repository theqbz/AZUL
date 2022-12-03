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

struct Game								// A j�t�k f� adatai
{
	string* Korongok;					// A korongok t�mbje
	int KorongSzam;						// A korongok sz�ma
	int Zsak[CSVARIACIOK];				// A zs�k t�mbj�nek helye 
	int Dobott[CSVARIACIOK];			// Az eldobott csemp�k t�mbj�nek helye
	int AsztalKozep[CSVARIACIOK + 1];	// Az asztal k�zep�t t�rol� t�mb helye
};

struct Player							// A j�t�kosokhoz tartoz� adatok
{
	string Nev;							// A j�t�kos neve
	int Pontok;							// A j�t�kos pontsz�ma
	string Mintasor[TABLAMERET];		// A mintasor, ahov� a korongokr�l vagy asztalr�l v�s�rolt csemp�ket gy�jti.
	string Fal[TABLAMERET];				// A fal, ahov� felteszi a mintasoron �sszegy�lt csemp�ket.
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
bool ElfogyottACsempe(int zsak[], int dobott[])
{
	bool ureszsak = false;							// Nincs csempe a Zs�kban
	bool uresdobott = false;						// Nincs csempe a Dobottakban
	int k = 0;
    // Addig n�zi am�g nem tal�l legal�bb egy nem �res rekeszt
	while (k < CSVARIACIOK && zsak[k] == 0) {
		ureszsak = true;							// Akkor kell belemennem, ha �res rekeszt tal�lok, egy�bk�nt (azaz nem �res) mehet�nk tov�bb, mert van m�g csempe
		k++;
	}
	k = 0;
	while (k < CSVARIACIOK && dobott[k] == 0) {
		uresdobott = true;
		k++;
	}
	if (ureszsak) {
		if (uresdobott) {
            // Ha elfogyott minden csempe a Zs�kb�l �s a Dobottak k�z�l, akkor v�ge a j�t�knak
			cout << "~ Elfogyott minden csempe => v�ge a j�t�knak.\n";
			return true;
		} else {
			// Ha csak a Zs�k �res, �tpakoljuk a Dobottakban t�rolt �rt�keket a Zs�kba
			cout << "~ A ki�r�lt Zs�k felt�lt�se...\n";
			for (int i = 0; i < CSVARIACIOK; i++) {
				zsak[i] = dobott[i];
				dobott[i] = 0;
			}
		}
	}
	return false;
}

// 0/b A J�T�K V�G�NEK ELLEN�RZ�SE
bool JatekVege(Player j[], int jsz, int zsak[], int dobott[])
{
	bool vansor = false;										// Sor-detekror
	int i = 0;													// J�t�kosok indexe
	cout << "~ J�t�k v�g�nek ellen�rz�se...\n";
	do {
		int s = 0;												// A Fal sorainak indexe
		do {
			if (j[i].Fal[s].find(".") == j[i].Fal[s].npos) {
                // Amint valakin�l tal�l teljes sort a Falon, v�ge a j�t�knak
				vansor = true;
				cout << "~ " << j[i].Nev << " fal�nak " << s << ". sora teljes lett => v�ge a j�t�knak.\n";
			}
			s++;
		} while (!vansor && s < TABLAMERET);
		i++;
	} while (!vansor && i < jsz);
	return vansor || ElfogyottACsempe(zsak, dobott);
}

// 0/c FORDUL� V�G�NEK ELLEN�RZ�SE
bool ForduloVege(string korongok[], int ksz, int asztal[])
{
	bool kures = true;					// A Korongok ki�r�ltek-e
	bool aures = true;					// Az asztal �res-e
	int kk = 0;							// Korong-szkennel�
	int ak = 0;							// Asztal-szkennel�

	cout << "~ Fordul� v�g�nek ellen�rz�se...\n";
    // Amint tal�l egy nem �res Korongot, r�gt�n hamisra �ll�tja
	do {
		if (korongok[kk] != "....") { kures = false; }
		kk++;
	} while (kures && kk < ksz);
    // Amint tal�l csemp�t az Asztalon, r�gt�n hamisra �ll�tja
	do {
		if (asztal[ak] != 0) { aures = false; }
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
		for (int n = 0; n < valasz.length(); n++) { valasz[n] = toupper(valasz[n]); }
	} while (valasz != "I" && valasz != "N");
	if (valasz == "I") { return true; }
	else { return false; }
}

// 1/b J�T�KOSOK NEV�NEK BEK�R�SE (OPCION�LIS) �S KEZD� KIJEL�L�SE
void JatekosNevek(Player j[], int jsz)
{
	string kezd = "\0";
	if (Dontes("Megadod a j�t�kosok becenev�t? (i / n) ")) {
		string kuka = "\0";											// A cin �r�t�se
		getline(cin, kuka, '\n');
		for (int i = 0; i < jsz; i++) {
			cout << i + 1 << ". j�t�kos neve: ";
			getline(cin, j[i].Nev, '\n');
		}
	}
    // A "k�vetkezik" v�ltoz� alapra �ll�t�sa
	for (int i = 0; i < jsz; i++) { j[i].kovetkezik = false; }
	bool jokezdo = false;
	do {
		cout << "Ki kezd? (A j�t�kos sorsz�m�t add meg!) ";
		cin >> kezd;
		if (kezd.length() != 1) { jokezdo = false; }
        // Ha a megadott sz�m nagyobb, mint ah�ny j�t�kos van, akkor nem j�
		else if (int(kezd[0]) < 49 || int(kezd[0]) - 48 > jsz) {
			jokezdo = false;
			cout << "HIBA: Ilyen sorsz�m� j�t�kos nincs!\n";
		}
		else { jokezdo = true; }
	} while (!jokezdo);
	j[int(kezd[0] - 49)].kovetkezik = true;
}

// 1. J�T�KOSOK SZ�M�NAK BEK�R�SE
int JatekosSzamBe()
{
	string jszbe = "";
	do {
		cout << "Add meg a j�t�kosok sz�m�t (2, 3 vagy 4 j�t�kos lehet!): ";
		cin >> jszbe;
	} while (jszbe != "2" && jszbe != "3" && jszbe != "4");
	return int(jszbe[0]) - 48;
}

// 2. ZS�K, ELDOBOTT �S ASZTALK�Z�P INICIALIZ�L�SA
void KezdoZsakok(int zs[], int d[], int a[])
{
	for (int i = 0; i < CSVARIACIOK; i++) {
		zs[i] = CSKEZDODARAB;				// A zs�k rekeszeinek felt�lt�se a csemp�k kezdeti darabsz�m�val
		d[i] = 0;							// A dobottak zs�k null�z�sa
		a[i] = 0;							// Az asztal k�zep�nek null�z�sa
	}
	a[CSVARIACIOK] = 1;						// Itt tessz�k le az asztal k�zep�re a kezd�j�t�kos jelz�t
}

// 3/a CSEMPE EL�H�Z�SA A ZS�KB�L
int CsempeHuzas(int zsak[])
{
	srand((unsigned)time(NULL));					// v�letlen-gener�tor inicializ�l�sa
	int csdb = 0;									// a Zs�kban maradt csemp�k sz�ma
    // A zs�k[]-ban l�v�, k�l�nb�z� sz�n� csemp�k sz�m�t �sszeadja a v�letlen sz�m gener�l�shoz
	for (int i = 0; i < CSVARIACIOK; i++) { csdb += zsak[i]; }
	int cs = rand() % csdb + 1;						// V�letlen sz�m 1 �s a csemp�k sz�ma k�z�tti �rt�kben

	// Ki kell keresni, hogy a v�letlen sz�m melyik csemp�t jel�li:

	int i = -1;										// Az aktu�lis csemperekesz, azaz a zs�k[] t�mb indexe
	int vektor = 0;									// Keres�vektor
	do {
		i++;										// A zs�k[] t�mb index�t n�veli
		vektor += zsak[i];							// B�v�ti a keres�vektort a zs�k[] t�mb aktu�lis index� elem�nek �rt�k�vel
	} while (vektor < cs && i < CSVARIACIOK);		// Amikor a keres�vektorn�l kisebb lesz a v�letlen sz�m, akkor a keresett csempesz�nt a zs�k[] aktu�lis indexe jel�li.
	return i;
}

// 3. KORONGOK FELT�LT�SE
bool KorongTolto(string korongok[], int zsak[], int dobott[], int asztal[], int korongszam)
{
	string csempek = "\0";							// H�z�s k�zben ide gy�jti a kih�zott csemp�ket

	cout << "~ Korongok felt�lt�se...\n";
    // Egyes�vel v�gigmegy a korongokon
	for (int i = 0; i < korongszam; i++) {
        // Annyi csemp�t h�z a zs�kb�l, ah�ny elf�r egy korongon
		for (int j = 0; j < KMERET; j++) {
            // Ha m�r nincs csempe a Zs�kban vagy a Dobottak k�z�tt, akkor v�ge a j�t�knak
			if (ElfogyottACsempe(zsak, dobott)) { return true; }
			else {
				int kihCs = CsempeHuzas(zsak);		// Egy kih�zott csempe, azaz a zs�k[] t�mb egyik rekesz�nek indexe
				csempek += char(65 + kihCs);		// A csemp�k v�ltoz�ba gy�jtjuk a karaktereket a kih�zott t�mbindex alapj�n.
				zsak[kihCs]--;						// A kih�zott csempefajt�b�l egyel kevesebb marad a zs�kban.
			}
		}
		korongok[i] = csempek;						// A kih�zott csemp�k az �ppen soron l�v� korongra mennek
		csempek = "\0";								// Null�zza gy�jt� v�ltoz�t
	}
	asztal[CSVARIACIOK] = 1;						// Kezd�j�t�kos jel�l� kihelyez�se az asztal k�zep�re
	return false;
}

// 4/a K�VETKEZ�-J�T�KOS JEL�L� RESETEL�SE
void KovJatekosReset(Player j[], int jsz)
{
    // A j�t�kosok t�mb indexe
	for (int i = 0; i < jsz; i++) { j[i].kovetkezik = false; }
}

// 4. A PLAYER STRUKT�R�J� T�MB EL�K�SZ�T�SE
int PlayerElokeszites(Player jatekos[], int jsz)
{
	for (int i = 0; i < jsz; i++) {
		jatekos[i].Nev = char(48 + i + 1);			// A n�v hely�re egyel�re csak sorsz�m ker�l
		jatekos[i].Pontok = 0;						// A pontok null�z�dnak
		jatekos[i].Padlovonal = "\0";				// A padl�vonal �res legyen
		string minta = "\0";						// A Mintasor kirajzols�shoz sz�ks�ges v�ltoz�
		for (int j = 0; j < TABLAMERET; j++) {
			jatekos[i].Mintasor[j] = minta + ".";	// A Mintasor kirajzol�sa hely�rz�kkel
			jatekos[i].Fal[j] = ".....";			// A Fal kirajzol�sa hely�rz�kkel
			minta += ".";
		}
		for (int k = 0; k < PADLOMERET; k++) {
			jatekos[i].Padlovonal += ".";			// A Padl�vonal kirajzol�sa hely�rz�kkel
		}
	}
	KovJatekosReset(jatekos, jsz);					// A k�vetkez�-j�t�kos jel�l�t mindenhol hamisra �ll�tja
	return 0;
}


// 5/a HELY�RZ� SZ�K�Z�K
string Helyorzo(string ertek)
{
	if (ertek.length() == 1) { return ertek + " "; }
	else { return ertek; }
}

// 5. KORONGOKON �S ASZTAL K�ZEP�N L�V� CSEMP�K MEGJELEN�T�SE
void KorongAsztalMegj(string korong[], int asztal[], int zsak[], int dobott[], int ksz)
{
	const int sorokszama = 8;						// A ki�rand� sorok sz�ma �sszesen
	string sorok[sorokszama];						// Ide gy�jtj�k a ki�rand� sorok tartalm�t
	string elv = "|  ";								// Elv�laszt� a t�bl�zat r�szei k�z�tt
    // A T�BL�ZAT BAL OLDALA
	for (int i = 0; i < sorokszama; i++) { sorok[i] = elv; }
	// A KORONGOK
	int kszel = (ksz / 2 + 1) * 5;										// Ilyen sz�les hely kell a korongok megjelen�t�s�hez
	sorok[0] += "Korongok:";											// A korongok r�sz c�msora
    // Kit�lti a sz�ks�ges mennyis�g� sz�k�zzel a korongok sz�m�nak f�ggv�ny�ben
	for (int k = 0; k < kszel - 9; k++) { sorok[0] += " "; }
	sorok[0] += elv;
    // A korongok sz�moz�sa: 1. sor
	for (int k = 0; k < ((ksz / 2) + 1); k++) { sorok[1] += to_string(k + 1) + ".   "; }
	sorok[1] += elv;
    // A korongok tartalma: els� k�t sor
	for (int i = 0; i < 2; i++) {
        // Elsz�mol a korongok sz�m�nak fel�n�l egyel tov�bb...
		// ...�s az aktu�lis korongr�l kettes�vel ki�rja a csemp�ket (el�bb az els� kett�t azt�n a m�sodik kett�t)
		for (int k = 0; k < ((ksz / 2) + 1); k++) { sorok[i + 2] += korong[k].substr((i * 2), 2) + "   "; }
		sorok[i + 2] += elv;
	}
    // Elv�laszt� �res sor
	for (int k = 0; k < kszel; k++) { sorok[4] += " "; }
	sorok[4] += elv;
    // A korongok sz�moz�sa: 2. sor
	for (int k = ((ksz / 2) + 1); k < ksz; k++) { sorok[5] += to_string(k + 1) + ".   "; }
	sorok[5] += "     " + elv;
    // A korongok tartalma: m�sdoik k�t sor
	for (int i = 0; i < 2; i++) {
        // A sz�mol�s a korongok sz�m�nak fel�n�l egyel t�bbr�l indul �s a korongok sz�m�ig tart...
        // ...�s az aktu�lis korongr�l kettes�vel ki�rja a csemp�ket
		for (int k2 = ((ksz / 2) + 1); k2 < ksz; k2++) { sorok[i + 6] += korong[k2].substr((i * 2), 2) + "   "; }
		sorok[i + 6] += "     " + elv;
	}
	// AZ ASZTAL
	// Az asztal r�sz c�msora
	sorok[0] += "Asztal:    " + elv;
    // A csemp�k sz�m�nak ki�r�sa
	for (int i = 0; i < CSVARIACIOK; i++) {
		sorok[i + 1] += char(65 + i);
		sorok[i + 1] += ": " + to_string(asztal[i]) + "       " + elv;
	}
    // �res sor
	sorok[6] += "           " + elv;
    // Ha m�g az asztalon van a kezd�j�t�kosjel�l�...
    // ...fel�rja azt is...
	if (asztal[CSVARIACIOK] == 1) { sorok[7] += "Kezd�      " + elv; }
    // ...k�l�nben �res sort �r.
	else { sorok[7] += "           " + elv; }
	// A ZS�K �S DOBOTTAK
	sorok[0] += "Zs�k:           ";
	for (int i = 0; i < CSVARIACIOK; i++) {
		sorok[1] += char(65 + i);
		sorok[1] += "  ";
	}
	for (int j = 0; j < CSVARIACIOK; j++) { sorok[2] += Helyorzo(to_string(zsak[j])) + " "; }
	sorok[3] += "                ";
	sorok[4] += "Dobott:         ";
	for (int i = 0; i < CSVARIACIOK; i++) {
		sorok[5] += char(65 + i);
		sorok[5] += "  ";
	}
	for (int j = 0; j < CSVARIACIOK; j++) { sorok[6] += Helyorzo(to_string(dobott[j])) + " "; }
    // SOROK KI�R�SA
	for (int ki = 0; ki < sorokszama; ki++) { cout << sorok[ki] << endl; }
	cout << endl;
}

// 6a PLAYER MEGJELEN�T�SE
void PlayerMegj(Player j[], int jsz, int aj)
{
	const int fs = 4;							// A fejl�cbe ki�rand� sorok sz�ma
	const int ps = 3;							// A padl�vonalra ki�rand� sorok sz�ma
	const int ssz = fs + TABLAMERET + ps;		// A ki�and� sorok sz�ma �sszesen: a fejl�c, a t�bla �s a padl�ra ki�rand� sorok sz�ma
	const int tszel = (PADLOMERET * 3) + 2;		// Az t�bl�k sz�less�ge
	string sorok[ssz];							// Ebbe gy�jtj�k a j�t�kos el�tti t�bla ki�rand� tartalm�t soronk�nt
    // Sorok null�z�sa
	for (int i = 0; i < ssz; i++) { sorok[i] = "\0"; }
    // A j�t�kosok indexe
	for (int ji = 0; ji < jsz; ji++) {
		// A ki�r�s el�k�sz�t�se: vonal �s t�ltel�k
		string vonal = "\0";							// Ez lesz a t�bl�zat v�zszintes vonala
		int sszel = tszel * (ji + 1);					// A gyarapod� sorok aktu�lis maximum sz�less�ge
        // Ha az adott j�t�kos k�vetkezik, akkor a v�zszintes vonal *-karakterb�l �ll
		if (ji == aj) { for (int i = 0; i < tszel - 1; i++) { vonal += "*"; } }
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
		sorok[1] += j[ji].Nev.substr(0, tszel - 7);					// N�v (csonkolva, ha hossz�)
		sorok[2] += " " + to_string(j[ji].Pontok) + " pont";		// Pontsz�m
        // A 4. sor elej�t t�lti fel sz�k�zzel
		while (sorok[3].length() < sszel - (TABLAMERET * 2)) { sorok[3] += " "; }
        // A 4. sor v�g�t felt�lti az oszlopok sorsz�m�val
		for (int i = 0; i < TABLAMERET; i++) { sorok[3] += to_string(i + 1) + " "; }
		// MINTASOR �S FAL
		for (int i = 0; i < TABLAMERET; i++) {
			sorok[i + fs] += "    " + toltelek + j[ji].Mintasor[i];	// T�ltel�k �s a Mintasor tartalma
			sorok[i + fs] += " " + to_string(i + 1) + " ";			// Sorsz�m a Mintasor �s a Fal k�z�tt
			for (int f = 0; f < TABLAMERET; f++) {
				sorok[i + fs] += j[ji].Fal[i][f];					// A Fal tartalma sz�k�z�kkel elv�lasztva
				sorok[i + fs] += " ";
			}
			toltelek = toltelek.substr(0, toltelek.size() - 1);		// Egyel cs�kkenti a t�ltel�k hossz�s�g�t
		}
		// PADL�VONAL
		sorok[fs + TABLAMERET] += vonal;
		int p = 0;													// Padl�vonal aktu�lis karaktere
		while (p < PADLOMERET && sorok[fs + TABLAMERET + 1].length() < sszel) {
			sorok[fs + TABLAMERET + 1] += "  ";
			sorok[fs + TABLAMERET + 1] += j[ji].Padlovonal[p];
			sorok[fs + TABLAMERET + 2] += " " + to_string(-(((p + 1) / 3) + 1));
			p++;
		}
		// V�G�L A MARAD�KOT MINDEN SORBAN FELT�LTI SZ�K�ZZEL
		for (int u = 0; u < ssz; u++) { while (sorok[u].length() < sszel) { sorok[u] += " "; } }
	}
    // AZ ELK�SZ�LT SOROK KI�R�SA
	for (int ki = 0; ki < ssz; ki++) { cout << sorok[ki] << "|" << endl; }
	cout << endl;
}

// 7/a J�T�KOS KORONG- �S CSEMPEV�LASZT�SA V�S�RL�SHOZ
string KCsValaszto(Player j[], string korong[], int korsz, int asztal[], int aj)
{
	string kivcs = "\0";		// A kiv�lasztott csempe
	bool jocsvalasztas = false;	// A j�t�kos csempev�laszt� bevitel�nek ellen�rzese

	do {
		cout << j[aj].Nev << ", v�lassz csemp�ket egy korongr�l vagy az asztalr�l!\n(Pl. 1. korong A csempe: 1A | Az asztalt a 0 jel�li: 0D) ";
		cin >> kivcs;
        // El�bb nagybet�ss� alak�tjuk a bevitt sz�veget
		for (int n = 0; n < kivcs.length(); n++) { kivcs[n] = toupper(kivcs[n]); }
        // Ha ki szeretne l�pni r�gt�n megy is vissza
		if (kivcs == "X") { jocsvalasztas = true; }
        // Ha nem k�t karaktert �rt be, m�r nem j�
		else if (kivcs.length() != 2) {
			jocsvalasztas = false;
			cout << "HIBA: nem ket karakter!\n";
		}
        // Ha az els� karakter nem, egy a korongok sz�m�n�l kisebb sz�m
		else if (int(kivcs[0]) < 48 || int(kivcs[0]) - 48 > korsz) {
			jocsvalasztas = false;
			cout << "HIBA: Ilyen korong nincs a j�t�kban!\n";
		}
        // Ha m�sodik karakter nem olyan NAGYbet�, ami megfelel a csemp�k bet�jeleinek
		else if (!(int(kivcs[1]) > 64 && int(kivcs[1]) - 65 < CSVARIACIOK)) {
			jocsvalasztas = false;
			cout << "HIBA: Ilyen bet�jel� csempe nincs a j�t�kban!\n";
		}
        // Ha korongot v�lasztott �s az �res, akkor nem j�
		else if (int(kivcs[0]) - 48 != 0 && korong[int(kivcs[0]) - 49] == "....") {
			jocsvalasztas = false;
			cout << "HIBA: A v�lasztott korong �res!\n";
		}
        // Ha az asztalt v�lasztotta, �s egy sincs rajta a k�rt csemp�b�l, akkor nem j�
		else if (int(kivcs[0]) - 48 == 0 && asztal[int(kivcs[1]) - 65] == 0) {
			jocsvalasztas = false;
			cout << "HIBA: A v�lasztott csemp�b�l nincs az asztalon!\n";
		}
        // Ha a korongot v�lasztott de nincs rajta a k�rt csempe, akkor nem j�
		else if (int(kivcs[0]) - 48 != 0 && korong[int(kivcs[0]) - 49].find(kivcs[1]) == korong[int(kivcs[0])].npos) {
			jocsvalasztas = false;
			cout << "HIBA: A korongon nincs ilyen csempe!\n";
		}
		else { jocsvalasztas = true; }
	} while (!jocsvalasztas);
	return kivcs;
}

// 7/b A J�T�KOS MINTASOR V�LASZT�SA V�R�SL�SHOZ
char SorValaszto(Player j[], string cs, int aj)
{
	string kivs;			// A kiv�lasztott sor
	bool josor = false;		// A j�t�kos Mintasor v�laszt�s�nak ellen�rz�se
	bool vanhely = false;	// A mintasorok tel�tetts�g�nek ellen�rz�se
	int szk = 0;			// V�ltoz� a szabad hely keres�s�hez
    // Van-e m�g egy�ltal�n olyan mintasor, ahov� csemp�t lehet rakni?
	do {
        // Ha a helykit�lt� '.' karaktert nem tal�lja az aktu�lis Mintasorban...
        // ...akkor ott m�r nincs szabad hely...
		if (j[aj].Mintasor[szk].find(".") == j[aj].Mintasor[szk].npos) { vanhely = false; }
        // ...k�l�nben m�g van szabad hely a Mintasorok valamelyik�ben
		else { vanhely = true; }
		szk++;
	} while (!vanhely && szk < TABLAMERET);
	if (vanhely) {
		do {
			cout << "Melyik Mintasorba szeretn�d tenni? (1-t�l " << CSVARIACIOK << "-ig | A Padl�t a 0-val adhatod meg.) ";
			cin >> kivs;
            // Ha a kil�p�st v�lasztja, m�r megy is vissza
			if (kivs == "x" || kivs == "X") { josor = true; }
            // Ha a megadott sz�m nagyobb, mint ah�ny mintasor van, akkor nem j�
			else if (int(kivs[0]) - 48 > TABLAMERET) {
				josor = false;
				cout << "HIBA: Ilyen sorsz�m� Mintasor nincs!\n";
			}
            // Ha a j�t�kos nem a padl�t v�lasztotta...
            // ...�sa kiv�lasztott Mintasor m�r nem �res...
            // ...�s nincs benne a kiv�lasztott fajt�j� csempe, akkor nem j�
			else if (kivs != "0" && j[aj].Mintasor[int(kivs[0]) - 49][0] != '.' && j[aj].Mintasor[int(kivs[0]) - 49][0] != cs[0]) {
				josor = false;
				cout << "HIBA: Abban a sorban m�r m�sfajta csemp�k vannak!\n";
			}
			else { josor = true; }
		} while (!josor);
	}
    // Ha m�r minden Mintasor tele van, akkor a csemp�k mindenk�ppen a Padl�vonalra ker�lnek
	else { kivs = "0"; }
	return kivs[0];
}

// 7/c CSEMPE A PADL�RA VAGY A DOBOTTAK K�Z� KER�L CSEMPEV�S�RL�SKOR
void CsempeAPadlora(Player j[], string csempe, int dobott[], int aj)
{
	if (j[aj].Padlovonal.find(".") == j[aj].Padlovonal.npos && csempe != "K") {
		cout << "~ A Padl� tele van, ez�rt egy " << csempe << " csemp�t el kell dobni.\n";
        // Ha a Padl� tele �s nem a Kezd�-jel�l�t kell �trakni, akkor a Csempe a Dobottak k�z� ker�l
		dobott[int(csempe[0]) - 65]++;
	}
    // Ha van hely a Padl�n akkor a Csempe vagy Kezd�-jel�l� oda ker�l
	else if (j[aj].Padlovonal.find(".") != j[aj].Padlovonal.npos) { j[aj].Padlovonal[j[aj].Padlovonal.find_first_of(".")] = csempe[0]; }
	// Ha a Kezd�-jel�l�t kell �trakni de nincs hely a padl�n, akkor nincs teend�, mert a Kezd�t nem lehet a dobottak k�z� tenni!
}

// 7/d CSEMP�K �TAD�SA A J�T�KOSNAK VAGY A PADL�RA CSEMPEV�S�RL�SKOR
void CsempeAtadas(Player j[], string csempe, int msor, int dobott[], int aj)
{
    // A j�t�kos a Padl�t v�lasztotta vagy a teli sor miatt a Padl�ra ker�lnek a csemp�k
	if (msor == 0) {
		cout << "~ Egy " << csempe << " csempe a padl�ra ker�l.\n";
		CsempeAPadlora(j, csempe, dobott, aj);
	}
	else {
        // Ha a mintasor m�r megtelt, akkor a csempe a padl�ra ker�l
		if (j[aj].Mintasor[msor - 1].find(".") == j[aj].Mintasor[msor - 1].npos) {
			cout << "~ Megtelt a(z) " << msor << ". sor, ez�rt egy " << csempe << " csempe a padl�ra ker�l.\n";
			CsempeAPadlora(j, csempe, dobott, aj);
		}
		else {
			cout << "~ Egy " << csempe << " csempe a(z) " << msor << ". Mintasorba ker�l.\n";
            // A Csempe a kiv�lasztott Mintasorba ker�l
			j[aj].Mintasor[msor - 1][j[aj].Mintasor[msor - 1].find_first_of(".")] = csempe[0];
		}
	}
}

// 7. CSEMP�K V�S�RL�SA KORONGOKR�L VAGY ASZTALR�L
bool CsempeVasar(Player j[], string korongok[], int ksz, int asztal[], int zsak[], int dobott[], int aj, int jsz)
{
	string kivkcs = KCsValaszto(j, korongok, ksz, asztal, aj);	// A kiv�lasztott korong �s csempe
	if (kivkcs == "X") { return true; }							// Ha ki akar l�pni, r�gt�n megy is vissza
	int korong = int(kivkcs[0]) - 48;							// A kiv�lasztott korong
	string csempe = kivkcs.substr(1, 1);						// A k�rt csempe
	int msor = int(SorValaszto(j, csempe, aj)) - 48;			// A kiv�lasztott Mintasor ahov� a csemp�k ker�lnek
	if (msor == 40 || msor == 72) { return true; }				// Ha ki akar l�pni, m�ris megy vissza

    // a nem a padl�t v�lasztotta a j�t�kos �s a kiv�lasztott sor tele van...
	while (msor != 0 && j[aj].Mintasor[msor - 1].find(".") == j[aj].Mintasor[msor - 1].npos) {
        // Ha �gy j� neki, akkor a csemp�k mennek a padl�ra
		if (Dontes("Ez a sor tele van, a csemp�k a Padl�ra ker�lnek. Biztosan ezt akarod? (i / n) ")) { msor = 0; }
        // Ha nem j�, akkor v�laszthat m�sik Mintasort
		else { msor = SorValaszto(j, csempe, aj) - 48; }
	}

    // Ha az asztalt v�lasztotta
	if (korong == 0) {
        // Ha m�g az asztalon van a kezd�j�t�kos jel�l�
		if (asztal[CSVARIACIOK] == 1) {
			cout << "~ A Kezd�j�t�kos-jel�l� �thelyez�se a padl�vonalra...\n";
			CsempeAPadlora(j, "K", dobott, aj);					// El�sz�r �tadja a padl�vonalra
			asztal[CSVARIACIOK] = 0;							// Leveszi az Asztalr�l a kezd�j�t�kos jel�l�t
			KovJatekosReset(j, jsz);							// Mindenkin�l hamisra �ll�tja a k�vetkez�-j�t�kos jel�l�t
			j[aj].kovetkezik = true;							// Az aktu�lis j�t�kos kezdi majd a k�vetkez� k�rt
			cout << "~ A k�vetkez� fordul�t " << j[aj].Nev << " kezdi.\n";
		}
        // Eges�vel kell �tadni az asztalon l�v� csemp�ket
		for (int i = 0; i < asztal[int(csempe[0]) - 65]; i++) { CsempeAtadas(j, csempe, msor, dobott, aj); }
        // V�g�l null�zza a csemp�k hely�t az asztalon
		asztal[int(csempe[0]) - 65] = 0;
	}
    // Ha valamelyik korongot v�lasztotta
	else {
		cout << "~ A kiv�lasztott csemp�k �thelyez�se...\n";
        // V�gigmegy a korongon t�rolt csemp�ken
		for (int i = 0; i < KMERET; i++) {
            // Ha az aktu�lis csempe a k�rt csempe, akkor �tadja a j�t�kosnak
			if (korongok[korong - 1].substr(i, 1) == csempe) { CsempeAtadas(j, csempe, msor, dobott, aj); }
            // A nem k�v�nt csemp�ket az asztalra teszi
			else { asztal[int(korongok[korong - 1][i]) - 65]++; }
		}
        // V�g�l a korong �r�t�se
		korongok[korong - 1] = "\0";
		for (int i = 0; i < KMERET; i++) { korongok[korong - 1] +="."; }
	}
	return false;
}

// 8/a MINTASOROK NULL�Z�SA
void MintasorNullazas(Player j[], int aktj, int akts)
{
	string minta = "\0";								// A Mintasor kirajzols�shoz sz�ks�ges v�ltoz�
    // A soron bel�li poz�ci�
	for (int i = 0; i < akts + 1; i++) {
		j[aktj].Mintasor[akts] = minta + ".";
		minta += ".";
	}
}

// 8/b PADL� NULL�Z�SA
void PadloNullazas(Player j[], int aj)
{
	j[aj].Padlovonal = "\0";							// A padl�vonal �res legyen
    // Padl�vonalon bel�li poz�ci�
    // A Padl�vonal kirajzol�sa hely�rz�kkel
	for (int k = 0; k < PADLOMERET; k++) { j[aj].Padlovonal += "."; }
}

// 8/c PONTSZ�M�T�S FALAZ�SKOR
int FalPontok(Player j[], int aj, int akts, int kivo)
{
	int pont = 1;										// A j�t�kos �ltal kapott pontok (egyet biztosan kap, mert felfalazta a csemp�t)
	int k = 0;											// Keres� index
	bool vansor = false;								// Tal�lat v�zszintes keres�s�n�l
	bool vanoszlop = false;								// Tal�lat f�gg�leges keres�s�n�l
    // Csak akkor n�z el balra, ha a lerakott csempe nem a bal sz�ls� oszlopba ker�lt
	if (kivo > 0) {
		k = kivo;
        // Akkor j�r pont, ha az aktu�lis poz�ci�t�l balra is van csempe
		while (k > 0 && j[aj].Fal[akts][k - 1] != '.') {
			pont++;
			k--;
			vansor = true;
		}
	}
    // Csak akkor n�z el jobbra, ha a lerakott csempe nem a jobb sz�ls� oszlopba ker�lt
	if (kivo < TABLAMERET - 1) {
		k = kivo;
        // Akkor j�r pont, ha az aktu�lis poz�ci�t�l jobbra is van csempe
		while (k < TABLAMERET - 1 && j[aj].Fal[akts][k + 1] != '.') {
			pont++;
			k++;
			vansor = true;
		}
	}
    // Csak akkor n�z felfel�, ha a lerakott csempe nem a legfels� oszlopba ker�lt
	if (akts > 0) {
		k = akts;
        // Akkor j�r pont, ha az aktu�lis poz�ci� felett is van csempe
		while (k > 0 && j[aj].Fal[k - 1][kivo] != '.') {
			pont++;
			k--;
			vanoszlop = true;
		}
	}
    // Csak akkor n�z lefel�, ha a lerakott csempe nem a legals� oszlopba ker�lt
	if (akts < TABLAMERET - 1) {
		k = akts;
        // Akkor j�r pont, ha az aktu�lis poz�ci� alatt is van csempe
		while (k < TABLAMERET - 1 && j[aj].Fal[k + 1][kivo] != '.') {
			pont++;
			k++;
			vanoszlop = true;
		}
	}
    // Ha volt sor �s oszlop is, akkor m�g egy pont j�r, mert az �ppen lerakott csemp�t egyikbe se sz�molta bele
	if (vansor && vanoszlop) { pont++; }
	cout << j[aj].Nev << ", " << pont << " pontot kapt�l a csempe felrak�s��rt.\n";
	return pont;
}

// 8/d PONTLEVON�S A PADL� MIATT
int PadloPontok(Player j[], int aj)
{
	int buntipontok = 0;								// A padl�ra esett csemp�k�rt j�r� b�ntet�pontok
    // A padl�n bel�li poz�ci�
	for (int p = 0; p < PADLOMERET; p++) { if (j[aj].Padlovonal[p] != '.') { buntipontok += -(((p + 1) / 3) + 1); } }
	cout << j[aj].Nev << ", " << buntipontok << " b�ntet�pontot kapt�l a padl�ra esett csemp�k miatt.\n";
	return buntipontok;
}

// 8/e FAL F�GG�LEGES (OSZLOP) VIZSG�LATA FALAZ�SHOZ
bool JoOszlop(Player j[], int aj, int ao, string csempe)
{
	bool joo = true;									// Az oszlop alkalmass�ga
	int i = 0;											// Oszlop-szkennel�
	do {
        // Amint tal�l egyet a megfelel� csemp�b�l hamisra �ll�tja
		if (j[aj].Fal[i][ao] == csempe[0]) { joo = false; }
		i++;
	} while (joo && i < TABLAMERET);
	return joo;
}

// 8/f CSEMP�K ELDOB�SA FALAZ�SKOR
void CsempeEldobas(Player j[], int dobott[], int aj, int msor)
{
	int i = 0;													// A Padl�vonal egy karakter�nek indexe
    // Ha a Padl�r�l kell eldobni csemp�t
	if (msor == 0) {
		while (i < PADLOMERET && j[aj].Padlovonal[i] != '.') {
            // Ha az eldoband� csempe a Kezd�j�t�kos-jel�l�, akkor nem ker�l a Dobottak k�z�
			if (j[aj].Padlovonal[i] == 'K') { i++; }
			else {
				dobott[int(j[aj].Padlovonal[i]) - 65]++;
				i++;
			}
		}
	}
    // Ha a Mintasorb�l kell eldobni csemp�t
	else {
		while (i < j[aj].Mintasor[msor - 1].length() - 1) {
			dobott[int(j[aj].Mintasor[msor - 1][i]) - 65]++;
			i++;
		}
	}
}

// 8. FALAZ�S
bool Falazas(Player j[], string korong[], int asztal[], int zsak[], int dobott[], int ksz, int jsz)	//j[]: A j�t�kosok t�mbje, g: Game v�ltoz�, dobott[]: a dobott csemp�k t�mbje, jsz: j�t�kosok sz�ma
{
	bool jovalasz = false;					// A megfelel� oszlop kivalasztasa
	string kivo = "\0";						// A kiv�lasztott oszlop
	string csempe = "\0";					// Az �ppen manipul�lt csempe
	int osszespluszp = 0;					// A csemp�k falaz�s��rt j�r� pontok
	int buntetopontok = 0;					// A padl�ra esett cemp�k miatt kapott b�ntet�pontok
	cout << "* * * Kezd�dik a falaz�s! * * *\n";;
    // Aktu�lis j�t�kos sorsz�ma
	for (int aj = 0; aj < jsz; aj++) {
        // Mintasor szorsz�ma
		for (int s = 0; s < TABLAMERET; s++) {
            // Ha az aktu�lis Mintasor nincs tele
			if (j[aj].Mintasor[s].find(".") == j[aj].Mintasor[s].npos) {
                // Ha a Falon abban a sorban m�g nincs a Mintasorban l�v�kkel azonos csempe
				if (j[aj].Fal[s].find(j[aj].Mintasor[s][0]) == j[aj].Fal[s].npos) {
					KorongAsztalMegj(korong, asztal, zsak, dobott, ksz);					// Megjelen�ti a Korongok, az Asztal, a Zs�k �s a Dobott tartalm�t
					PlayerMegj(j, jsz, aj);													// Megjelen�ti a j�t�kosok t�bl�it
					csempe = j[aj].Mintasor[s][0];
					cout << j[aj].Nev << ", a " << s + 1 << ". sorb�l egy " << csempe << " csemp�t felfalazhatsz.\n";
					do {
						cout << "Melyik oszlopba szeretn�d teni? (1-t�l " << TABLAMERET << "-ig, a padl�t a 0-val adhatod meg) ";
						cin >> kivo;
                        // A j�t�kos ki szeretne l�pni, ez�rt m�r megy�nk is vissza
						if (kivo == "x" || kivo == "X") { return true; }
                        // Ha a megadott oszlop nagyobb, mint az oszlopok sz�ma (vagy nem sz�m)
						else if (int(kivo[0]) - 48 > TABLAMERET) {
							jovalasz = false;
							cout << "HIBA: Ilyen sorsz�m� oszlop nincs a falon!\n";
						}
                        // Ha nem a Padl�t v�lasztotta �s a kiv�lasztott oszlopban l�v� hely nem �res
						else if (kivo != "0" && j[aj].Fal[s][int(kivo[0]) - 49] != '.') {
							jovalasz = false;
							cout << "HIBA: A kiv�lasztott hely foglalt!\n";
						}
                        // Ha nem a Padl�t v�lasztotta de a megadott oszlopban valahol m�r van ilyen csempe
						else if (kivo != "0" && !JoOszlop(j, aj, int(kivo[0]) - 49, csempe)) {
							jovalasz = false;
							cout << "HIBA: Ebben az oszlopban m�r van ilyen csempe!\n";
						}
						else { jovalasz = true; }
					} while (!jovalasz);
                    // Ha a j�t�kos a Padl�t v�lasztotta
					if (kivo == "0") {
                        // Annyiszor dobok csemp�t a padl�ra, ah�ny darab a Mintasorban volt (azaz a Mintasor hossza)
						for (int p = 0; p < j[aj].Mintasor[s].length(); p++) {
							cout << "~ Egy " << csempe << " a(z) " << s + 1 << ". sorb�l a Padl�ra ker�l.\n";
                            // Egyes�vel a Padl�ra ker�lnek a csemp�k
							CsempeAPadlora(j, csempe, dobott, aj);
						}
					}
					else {
						cout << "~ Egy " << csempe << " a(z) " << s + 1 << ". sorb�l a Fal " << kivo[0] - 48 << ". oszlop�ba ker�l.\n";
						j[aj].Fal[s][int(kivo[0]) - 49] = csempe[0];				// A falra ker�l a csempe
						j[aj].Pontok += FalPontok(j, aj, s, int(kivo[0]) - 49);		// A falaz�s�rt pontot kap a j�t�kos
						cout << "~ A Mintasorban maradt csemp�k eldob�sa...\n";
						CsempeEldobas(j, dobott, aj, s + 1);						// Eldobjuk a marad�k csemp�t a Mintasorb�l
					}
					cout << "~ Mintasor �r�t�se...\n";
					MintasorNullazas(j, aj, s);										// A Mintasor �r�t�se
				}
                // Ha a Falon m�r van a Mintasorban l�v�kkel azonos csempe
				else {
					csempe = j[aj].Mintasor[s][0];
					cout << "~ Van m�r " << csempe << " csempe a Fal " << s << ". sor�ban, ez�rt a Mintasorb�l az �sszes a Padl�ra ker�l.\n";
                    // Annyiszor dobok csemp�t a padl�ra, ah�ny a Mintasorban volt
					for (int p = 0; p < j[aj].Mintasor[s].length(); p++) {
                        // Egyes�vel a Padl�ra ker�lnek a csemp�k
						CsempeAPadlora(j, csempe, dobott, aj);
					}
					cout << "~ Mintasor �r�t�se...\n";
					MintasorNullazas(j, aj, s);								// A Mintasor �r�t�se
				}
			}
		}
		cout << "~ B�ntet�pontok sz�m�t�sa...\n";
		buntetopontok = PadloPontok(j, aj);				// A padl�n l�v� csemp�k ut�n j�r� b�ntet�pontok kisz�m�t�sa
        // Ha a b�ntet�pontok miatt negat�v lenne a j�t�kos pontsz�ma, akkor nulla pontja lesz
		if (j[aj].Pontok + buntetopontok < 0) { j[aj].Pontok = 0; }
		else { j[aj].Pontok += buntetopontok; }
		CsempeEldobas(j, dobott, aj, 0);														// Eldobjuk a csemp�ket a Padl�r�l
		PadloNullazas(j, aj);																	// �r�ti a Padl�t
		cout << j[aj].Nev << ", a falaz�s ut�n " << j[aj].Pontok << " pontod van.\n";
	}
	return false;
}

// 9. B�NUSZPONTOK SZ�M�T�SA
void BonuszPontok(Player j[], Score e[], int jsz)
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
	for (int i = 0; i < jsz; i++) {
        // Null�zzuk a sorok sz�m�t az adott j�t�kos eredm�nyjelz�j�n�l
		e[i].SorokSzama = 0; 
		for (int s = 0; s < TABLAMERET; s++) {
            // Ha a Fal adott sor�ban nincs �res hely, akkor j�r a 2 pont
			if (j[i].Fal[s].find(".") == j[i].Fal[s].npos) {
				bonuszsor += 2;
				e[i].SorokSzama++;							// Ha tal�ltunk n�la egy sort, akkor az eredm�nyjelz�n is kap �rte 1 pontot
			}
			teljesoszlop = true;
			o = 0;
			do {
                // Amint tal�l az oszlopban egy �res helyet, m�r nem teljes az osztlop
				if (j[i].Fal[o][s] == '.') { teljesoszlop = false; }
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
				if (j[i].Fal[sz].find(char(c + 65)) == j[i].Fal[sz].npos) { teljesszin = false; }
				sz++;
			} while (teljesszin && sz < TABLAMERET);
            // Ha nincs olyan sor a Falon, amiben ne szerepelne az adott Csempe, akkor j�r a 10 pont
			if (teljesszin) { bonuszszin += 10; }
		}
		j[i].Pontok += bonuszsor + bonuszoszlop + bonuszszin;
		cout << j[i].Nev << " b�nuszpontjai:\n\t+"
			<< bonuszsor << " pont a sorok�rt\n\t+"
			<< bonuszoszlop << " pont az oszlopok�rt\n\t+" 
			<< bonuszszin << " pont a teljes sz�nek�rt.\n";
	}
}

// 10. EREDM�NYEK KI�R�SA
void Vegeredmeny(Player j[], Score e[], int jsz)
{
	string n = "\0";									// Neveket t�rol� seg�dv�ltoz�
	int p = 0;											// Pontokat t�rol� seg�dv�ltoz�
	int s = 0;											// A Sorok sz�m�t t�rol� seg�dv�ltoz�

    // �tm�soljuk az eredm�nyjelz� t�mbbe a j�t�kosok neveit �s pontsz�mait
	for (int m = 0; m < jsz; m++) {
		e[m].Nev = j[m].Nev;
		e[m].Pont = j[m].Pontok;
	}

    // A kisebb index� elem
	for (int i = 0; i < jsz - 1; i++) {
        // Az egyel nagyobb index� elem
		for (int k = i + 1; k < jsz; k++) {
            // Ha a kisebbik index� kisebb, mint a nagyobb index�, akkor csere (mert a helyez�seket az index hat�rozza meg)
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
            // Ha a k�t egym�s melletti elem egyenl�, akkor a sorok sz�ma d�nt
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
	cout << "\n\n* * * V�GEREDM�NY * * *\n\n";
	for (int l = 0; l < jsz; l++) { cout << l + 1 << ". helyezett: " << e[l].Nev << ", " << e[l].Pont << " ponttal, " << e[l].SorokSzama << " teljes sorral\n"; }
}

// 11. KEZD� K�PERNY�
void KezdoKepernyo()
{
	system("cls");
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
// 11. Z�R� K�PERNY�
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

int main()
{
	setlocale(LC_ALL, "hun");		// Magyarra �ll�tja a nyelvet

	Game g;							// V�ltoz� a Game strukt�r�val
	Player* p;						// T�mb a Palyer stukt�r�val
	Score* e;						// T�mb a Score strukt�r�val

	bool jatekvege = false;			// A j�t�k v�ge
	bool fordulovege = false;		// A fordul� v�ge
	bool kilepes = false;			// A j�t�kos ki akar l�pni
	int fordulo = 0;				// A fordul� sorsz�ma
	int lepes = 0;					// A fordul�n bel�li l�p�sek sz�ma

	KezdoKepernyo();
	int jsz = JatekosSzamBe();							// A j�t�kosok sz�ma
	p = new Player[jsz];								// A j�t�kosok adatait tartalmaz� Player t�pus� t�mb l�trehoz�sa a j�t�kossz�m alapj�n
	e = new Score[jsz];									// Az eredm�nyek kijelz�s�hez sz�ks�ges t�mb l�trehoz�sa a j�t�kossz�m alapj�n
	g.KorongSzam = jsz * 2 + 1;							// A korongok sz�ma (a g v�ltoz�ban) egyel t�bb, mint a j�t�kosok sz�m�nak k�tszerese
	g.Korongok = new string[g.KorongSzam];				// A Korongok t�mb elk�sz�t�se a korongok sz�ma alapj�n

	KezdoZsakok(g.Zsak, g.Dobott, g.AsztalKozep);		// A Zs�k, Eldobottak zs�kja �s az Asztal k�zep�nek el�k�sz�t�se
	PlayerElokeszites(p, jsz);							// A Player t�pus� t�mb el�k�sz�t�se a j�t�kossz�m alapj�n
	JatekosNevek(p, jsz);								// Opcion�lisan megadhat� a j�t�kosok beceneve

	while (!jatekvege) {
		jatekvege = KorongTolto(g.Korongok, g.Zsak, g.Dobott, g.AsztalKozep, g.KorongSzam);			// Felt�lti a Korongokat, az Asztalta teszi a kezd�j�t�kos jel�l�t
		int aj = 0;																					// Aktu�lis j�t�kos
		while (!p[aj].kovetkezik && aj < jsz) { aj++; }												// Megkeresi melyik j�t�kos k�vetkezik
		fordulo++;																					// N�velj�k a fordul� sorsz�m�t
		lepes = 0;																					// Vissza�ll�tjuk a fordul�n bel�li l�p�ssz�mot
		fordulovege = false;																		// A fordul� most kezd�dik, �gyhogy m�g nincs v�ge
		while (!fordulovege && !jatekvege) {
			lepes++;
//			system("cls");
			cout << "* * *  CSEMPEV�S�R " << fordulo << ". fordul� " << lepes << ". l�p�s  * * *\n\n";			// K�rja hol tartunk
			KorongAsztalMegj(g.Korongok, g.AsztalKozep, g.Zsak, g.Dobott, g.KorongSzam);						// Megjelen�ti a Korongok, az Asztal, a Zs�k �s a Dobott tartalm�t
			PlayerMegj(p, jsz, aj);																				// Megjelen�ti a j�t�kosok t�bl�it
			kilepes = CsempeVasar(p, g.Korongok, g.KorongSzam, g.AsztalKozep, g.Zsak, g.Dobott, aj, jsz);		// A j�t�kos csemp�ket v�s�rolhat a Korongokr�l vagy az Asztalr�l
			if (aj == jsz - 1) { aj = 0; }																		// Egyel l�ptei az j�t�kosokat
			else { aj++; }
			fordulovege = ForduloVege(g.Korongok, g.KorongSzam, g.AsztalKozep) || kilepes;						// A fordul� v�g�nek leellen�rz�se
		}
		if (!kilepes) {
			kilepes = Falazas(p, g.Korongok, g.AsztalKozep, g.Zsak, g.Dobott, g.KorongSzam, jsz);		// Falaz�s + pontok
		}
		jatekvege = JatekVege(p, jsz, g.Zsak, g.Dobott) || kilepes;										// A j�t�k v�g�nek ellen�rz�se
	}
	BonuszPontok(p, e, jsz);
	Vegeredmeny(p, e, jsz);
	cout << "\nGRATUL�LOK!\n";
	cout << "K�sz�n�m, hogy j�tszottatok!\n\n\n";
	system("pause");
	ZaroKepernyo();
	system("pause");
	delete[]g.Korongok;
	delete[]p;
	return 0;
}