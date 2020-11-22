#include"iostream"
#include"iomanip"
#include"cmath"
#include"cstdlib"
#include"ctime"
#include"string"

using namespace std;


#define CSVARIACIOK 5	// Ennyi k�l�nb�z� csempefajta van a j�t�kban.
#define TABLAMERET 5	// A Mintasor illetve a Fal m�retei a j�t�kosok t�bl�j�n. (�ltal�ban megegyezik a csempefajt�k sz�m�val azaz CSVARIACIOK v�ltoz�val.)


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



// KORONGOK �S ASZTAL K�ZEP�N L�V� CSEMP�K MEGJELEN�T�SE
// A f�ggv�nyh�v�sba be kell k�ldeni a Game strukt�r�val l�trehozott v�ltoz� r�szeit:
// KorongAsztalMegjelenites(g.Korongok, g.AsztalKozep, g.Zsak, g.Dobott, g.KorongSzam);
//
// A Zs�k �s a Dobottak megjelen�t�s�t nem k�ri a feladat �s az igazi j�t�kban se l�tjuk,
// hogy mi van benn�k, �gyhogy azt csak az�rt tettem hozz�, hogy l�tsz�djon a program m�k�d�se, 
// de ki lehet venni term�szetesen.

void KorongAsztalMegjelenites(string korong[], int asztal[], int zsak[], int dobott[], int ksz)
{
	cout << "Korongok:\t\tAsztal:\t\tZs�k:\t\tDobott:" << endl;	// A t�bl�zat fejl�ce
	if (asztal[CSVARIACIOK] == 1)								// Ha a kezd�j�t�kos jel�l� az asztalon van, akkor ki�rja
	{
		cout << "\t\t\tKezd�" << endl;
	}
	for (int i = 0; i < CSVARIACIOK; i++)						// El�szor csak addig sz�molunk el, am�g minden t�mb biztosan tartalmaz elemeket azaz, addig ah�ny csempefajta van (CSVARIACIOK)
	{
		cout << i + 1 << ".: " << korong[i] << "\t\t"
			<< char(65 + i) << ": " << asztal[i] << "\t\t"
			<< char(65 + i) << ": " << zsak[i] << "\t\t"	//!! Ezt a sort h�zd ki, ha nem akarod megjelen�teni a zs�kot
			<< char(65 + i) << ": " << dobott[i] << endl;	//!! Ezt a sort h�zd ki, ha a dobottakat nem akarod megjelen�teni
	}
	if (ksz > CSVARIACIOK)										// Ha t�bb a korong, mint ah�ny csempefajta van, akkor m�r csak a korongok tartalm�t �rjuk ki
	{
		for (int i = CSVARIACIOK; i < ksz; i++)
		{
			cout << i + 1 << ".: " << korong[i] << endl;
		}
	}
	cout << endl;
}



// PLAYER MEGJELEN�T�SE
// H�v�skor ebbe be kell k�ldeni a Player strukt�r�val l�trehozott t�mb�t �s a j�t�kosok sz�m�t:
// PlayerMegjelenites(j, jsz);

void PlayerMegjelenites(Player j[], int jsz)
{
	const int fs = 7;							// A fejl�cbe ki�rand� sorok sz�ma
	const int ps = 3;							// A padl�vonalra ki�rand� sorok sz�ma
	const int soroksz = fs + TABLAMERET + ps;	// A ki�and� sorok sz�ma �sszesen: a fejl�c, a t�bla �s a padl�ra ki�rand� sorok sz�ma
	string kiiras[soroksz];						// Ebbe gy�jtj�k a j�t�kos el�tti t�bla ki�rand� tartalm�t soronk�nt
	string toltelek;							// Helykit�lt� sz�k�z�k a Mintasor el�, hogy balra z�rt legyen
	string elv = " | ";							// Ezzel v�lasztja el egym�st�l a j�t�kosok t�bl�it
	string vonal = "----------------";			// Ez lesz a t�bl�zat als� �s fels� vonala

	for (int i = 0; i < soroksz; i++)			// A t�bl�zat bal oldala
	{
		kiiras[i] = "| ";
	}

	// A j�t�kosok t�bl�inak ki�r�sa egym�s mell�

	for (int kisz = 0; kisz < jsz; kisz++)
	{
		toltelek = "\0";

		for (int i = 0; i < TABLAMERET - 1; i++)					// A t�ltel�kkarakterek elk�sz�t�se
		{
			toltelek += " ";
		}

		// itt ker�l bele a fejl�c tartalma

		kiiras[0] += vonal + elv;
		kiiras[1] += to_string(kisz + 1) + ". J�T�KOS:     " + elv;
		kiiras[2] += j[kisz].Nev.substr(0, 16);						// A n�vnek legfeljebb az els� 16 karaktere lesz ki�rva
		int k2h = 16 - j[kisz].Nev.length();						// A [2]-hez sz�ks�ges sz�k�z�k sz�ma
		for (int i = 0; i < k2h; i++)								// Felt�ltj�k a sz�ks�ges mennyis�g� sz�k�zzel
		{
			kiiras[2] += " ";
		}
		kiiras[2] += elv;
		kiiras[3] += "(" + to_string(j[kisz].Pontok) + " pont)";
		int k3h = 16 - to_string(j[kisz].Pontok).length() - 7;		// A [3]-hoz sz�ks�ges sz�k�z�k sz�ma
		for (int i = 0; i < k3h; i++)								// Felt�ltj�k a sz�ks�ges mennyis�g� sz�k�zzel
		{
			kiiras[3] += " ";
		}
		kiiras[3] += elv;
		kiiras[4] += "---------+------" + elv;
		kiiras[5] += "         | 12345" + elv;
		kiiras[6] += "---------+------" + elv;

		// itt ker�l bele a Mintasor �s a Fal tartalma

		for (int i = 0; i < TABLAMERET; i++)
		{
			kiiras[i + fs] += to_string(i + 1) + ". " + toltelek + j[kisz].Mintasor[i] + elv + j[kisz].Fal[i] + elv;
			toltelek = toltelek.substr(0, toltelek.size() - 1);
		}

		// itt ker�l be a Padl�vonal tartalma

		kiiras[fs + TABLAMERET] += "---------+------" + elv;
		kiiras[fs + TABLAMERET + 1] += "Padlo: " + j[kisz].Padlovonal;
		int kp1h = 16 - j[kisz].Padlovonal.length() - 7;
		for (int i = 0; i < kp1h; i++)								// Felt�ltj�k a sz�ks�ges mennyis�g� sz�k�zzel
		{
			kiiras[fs + TABLAMERET + 1] += " ";
		}
		kiiras[fs + TABLAMERET + 1] += elv;
		kiiras[fs + TABLAMERET + 2] += vonal + elv;
	}

	for (int ki = 0; ki < soroksz; ki++)							// Az elk�sz�lt sorok ki�r�sa
	{
		cout << kiiras[ki] << endl;
	}
}
