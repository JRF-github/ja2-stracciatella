#pragma once

#include <stdint.h>
#include <vector>
#include "sgp/AutoObj.h"

struct SGP_FILETIME
{
	uint32_t Lo;
	uint32_t Hi;
};

enum SGPFileFlags
{
	SGPFILE_NONE = 0U,
	SGPFILE_REAL = 1U << 0
};

struct File;
struct VfsFile;
struct SGPFile;

void FileRead( SGPFile*, void*       pDest, size_t uiBytesToRead);
void FileWrite(SGPFile*, void const* pDest, size_t uiBytesToWrite);

struct SGPFile
{
	SGPFileFlags flags;
	union
	{
		File* file;
		VfsFile* vfile;
	} u;

	template<typename T>
	T read(void)
	{
		T value;
		FileRead(this, &value, sizeof(T));
		return value;
	}

	template<typename T>
	void write(T const& value)
	{
		FileWrite(this, &value, sizeof(T));
	}

	template<typename size_type, typename value_type>
	void readVector(std::vector<value_type> & vector)
	{
		vector.clear();

		size_type const n{read<size_type>()};
		if (n == 0) return;

		vector.reserve(n);
		FileRead(this, vector.data(), n * sizeof(value_type));
	}

	template<typename size_type, typename value_type>
	void writeVector(std::vector<value_type> const& vector)
	{
		size_type const n{static_cast<size_type>(vector.size())};
		write(n);

		if (n != 0) FileWrite(this, vector.data(), n * sizeof(value_type));
	}
};

enum FileSeekMode
{
	FILE_SEEK_FROM_START,
	FILE_SEEK_FROM_END,
	FILE_SEEK_FROM_CURRENT
};

extern void FileClose(SGPFile*);

typedef SGP::AutoObj<SGPFile, FileClose> AutoSGPFile;
