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

	//FUNKCJE ZDARZEÑ
	void OnMenuEvent(wxCommandEvent& event);		//Obsluga Zdarzen Menu Glownego
	void CloseApp(wxCloseEvent& event);				//Realizacja pytania o wyjscie z programu (zdarzenie wxEVT_CLOSE_WINDOW)
	void OnWxSite(wxCommandEvent& event);			//Obsluga zdarzen przycisku wxWidgets z Paska Narzedzi
	void OnLogCopySave(wxCommandEvent& event);		//Obsluga zdarzenia przyciskow Logu
	void OnKey(wxKeyEvent& event);					//Funkcja Zdarzen z Klawiatury
	void OnTimer(wxTimerEvent& event);				//Funkcja obs³uguj¹ca up³yw czasu

	//FUNKCJE GRY
	void InitGame();				//Inicjowanie zmiennych i kontrolek
	void Game(int gameState);		//Sterowanie przebiegiem gry
	void EndGame();					//Zakoñczenie gry
	void ShowWinnerInfo();			//Wyœwietlenie info o zwyciêzcy
	void SetWord(wxString s);		//Wypisywanie has³a w oknie gry

	//FUNKCJE LOGU
	void Log(wxString s);							// Wyœwietlenie zwyk³ego tekstu
	void LogF(wxString s, int val);					// Wyœwietlenie tekstu formatowanego
	void LogC(wxString s, const wxColour&c);		// Wyœwietlenie tekstu w kolorze c

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
	int					error;				//Numer kolejnego b³êdu
	int					errorMax;			//Maksymalna liczba b³êdów na ruch
	int					roundNumber;		//Numer bie¿¹cej rundy
	int					difficulty;			//Poziom trudnoœci
	int					gameTime;			//Czas na ruch
	wxString			currWord;			//S³owo do odgadniêcia (do porównañ)
	int					letters;			//Liczba liter do odgadniêcia
	wxString			letter;				//Aktualny wprowadzony znak
	wxArrayString		words;				//Tablica s³ów do odgadniêcia
	bool				game;				//Czy gra w toku? (do sterowania niektórymi akcjami)

	wxTimer				timer;				//Stoper


	DECLARE_EVENT_TABLE()	//Deklaracja Tablicy Zdarzen (makro)
};

#endif
