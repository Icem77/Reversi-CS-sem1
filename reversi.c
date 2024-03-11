/***************************************************************************************************
    AUTOR: Szymon Kruk

    Program umozliwia gre w Reversi dwom graczom.

    Program, w petli:

    - pisze tekst zachety wskazujacy, do ktorego gracza nalezy ruch
    i jakie legalne ruchy moze on wykonac
    - czyta polecenie gracza do ktorego nalezy ruch
    - jeśli wczytal polecenie przerwania gry, konczy prace
    - jeśli wczytal polecenie rezygnacji z ruchu, wraca na poczatek petli
    i przekazuje ruch przeciwnikowi
    - jeśli wczytał polecenie wykonania ruchu, to wykonuje ten ruch i wraca na poczatek petli.

    KONWENCJA:
    Plansza jest reprezentowana przez dwuwymiarowa tablice liczb calkowitych o wymiarze 8x8.
    Ponizej znajduje sie wizualizacja tego jakim indeksom odpowiadaja odpowiednie
    pola na realnej planszy:

    0 1 |
    1 2 |
    2 3 |
    3 4 |
    4 5 |
    5 6 |
    6 7 |
    7 8 |---------------------  
         a  b  c  d  e  f  g  h
         0  1  2  3  4  5  6  7

    Jezeli wartosc danego pola jest rowna '-1' to na tym polu nie ma zadnego piona.
    Wartosc '0' oznacza piona czarnego, a wartosc '1' bialego.

    W naszym programie znajduja sie jedynie dwa odstepstwa od klasycznych regul gry:
    - gracz moze zrezygnowac z ruchu nawet gdy moze wykonac ruch legalny
    - gra nie konczy sie automatycznie gdy zaden z graczy nie ma legalnego ruchu
    
    Poza tym obowiazuja nas klasyczne zasady i terminologia gry Reversi.
    
    WEJSCIE:
    Polecenia graczy zajmujace jeden wiersz.  Wszystkie polecenia mają poprawną postac
    i koncza sie reprezentacja konca wiersza '\n'. Polecenia wykonania ruchu wskazują ruch legalny.
    Po poleceniu przerwania gry jest dowolny tekst, ignorowany przez program.

    - Polecenie przerwania gry ma postac wiersza o tresci '='.
    - Polecenie rezygnacji z ruchu ma postac wiersza o tresci '-'.
    - Polecenie wykonania ruchu ma postac wiersza z nazwa pola,
    czyli cyfre od '1' do '8' poprzedzona mala litera od 'a' do 'h'

    WYJSCIE:
    Na wyjściu programu jest ciąg tekstow zachety. Kazdy z nich zajmuje jeden wiersz
    i konczy sie reprezentacja konca wiersza '\n'.
    Tekst zachęty zaczyna się od wskazania gracza, do ktorego nalezy ruch,
    znak 'C' wskazuje gracza Czarnego, a znak 'B' wskazuje gracza Bialego.
    Reszte tresci tekstu zachety stanowi ciag nazw pol, na ktorych gracz moze polozyc swojego piona.
    Nazwy są uporządkowane leksykograficznie. Kazda nazwa jest poprzedzona spacja.
    Nazwa pola to tak jak w przypadku wejscia cyfra od '1' do '8' poprzedzona mala litera
    od 'a' do 'h'.
***************************************************************************************************/


#include <stdio.h>
#include <stdbool.h>


/**
    'kolumna' - mala litera od 'a' do 'h' oznaczajaca kolumne na realnej planszy

    Zwraca numer kolumny tablicy odpowiadajacy literze 'kolumna' wedlug konwencji.
    W przypadku niepoprawnego wejscia, zostanie zwrocona wartosc '-1'.
*/
int numer_kolumny(char kolumna)
{
    switch (kolumna)
    {
        case 'a':
            return 0;
        case 'b':
            return 1;
        case 'c':
            return 2;
        case 'd':
            return 3;
        case 'e':
            return 4;
        case 'f':
            return 5;
        case 'g':
            return 6;
        case 'h':
            return 7;
        default:
            return -1;
    }
}

/**
    'wiersz' - cyfra od '1' do '8' oznaczajaca kolumne na realnej planszy

    Zwraca numer wiersza tablicy odpowiadajacy cyfrze 'wiersz' wedlug konwencji
*/
int numer_wiersza(char wiersz)
{
    return wiersz - '0' - 1;
}

