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
	std::string merkle_root;
	std::string BlockInfo;
	uint32_t maxiter;

	std::string calculateHash() {
		std::string blockInfo = BlockInfo + std::to_string(nonce);
		return manoHash(blockInfo);
	};
	std::string create_merkle(std::vector<transaction>& data);
public:
	std::string prevHash;
	Block() {};
	Block(uint16_t index_, std::vector<transaction> data_, uint8_t difficulty, uint32_t maxIter) {
		index = index_;
		nonce = -1;
		data = data_;
		time = std::time(nullptr);
		std::stringstream blockInfo;
		merkle_root = create_merkle(data);
		blockInfo << index << merkle_root << time << prevHash;
		BlockInfo = blockInfo.str();
		maxiter = maxIter;
		mineBlock(difficulty);
		std::cout << "iterations: " << nonce + 2 << std::endl;
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
			Hash = calculateHash();
			nonce++;
		} while (Hash.substr(0, difficulty) != str && nonce < maxiter - 1);
		nonce--;
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
	uint32_t maxiter;
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
		maxiter = maxiter + 1000;
	}
	Block_Chain() {
		difficulty = 3;
		Size = 0;
		maxiter = 1000;
	};
	void addBlock(std::vector<transaction> &data) {
		Block bNew(Size, data, difficulty, maxiter);
		if (bNew.getHash() != "") {
			if (Size > 0) {
				bNew.prevHash = getLastBlock().getHash();
			}
			chain.push_back(bNew);
			Size = chain.size();
			maxiter = 1000;
		}
	};
};

