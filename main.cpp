/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     Rozdzial 6 - Wisielec
// Author:      MateoSan
// Created:     2019-02-12
// Copyright:   (c) 2018 by MateoSan
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/statline.h>
#endif

#include "main.h"
#include "utils.h"
#include "global_kbd_event.h"

const wxString appName = wxT("Wisielec 1.0.0");

//Implementacja Aplikacji
IMPLEMENT_APP(MyApp)

//Identyfikatory
enum
{
	ID_NEW_GAME = wxID_HIGHEST + 1,
	ID_TERMINATE_GAME,
	ID_CLOSE,
	ID_ABOUT_PROGRAM,
	ID_WX_SITE,
	ID_WX_SITE_FORUM,
	ID_WX_SITE_WIKI,
	ID_LOG_COPY,
	ID_LOG_SAVE,
	ID_TIMER
};

//Tablica Zdarzen
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU_RANGE(ID_NEW_GAME, ID_ABOUT_PROGRAM, MyFrame::OnMenuEvent)	//zakresowa obsluga zdarzen MENU
EVT_CLOSE(MyFrame::CloseApp)			//obsluga konczenia programu
EVT_TOOL_RANGE(ID_WX_SITE, ID_WX_SITE_WIKI, MyFrame::OnWxSite)	//obsluga przycisku wxWidgets z Paska Narzedzi (pozostale przyciski obsluguja te same id co z EVT_MENU_RANGE, poniewaz Marka EVT_MENU i EVT_TOOL mozna stosowac zamiennie). Tu tez mozna by uzyc EVT_MENU zamiast EVT_TOOL
EVT_BUTTON(ID_LOG_COPY, MyFrame::OnLogCopySave)
EVT_BUTTON(ID_LOG_SAVE, MyFrame::OnLogCopySave)
EVT_TIMER(ID_TIMER, MyFrame::OnTimer)
END_EVENT_TABLE()

bool MyApp::OnInit()
{
	wxInitAllImageHandlers();

	MyFrame *frame = new MyFrame(NULL, wxID_ANY, appName, wxDefaultPosition, wxSize(600, 650), wxDEFAULT_FRAME_STYLE);
	frame->Show(true);

	return true;
}