/**
    'kolumna' - numer kolumny od '0' do '7' w tablicy reprezentujacej plansze

    Zwraca litere odpowiadajaca kolumnie tablicy o numerze 'kolumna' na realnej planszy
    wedlug konwencji. W przypadku nieprawidlowego wejscia zostanie zwrocony pusty znak ' '.
*/
char symbol_kolumny(int kolumna)
{
    switch (kolumna)
    {
        case 0:
            return 'a';
        case 1:
            return 'b';
        case 2:
            return 'c';
        case 3:
            return 'd';
        case 4:
            return 'e';
        case 5:
            return 'f';
        case 6:
            return 'g';
        case 7:
            return 'h';
        default:
            return ' ';
    }
}

/**
    'plansza' - niezainicjalizowana, dwuwymiarowa tablica 8x8 reprezentujaca plansze

    Inicjalizuje dwuwymiarowa tablice 'plansza' 8x8 wartosciami '-1',
    nastepnie na pozycjach odpowiadajacych wedle konwencji
    polom c5 e4 ustawia piony czarne (wartosc 0), a na polach c4 e5
    ustawia piony biale (wartosc 1).
     
*/
void inicjalizuj_plansze(int plansza[8][8])
{   
    // ustawienia wartosci wszystkich pol na '-1'
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            plansza[i][j] = -1;
        }
    }

    // ustawienie pionow graczy na odpowiednich polach
    plansza[4][3] = 0;
    plansza[3][4] = 0;
    plansza[3][3] = 1;
    plansza[4][4] = 1;
}

/**
    FUNKCJE SPRAWDZAJACE LEGALNOSC RUCHU sprawdz_X

    'wiersz' - numer wiersza tablicy reprezentujacej plansze [0 - 7]
    'kolumna' - numer kolumny tablicy reprezentujacej plansze [0 - 7]
    'gracz' - cyfra '0' lub '1' oznaczajaca pion gracza do ktorego nalezy tura
    'plansza' - dwuwymiarowa tablica 8x8 reprezentujaca plansze z aktualnym stanem gry

    Zwraca wartosc logiczna informujaca o legalnosci ruchu gracza 'gracz' ze wzgledu na kierunek
    X podany w nazwie funkcji, od pustego pola 'wiersz' 'kolumna' tablicy 'plansza'.
*/
bool sprawdz_pion_w_dol(int wiersz, int kolumna, int gracz, int plansza[8][8])
{
    int przeciwnik = 1 - gracz;

    int y = wiersz;
    y++;

    // sprawdzenie czy w bezposrednim sasiedztwie pola jest pion przeciwnika
    if (y < 8 && plansza[y][kolumna] == przeciwnik)
    {   
        // przejscie linii pionow przeciwnika
        while (y < 8 && plansza[y][kolumna] == przeciwnik)
        {
            y++;
        }

        // sprawdzenie czy bezposrednio za linia przeciwnika jest pion gracza
        if (y < 8 && plansza[y][kolumna] == gracz)
        {
            return true;
        }
    }

    return false;
}

/**
    Patrz FUNCKJE SPRAWDZAJACE LEGALNOSC RUCHU (linijka 164)
*/
bool sprawdz_pion_w_gore(int wiersz, int kolumna, int gracz, int plansza[8][8])
{   
    int przeciwnik = 1 - gracz;

    int y = wiersz;
    y--;

    // sprawdzenie czy w bezposrednim sasiedztwie pola jest pion przeciwnika
    if (y >= 0 && plansza[y][kolumna] == przeciwnik)
    {   
        // przejscie linii pionow przeciwnika
        while (y >= 0 && plansza[y][kolumna] == przeciwnik)
        {
            y--;
        }

        // sprawdzenie czy bezposrednio za linia przeciwnika jest pion gracza
        if (y >= 0 && plansza[y][kolumna] == gracz)
        {
            return true;
        }
    }

    return false;
}

