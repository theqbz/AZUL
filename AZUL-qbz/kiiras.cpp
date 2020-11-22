// 5/x (egyszerû) KORONGOKON ÉS ASZTAL KÖZEPÉN LÉVÕ CSEMPÉK MEGJELENÍTÉSE
void KorongAsztalMegj0(string korong[], int asztal[], int zsak[], int dobott[], int ksz)
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
			<< char(65 + i) << ": " << zsak[i] << "\t\t"
			<< char(65 + i) << ": " << dobott[i] << endl;
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
