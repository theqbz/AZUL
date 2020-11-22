// 5/x (egyszer�) KORONGOKON �S ASZTAL K�ZEP�N L�V� CSEMP�K MEGJELEN�T�SE
void KorongAsztalMegj0(string korong[], int asztal[], int zsak[], int dobott[], int ksz)
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
			<< char(65 + i) << ": " << zsak[i] << "\t\t"
			<< char(65 + i) << ": " << dobott[i] << endl;
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