/**
    Patrz FUNCKJE SPRAWDZAJACE LEGALNOSC RUCHU (linijka 164)
*/
bool sprawdz_poziom_w_prawo(int wiersz, int kolumna, int gracz, int plansza[8][8])
{
    int przeciwnik = 1 - gracz;

    int x = kolumna;
    x++;

    // sprawdzenie czy w bezposrednim sasiedztwie pola jest pion przeciwnika
    if (x < 8 && plansza[wiersz][x] == przeciwnik)
    {   
        // przejscie linii pionow przeciwnika
        while (x < 8 && plansza[wiersz][x] == przeciwnik)
        {
            x++;
        }

        // sprawdzenie czy bezposrednio za linia przeciwnika jest pion gracza
        if (x < 8 && plansza[wiersz][x] == gracz)
        {
            return true;
        }
    }

    return false;

}

/**
    Patrz FUNCKJE SPRAWDZAJACE LEGALNOSC RUCHU (linijka 164)
*/
bool sprawdz_poziom_w_lewo(int wiersz, int kolumna, int gracz, int plansza[8][8])
{   
    int przeciwnik = 1 - gracz;

    int x = kolumna;
    x--;

    // sprawdzenie czy w bezposrednim sasiedztwie pola jest pion przeciwnika
    if (x >= 0 && plansza[wiersz][x] == przeciwnik)
    {   
        // przejscie linii pionow przeciwnika
        while (x >= 0 && plansza[wiersz][x] == przeciwnik)
        {
            x--;
        }

        // sprawdzenie czy bezposrednio za linia przeciwnika jest pion gracza
        if (x >= 0 && plansza[wiersz][x] == gracz)
        {
            return true;
        }
    }

    return false;
}

/**
    Patrz FUNCKJE SPRAWDZAJACE LEGALNOSC RUCHU (linijka 164)
*/
bool sprawdz_skos_w_prawo_w_gore(int wiersz, int kolumna, int gracz, int plansza[8][8])
{   
    int przeciwnik = 1 - gracz;

    int x = kolumna;
    int y = wiersz;

    x++;
    y--;

    // sprawdzenie czy w bezposrednim sasiedztwie pola jest pion przeciwnika
    if (x < 8 && y >= 0 && plansza[y][x] == przeciwnik)
    {   
        // przejscie linii pionow przeciwnika
        while (x < 8 && y >= 0 && plansza[y][x] == przeciwnik)
        {
            x++; 
            y--;
        }

        // sprawdzenie czy bezposrednio za linia przeciwnika jest pion gracza
        if (x < 8 && y >= 0 && plansza[y][x] == gracz)
        {
            return true;
        }
    }

    return false;
}

/**
    Patrz FUNCKJE SPRAWDZAJACE LEGALNOSC RUCHU (linijka 164)
*/
bool sprawdz_skos_w_lewo_w_gore(int wiersz, int kolumna, int gracz, int plansza[8][8])
{
    int przeciwnik = 1 - gracz;

    int x = kolumna;
    int y = wiersz;

    x--;
    y--;

    // sprawdzenie czy w bezposrednim sasiedztwie pola jest pion przeciwnika
    if (x >= 0 && y >= 0 && plansza[y][x] == przeciwnik)
    {   
        // przejscie linii pionow przeciwnika
        while (y >= 0 && x >= 0 && plansza[y][x] == przeciwnik)
        {
            x--;
            y--;
        }

        // sprawdzenie czy bezposrednio za linia przeciwnika jest pion gracza
        if (y >= 0 && x >= 0 && plansza[y][x] == gracz)
        {
            return true;
        }
    }

    return false;
}

/**
    Patrz FUNCKJE SPRAWDZAJACE LEGALNOSC RUCHU (linijka 164)
*/
bool sprawdz_skos_w_lewo_w_dol(int wiersz, int kolumna, int gracz, int plansza[8][8])
{   
    int przeciwnik = 1 - gracz;

    int x = kolumna;
    int y = wiersz;

    x--;
    y++;

    // sprawdzenie czy w bezposrednim sasiedztwie pola jest pion przeciwnika
    if (x >= 0 && y < 8 && plansza[y][x] == przeciwnik)
    {   
        // przejscie linii pionow przeciwnika
        while(x >= 0 && y < 8 && plansza[y][x] == przeciwnik)
        {
            x--;
            y++;
        }

        // sprawdzenie czy bezposrednio za linia przeciwnika jest pion gracza
        if (x >= 0 && y < 8 && plansza[y][x] == gracz)
        {
            return true;
        }
    }

    return false;
}

