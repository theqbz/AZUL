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


// GLOB�LIS V�LTOZ�K
#define CSVARIACIOK 5	//Ennyi k�l�nb�z� csempefajta van a j�t�kban.
#define CSKEZDODARAB 20	//Egy adott fajta csemp�kb�l ennyi darab van �sszesen.
#define KMERET 4		//A korongokra helyezhet� csepmp�k maxim�lis sz�ma.
#define TABLAMERET 5	//A Mintasor illetve a Fal m�retei a j�t�kosok t�bl�j�n. (�ltal�ban megegyezik a CSVARIACIOK v�ltoz� �rt�k�vel.
#define PADLOMERET 7	//A Padlovonal m�rete a j�t�kosok t�bl�j�n.
string* KTOMB = NULL;	//A csemp�ket t�rol� korongok


struct Game // A j�t�k f� adatai
{
	string* Korongok;				//A korongok t�mbje
	int KorongSzam;					//A korongok sz�ma
	int AsztalKozep[CSVARIACIOK+1];	//Az asztal k�zep�t t�rol� t�mb helye
	int Zsak[CSVARIACIOK];			//A zs�k t�mbj�nek helye 
	int Eldobott[CSVARIACIOK];		//Az eldobott csemp�k t�mbj�nek helye
};


struct Player
{
	string Nev;						//A j�t�kos neve
	int Pont;						//A j�t�kos pontsz�ma
	string Mintasor[TABLAMERET];	//A j�t�kos t�bl�j�n l�v� mintasor, ahov� a korongokr�l vagy asztalr�l v�s�rolt csemp�ket gy�jti.
	string Fal[TABLAMERET];			//A j�t�kos t�bl�j�n l�v� fal, ahov� felteszi a mintasoron �sszegy�lt csemp�ket.
	string Padlovonal[PADLOMERET];	//A j�t�kos t�bl�j�n l�v� padl�vonal ahol a b�ntet�pontok gy�lnek.
};


// 1. J�T�KOSOK SZ�M�NAK BEK�R�SE
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

// 2. KEZDETI ZS�K, ELDOBOTT �S ASZTALK�Z�P L�TREHOZ�SA
void KezdoZsakok(int zs[], int d[], int a[]) {
	for (int i = 0; i < CSVARIACIOK; i++)
	{
		zs[i] = CSKEZDODARAB;	//A zs�k rekeszeinek felt�lt�se a csemp�k kezdeti darabsz�m�val
		d[i] = NULL;			//A dobottak zs�k null�z�sa
		a[i] = NULL;			//Az asztal k�zep�nek null�z�sa
	}
	a[CSVARIACIOK] = 1;			//Itt tessz�k le az asztal k�zep�re a kezd�j�t�kos jelz�t
}

// 3. V�LETLEN GENER�L�SA
int veletlen(int x)
{

	return rand() % x + 1;
}

// 4. CSEMPE EL�H�Z�SA A ZS�KB�L
int CsempeHuzas(const int hzs[])	//hzs[]: a strukt�ra zs�k eleme, azaz a csempezs�k
{
	srand((unsigned)time(NULL));	//v�letlen-gener�tor inicializ�l�sa
	int csdb = 0;					//a zs�kban maradt csemp�k sz�ma
	for (int i = 0; i < CSVARIACIOK; i++)
	{
		csdb += hzs[i];				//A zs�k[]-ban l�v�, k�l�nb�z� sz�n� csemp�k sz�m�t (a t�mb egyes rekeszeiben t�rolt �rt�keket) �sszeadja
	}
	int v = rand() % csdb + 1;		//v�letlen sz�m
	int cs = -1;					//kivalasztott csemperekesz azonos�t�ja
	int cskeres = 0;				//Keres�v�ltoz�
	do											//Azt keresi, hogy az el�bb gener�lt v�letlen sz�m melyik sz�n� csemp�t jel�li:
	{
		cs++;									//A zs�k[] t�mb index�t n�veli (azaz l�pdel a k�l�nb�z� sz�n� csemp�k k�z�tt el�re)
		cskeres += hzs[cs];						//A zs�k[] t�mb adott index� elem�nek �rt�k�t hozz�adja a keres�v�ltoz�hoz.
	} while (cskeres < v && cs < CSVARIACIOK);	//Ha a v�letlen sz�m a keres�v�ltoz�n�l kisebb vagy egyenl�, akkor a keresett csempesz�nt a zs�k[] aktu�lis indexe jel�li.
	return cs;
}

