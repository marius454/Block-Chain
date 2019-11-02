#pragma once

#include "stdafx.h"
#include "storage.h"
std::string manoHash(std::string input);

//Sukuriama bloko klase
class Block {
private:
	uint16_t index;
	std::vector<transaction> data = std::vector<transaction>(100);
	std::string hash;
	int64_t nonce;
	time_t time;
	std::string BlockInfo;
	uint32_t iterations;

	std::string calculateHash() {
		std::string blockInfo = BlockInfo + std::to_string(nonce);
		return manoHash(blockInfo);
	};
public:
	std::string prevHash;
	Block() {};
	Block(uint16_t index_, std::vector<transaction> data_, uint8_t difficulty, uint32_t Iterations) {
		index = index_;
		nonce = -1;
		data = data_;
		time = std::time(nullptr);
		std::stringstream blockInfo;
		std::string sdata = "";
		for (uint8_t i = 0; i < data_.size(); i++) {
			sdata = sdata + data[i].getHash();
		}
		blockInfo << index << sdata << time << prevHash;
		BlockInfo = blockInfo.str();
		iterations = Iterations;
		mineBlock(difficulty);
		std::cout << "iterations: " << nonce << std::endl;
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
		std::string Hash;
		auto t1 = std::chrono::high_resolution_clock::now();
		do {
			nonce++;
			Hash = calculateHash();
		} while (Hash.substr(0, difficulty) != str && nonce < iterations);
		auto t2 = std::chrono::high_resolution_clock::now();
		long long duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		double dur = (double)duration * 0.000001;
		std::cout << "mining time: " << dur << "s" << std::endl;

		if (Hash.substr(0, difficulty) == str) {
			blockMined(Hash);
		}
		else {
			hash = "";
		}
	};
	void blockMined(std::string Hash) {
		hash = Hash;
	}
	void print1() {
		std::cout << "Block " << index << ":" << std::endl;
		std::cout << "Previous Hash - \"" << prevHash << "\"" << std::endl;
		std::cout << "Hash - \"" << hash << "\"" << std::endl;
		std::cout << "Transactions:" << std::endl;
		for (uint8_t i = 0; i < data.size(); i++) {
			std::cout << "\"" << data[i].getHash() << "\"" << std::endl;
		}
	}
	void print2() {
		std::cout << "Block " << index << " hash:" << std::endl;
		std::cout << "\"" << hash << "\"" << std::endl;
	}
};

//Sukuriama bloku grandines klase
class Block_Chain {
private:
	uint8_t difficulty;
	std::vector<Block> chain;
	size_t Size;
	uint32_t iterations;
public:
	size_t size() {
		return Size;
	}
	Block getLastBlock() const {
		return chain.back();
	};
	Block getBlock(short i) const {
		return chain[i];
	}
	void incrementIter() {
		iterations = iterations + 200;
	}
	Block_Chain() {
		difficulty = 2;
		Size = 0;
		iterations = 200;
	};
	void addBlock(std::vector<transaction> &data) {
		Block bNew(Size, data, difficulty, iterations);
		if (bNew.getHash() != "") {
			if (Size > 0) {
				bNew.prevHash = getLastBlock().getHash();
			}
			chain.push_back(bNew);
			Size = chain.size();
			iterations = 200;
		}
	};
};

