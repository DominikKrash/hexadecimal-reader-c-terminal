//Visual Studio 2015
//Libraries
#include <iostream>
#include <time.h>
#include <math.h>
#include <fstream>
#include <stdlib.h>
using namespace std;
///////////
//Constants
///////////
//Tryby wyswietlania pliku
const bool TEXT = true;
const bool HEXADECIMAL = false;
//miejsce na wczytywanie nazw plikow
const int BUFOR_SIZE = 80;
//czas wyswietlania raportow (milisekundy: 1000 = 1s)  
const int WAIT_TIME = 1200;
//w ramach czytelnosci programu ;)
const int FIRST_PAGE = 1;
//system plikowy w jakim pracujemy ;)
const int NUM_MODE = 16;
//to jest dlugosc linijki
const int LINE_LENGTH =  40;
// tyle literek wyswietla 1 strona, dokladnie ilosc lini w tekscie
const int PAGE_LENGTH = 6 * LINE_LENGTH;
//przerwy w trybie szesnastkowym
const int HEX_SPACE = 3;
///////////
//Structs//
///////////
struct Book {
	//zmienne///////
	////////////////
	int page; //obecna strona 
	long int pointer; //nasz celownik, wskazuje miejsce od ktorego wczytujemy tekst do tablicy
	long int length; //sprawdzamy calkowita ilosc liter wczytanego tekstu
	int maxPage; //max ilosc stron
	fstream source;
	//funkcje skladowe
	//////////////////
	//konstruktor/////
	Book() {
		this->page = FIRST_PAGE;
		this->pointer = 0;
	}
	~Book() {
		if (source.is_open()) source.close();
	}
	/////////////////////
	void SetFileLength() {
		if (this->source.is_open()) {
			char temp;
			long int dl = 0;
			while (this->source.get(temp)) {
				dl++;
			}
			this->length = dl;
		}
	}
	////////////////////
	void SetMaxPage() {
		if (this->source.is_open()) {
			this->maxPage = this->length / PAGE_LENGTH;
			if (this->length % PAGE_LENGTH != 0) {
				maxPage++;
			}
		}
	}
	////////////////////
	bool IsOpened() {
		if (!(this->source.is_open())) {
			cout << "No content" << endl;
			_sleep(WAIT_TIME);
			return false;
		}
		else return true;
	}
	void Initiate() {
		if (this->source.is_open()) {
			SetFileLength();
			SetMaxPage();
		}
	}
	void Reset() {
		this->page = FIRST_PAGE;
		this->pointer = 0;
		Initiate();
	}
};
///////////
//Functions
///////////
void SpaceMaker(int howMany) {
	while (howMany--) {
		cout << " ";
	}
}
//Rysuje menu wyboru
void Menu(bool mode,Book &book){
	cout << "Dominik Kraszkiewicz TEXT READER" << endl;
	cout << "++++++++++++++Main Menu+++++++++++++" << endl;
	cout << "1:Open File" << endl;
	cout << "2:Close File" << endl;
	cout << "3:Reading Mode : TEXT" << endl;
	cout << "4:Reading Mode : HEXADECIMAL + TEXT" << endl;
	cout << "5:Page Down" << endl;
	cout << "6:Page Up" << endl;
	cout << "7:Quit" << endl;
	cout << "------------------------------------" << endl;
	if (mode) cout << "Text Mode:" << endl;
	else cout << "Text + Hexadecimal Mode" << endl;
	if (!book.source.is_open())cout << "Brak pliku to odczytu!!!"<<endl;
}
//wczytujemy plik, ustawiamy strukture 
void FileOpen(Book &book) {
	system("cls");
	cout << "Podaje nazwe pliku" << endl;
	char name[BUFOR_SIZE];
	cin >> name;
	book.source.open(name, ios::in);
	if (!book.source) {
		cout << "Niepowodzenie ;(" << endl;
		_sleep(WAIT_TIME);
		return;
	}
	else {
		book.Reset();
		system("cls");
		cout << "Powodzenie !";
		_sleep(WAIT_TIME);
		return;
	}
}
//zamykamy plik 
void FileClose(Book &book) {
	book.source.clear();
	book.source.close();
}
//osbluga stron
void PageDown(Book &book){
	if (!book.IsOpened())return;
	if(book.page < book.maxPage){
		book.pointer += PAGE_LENGTH;
		book.page++;
	}
	else {
		cout << "No more content" << endl;
		_sleep(WAIT_TIME);
	}
}
void PageUp(Book &book) {
	if (book.IsOpened() == false)return;
	if (book.page >FIRST_PAGE) {
		book.pointer -= PAGE_LENGTH;
		book.page--;
	}
	else {
		cout << "No more content" << endl;
		_sleep(WAIT_TIME);
	}
}
//sterowanie
void Input(char input,bool &run,bool &mode,Book &book) {
	switch (input) {
	case '1':
		FileOpen(book);
		break;
	case '2':
		FileClose(book);
		break;
	case '3':
		mode = TEXT;
		break;
	case'4':
		mode = HEXADECIMAL;
		break;
	case'5':
		PageDown(book);
		break;
	case'6':
		PageUp(book);
		break;
	case '7':
		run = !run;
		break;
	default:
		cout << "Nieprawidlowa komenda!" << endl;
			_sleep(WAIT_TIME);
			break;
	}
}
//stopka naszej strony 
inline void Footer(Book &book) {
	cout <<endl<< "-------------------------------Strona:" << book.page << "/" << book.maxPage << endl;
}
//wrzucamy do naszej tablicy zawartosc pliku tekstowego
void FillRender(char stream[],Book &book) {
	int i = 0;
	book.source.clear();
	book.source.seekg(book.pointer);
	while (i < PAGE_LENGTH) {
		if (book.source.eof() == true) {
			i--;
			break;
		}
		book.source.get(stream[i]);
		i++;
	}
	stream[i] = '\0';
}
void ModeUpperContent(){
	char bufor[BUFOR_SIZE];
	cout << "Adres"; SpaceMaker(5);
	for (int i = 0; i < NUM_MODE; i++) {
		SpaceMaker(2); cout<< itoa(i, bufor, NUM_MODE);
	}
	SpaceMaker(2); cout<< "Tekst" << endl;

}
//wypisujemy teskst w trybie normalnym
void RenderText(Book &book,char stream[]) {
	int i = 0;
	int k = 0;
	while (stream[i] != '\0') {
		if (k%LINE_LENGTH == 0)  cout << endl; 
		cout << stream[i];
		if (stream[i] == '\n') k = 0;
		i++;
		k++;
	}
	Footer(book);
}
//zwieksza nam numer adresu szesnastkowgo i 
//zwraca true jak powinnismy zwiekszyc nastepny adres(przejscie z f na 0)
bool Replace(char tab[],int where) {
	if (tab[where] < (int)'9') { //jesli mniejsze niz 9 to zwiekszamy
		tab[where]++;
		return false;
	}
	else if (tab[where] == (int)'9') {
		tab[where] = 'a'; //hjesli 9 to wrzucamy a czyli 10
		return false;
	}
	else if (tab[where] < (int)'f') { //
		tab[where]++;
		return false;
	}
	else {
		tab[where] = '0';
		return true;
	}
}
void AdressIncrease(char tab[]) {
	int where = strlen(tab) - 2;
	int i = 0;
	while (Replace(tab, where - i) == true) {
		i++;
	}
	
}
//wypisujemy w trybie szesnastkowo-normalnym
void RenderHex(Book &book, char stream[]) {
	char bufor[BUFOR_SIZE];
	ModeUpperContent();
	int i = 0;
	char hexAdress[] = { "00000000" };
	while (stream[i] != '\0') {
		if (i%NUM_MODE == 0) { //przejscie do nastepnej liniii
			cout << endl << hexAdress; SpaceMaker(HEX_SPACE);
			AdressIncrease(hexAdress);
		}
		if(stream[i] != '\n')cout << itoa(stream[i], bufor, NUM_MODE)<< " "; //wypisywanie	szesnastkowo
		if ((i + 1) % NUM_MODE == 0 && i > 0 && (book.page != book.maxPage || stream[i + 1] != '\0')) { //pisanie stron textowo
			int k = (i+1) - NUM_MODE;
			while (k <= i) {
				cout << stream[k];
				k++;
			}
		}
		i++;
	}
	//Wypisanie ostatniej strony jesli zostalo cos
	i--;
	if ((i) % NUM_MODE != 0 && book.page == book.maxPage) {
		int k = (i % NUM_MODE);
		SpaceMaker((NUM_MODE - k+1)*HEX_SPACE);
		k = i - k;
		while (k<=i) {
			cout << stream[k];
			k++;
		}
	}
	Footer(book);
}
//obsluga odczytu plikow
void RedirectRender(bool mode,Book &book) {
	char stream[PAGE_LENGTH + 1];
	if (book.source.is_open()) {
		FillRender(stream, book);
		if (mode == TEXT) RenderText(book,stream);
		else RenderHex(book,stream);
	}
	else return;
}
//logika programu
void Program() {
	bool mode = TEXT; //tryb standardowo ustawiamy na tektowy
	bool isRunning = true; //czas dzialania Programu
	char choice; //zmienna do sterowania
	Book book; //plik do odczytu
	do {
		Menu(mode,book);
		RedirectRender(mode,book);
		cin >> choice;
		Input(choice, isRunning, mode,book);
		system("cls");
	} while (isRunning);
}
///////////////
//////main/////
///////////////
int main() {
	Program();
}