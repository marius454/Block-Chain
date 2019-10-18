#pragma once

#include "stdafx.h"


//Sukuriama bloko klase
class Block {
private:
	uint16_t index;
	std::string data;
	std::string hash;
	int64_t nonce;
	time_t time;

public:
	std::string prevHash;
	Block(uint16_t _index, const std::string &_data);
	std::string getHash();
	void mineBlock(uint8_t Difficulty);

};