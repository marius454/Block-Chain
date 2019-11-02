#include "stdafx.h"
#include "storage.h"
#include "Block.h"
#include <random>
#include <algorithm>

std::vector<transaction> getTX(std::vector<transaction> &txPool, uint8_t cnt, std::vector<uint16_t> &txi);
Block getBlockByIndex(Block_Chain Chain);

Block_Chain initializeChain(std::vector<transaction> &txPool, uint16_t txNR) {
	
	uint8_t cnt = 0;
	Block_Chain Chain;
	while (txPool.size() > 0) {
		std::vector<std::vector<uint16_t>> txi(5);
		std::vector<std::vector<transaction>> blockTx(5);
		for (uint8_t i = 0; i < 5; i++) {
			blockTx[i] = getTX(txPool, cnt, txi[i]);
		}
		uint8_t successfulCandidate = -1;

		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr(0, 4);

	addBlock:
		std::vector<uint8_t> index;
		for (uint8_t i = 0; i < 5; i++) {
			uint8_t ind = distr(eng);
			while (std::find(index.begin(), index.end(), ind) != index.end()) {
				ind = distr(eng);
			}
			uint16_t check = Chain.size();
			std::cout << std::endl << "Current Candidate: " << std::to_string(ind + 1) << std::endl;
			index.emplace_back(ind);
			Chain.addBlock(blockTx[index.back()]);
			if (check < Chain.size()) {
				successfulCandidate = index.back();
				break;
			}
			if (index.size() == 5) {
				Chain.incrementIter();
				goto addBlock;
			}
		}
		
		std::cout << "Successful Candidate: " << std::to_string(successfulCandidate + 1) << std::endl;
		std::cout << "---------------------------" << std::endl;
		if (txPool.size() >= 100) {
			std::vector<transaction> newPool;
			std::sort(txi[successfulCandidate].begin(), txi[successfulCandidate].end());
			uint16_t a = 0;
			uint16_t b = *(txi[successfulCandidate].begin());
			for (uint16_t i = 0; i < txi[successfulCandidate].size(); i++) {
				b = *(txi[successfulCandidate].begin() + i);
				std::copy(txPool.begin() + a, txPool.begin() + b, std::back_inserter(newPool));
				a = b + 1;
			}
			std::copy(txPool.begin() + a, txPool.end(), std::back_inserter(newPool));
			txPool = newPool;
		}
		else {
			txPool.clear();
			txPool.shrink_to_fit();
		}
		cnt++;
	}
	return Chain;
}

short menu1() {
	short select = 0;
	std::cout << "What data would you like to use:" << std::endl;
	std::cout << "1) Saved data" << std::endl;
	std::cout << "2) Generate new data" << std::endl;
	std::cout << "3) Exit Program" << std::endl;
	std::cin >> select;
	return select;
}

short menu2() {
	short select = 0;
	std::cout << "Choose what you would like to do:" << std::endl;
	std::cout << "1) See all blocks in block-chain" << std::endl;
	std::cout << "2) See block by index" << std::endl;
	std::cout << "3) See block by hash" << std::endl;
	std::cout << "4) See transaction by hash" << std::endl;
	std::cout << "5) Exit Program" << std::endl;

	std::cin >> select;
	return select;
}

Block getBlockByIndex(Block_Chain Chain) {
	int16_t index = 0;
	std::cout << "Input block index:" << std::endl;
selection:
	std::cin >> index;
	if (index + 1 > Chain.size()) {
		std::cout << "No such block, try again:" << std::endl;
		goto selection;
	}
	else if (index == -1) {
		Block block;
		return block;
	}
	else {
		Block block = Chain.getBlock(index);
		return block;
	}
}

Block getBlockByHash(Block_Chain Chain) {
	uint16_t index = -1;
	std::string hash;
	std::cout << "Input block hash:" << std::endl;
selection:
	std::cin >> hash;
	if (hash == "-1") {
		Block block;
		return block;
	}
	for (uint16_t i = 0; i < Chain.size(); i++) {
		Block block = Chain.getBlock(i);
		if (block.getHash() == hash) {
			index = i;
			break;
		}
	}
	if (index + 1 > Chain.size()) {
		std::cout << "No such block, try again:" << std::endl;
		goto selection;
	}
	else {
		Block block = Chain.getBlock(index);
		return block;
	}
}