/**
    Patrz FUNCKJE SPRAWDZAJACE LEGALNOSC RUCHU (linijka 164)
*/
bool sprawdz_skos_w_prawo_w_dol(int wiersz, int kolumna, int gracz, int plansza[8][8])
{   
    int przeciwnik = 1 - gracz;

    int x = kolumna;
    int y = wiersz;

    x++;
    y++;

    // sprawdzenie czy w bezposrednim sasiedztwie pola jest pion przeciwnika
    if (x < 8 && y < 8 && plansza[y][x] == przeciwnik)
    {   
        // przejscie linii pionow przeciwnika
        while (x < 8 && y < 8 && plansza[y][x] == przeciwnik)
        {
            x++;
            y++;
        }

        // sprawdzenie czy bezposrednio za linia przeciwnika jest pion gracza
        if (y < 8 && x < 8 && plansza[y][x] == gracz)
        {
            return true;
        }
    }

    return false;
}

/**
    FUNKCJE WYKONUJACE RUCH zmien_X

    'wiersz' - numer wiersza tablicy reprezentujacej plansze [0 - 7]
    'kolumna' - numer kolumny tablicy reprezentujacej plansze [0 - 7]
    'gracz' - cyfra '0' lub '1' oznaczajaca pion gracza do ktorego nalezy tura
    'plansza' - dwuwymiarowa tablica 8x8 reprezentujaca plansze z aktualnym stanem gry

    Wywoływana jezeli ruch dla gracza 'gracz' w kierunku X podanym w nazwie funkcji jest legalny
    zmienia kolor wszystkich pionow przeciwnika znajdujacych sie pomiedzy wczesniej wspomnianym polem,
    a nastepnym pionem gracza 'gracz' w kierunku X.
*/
void zmien_pion_w_gore(int wiersz, int kolumna, int gracz, int plansza[8][8])
{   
    int przeciwnik = 1 - gracz;

    int y = wiersz;
    y--;

    // przejscie przez linie pionow przeciwnika i jednoczesna zmiana ich koloru
    while (y >= 0 && plansza[y][kolumna] == przeciwnik)
    {
        plansza[y][kolumna] = gracz;
        y--;
    }
}

/**
    Patrz FUNCKJE WYKONUJACE LEGALNY RUCH (linijka 365)
*/
void zmien_pion_w_dol(int wiersz, int kolumna, int gracz, int plansza[8][8])
{   
    int przeciwnik = 1 - gracz;

    int y = wiersz;
    y++;

    // przejscie przez linie pionow przeciwnika i jednoczesna zmiana ich koloru
    while (y < 8 && plansza[y][kolumna] == przeciwnik)
    {
        plansza[y][kolumna] = gracz;
        y++;
    }
}

/**
    Patrz FUNCKJE WYKONUJACE LEGALNY RUCH (linijka 365)
*/
void zmien_poziom_w_prawo(int wiersz, int kolumna, int gracz, int plansza[8][8])
{
    int przeciwnik = 1 - gracz;

    int x = kolumna;
    x++;

    // przejscie przez linie pionow przeciwnika i jednoczesna zmiana ich koloru
    while (x < 8 && plansza[wiersz][x] == przeciwnik)
    {
        plansza[wiersz][x] = gracz;
        x++;
    }
}

/**
    Patrz FUNCKJE WYKONUJACE LEGALNY RUCH (linijka 365)
*/
void zmien_poziom_w_lewo(int wiersz, int kolumna, int gracz, int plansza[8][8])
{
    int przeciwnik = 1 - gracz;

    int x = kolumna;
    x--;

    // przejscie przez linie pionow przeciwnika i jednoczesna zmiana ich koloru
    while (x >= 0 && plansza[wiersz][x] == przeciwnik)
    {
        plansza[wiersz][x] = gracz;
        x--;
    }
}

/**
    Patrz FUNCKJE WYKONUJACE LEGALNY RUCH (linijka 365)
*/
void zmien_skos_w_lewo_w_gore(int wiersz, int kolumna, int gracz, int plansza[8][8])
{
    int przeciwnik = 1 - gracz;

    int x = kolumna;
    int y = wiersz;

    y--;
    x--;

    // przejscie przez linie pionow przeciwnika i jednoczesna zmiana ich koloru
    while (x >= 0 && y >= 0 && plansza[y][x] == przeciwnik)
    {
        plansza[y][x] = gracz;
        y--;
        x--;
    }
}

