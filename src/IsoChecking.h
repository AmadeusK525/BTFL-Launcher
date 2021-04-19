#ifndef ISOCHECK_H_
#define ISOCHECK_H_
#pragma once

#include <wx\wx.h>
#include <wx\wfstream.h>
#include <wx\progdlg.h>

#include <digestpp.hpp>
#include "IsoHashes.h"

enum ISO_Region
{
	ISO_Usa,
	ISO_Brazil,

	ISO_Invalid
};

inline bool IsHashFromIso(const wxString& userHash, ISO_Region region)
{
	wxString hash;
	switch ( region )
	{
	case ISO_Usa:
		hash = iso_usa;
		break;

	default:
		break;
	}

	return hash == userHash;
}

inline wxString GetFileHash(wxFileInputStream* stream)
{
	size_t fullLength = stream->GetLength();
	const size_t READ_LENGTH = 100000000;
	size_t length = 0;
	size_t toRead = READ_LENGTH;
	char* buffer;

	digestpp::sha3 hasher(256);
	int index = 1;

	while ( length < fullLength )
	{
		length += toRead;
		if ( length > fullLength )
		{
			length -= READ_LENGTH;
			toRead = fullLength - length;
		}

		buffer = new char[toRead];
		stream->Read(buffer, toRead);

		hasher.absorb(buffer);
		delete[] buffer;
	}

	return hasher.hexdigest();
}

inline bool CheckIsoValidity(const wxString& path)
{
	wxProgressDialog progdlg("Verifying iso...", "Please do not close the launcher.");
	progdlg.Pulse();

	wxFileInputStream userIso(path);
	if ( !userIso.IsOk() )
		return false;

	wxString userHash = GetFileHash(&userIso);
	for ( ISO_Region region = ISO_Usa; region != ISO_Invalid; region = (ISO_Region)(region + 1) )
	{
		if ( IsHashFromIso(userHash, region) )
			return true;
	}

	return false;
}
#endif