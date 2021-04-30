#include "LoadSaveData.h"

#include "Debug.h"
#include "FileMan.h"
#include <string_theory/format>
#include <string_theory/string>

#include <algorithm>


////////////////////////////////////////////////////////////////////////////
// DataWriter
////////////////////////////////////////////////////////////////////////////

/** Constructor.
 * @param buf Pointer to the buffer for writing data. */
DataWriter::DataWriter(uint8_t* buf)
	:m_buf(buf), m_original(buf)
{
}

void DataWriter::writeUTF8(const ST::string& str, size_t numChars)
{
	size_t n = std::min<size_t>(str.size() + 1, numChars);
	if (str.size() > n)
	{
		STLOGW("DataWriter::writeUTF8: truncating '{}' {}->{}", str, str.size(), n);
	}
	writeArray(str.c_str(), n);
	skip(sizeof(char) * (numChars - n));
}

void DataWriter::writeUTF16(const ST::string& str, size_t numChars)
{
	ST::utf16_buffer buf = str.to_utf16();
	size_t n = std::min<size_t>(buf.size() + 1, numChars);
	if (buf.size() > n)
	{
		STLOGW("DataWriter::writeUTF16: truncating '{}' {}->{}", str, buf.size(), n);
	}
	writeArray(buf.c_str(), n);
	skip(sizeof(char16_t) * (numChars - n));
}

void DataWriter::writeUTF32(const ST::string& str, size_t numChars)
{
	ST::utf32_buffer buf = str.to_utf32();
	size_t n = std::min<size_t>(buf.size() + 1, numChars);
	if (buf.size() > n)
	{
		STLOGW("DataWriter::writeUTF32: truncating '{}' {}->{}", str, buf.size(), n);
	}
	writeArray(buf.c_str(), n);
	skip(sizeof(char32_t) * (numChars - n));
}

void DataWriter::writeU8 (uint8_t  value)
{
	write(value);
}

void DataWriter::writeU16(uint16_t value)
{
	write(value);
}

void DataWriter::writeU32(uint32_t value)
{
	write(value);
}

void DataWriter::skip(size_t numBytes)
{
	std::fill_n(m_buf, numBytes, 0);
	move(numBytes);
}

/** Move pointer to \a numBytes bytes forward. */
void DataWriter::move(size_t numBytes)
{
	m_buf += numBytes;
}

/** Get number of the consumed bytes during writing. */
size_t DataWriter::getConsumed() const
{
	return m_buf - m_original;
}

////////////////////////////////////////////////////////////////////////////
// DataReader
////////////////////////////////////////////////////////////////////////////


DataReader::DataReader(const uint8_t* buf)
	: m_buf(buf), m_original(buf)
{
}

ST::string DataReader::readUTF8(size_t numChars, ST::utf_validation_t validation)
{
	ST::char_buffer buf{numChars, '\0'};
	readArray(buf.data(), numChars);
	return ST::string(buf.c_str(), ST_AUTO_SIZE, validation);
}

ST::string DataReader::readUTF16(size_t numChars, const IEncodingCorrector* fixer, ST::utf_validation_t validation)
{
	ST::utf16_buffer buf{numChars, u'\0'};
	readArray(buf.data(), numChars);
	if (fixer)
	{
		for (char16_t& c : buf)
		{
			c = fixer->fix(c);
			if (c == u'\0')
			{
				break;
			}
		}
	}
	return ST::string(buf.c_str(), ST_AUTO_SIZE, validation);
}

ST::string DataReader::readUTF32(size_t numChars, ST::utf_validation_t validation)
{
	ST::utf32_buffer buf{numChars, U'\0'};
	readArray(buf.data(), numChars);
	return ST::string(buf.c_str(), ST_AUTO_SIZE, validation);
}

uint8_t DataReader::readU8()
{
	return read<uint8_t>();
}

uint16_t DataReader::readU16()
{
	return read<uint16_t>();
}

uint32_t DataReader::readU32()
{
	return read<uint32_t>();
}

void DataReader::skip(size_t numBytes)
{
	move(numBytes);
}

void DataReader::move(size_t numBytes)
{
	m_buf += numBytes;
}

size_t DataReader::getConsumed() const
{
	return m_buf - m_original;
}

////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>

FileDataWriter::FileDataWriter(size_t const size, SGPFile * const file)
	: DataWriter{&buf_[0]}, file_{file}, size_{size}
{
	Assert(size > 0 && size <= sizeof(buf_));
}


FileDataWriter::~FileDataWriter()
{
	if (getConsumed() != size_) 	{ printf("!!!!!!!!!!!!!! %d %d\n", (int)getConsumed(), (int)size_); abort();}

	FileWrite(file_, &buf_[0], size_);
}


FileDataReader::FileDataReader(size_t const size, SGPFile * const file)
	: DataReader{&buf_[0]}, size_{size}
{
	Assert(size > 0 && size <= sizeof(buf_));
	FileRead(file, &buf_[0], size);
}


FileDataReader::~FileDataReader()
{
	if (getConsumed() != size_) 
	{ printf("!!!!!!!!!!!!!! %d %d\n", (int)getConsumed(), (int)size_); abort();}
}
