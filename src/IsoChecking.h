#ifndef ISOCHECK_H_
#define ISOCHECK_H_
#pragma once

#include <wx\wx.h>
#include <wx\wfstream.h>

#include <digestpp.hpp>
#include "IsoHashes.h"

namespace iso
{
	const int NUMBER_OF_ISOS = 2;
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

		case ISO_Brazil:
			break;

		case ISO_Invalid:
			break;
		}

		return hash == userHash;
	}

	inline wxString GetFileHash(const wxString& filePath)
	{
		wxFileInputStream stream(filePath);
		if ( !stream.IsOk() )
			return "";

		size_t fullLength = stream.GetLength();
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
			stream.Read(buffer, toRead);

			hasher.absorb(buffer);
			delete[] buffer;
		}

		return hasher.hexdigest();
	}

	// Currently unused
	inline ISO_Region CheckIsoValidity(const wxString& path)
	{
		wxString userHash = GetFileHash(path);
		for ( ISO_Region region = ISO_Usa; region != ISO_Invalid; region = (ISO_Region)(region + 1) )
		{
			if ( IsHashFromIso(userHash, region) )
				return region;
		}

		return ISO_Invalid;
	}

};

#endif