#include "stdafx.h"
#include "storage.h"
#include "Block.h"
#include <random>

Block_Chain initializeChain(transaction* txPool, uint16_t txNR) {
	short cnt = 0;
	transaction* blockTx = new transaction[100];
	for (uint16_t i = 0; i < 100; i++) {
		blockTx[i] = txPool[i];
		blockTx[i].setHeight(cnt);
	}
	cnt++;
	Block_Chain Chain(blockTx);
	while (cnt * 100 < txNR) {
		transaction* blockTx = new transaction[100];
		for (uint16_t i = 0; i < 100; i++) {
			blockTx[i] = txPool[i + cnt*100];
			blockTx[i].setHeight(cnt);
		}
		cnt++;
		Chain.addBlock(blockTx);
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

Block getBlockByIndex(Block_Chain Chain, uint16_t txNR) {
	int16_t index = 0;
	std::cout << "Input block index:" << std::endl;
selection:
	std::cin >> index;
	if ((index + 1) * 100 > txNR) {
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

Block getBlockByHash(Block_Chain Chain, uint16_t txNR) {
	uint16_t index = txNR;
	std::string hash;
	std::cout << "Input block hash:" << std::endl;
selection:
	std::cin >> hash;
	if (hash == "-1") {
		Block block;
		return block;
	}
	for (short i = 0; i < txNR / 100; i++) {
		Block block = Chain.getBlock(i);
		if (block.getHash() == hash) {
			index = i;
			break;
		}
	}
	if ((index + 1) * 100 > txNR) {
		std::cout << "No such block, try again:" << std::endl;
		goto selection;
	}
	else {
		Block block = Chain.getBlock(index);
		return block;
	}
}

