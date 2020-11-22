// N�h�ny sz�k�r m�g hi�nyozhat, hogy minden t�bl�zatoszlop j�l n�zzen ki!!!


#include"iostream"
#include"iomanip"
#include"cmath"
#include"cstdlib"
#include"ctime"
#include"string"

using namespace std;

#define CSVARIACIOK 5	// Ennyi k�l�nb�z� csempefajta van a j�t�kban.
#define CSKEZDODARAB 20	// Egy adott fajta csemp�kb�l ennyi darab van �sszesen.
#define KMERET 4		// A korongokra helyezhet� csepmp�k maxim�lis sz�ma.
#define TABLAMERET 5	// A Mintasor illetve a Fal m�retei a j�t�kosok t�bl�j�n. (�ltal�ban megegyezik a csempefajt�k sz�m�val azaz CSVARIACIOK v�ltoz�val.)
#define PADLOMERET 7	// A Padlovonal m�rete a j�t�kosok t�bl�j�n.


struct Player						// A j�t�kosokhoz tartoz� adatok
{
	string Nev;						// A j�t�kos neve
	int Pontok;						// A j�t�kos pontsz�ma
	string Mintasor[TABLAMERET];	// A mintasor, ahov� a korongokr�l vagy asztalr�l v�s�rolt csemp�ket gy�jti.
	string Fal[TABLAMERET];			// A fal, ahov� felteszi a mintasoron �sszegy�lt csemp�ket.
	string Padlovonal;				// A padl�vonal ahol a b�ntet�pontok gy�lnek.
	bool kovetkezik;				// Azt mutatja, hogy a j�t�kos k�vetkezik-e
};


void PlayerMegj(Player j[], int ssz)			// j[]: Player tipus� t�mb, a main 'p' v�ltoz�j�b�l, ssz: a j�t�kosok sz�ma
{
	const int fs = 7;							// A fejl�cbe ki�rand� sorok sz�ma
	const int ps = 3;							// A padl�vonalra ki�rand� sorok sz�ma
	const int soroksz = fs + TABLAMERET + ps;	// A ki�and� sorok sz�ma �sszesen: a fejl�c, a t�bla �s a padl�ra ki�rand� sorok sz�ma
	string kiiras[soroksz];						// Ebbe gy�jtj�k a j�t�kos el�tti t�bla ki�rand� tartalm�t soronk�nt
	string toltelek;							// Helykit�lt� sz�k�z�k a Mintasor el�, hogy balra z�rt legyen
	string elv = " | ";							// Ezzel v�lasztja el egym�st�l a j�t�kosok t�bl�it
	string vonal;								// Ez lesz a t�bl�zat als� �s fels� vonala

	for (int i = 0; i < soroksz; i++)			// A t�bl�zat bal oldala
	{
		kiiras[i] = "| ";
	}

	// A j�t�kosok t�bl�inak ki�r�sa egym�s mell�

	for (int kisz = 0; kisz < ssz; kisz++)
	{
		if (j[kisz].kovetkezik)										// Ha az adott j�t�kos k�vetkezik, akkor a t�bl�zat als� fels� vonala *-karakterb�l �ll
		{
			vonal = "****************";
		}
		else
		{
			vonal = "----------------";
		}
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
