// Block-Chain.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Storage.h"
#include "Block.h"
#include <cstdlib>
#include <ctime>
#include <random>
user userGenerator(short i);
transaction txGenerator(int i, user* users);
void saveData(user* users, transaction* txPool, uint16_t userNR, uint16_t txNR);
void loadData(user* users, transaction* txPool, uint16_t userNR, uint16_t txNR);
Block_Chain initializeChain(transaction* txPool, uint16_t txNR);
short menu1();
short menu2();
Block getBlockByIndex(Block_Chain Chain, uint16_t txNR);
Block getBlockByHash(Block_Chain Chain, uint16_t txNR);

int main()
{
	short select = 0;
	uint16_t userNR = 100;
	uint16_t txNR = 10000;
	user* users = new user[userNR];
	transaction* txPool = new transaction[txNR];
start:
	select = menu1();
	if (select == 1) {
		loadData(users, txPool, userNR, txNR);
	}
	else if (select == 2) {
		//Vartotoju generavimas
		for (uint16_t i = 0; i < userNR; i++) {
			users[i] = userGenerator(i);
		}
		//Transakciju generavimas
		for (uint16_t i = 0; i < txNR; i++) {
			txPool[i] = txGenerator(i, users);
		}
		saveData(users, txPool, userNR, txNR);
	}
	else if (select == 3) {
		return 0;
	}
	else {
		std::cout << "Invalid input try again";
		goto start;
	}
	Block_Chain Chain = initializeChain(txPool, txNR);
menu:
	select = menu2();
	if (select == 1) {
		for (short i = 0; i < txNR / 100; i++) {
			Block block = Chain.getBlock(i);
			block.print2();
		}
		std::cout << std::endl;
		goto menu;
	}
	else if (select == 2) {
		Block block = getBlockByIndex(Chain, txNR);
		if (block.getHash() != "") {
			block.print1();
		}
		std::cout << std::endl;
		goto menu;
	}
	else if (select == 3) {
		Block block = getBlockByHash(Chain, txNR);
		if (block.getHash() != "") {
			block.print1();
		}
		std::cout << std::endl;
		goto menu;
	}
	else if (select == 4) {
		std::string hash;
		Block block = getBlockByIndex(Chain, txNR);
		if (block.getHash() == "") {
			std::cout << std::endl;
			goto menu;
		}
		std::cout << "Input transaction hash:" << std::endl;
input:
		std::cin >> hash;
		if (hash == "-1") {
			std::cout << std::endl;
			goto menu;
		}
		transaction* blockTx = block.getData();
		for (short i = 0; i < 100; i++) {
			if (blockTx[i].getHash() == hash) {
				blockTx[i].print();
				break;
			}
			else if (i == 99) {
				std::cout << "No such transaction, try again:" << std::endl;
				goto input;
			}
		}
		std::cout << std::endl;
		goto menu;
	}
	else if (select == 5) {
		return 0;
	}
	else {
		std::cout << "invalid input" << std::endl;
		std::cout << std::endl;
		goto menu;
	}
}

user userGenerator(short i) {
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> distr(100, 1000000);

	std::string name = "user" + std::to_string(i);
	std::string pbKey = Hash(name);
	uint32_t balance = distr(eng);
	user newUser(name, pbKey, balance);
	return newUser;
}

transaction txGenerator(int i, user* users) {
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> distr(0, 99);
	uint16_t n = distr(eng);
	uint16_t m = -1;
	do {
		m = distr(eng);
	} while (n == m);
	//std::cout << n << " " << m << std::endl;
	std::string senderHash = users[n].getPubKey();
	//std::cout << senderHash << std::endl;
	std::string senderName = users[n].getName();
	std::string receiverHash = users[m].getPubKey();
	std::string receiverName = users[m].getName();

	std::uniform_int_distribution<> distr1(0, 10000);
	uint32_t amount = distr1(eng);

	transaction newTx(senderHash, receiverHash, senderName, receiverName, amount);
	return newTx;
}
