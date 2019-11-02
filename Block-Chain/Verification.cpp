#include "stdafx.h"
#include "Block.h"
#include "Storage.h"
#include <algorithm>
#include <random>

void verification(std::vector<transaction> &txPool, std::vector<user> &users) {
	uint16_t index = 0;
	std::vector<uint16_t> txi;
	for (auto &tx : txPool) {
		std::string check = tx.getSenderHash();
		auto it = std::find_if(users.begin(), users.end(), [&check](user& obj) {return obj.getPubKey() == check; });
		if (it != users.end()) {
			if ((*it).getBalance() <= tx.getAmount()) {
				txi.emplace_back(index);
			}
			else if (tx.recalculateHash() != tx.getHash()) {
				txi.emplace_back(index);
			}
		}
		index++;
	}
	std::vector<transaction> newPool;
	std::sort(txi.begin(), txi.end());
	uint16_t a = 0;
	uint16_t b = *txi.begin();
	for (uint16_t i = 0; i < txi.size(); i++) {
		b = *(txi.begin() + i);
		std::copy(txPool.begin() + a, txPool.begin() + b, std::back_inserter(newPool));
		a = b + 1;
	}
	std::copy(txPool.begin() + a, txPool.end(), std::back_inserter(newPool));
	txPool = newPool;
}

std::vector<transaction> getTX(std::vector<transaction> &txPool, uint8_t cnt, std::vector<uint16_t> &txi) {
	std::vector<transaction> blockTx;
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> distr(0, txPool.size() - 1);
	if (txPool.size() >= 100) {
		txi = std::vector<uint16_t>(100);
		blockTx = std::vector<transaction>(100);
		for (uint16_t i = 0; i < 100; i++) {
			uint16_t x = distr(eng);
			while (std::find(txi.begin(), txi.end(), x) != txi.end()) {
				x = distr(eng);
			}
			txi[i] = x;
			blockTx[i] = txPool[x];
		}
		
	}
	else {
		txi = std::vector<uint16_t>(txPool.size());
		blockTx = std::vector<transaction>(txPool.begin(), txPool.end());
		
	}
	for (uint16_t i = 0; i < blockTx.size(); i++) {
		blockTx[i].setHeight(cnt);
	}
	return blockTx;
}