/**
    Patrz FUNCKJE WYKONUJACE LEGALNY RUCH (linijka 365)
*/
void zmien_skos_w_lewo_w_dol(int wiersz, int kolumna, int gracz, int plansza[8][8])
{
    int przeciwnik = 1 - gracz;

    int x = kolumna;
    int y = wiersz;

    y++;
    x--;

    // przejscie przez linie pionow przeciwnika i jednoczesna zmiana ich koloru
    while (x >= 0 && y < 8 && plansza[y][x] == przeciwnik)
    {
        plansza[y][x] = gracz;
        y++;
        x--;
    }
}

/**
    Patrz FUNCKJE WYKONUJACE LEGALNY RUCH (linijka 365)
*/
void zmien_skos_w_prawo_w_gore(int wiersz, int kolumna, int gracz, int plansza[8][8])
{
    int przeciwnik = 1 - gracz;

    int x = kolumna;
    int y = wiersz;

    y--;
    x++;

    // przejscie przez linie pionow przeciwnika i jednoczesna zmiana ich koloru
    while (x < 8 && y >= 0 && plansza[y][x] == przeciwnik)
    {
        plansza[y][x] = gracz;
        y--;
        x++;
    }
}

/**
    Patrz FUNCKJE WYKONUJACE LEGALNY RUCH (linijka 365)
*/
void zmien_skos_w_prawo_w_dol(int wiersz, int kolumna, int gracz, int plansza[8][8])
{
    int przeciwnik = 1 - gracz;

    int x = kolumna;
    int y = wiersz;

    y++;
    x++;

    // przejscie przez linie pionow przeciwnika i jednoczesna zmiana ich koloru
    while (x < 8 && y < 8 && plansza[y][x] == przeciwnik)
    {
        plansza[y][x] = gracz;
        y++;
        x++;
    }
}

/**
    'plansza' - dwuwymiarowa tablica 8x8 reprezentujaca plansze z aktualnym stanem gry
    'gracz' - cyfra '0' lub '1' oznaczajaca pion gracza do ktorego nalezy tura

    Wyswietla legalne ruchy gracza 'gracz' w porzadku leksykogfraficznym,
    nazwa kazdego pola jest poprzedzona spacja
*/
void pokaz_mozliwe_ruchy(int plansza[8][8], int gracz)
{   
    // przejscie w kolejnosci leksykograficznej przez pola planszy
    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 8; i++)
        {   
            // sprawdzenie czy pole jest puste
            if (plansza[i][j] == -1)
            {   
                // wyswietlenie nazwy pola poprzedzonej spacja jesli ruch jest legalny
                if (sprawdz_pion_w_gore(i, j, gracz, plansza) ||
                    sprawdz_pion_w_dol(i, j, gracz, plansza) ||
                    sprawdz_poziom_w_lewo(i, j, gracz, plansza) ||
                    sprawdz_poziom_w_prawo(i, j, gracz, plansza) ||
                    sprawdz_skos_w_lewo_w_dol(i, j, gracz, plansza) ||
                    sprawdz_skos_w_lewo_w_gore(i, j, gracz, plansza) ||
                    sprawdz_skos_w_prawo_w_dol(i, j, gracz, plansza) ||
                    sprawdz_skos_w_prawo_w_gore(i, j, gracz, plansza))
                {
                    printf(" %c%d", symbol_kolumny(j), i + 1);
                }
            }
        }
    }
}

