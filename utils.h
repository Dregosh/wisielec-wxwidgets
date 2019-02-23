/////////////////////////////////////////////////////////////////////////////
// Name:        utils.h
// Purpose:     Rozdzial 6 - Wisielec
// Author:      MateoSan
// Created:     2019-02-13
// Copyright:   (c) 2018 by MateoSan
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _UTILS_H_
#define _UTILS_H_

#include <wx/stdpaths.h>

#define _INSTALATOR_ 0

wxString wxGetAppFile(wxString file)
{
	wxString path = wxPathOnly(wxStandardPaths::Get().GetExecutablePath());

	#if _INSTALATOR_ && defined (__WXGTK__)
	path = wxStandardPaths::Get().GetDataDir();
	#endif

	path = path + wxT("/") + file;
	path.Replace("\\", "/", true);
	path.Replace("//", "/", true);
	return path;
}

#endif