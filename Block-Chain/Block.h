#pragma once

#include "stdafx.h"
#include "storage.h"


//Sukuriama bloko klase
class Block {
private:
	uint16_t index;
	std::vector<transaction> data = std::vector<transaction>(100);
	std::string hash;
	int64_t nonce;
	time_t time;
	std::string BlockInfo;

	std::string calculateHash() {
		std::string blockInfo = BlockInfo + std::to_string(nonce);
		return Hash(blockInfo);
	};
public:
	std::string prevHash;
	Block() {};
	Block(uint16_t index_, std::vector<transaction> data_, uint8_t difficulty) {
		index = index_;
		nonce = -1;
		data = data_;
		time = std::time(nullptr);
		std::stringstream blockInfo;
		std::string sdata = "";
		for (short i = 0; i < data_.size(); i++) {
			sdata = sdata + data[i].getHash();
		}
		blockInfo << index << sdata << time <<  prevHash;
		BlockInfo = blockInfo.str();
		mineBlock(difficulty);
		std::cout << nonce << std::endl;
	};
	std::string getHash() {
		return hash;
	};
	uint16_t getIndex() {
		return index;
	}
	std::vector<transaction> getData() {
		return data;
	}
	void mineBlock(uint8_t difficulty) {
		char* cstr = new char[difficulty + 1];
		for (uint8_t i = 0; i < difficulty; ++i) {
			cstr[i] = '0';
		}
		cstr[difficulty] = '\0';
		std::string str(cstr);
		do {
			nonce++;
			hash = calculateHash();
		} while (hash.substr(0, difficulty) != str);
	};
	void print1() {
		std::cout << "Block " << index << ":" << std::endl;
		std::cout << "Previous Hash - \"" << prevHash << "\"" << std::endl;
		std::cout << "Hash - \"" << hash << "\"" << std::endl;
		std::cout << "Transactions:" << std::endl;
		for (short i = 0; i < data.size(); i++) {
			std::cout << "\"" << data[i].getHash() << "\"" << std::endl;
		}
	}
	void print2() {
		std::cout << "Block " << index << " hash:" << std::endl;
		std::cout << "\"" << hash << "\"" << std::endl;
	}
};

class Block_Chain {
private:
	uint8_t difficulty;
	std::vector<Block> chain;
	uint16_t Size;
public:
	uint16_t size() {
		return Size;
	}
	Block getLastBlock() const {
		return chain.back();
	};
	Block getBlock(short i) const {
		return chain[i];
	}
	Block_Chain(std::vector<transaction> data) {
		difficulty = 1;
		chain.emplace_back(Block(0, data, difficulty));
	};
	void addBlock(std::vector<transaction> &data) {
		Block bNew(getLastBlock().getIndex() + 1, data, difficulty);
		bNew.prevHash = getLastBlock().getHash();
		chain.push_back(bNew);
		Size = chain.size();
	};
};