/**
    'wiersz' - numer wiersza tablicy reprezentujacej plansze [0 - 7]
    'kolumna' - numer kolumny tablicy reprezentujacej plansze [0 - 7]
    'gracz' - cyfra '0' lub '1' oznaczajaca pion gracza do ktorego nalezy tura
    'plansza' - dwuwymiarowa tablica 8x8 reprezentujaca plansze z aktualnym stanem gry

    Ustawia pion gracza 'gracz' na pustym polu 'wiersz' 'kolumna' w tablicy 'plansza'
    a nastepnie zmienia piony przeciwnika w kierunkach ze wzgledu na ktore
    ruch zostal uznany za legalny.
*/
void wykonaj_ruch(int wiersz, int kolumna, int gracz, int plansza[8][8])
{   
    // ustawienie piona gracza na podanym polu
    plansza[wiersz][kolumna] = gracz;

    // sprawdzenie i ewentualna zmiana kazdego z kierunkow
    if (sprawdz_pion_w_gore(wiersz, kolumna, gracz, plansza))
    {
        zmien_pion_w_gore(wiersz, kolumna, gracz, plansza);
    }
    if (sprawdz_pion_w_dol(wiersz, kolumna, gracz, plansza))
    {
        zmien_pion_w_dol(wiersz, kolumna, gracz, plansza);
    }
    if (sprawdz_poziom_w_prawo(wiersz, kolumna, gracz, plansza))
    {
        zmien_poziom_w_prawo(wiersz, kolumna, gracz, plansza);
    }
    if (sprawdz_poziom_w_lewo(wiersz, kolumna, gracz, plansza))
    {
        zmien_poziom_w_lewo(wiersz, kolumna, gracz, plansza);
    }
    if (sprawdz_skos_w_lewo_w_gore(wiersz, kolumna, gracz, plansza))
    {
        zmien_skos_w_lewo_w_gore(wiersz, kolumna, gracz, plansza);
    }
    if (sprawdz_skos_w_prawo_w_gore(wiersz, kolumna, gracz, plansza))
    {
        zmien_skos_w_prawo_w_gore(wiersz, kolumna, gracz, plansza);
    }
    if (sprawdz_skos_w_lewo_w_dol(wiersz, kolumna, gracz, plansza))
    {
        zmien_skos_w_lewo_w_dol(wiersz, kolumna, gracz, plansza);
    }
    if (sprawdz_skos_w_prawo_w_dol(wiersz, kolumna, gracz, plansza))
    {
        zmien_skos_w_prawo_w_dol(wiersz, kolumna, gracz, plansza);
    }
}

int main(void)
{   
    // zmienna logiczna informujaca o kontynuacji gry
    bool gra_trwa = true;
    // informuje do ktorego gracza nalezy ruch
    int tura = 0;

    // symbole graczy do ktorych latwo mozna sie odwolac z uzyciem 'gracze['tura']'
    char gracze[] = {'C', 'B'};

    int plansza[8][8];
    inicjalizuj_plansze(plansza);

    // liczba odpowiadajaca pierwszemu wprowadzonemu znakowi wedlug systemu ASCII
    int wiersz_ascii;
    // liczba odpowiadajaca drugiemu ewentualnie wprowadzonemu znakowi wedlug systemu ASCII
    int kolumna_ascii;
    // pierwsza wspolrzedna pola w tablicy 'plansza'
    int wiersz;
    // druga wspolrzedna pola w tablicy 'plansza'
    int kolumna;
    
    while (gra_trwa)
    {   
        // wyswietlenie tekstu zachety
        printf("%c", gracze[tura]);
        pokaz_mozliwe_ruchy(plansza, tura);
        printf("\n");

        // zczytanie pierwszego znaku
        kolumna_ascii = getchar();
        
        switch (kolumna_ascii)
        {
            case '-': // rezygnacja z ruchu
                break;
            case '=': // przerwanie gry
                gra_trwa = false;
                break;
            default: // zczytanie drugiego symobolu i wykonanie legalnego ruchu

                /* 
                    Konwersja zczytanego symbolu kolumny na odpowiadajacy mu numer
                    kolumny w tablicy 'plansza' reprezentujacej plansze
                */
                kolumna = numer_kolumny((char) kolumna_ascii);
                

                /*
                    Zczytanie i konwersja numeru wiersza na odpowiadajacy mu numer
                    wiersza w tablicy 'plansza' reprezentujacej plansze
                */
                wiersz_ascii = getchar();
                wiersz = numer_wiersza((char) wiersz_ascii);

                wykonaj_ruch(wiersz, kolumna, tura, plansza);
                
                break;
        }
        
        // zczytanie znaku reprezentujacego koniec wiersza
        getchar();

        // przekazanie ruchu nastepnemu graczowi
        tura = 1 - tura;
    }
    
    return 0;
}