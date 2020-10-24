#include "PersistedGameState.h"
#include "LoadSaveData.h"

std::map<ST::string, int32_t> ReadExtraStates(DataReader r)
{
	std::map<ST::string, int32_t> states;
	uint32_t numEntries = r.readU32();
	for (int i = 0; i < numEntries; i++)
	{
		uint8_t keyLen = r.readU8();
		ST::string key = r.readUTF16(keyLen);
		int32_t val = static_cast<int16_t>(r.readU32());
		states[key] = val;
	}
	return states;
}

uint32_t GetExtraStatesLength(std::map<ST::string, int32_t> const states)
{
	size_t numBytes = 0;
	numBytes += sizeof(uint32_t); // dictionary size
	numBytes += states.size() * (sizeof(uint8_t) + sizeof(int32_t));   // key lengths and values
	for (auto& pair : states)
	{
		numBytes += pair.first.size(); // key data
	}
	if (numBytes > UINT32_MAX) throw std::runtime_error("states length too long");
	return numBytes;
}

void WriteExtraStates(DataWriter w, std::map<ST::string, int32_t> const states)
{
	size_t dictSize = states.size();
	if (dictSize > UINT32_MAX) throw std::runtime_error("too many items in states");
	w.writeU32(dictSize);

	for (auto& pair : states)
	{
		ST::string key = pair.first;
		if (key.size() > UINT8_MAX) throw std::runtime_error("state key too long");
		w.writeU8(key.size());
		w.writeUTF8(key, key.size());
		w.writeU32(static_cast<uint32_t>(pair.second));
	}

	return;
}

