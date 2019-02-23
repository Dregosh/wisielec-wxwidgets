/////////////////////////////////////////////////////////////////////////////
// Name:        main.h
// Purpose:     Rozdzial 6 - Wisielec
// Author:      MateoSan
// Created:     2019-02-12
// Copyright:   (c) 2018 by MateoSan
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _MAIN_H_
#define _MAIN_H_

#include <wx/spinctrl.h>

class MyApp : public wxApp
{
public:
	virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
	MyFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style);
	~MyFrame();

	//FUNKCJE ZDARZE�
	void OnMenuEvent(wxCommandEvent& event);		//Obsluga Zdarzen Menu Glownego
	void CloseApp(wxCloseEvent& event);				//Realizacja pytania o wyjscie z programu (zdarzenie wxEVT_CLOSE_WINDOW)
	void OnWxSite(wxCommandEvent& event);			//Obsluga zdarzen przycisku wxWidgets z Paska Narzedzi
	void OnLogCopySave(wxCommandEvent& event);		//Obsluga zdarzenia przyciskow Logu
	void OnKey(wxKeyEvent& event);					//Funkcja Zdarzen z Klawiatury
	void OnTimer(wxTimerEvent& event);				//Funkcja obs�uguj�ca up�yw czasu

	//FUNKCJE GRY
	void InitGame();				//Inicjowanie zmiennych i kontrolek
	void Game(int gameState);		//Sterowanie przebiegiem gry
	void EndGame();					//Zako�czenie gry
	void ShowWinnerInfo();			//Wy�wietlenie info o zwyci�zcy
	void SetWord(wxString s);		//Wypisywanie has�a w oknie gry

	//FUNKCJE LOGU
	void Log(wxString s);							// Wy�wietlenie zwyk�ego tekstu
	void LogF(wxString s, int val);					// Wy�wietlenie tekstu formatowanego
	void LogC(wxString s, const wxColour&c);		// Wy�wietlenie tekstu w kolorze c

	//RYSOWANIE SZUBIENICY
	void DrawGallows(int step);
	void DrawGallows2(int step);

	enum
	{
		INIT_PLAYER = 1,
		ANALYSIS,
		CHANGE_PLAYER
	};

private:
	wxMenuBar			*mb;				//wskaznik Glownego Paska Menu
	wxMenu				*mFile;				//wskaznik submenu Plik
	wxMenu				*mHelp;				//wskaznik submenu Pomoc
	wxToolBar			*tb;				//wskaznik Paska Narzedzi
	wxStatusBar			*sb;				//wskaznik dolnego Paska Stanu
	wxMenu				*wxSiteMenu;		//wskaznik podmenu przycisku wxWidgets w Pasku Narzedzi
	wxPanel				*panel;				//wskaznik Panelu Glownego dla Obszatu Klienta

	wxBoxSizer			*mainSizer;			//Sizer Glowny
	wxBoxSizer			*rightSizer;		//Kolumna Prawa
	wxStaticBoxSizer	*leftSizer;			//Kolumna Lewa
	wxStaticBoxSizer	*infoSizer;			//Informacje
	wxStaticBoxSizer	*logSizer;			//Log
	wxStaticBoxSizer	*propSizer;			//Narzedzia
	wxStaticBox			*leftSb;
	wxStaticBox			*infoSb;
	wxStaticBox			*logSb;
	wxStaticBox			*propSb;

	wxStaticBitmap		*gallows;			//Szubienica graficznie
	wxStaticText		*word;				//odgadywane slowo
	wxStaticText		*round;				//Runda
	wxStaticText		*roundLabel;		//Runda - etykieta
	wxStaticText		*points;			//Punkty
	wxStaticText		*pointsLabel;		//Punkty - etykieta
	wxStaticText		*time;				//Stoper
	wxStaticText		*timeLabel;			//Stoper - etykieta
	wxStaticText		*player;			//Aktualny gracz
	wxStaticText		*playerLabel;		//Aktualny gracz - etykieta
	wxStaticText		*levelLabel;		//Poziom trudnosci - etykieta
	wxStaticText		*roundsLabel;		//Liczba rund - etykieta

	wxTextCtrl			*log;				//Okno Logu zdarzen
	wxButton			*copyLog;
	wxButton			*saveLog;
	wxComboBox			*level;				//Kontrolka wyboru poziomu trudnosci
	wxSpinCtrl			*rounds;			//Wirowka wyboru ilosci rund

	int					currPlayer;			//Aktualny gracz
	int					player1pts;			//Punkty gracza 1
	int					player2pts;			//Punkty gracza 2
	int					error;				//Numer kolejnego b��du
	int					errorMax;			//Maksymalna liczba b��d�w na ruch
	int					roundNumber;		//Numer bie��cej rundy
	int					difficulty;			//Poziom trudno�ci
	int					gameTime;			//Czas na ruch
	wxString			currWord;			//S�owo do odgadni�cia (do por�wna�)
	int					letters;			//Liczba liter do odgadni�cia
	wxString			letter;				//Aktualny wprowadzony znak
	wxArrayString		words;				//Tablica s��w do odgadni�cia
	bool				game;				//Czy gra w toku? (do sterowania niekt�rymi akcjami)

	wxTimer				timer;				//Stoper


	DECLARE_EVENT_TABLE()	//Deklaracja Tablicy Zdarzen (makro)
};

#endif
