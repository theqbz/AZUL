#include"iostream"
#include"iomanip"
#include"cmath"
#include"cstdlib"
#include"ctime"

using namespace std;



void GZsakListazas(int z[], int d[], int a[], int cv, string szoveg) //z[] zs�k-, d, dobott-, a asztal t�mb, cv a csempevari�ci�k, szoveg a ki�rand� sz�veg
{
	cout << "\n\n* * * HIBAKERESES:\n* * * " << szoveg << endl;
	cout << "* * * Zsak:" << endl;
	for (int i = 0; i < cv; i++)
	{
		cout << "* * * " << i << ". zsebben, " << char(65 + i) << " csempe: " << z[i] << endl;
	}
	cout << endl;
	cout << "* * * Asztal kozepe:" << endl;
	for (int i = 0; i < cv+1; i++)
	{
		cout << "* * * " << i << ". zsebben, " << char(65 + i) << " csempe: " << a[i] << endl;
	}
	cout << endl;
	cout << "* * * Dobottak:" << endl;
	for (int i = 0; i < cv; i++)
	{
		cout << "* * * " << i << ". zsebben, " << char(65 + i) << " csempe: " << d[i] << endl;
	}
}

void GKorongListazas(string k[], int ksz, string szoveg) //k[] a korongok t�mb, ksz a korongok sz�ma, szoveg a ki�rand� sz�veg
{
	cout << "\n\n* * * HIBAKERESES:\n* * * " << szoveg << endl;
	for (int i = 0; i < ksz; i++)
	{
		cout << "* * * " << i << ". korong tartalma: " << k[i] << endl;
	}
}

void PNevlista(string n[], int jsz, string szoveg) //Az n[] a Player strukt�r�j� t�mb N�v elemem, jsz a j�t�kosok sz�ma, sz�veg a ki�rand� sz�veg
{
	cout << "\n\n * * * HIBAKERESES:\n* * * " << szoveg << endl;
	for (int i = 0; i < jsz; i++)
	{
		cout << "* * * " << i << ". jatekos neve: " << n[i] << endl;
	}
}



void ZsakListazas(int z[], int cv, string szoveg) //z[] a t�mb, cv a csempevari�ci�k, szoveg a ki�rand� sz�veg
{
	cout << "\n\n* * * HIBAKERESES:\n" << szoveg << endl;
	for (int i = 0; i < cv; i++)
	{
		cout << i << ". zsebben, " << char(65 + i) << " csempe: " << z[i] << endl;
	}
}

void KorongListazas(string k[], int ksz, string szoveg) //k[] a korongok t�mb, ksz a korongok sz�ma, szoveg a ki�rand� sz�veg
{
	cout << "\n\n* * * HIBAKERESES:\n" << szoveg << endl;
	for (int i = 0; i < ksz; i++)
	{
		cout << i << ". korong tartalma: " << k[i] << endl;
	}
}