//Konstruktor MyFrame
MyFrame::MyFrame(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
	: wxFrame(parent, id, title, pos, size, style)
{
	//0. Definicje zmiennych lokalnych
	//Czcionki
	wxFont wordFont(36, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Courier New"));	//czcionka do reprezentacji hasla
	wxFont statFont(16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial"));		//czcionka danych statystycznych
	wxFont labelFont(7, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial"));		//czcionka etykiet

	//Kolory
	wxColour myBlue(0, 180, 200);
	wxColour myGreen1(0, 140, 50);
	wxColour myGreen2(0, 160, 50);
	wxColour myGreen3(0, 180, 50);
	wxColour myGreen4(0, 200, 50);	
	wxColour myRed(*wxRED);	//jest to uzycie wxStockGDI (Graphics Device Interface) i jest tozsame z: wxColour(255,0,0) lub wxColour(wxT("RED"))

	//1. Ikona Programu - inicjalizacja i dolaczenie
	wxIcon icon(wxGetAppFile("gfx/icon.ico"), wxBITMAP_TYPE_ICO);
	SetIcon(icon);

	//2. Wywolanie funkcji inicjujacych zmienne wymagajace nadania wartosci na tym etapie pracy programu
	this->SetMinSize(this->GetSize());	//Ustawia minimalna wielkosc okna na taka z jaka jest na poczatku tworzony program przy wywolaniu konstruktora
	game = false;		//Po uruchomieniu programu rozgrywka nie jest rozpoczêta

	//3. Pasek Menu
	mb = new wxMenuBar();

	mFile = new wxMenu();
	mHelp = new wxMenu();

	mFile->Append(ID_NEW_GAME, wxT("&Nowa gra\tCtrl-N"), wxT("Nowa gra"));
	mFile->Append(ID_TERMINATE_GAME, wxT("&Przerwij grê"), wxT("Przerywa grê"));
	mFile->AppendSeparator();
	mFile->Append(ID_CLOSE, wxT("&Zamknij program\tCtrl-X"), wxT("Zamyka program"));

	mHelp->Append(ID_ABOUT_PROGRAM, wxT("&O programie\tF1"), wxT("O programie"));

	mb->Append(mFile, wxT("&Plik"));
	mb->Append(mHelp, wxT("&Pomoc"));

	SetMenuBar(mb);	//dodaje Pasek Menu do okna programu

	//4. Pasek Narzedzi
	tb = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_HORIZONTAL | wxTB_HORZ_TEXT);
	const wxSize toolBarBitmapSize(22, 22);
	tb->SetToolBitmapSize(toolBarBitmapSize);	//dolaczamy do Paska informacje o rozmiarze ikon (22x22 pix to najczesciej tu uzywany)
	//dolaczamy do paska poszczegolne pozycje:
	tb->AddTool(ID_NEW_GAME, wxT("Nowa gra"),
		wxBitmap(wxGetAppFile("gfx/new_game.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL, wxT("Nowa gra"), wxT("Rozpoczêcie nowej gry"));

	//ALTERNATYWNIE taka pozycje "Nowa gra" mozna tez dodac korzystajac z konstruktora wxToolBarToolBase 
	//(uwaga ma 2 dodatkowe argumenty: 1-szy i 7-my argument (wskazniki). I potem dodajac go do paska funkcja AddTool:
	/*wxToolBarToolBase *t1 = new wxToolBarToolBase(NULL, ID_NEW_GAME, wxT("Nowa gra"),
		wxBitmap(wxGetAppFile("gfx/new_game.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL, NULL, wxT("Nowa gra"), wxT("Rozpoczêcie nowej gry"));
	tb->AddTool(t1);
	//Taki obiekt t1 mozna wykorzystac w 1 programie w kilku roznych paskach.
	*/

	tb->AddTool(ID_TERMINATE_GAME, wxT("Przerwij gre"),
		wxBitmap(wxGetAppFile("gfx/stop_game.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL, wxT("Przerwij"), wxT("Przerwanie gry"));
	tb->AddSeparator();
	tb->AddTool(ID_WX_SITE, wxT("wxWidgets"),
		wxBitmap(wxGetAppFile("gfx/wx.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_DROPDOWN, wxT("wxWidgets"), wxT("www.wxwidgets.org"));

	//podmenu przycisku wxWidgets w Pasku Narzedzi:
	wxSiteMenu = new wxMenu();
	wxSiteMenu->Append(ID_WX_SITE, wxT("Strona wxWidgets"), wxT("Strona wxWidgets"));
	wxSiteMenu->Append(ID_WX_SITE_FORUM, wxT("Forum wxWidgets"), wxT("Forum wxWidgets"));
	wxSiteMenu->Append(ID_WX_SITE_WIKI, wxT("wxWiki"), wxT("wxWiki"));
	tb->SetDropdownMenu(ID_WX_SITE, wxSiteMenu);

	tb->AddStretchableSpace();	//dodaje pusta przestrzen "rozpychajaca" obiekty obok i skalujaca sie wraz ze zmiana rozmiaru calego okna
	tb->AddTool(ID_CLOSE, wxT("Zakoñcz"),
		wxBitmap(wxGetAppFile("gfx/close.png"), wxBITMAP_TYPE_PNG),
		wxNullBitmap, wxITEM_NORMAL, wxT("Zakoñcz"), wxT("Zamyka program"));

	tb->SetMargins(3, 3);	//dodajemy marginesy: lewy 3px i prawy 3px (mozna tu tez uzyc typ wxSize)
	tb->Realize();			//buduje pasek wg powyzszych wytycznych
	SetToolBar(tb);			//dodaje pasek do okna MyFrame

	//5. Pasek Statusu (Stanu)
	sb = new wxStatusBar(this); //Utworzenie nowego Paska Stanu wg konstruktora wxStatusBar
	sb->SetFieldsCount(1);
	SetStatusBar(sb);	//dodaje Pasek Stanu do okna MyFrame
	SetStatusText(title, 0);

	//6. Definicja glownego panelu wxPanel
	//Konstruktor wxPanel. Przyjmuje 6 arg., ale 5 z nich ma wart. domyslne, wiec podajemy tylko pierwszy - wsk. rodzica
	panel = new wxPanel(this);

	//7. Definicje Sizerow
	//Ramki
	leftSb = new wxStaticBox(panel, wxID_ANY, wxEmptyString);
	infoSb = new wxStaticBox(panel, wxID_ANY, wxT(" Informacje "));
	logSb = new wxStaticBox(panel, wxID_ANY, wxT(" Przebieg gry "));
	propSb = new wxStaticBox(panel, wxID_ANY, wxT(" Ustawienia "));
	
	//Sizery
	mainSizer = new wxBoxSizer(wxHORIZONTAL);
	rightSizer = new wxBoxSizer(wxVERTICAL);

	leftSizer = new wxStaticBoxSizer(leftSb, wxVERTICAL);
	infoSizer = new wxStaticBoxSizer(infoSb, wxVERTICAL);
	logSizer = new wxStaticBoxSizer(logSb, wxVERTICAL);
	propSizer = new wxStaticBoxSizer(propSb, wxVERTICAL);

	panel->SetSizer(mainSizer);

	//8. Definicje Kontrolek

		//Napisy
	gallows = new wxStaticBitmap(panel, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE);

	word = new wxStaticText(panel, wxID_ANY, wxT("Has³o"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);
	word->SetFont(wordFont);
	word->SetForegroundColour(myBlue);		//zmienia kolor wyswietlanego tekstu (a nie tla)
	round = new wxStaticText(panel, wxID_ANY, wxT("1 z 1"));
	round->SetFont(statFont);
	round->SetForegroundColour(myGreen1);
	points = new wxStaticText(panel, wxID_ANY, wxT("00:00"));
	points->SetFont(statFont);
	points->SetForegroundColour(myGreen2);
	player = new wxStaticText(panel, wxID_ANY, wxT("Gracz 1"));
	player->SetFont(statFont);
	player->SetForegroundColour(myGreen3);
	time = new wxStaticText(panel, wxID_ANY, wxT("000"));
	time->SetFont(statFont);
	time->SetForegroundColour(myGreen4);

	roundLabel = new wxStaticText(panel, wxID_ANY, wxT("Runda"));
	roundLabel->SetFont(labelFont);
	pointsLabel = new wxStaticText(panel, wxID_ANY, wxT("Punkty"));
	pointsLabel->SetFont(labelFont);
	playerLabel = new wxStaticText(panel, wxID_ANY, wxT("Odgaduj¹cy"));
	playerLabel->SetFont(labelFont);
	timeLabel = new wxStaticText(panel, wxID_ANY, wxT("Pozosta³y czas"));
	timeLabel->SetFont(labelFont);
	levelLabel = new wxStaticText(panel, wxID_ANY, wxT("Poziom trudnoœci"));
	levelLabel->SetFont(labelFont);
	roundsLabel = new wxStaticText(panel, wxID_ANY, wxT("Liczba rund"));
	roundsLabel->SetFont(labelFont);

		//Log
	log = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH | wxBORDER_NONE);
	log->SetEditable(false);
	log->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_FRAMEBK));
	log->SetFont(labelFont);

		//Przyciski logu
	wxBoxSizer *btns = new wxBoxSizer(wxHORIZONTAL);
	copyLog = new wxButton(panel, ID_LOG_COPY, wxT("Kopiuj"));
	saveLog = new wxButton(panel, ID_LOG_SAVE, wxT("Zapisz"));

		//Kontrolka Poziomu Trudnosci
	wxArrayString arr;
	arr.Add(wxT("£atwy (120 s, 9 b³.)"));
	arr.Add(wxT("Œredni (60 s, 6 b³.)"));
	arr.Add(wxT("Trudny (30 s, 3 b³.)"));
	level = new wxComboBox(panel, wxID_ANY, arr.Item(0), wxDefaultPosition, wxDefaultSize, arr, wxCB_READONLY);

		//Kontrolka Liczby Rund
	rounds = new wxSpinCtrl(panel, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 10);

	//9. Skladanie GUI (dodawanie kontrolek do sizerow)
	leftSizer->Add(gallows, 1, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 4);
	leftSizer->Add(word, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

	infoSizer->Add(roundLabel, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 4);
	infoSizer->Add(round, 0, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);
	infoSizer->Add(pointsLabel, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 4);
	infoSizer->Add(points, 0, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);
	infoSizer->Add(playerLabel, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 4);
	infoSizer->Add(player, 0, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);
	infoSizer->Add(timeLabel, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 4);
	infoSizer->Add(time, 0, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);

	propSizer->Add(levelLabel, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 4);
	propSizer->Add(level, 0, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);
	propSizer->Add(roundsLabel, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 4);
	propSizer->Add(rounds, 0, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);

	btns->Add(copyLog, 1, wxEXPAND | wxALL, 0);
	btns->AddSpacer(4);
	btns->Add(saveLog, 1, wxEXPAND | wxALL, 0);

	logSizer->Add(log, 1, wxEXPAND | wxALL, 4);
	logSizer->Add(btns, 0, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 4);

	rightSizer->Add(infoSizer, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 4);
	rightSizer->Add(logSizer, 1, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 4);
	rightSizer->Add(propSizer, 0, wxEXPAND | wxALL, 4);
	mainSizer->Add(leftSizer, 2, wxEXPAND | wxALL, 4);
	mainSizer->Add(rightSizer, 1, wxEXPAND | wxALL, 0);

	//10. Funkcje inicjujace kontrolki
	DrawGallows2(9);
	GlobalKeyEvtHandler::Register(this);
	timer.SetOwner(this, ID_TIMER);

	//11. Wywolanie funkcji okreslajacych zachowanie glownego okna aplikacji
	CenterOnScreen();
}

// Destruktor MyFrame
MyFrame::~MyFrame()
{
	GlobalKeyEvtHandler::Unregister(this);
}

//FUNKCJE ZDARZEN

void MyFrame::OnMenuEvent(wxCommandEvent& event)
{
	switch (event.GetId())
	{
	case ID_NEW_GAME:
	{
		wxString tmp;

		//Zainicjowanie zmiennych
		InitGame();

		//Wyswietlenie informacji na pocz¹tku gry:
		this->log->Clear();
		Log(wxT("NOWA GRA"));
		Log(wxT("Poziom: ") + level->GetStringSelection());
		LogF(wxT("Liczba rund: %d"), rounds->GetValue());

		//Wy³¹czenie kontrolek, których nie powinno siê zmieniaæ po rozpoczêciu gry
		level->Disable();
		rounds->Disable();
		tb->EnableTool(ID_NEW_GAME, false);
		mb->Enable(ID_NEW_GAME, false);

		//Wprowadzanie s³ów do odgadniêcia przez obu graczy
		for (int i = 1; i <= 2; i++)
		{
			wxMessageBox(wxString::Format(wxT("Gracz %d wprowadŸ s³owa do odgadniêcia.\nS³owo mo¿e mieæ maksymalnie 10 liter."), i),
											wxT("WprowadŸ s³owa"), wxOK | wxICON_EXCLAMATION);

			for (int j = 1; j <= rounds->GetValue(); j++)
			{
				tmp = wxEmptyString;
				while (tmp.IsEmpty() || !tmp.IsWord() || tmp.Len() > 10)
				{
					tmp = wxGetTextFromUser(
					wxString::Format(wxT("Gracz %d\nS³owo %d z %d"), i, j, rounds->GetValue()),
						wxT("WprowadŸ s³owo"), wxEmptyString, this);
				}
				tmp.MakeLower();	//konwersja s³owa na ma³e litery
				words.Add(tmp);
			}
			LogF(wxT("Gracz %d wprowadzi³ s³owa..."), i);
		}
		Log(wxT("ROZPOCZÊCIE GRY"));
		log->Bind(wxEVT_KEY_UP, &MyFrame::OnKey, this);
		Game(INIT_PLAYER);
		break;
	}
	case ID_TERMINATE_GAME:
	{
		if (game)
		{
			if (wxMessageBox(wxT("Czy rzeczywiœcie chcesz przerwaæ grê?"), wxT("Uwaga!"), wxYES_NO | wxICON_QUESTION) == wxYES)
			{
				LogC(wxT("Gra przerwana!"), *wxRED);
				EndGame();
			}
			else return;
		}
		event.Skip();
		break;
	}
	case ID_CLOSE:
		this->Close(true);
		event.Skip();
		break;
	case ID_ABOUT_PROGRAM:
		wxMessageBox(wxT("Copyright (c) 2019 by Mat"), wxT("O programie..."), wxOK | wxICON_INFORMATION);
		event.Skip();
		break;
	}
}

//Funkcja obslugujaca procedure Zakonczenia Programu
void MyFrame::CloseApp(wxCloseEvent& WXUNUSED(event))
{
	int action = wxMessageBox(wxT("Czy zamkn¹æ program?"), wxT("Pytanie"), wxOK | wxCANCEL | wxICON_QUESTION);
	if (action == wxOK)
	{
		this->Destroy();
	}
	else
	{
		return;
	}
}

void MyFrame::OnWxSite(wxCommandEvent& event)
{
	wxString site = wxEmptyString;
	switch (event.GetId())
	{
	case ID_WX_SITE: site = wxT("https://www.wxwidgets.org"); break;
	case ID_WX_SITE_FORUM: site = wxT("https://forums.wxwidgets.org"); break;
	case ID_WX_SITE_WIKI: site = wxT("https://wiki.wxwidgets.org"); break;
	}
	if (!wxLaunchDefaultBrowser(site))
	{
		wxMessageBox("Nie mogê po³¹czyæ siê z Internetem.", "B³¹d", wxICON_ERROR | wxOK);
	}
	event.Skip();
}

void MyFrame::OnLogCopySave(wxCommandEvent& event)
{
	switch (event.GetId())
	{
	case ID_LOG_COPY:
		log->SelectAll();
		log->Copy();
		log->SelectNone();
		wxMessageBox(wxT("Zawartoœæ logu zosta³a skopiowana do schowka..."));
		break;
	case ID_LOG_SAVE:
		log->SaveFile(wxT("Wisielec_LOG_") + wxDateTime::Now().FormatISODate() + wxT(".log"));
		wxMessageBox(wxT("Log zapisano do pliku..."));
		break;
	}
}

void MyFrame::OnKey(wxKeyEvent& event)
{
	switch (event.GetKeyCode())
	{
	case 65: letter = "a";
		if (event.GetModifiers() == wxMOD_ALTGR) letter = "¹"; break;
	case 66: letter = "b"; break;
	case 67: letter = "c";
		if (event.GetModifiers() == wxMOD_ALTGR) letter = "æ"; break;
	case 68: letter = "d"; break;
	case 69: letter = "e";
		if (event.GetModifiers() == wxMOD_ALTGR) letter = "ê"; break;
	case 70: letter = "f"; break;
	case 71: letter = "g"; break;
	case 72: letter = "h"; break;
	case 73: letter = "i"; break;
	case 74: letter = "j"; break;
	case 75: letter = "k"; break;
	case 76: letter = "l";
		if (event.GetModifiers() == wxMOD_ALTGR) letter = "³"; break;
	case 77: letter = "m"; break;
	case 78: letter = "n";
		if (event.GetModifiers() == wxMOD_ALTGR) letter = "ñ"; break;
	case 79: letter = "o";
		if (event.GetModifiers() == wxMOD_ALTGR) letter = "ó"; break;
	case 80: letter = "p"; break;
	case 81: letter = "q"; break;
	case 82: letter = "r"; break;
	case 83: letter = "s";
		if (event.GetModifiers() == wxMOD_ALTGR) letter = "œ"; break;
	case 84: letter = "t"; break;
	case 85: letter = "u"; break;
	case 86: letter = "v"; break;
	case 87: letter = "w"; break;
	case 88: letter = "x";
		if (event.GetModifiers() == wxMOD_ALTGR) letter = "Ÿ"; break;
	case 89: letter = "y"; break;
	case 90: letter = "z";
		if (event.GetModifiers() == wxMOD_ALTGR) letter = "¿"; break;
	default: return; break;
	}
	event.Skip();
	Game(ANALYSIS);	//Wykonanie funkcji steruj¹cej gr¹: (ANALYSIS = 2) czyli nast¹pi analiza wprowadzonej litery
}

void MyFrame::OnTimer(wxTimerEvent& WXUNUSED(event))
{
	gameTime--;
	time->SetLabel(wxString::Format(wxT("%d"), gameTime));
	if (gameTime == 0)
	{
		Log(wxT("Czas up³yn¹³..."));
		timer.Stop();
		Game(CHANGE_PLAYER);
	}
	else if (gameTime == 10)
	{
		time->SetForegroundColour(*wxRED);
	}
}

//FUNKCJE GRY

void MyFrame::InitGame()
{
	game = true;
	player1pts = 0;
	player2pts = 0;
	gameTime = 0;
	words.Clear();
	roundNumber = 1;
	currPlayer = 1;
	
	difficulty = level->GetSelection();
	switch (difficulty)
	{
	case 0: gameTime = 120; errorMax = 9; break;
	case 1: gameTime = 60; errorMax = 6; break;
	case 2: gameTime = 30; errorMax = 3; break;
	}

	round->SetLabel(wxString::Format(wxT("%d z %d"), 1, rounds->GetValue()));
	points->SetLabel(wxT("(G1) 0:0 (G2)"));
	player->SetLabel(wxT("Gracz 1"));
	time->SetLabel(wxT("000"));
	DrawGallows2(0);
}

void MyFrame::Game(int gameState)
{
	wxString tmp;
	wxString tmp_word;
	bool isOk = false;
	int easy[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	int medi[6] = { 1, 3, 4, 6, 7, 9 };
	int hard[3] = { 3, 6, 9 };

	switch (gameState)
	{
	case INIT_PLAYER:	//Rozpoczêcie dla gracza n
		//Przerwanie gry gdy osi¹gniêto liczbê rund
		if (roundNumber == rounds->GetValue() + 1)
		{
			EndGame();
			return;
		}
		//Zerowanie szubienicy
		DrawGallows2(0);
		
		//Wyœwietlanie czasu pocz¹tkowego
		time->SetForegroundColour(wxColor(0, 180, 50));
		time->SetLabel(wxString::Format(wxT("%d"), gameTime));
		
		//Ustawienie s³owa do odgadniêcia
		if (currPlayer == 1)
			currWord = words.Item((rounds->GetValue() - 1) + roundNumber);
		else
			currWord = words.Item(roundNumber - 1);
		letters = currWord.Len();

		//Komunikat + Log
		tmp = wxString::Format(wxT("GRACZ %d\nodgaduje has³o nr %d\n(litery: %d)"), currPlayer, roundNumber, letters);
		LogC(tmp, *wxBLUE);
		wxMessageBox(tmp, wxT("Uwaga!"), wxICON_EXCLAMATION);

		//Wyœwietlenie kropek w miejscu liter
		word->SetLabel(wxEmptyString);
		tmp = wxEmptyString;
		for (int j = 0; j < letters; j++)
		{
			tmp += ".";
		}
		SetWord(tmp);
		error = 0;

		//Start stopera
		timer.Start(1000);
		break;

	case ANALYSIS:	//Odgadywanie litery i rysowanie szubienicy
		tmp = wxEmptyString;
		tmp_word = word->GetLabel();

		//Sprawdzanie litery
		for (unsigned int i = 0; i < currWord.Len(); i++)
		{
			if (letter.IsSameAs(currWord.GetChar(i)) && letter.IsSameAs(tmp_word.GetChar(i)))
			{
				tmp << letter;
				isOk = true;
			}
			else if (letter.IsSameAs(currWord.GetChar(i)))
			{
				tmp << letter;
				letters--;
				isOk = true;
			}
			else tmp << tmp_word.GetChar(i);
		}
		SetWord(tmp);
		if (!isOk)
		{
			Log(letter + wxT(" (Pud³o)"));
			switch (difficulty)
			{
			case 0: DrawGallows2(easy[error]); break;
			case 1: DrawGallows2(medi[error]); break;
			case 2: DrawGallows2(hard[error]); break;
			}
			error++;
			if ((error) == errorMax)
			{
				timer.Stop();
				wxMessageBox(wxT("WISISZ!"), wxT("Przykro mi..."), wxICON_ERROR);
				Game(CHANGE_PLAYER);
			}
		}
		else
		{
			Log(letter + wxT(" (OK)"));
		}
		if (letters == 0)
			Game(CHANGE_PLAYER);
		break;

	case CHANGE_PLAYER:		//Zmiana gracza
		if (currPlayer == 1)
		{
			if (letters == 0)
				player1pts++;
			currPlayer = 2;
		}
		else
		{
			if (letters == 0)
				player2pts++;
			currPlayer = 1;
			roundNumber++;
			if (roundNumber <= rounds->GetValue())
			{
				round->SetLabel(wxString::Format(wxT("%d z %d"), roundNumber, rounds->GetValue()));
			}
		}
		timer.Stop();
		Log(wxT("Has³o: ") + currWord);
		points->SetLabel(wxString::Format(wxT("(G1) %d z %d (G2)"), player1pts, player2pts));
		player->SetLabel(wxString::Format(wxT("Gracz %d"), currPlayer));
		Game(INIT_PLAYER);
		break;
	default:
		break;
	}
}

void MyFrame::ShowWinnerInfo()
{
	wxString win;
	if (player1pts > player2pts)
		win = wxT("Zwyciê¿y³ GRACZ 1!");
	else if (player2pts > player1pts)
		win = wxT("Zwyciê¿y³ GRACZ 2!");
	else
		win = wxT("REMIS!");

	LogC(win, *wxRED);
	wxMessageBox(wxT("Wynik gry:\n") + win, wxT("Wynik gry"), wxOK | wxICON_INFORMATION);
}

void MyFrame::EndGame()
{
	log->Unbind(wxEVT_KEY_UP, &MyFrame::OnKey, this);
	game = false;
	ShowWinnerInfo();
	timer.Stop();
	SetWord(wxT("Wisielec"));
	DrawGallows2(9);

	//W³¹czanie kontrolek
	level->Enable(true);
	rounds->Enable(true);
	tb->EnableTool(ID_NEW_GAME, true);
	mb->Enable(ID_NEW_GAME, true);
}

void MyFrame::SetWord(wxString s)		//Wypisywanie has³a w oknie gry
{
	word->SetLabel(s);
	leftSizer->FitInside(word);		//Optymalizacja rozmiaru sizera wzgledem nowej zawartosci
	leftSizer->RecalcSizes();		//Przeliczenie rozmiaru kontrolek w sizerze co powoduje wysrodkowanie hasla o nowej dlugosci
}

//FUNKCJE LOGU

void MyFrame::Log(wxString s)						//Wyœwietlenie zwyk³ego tekstu
{
	this->log->AppendText(s + wxT("\n"));
}

void MyFrame::LogF(wxString s, int val)				//Wyœwietlenie tekstu formatowanego
{
	this->log->AppendText(wxString::Format(s, val) + wxT("\n"));
}

void MyFrame::LogC(wxString s, const wxColour&c)	//Wyœwietlenie tekstu w kolorze c
{
	this->log->SetDefaultStyle(wxTextAttr(c));
	Log(s);
	this->log->SetDefaultStyle(wxTextAttr(*wxBLACK));
}

//Rysowanie szubienicy
void MyFrame::DrawGallows(int step)
{
	if (step < 1 && step > 9) return;
	//Tworzenie kontekstu:
	wxBitmap bmp(340, 400);
	wxMemoryDC dc(bmp);		//kontekts rysowania w pamiêci z podpiêt¹ map¹ bitów
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();
	//Ustawienie nietypowych piór i pêdzli:
	wxPen brownPen(wxColour(145, 80, 0), 5);
	wxPen orangePen(wxColour(255, 200, 0), 3);
	wxPen blackPen(*wxBLACK, 5);
	wxBrush brownBrush(wxColour(145, 80, 0), wxBRUSHSTYLE_BDIAGONAL_HATCH);

	switch (step)
	{
	case 9:		//nogi
		dc.SetPen(wxPen(*wxBLACK));
		dc.DrawSpline(170, 240, 163, 260, 165, 280);
		dc.DrawSpline(170, 240, 177, 260, 175, 280);
		dc.DrawEllipse(163, 280, 4, 20);
		dc.DrawEllipse(173, 280, 4, 20);
	case 8:		//rêce
		dc.SetPen(wxPen(*wxBLACK));
		dc.DrawSpline(170, 160, 140, 170, 130, 200);
		dc.DrawSpline(130, 200, 133, 205, 126, 210);
		dc.DrawSpline(130, 200, 135, 205, 130, 210);
		dc.DrawSpline(130, 200, 137, 205, 134, 210);
		dc.DrawSpline(170, 160, 200, 170, 210, 200);
		dc.DrawSpline(210, 200, 213, 205, 206, 210);
		dc.DrawSpline(210, 200, 215, 205, 210, 210);
		dc.DrawSpline(210, 200, 217, 205, 214, 210);
	case 7:		//tu³ów
		dc.SetPen(blackPen);
		dc.SetBrush(*wxLIGHT_GREY_BRUSH);
		dc.DrawEllipse(160, 160, 20, 80);
		dc.SetPen(orangePen);
		dc.DrawSpline(160, 170, 170, 173, 180, 170);
	case 6:		//g³owa
		dc.SetPen(orangePen);
		dc.DrawLine(170, 40, 170, 80);
		dc.SetPen(blackPen);
		dc.SetBrush(*wxWHITE_BRUSH);
		dc.DrawCircle(170, 120, 40);
		dc.SetPen(wxPen(*wxBLACK));
		dc.DrawEllipse(160, 100, 10, 25);
		dc.DrawEllipse(180, 100, 10, 25);
		dc.SetBrush(*wxBLUE_BRUSH);
		dc.DrawCircle(165, 117, 5);
		dc.DrawCircle(185, 117, 5);
	case 5:		//wspornik 2
		dc.SetPen(brownPen);
		dc.DrawLine(60, 80, 100, 40);
	case 4:		//poprzeczka
		dc.SetPen(brownPen);
		dc.DrawLine(60, 40, 280, 40);
	case 3:		//wspornik 1
		dc.SetPen(brownPen);
		dc.DrawLine(60, 290, 100, 330);
	case 2:		//s³up
		dc.SetPen(brownPen);
		dc.DrawLine(60, 40, 60, 330);
	case 1:		//podest
		dc.SetPen(brownPen);
		dc.SetBrush(brownBrush);
		dc.DrawRectangle(20, 330, 300, 40);
		break;
	default:
		break;
	}
	//Ustawiamy szubienice:
	gallows->SetBitmap(bmp);
	bmp = wxNullBitmap;
	leftSizer->RecalcSizes();	//odœwie¿enie sizera
}

void MyFrame::DrawGallows2(int step)
{
	if (step < 1 && step > 9) return;
	//Tworzenie kontekstu:
	wxBitmap bmp(340, 400);
	wxMemoryDC dc(bmp);		//kontekts rysowania w pamiêci z podpiêt¹ map¹ bitów
	dc.SetBackground(*wxBLACK_BRUSH);
	dc.Clear();
	
	switch (step)
	{
	case 9:		//nogi
	{
		wxImage s9(wxGetAppFile("gfx/s9.jpg"), wxBITMAP_TYPE_JPEG);
		s9.Rescale(340, 212);
		dc.DrawBitmap(s9, wxPoint(0, 94));
		break;
	}
	case 8:		//rêce
	{
		wxImage s8(wxGetAppFile("gfx/s8.jpg"), wxBITMAP_TYPE_JPEG);
		s8.Rescale(340, 212);
		dc.DrawBitmap(s8, wxPoint(0, 94));
		break;
	}
	case 7:		//tu³ów
	{
		wxImage s7(wxGetAppFile("gfx/s7.jpg"), wxBITMAP_TYPE_JPEG);
		s7.Rescale(340, 212);
		dc.DrawBitmap(s7, wxPoint(0, 94));
		break;
	}
	case 6:		//g³owa
	{
		wxImage s6(wxGetAppFile("gfx/s6.jpg"), wxBITMAP_TYPE_JPEG);
		s6.Rescale(340, 212);
		dc.DrawBitmap(s6, wxPoint(0, 94));
		break;
	}
	case 5:		//wspornik 2
	{
		wxImage s5(wxGetAppFile("gfx/s5.jpg"), wxBITMAP_TYPE_JPEG);
		s5.Rescale(340, 212);
		dc.DrawBitmap(s5, wxPoint(0, 94));
		break;
	}
	case 4:		//poprzeczka
	{
		wxImage s4(wxGetAppFile("gfx/s4.jpg"), wxBITMAP_TYPE_JPEG);
		s4.Rescale(340, 212);
		dc.DrawBitmap(s4, wxPoint(0, 94));
		break;
	}
	case 3:		//wspornik 1
	{
		wxImage s3(wxGetAppFile("gfx/s3.jpg"), wxBITMAP_TYPE_JPEG);
		s3.Rescale(340, 212);
		dc.DrawBitmap(s3, wxPoint(0, 94));
		break;
	}
	case 2:		//s³up
	{
		wxImage s2(wxGetAppFile("gfx/s2.jpg"), wxBITMAP_TYPE_JPEG);
		s2.Rescale(340, 212);
		dc.DrawBitmap(s2, wxPoint(0, 94));
		break;
	}
	case 1:		//podest
	{
		wxImage s1(wxGetAppFile("gfx/s1.jpg"), wxBITMAP_TYPE_JPEG);
		s1.Rescale(340, 212);
		dc.DrawBitmap(s1, wxPoint(0, 94));
		break;
	}
	case 0:		//nic (start gry)
	{
		wxImage s0(wxGetAppFile("gfx/s0.jpg"), wxBITMAP_TYPE_JPEG);
		s0.Rescale(340, 212);
		dc.DrawBitmap(s0, wxPoint(0, 94));
		break;
	}
	default:
		break;
	}
	//Ustawiamy szubienice:
	gallows->SetBitmap(bmp);
	bmp = wxNullBitmap;
	leftSizer->RecalcSizes();	//odœwie¿enie sizera
}