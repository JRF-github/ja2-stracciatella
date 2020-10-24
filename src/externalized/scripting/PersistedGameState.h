#pragma once
#include <cstdint>
#include <vector>
#include <map>
#include <string_theory/string>

class DataReader;
class DataWriter;

std::map<ST::string, int32_t> ReadExtraStates(DataReader r);
void WriteExtraStates(DataWriter w, std::map<ST::string, int32_t> states);