// 5. KORONGOK FELT�LT�SE
int KorongTolto(string korongok[], int zsak[], int dobott[], int korongszam)
{
	//Ha a Zs�k �res, fel kell t�lteni a Dobottakb�l, ha ott sincs semmi, akkor elfogytak a csemp�k �s v�ge a j�t�knak.

	int zscsdb = 0;							//a zs�kban maradt csemp�k sz�ma
	int dcsdb = 0;							//a dobott zs�kban l�v� csemp�k sz�ma
	bool jatekvege = false;					//Ha elfogott minden csempe a z�kb�l �s a dobottak k�z�l, akkor v�ge a j�t�knak
	for (int i = 0; i < CSVARIACIOK; i++)
	{
		zscsdb += zsak[i];					//A zs�k[]-ban l�v�, k�l�nb�z� sz�n� csemp�k sz�m�t (a t�mb egyes rekeszeiben t�rolt �rt�keket) �sszeadja
		dcsdb += dobott[i];					//A dobott zs�kban l�v� csemp�k sz�m�t adja �ssze
	}
	if (zscsdb == 0)
	{						//A Zs�k �res...
		if (dcsdb == 0)
		{					//...�s a Dobottak is �res...
			jatekvege = true;
		} 
		else
		{
			for (int i = 0; i < CSVARIACIOK; i++)
			{
				zsak[i] = dobott[i];				//�tpakoljuk a dobottakban t�rolt �rt�keket a zs�kba.
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

	//A Zs�k m�g nem �res, �gyhogy lehet h�zni a csemp�ket a korongokra:

		string csempek="\0";					//H�z�s k�zben ide gy�jti a kih�zott csemp�ket
		for (int i = 0; i < korongszam; i++)	//Egyes�vel v�gigmegy a korongokon
		{
			for (int j = 0; j < KMERET; j++)	//Annyi csemp�t h�z a zs�kb�l, ah�ny elf�r egy korongon
			{
				int kihCs = CsempeHuzas(zsak);	//egy kih�zott csempe, azaz a zs�k[] t�mb egyik rekesz�nek indexsze
				csempek += char(65 + kihCs);	//A csemp�k v�ltoz�ba gy�jtjuk a karaktereket a kih�zott t�mbindex alapj�n.
				zsak[kihCs]--;					//A kih�zott csempefajt�b�l egyel kevesebb marad a zs�kban.
			}
			korongok[i] = csempek;				//A kih�zott csemp�k az �ppen soron l�v� korongra mennek
			csempek = "\0";						//Null�zza gy�jt� v�ltoz�t
		}
		GKorongListazas(korongok, korongszam, "Korongok tartalma a feltoltes utan:");
		return 0;
	}
}

// 6. KORONGOKON �S ASZTAL K�ZEP�N L�V� CSEMP�K MEGJELEN�T�SE
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
	if (asztal[CSVARIACIOK] == 1)											 //Ha ott van m�g a kezd�j�t�kos jelz� (asztal[5]==1), akkor azt is ki�rja
	{
		cout << "+ a Kezdojatekos jelzo (K)" << endl;
	}
}




int main ()
{

	Game g;						//V�ltoz� a Game strukt�r�val

	int zsak[CSVARIACIOK];		//A zs�k, amiben a csemp�k vannak.
	int dobott[CSVARIACIOK];	//Az eldobott csemp�k ide ker�lnek.
	int kozep[CSVARIACIOK + 1];	//Az asztal k�zep�n l�v� csemp�k + a kezd�j�t�kosjel�l�
	int jsz = JatekosSzamBe();	//A j�t�kosok sz�ma.

	g.KorongSzam = jsz * 2 + 1;							//A korongok sz�ma (a g v�ltoz�ban) egyel t�bb, mint a j�t�kosok sz�m�nak k�tszerese
	KezdoZsakok(g.Zsak, g.Eldobott, g.AsztalKozep);		//A Zs�k, Eldobottak zs�kja �s az Asztal k�zep�nek el�k�sz�t�se
	g.Korongok = new string[g.KorongSzam];				//A Korongok t�mb elk�sz�t�se a j�t�kossz�m alapj�n

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


	//KorongKeszito(jsz); // itt majd vissza kell adnia a t�mb c�m�t!
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









// 5. KORONGOK ELK�SZ�T�SE
int KorongKeszito(string korong[], int ksz)
{
	korong = new string[ksz]; //A korongok t�mb k�sz�t�se a j�t�kossz�mnak megfelel�en
	for (int i = 0; i < ksz; i++)
	{
		korong[i] = "456";
	}
	return 0;



}





