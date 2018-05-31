#include <fstream>
#include <iostream> 

using namespace std;


#define ILOSC 1000

double licz_ema(double* tab, int n, int x) //n = dla ilu elementow liczymy macd
{											//x = od ktorego elementu liczymy macd
	double mianownik = 1;
	double licznik = tab[x]; //p0

	if (x < n)
		n = x; //dla poczatkowych wartosci liczymy z tylu danych ile mo¿emy
	double alfa = 2.0 / (n - 1.0);

	if (n - 1.0 == 0)

		alfa = 0;  //w momencie gdy dzielimy przez zero alfa to po prostu zero

	for (int i = 1; i <= n; i++)
	{
		licznik += (pow(1 - alfa, i)*tab[x - i]);
		mianownik += pow(1 - alfa, i);
	}
	return (licznik / mianownik);
}

void bot(double *dane, double* macd, double *signal)
{
	double zysk = 0;
	double gotowka;
	long long int iloscAkcji = 1000;
	int ileKupowac = 1000;
	bool pierwszaAkcja = true;
	for (int i = 1; i < ILOSC; i++)
	{
		if (macd[i] < signal[i] && macd[i - 1] >= signal[i - 1]) //sprzedajemy
		{
			zysk += iloscAkcji * dane[i];
			iloscAkcji = 0;
			pierwszaAkcja = false;
		}
		else if (macd[i] > signal[i] && macd[i - 1] <= signal[i - 1] && pierwszaAkcja == false) //kupujemy
		{
			ileKupowac = zysk / dane[i]; //
			zysk -= ileKupowac * dane[i];
			iloscAkcji = ileKupowac;

		}
	}
	gotowka = 1000 * dane[0];
	cout << "Gotowka na start: " << gotowka << endl;
	zysk += iloscAkcji * dane[999];
	cout << "Gotowka po wakacjach: " << zysk << endl;
	cout << "Ilosc akcji na koniec wakacji: " << iloscAkcji << endl;

	zysk = (zysk - gotowka) / gotowka; //stosunek teraz do wtedy
	cout << "Zysk w porcentach: " << zysk * 100 << endl;
}
int main(int argc, char **argv)
{

	double dane[1000];
	double macd[1000];
	double signal[1000];
	ifstream in;
	ofstream out;
	string zapis = "out.txt";
	
	in.open(argv[1]);
	out.open(zapis);

	for (int i = 0; i < ILOSC; i++)
	{
		in >> dane[i];
		macd[i] = (licz_ema(dane, 12, i) - licz_ema(dane, 26, i));
		out << macd[i];


		signal[i] = licz_ema(macd, 9, i);
		out << " " << signal[i] << endl;
	}

	
	bot(dane, macd, signal);
	getchar();

	out.close();
	in.close();

	return 0;
}


/*void ZamienKropke(string nazwa)
{
fstream przecinki;
przecinki.open(nazwa);
for (int i = 0; i < ILOSC; i++)
{
char tab[50];
przecinki >> tab;
for (int j = 0; j < 50; j++)
{
if (tab[j] == '.')
{
tab[j] = ',';
}
}
przecinki << tab << endl;
}
przecinki.close();
}*/
