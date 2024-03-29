// Block-Chain.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Storage.h"
#include "Block.h"
#include <cstdlib>
#include <ctime>
#include <random>
#include <streambuf>
user userGenerator(short i);
transaction txGenerator(int i, std::vector<user> users);
void saveData(std::vector<user> &users, std::vector<transaction> &txPool, uint16_t userNR, uint16_t txNR);
void loadData(std::vector<user> &users, std::vector<transaction> &txPool, uint16_t userNR, uint16_t txNR);
Block_Chain initializeChain(std::vector<transaction> &txPool, uint16_t txNR);
short menu1();
short menu2();
Block getBlockByIndex(Block_Chain Chain);
Block getBlockByHash(Block_Chain Chain);
void verification(std::vector<transaction> &txPool, std::vector<user> &users);
std::string manoHash(std::string input);

int main()
{
	short select = 0;
	uint16_t userNR = 100;
	uint16_t txNR = 1000;
	std::vector<user> users(userNR);
	std::vector<transaction> txPool(txNR);
start:
	select = menu1();
	auto t1 = std::chrono::high_resolution_clock::now();
	if (select == 1) {
		loadData(users, txPool, userNR, txNR);
		verification(txPool, users);
		//std::cout << txPool.size() << std::endl;
		txNR = txPool.size();
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
	auto t2 = std::chrono::high_resolution_clock::now();
	Block_Chain Chain = initializeChain(txPool, txNR);
	auto t3 = std::chrono::high_resolution_clock::now();
	long long duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t2).count();
	long long duration2 = std::chrono::duration_cast<std::chrono::microseconds>(t3 - t1).count();
	double dur = (double)duration1 * 0.000001;
	std::cout << "Block-Chain mining time: " << dur << "s" << std::endl;
	dur = (double)duration2 * 0.000001;
	std::cout << "Total time: " << dur << "s" << std::endl;
menu:
	select = menu2();
	if (select == 1) {
		for (size_t i = 0; i < Chain.size(); i++) {
			Block block = Chain.getBlock(i);
			block.print2();
		}
		std::cout << std::endl;
		goto menu;
	}
	else if (select == 2) {
		Block block = getBlockByIndex(Chain);
		if (block.getHash() != "") {
			block.print1();
		}
		std::cout << std::endl;
		goto menu;
	}
	else if (select == 3) {
		Block block = getBlockByHash(Chain);
		if (block.getHash() != "") {
			block.print1();
		}
		std::cout << std::endl;
		goto menu;
	}
	else if (select == 4) {
		std::string hash;
		Block block = getBlockByIndex(Chain);
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
		std::vector<transaction> blockTx = block.getData();
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
	std::string pbKey = manoHash(name);
	uint32_t balance = distr(eng);
	user newUser(name, pbKey, balance);
	return newUser;
}

transaction txGenerator(int i, std::vector<user> users) {
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> distr(0, 99);
	uint16_t n = distr(eng);
	uint16_t m = -1;

	do {
		m = distr(eng);
	} while (n == m);

	std::string senderHash = users[n].getPubKey();
	std::string senderName = users[n].getName();
	std::string receiverHash = users[m].getPubKey();
	std::string receiverName = users[m].getName();

	std::uniform_int_distribution<> distr1(0, 10000);
	uint32_t amount = distr1(eng);

	transaction newTx(senderHash, receiverHash, senderName, receiverName, amount);
	return newTx;
}
