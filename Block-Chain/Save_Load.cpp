#include "stdafx.h"
#include "storage.h"
#include "Block.h"

void saveData(std::vector<user> &users, std::vector<transaction> &txPool, uint16_t userNR, uint16_t txNR) {
	std::ofstream fu;
	std::ofstream ftx;
	fu.open("Users.txt", std::ios::out);
	ftx.open("Tx Pool.txt", std::ios::out);
	for (uint16_t i = 0; i < userNR; i++) {
		fu << users[i].asString();
		fu << std::endl;
	}
	for (uint16_t i = 0; i < txNR; i++) {
		ftx << txPool[i].asString();
		ftx << std::endl;
	}
	fu.close();
	ftx.close();
}

void loadData(std::vector<user> &users, std::vector<transaction> &txPool, uint16_t userNR, uint16_t txNR) {
	std::ifstream fu;
	std::ifstream ftx;
	fu.open("Users.txt");
	ftx.open("Tx Pool.txt");
	for (uint16_t i = 0; i < userNR; i++) {
		std::string name, pbKey, balance;
		fu >> name;
		fu >> pbKey;
		fu >> balance;
		user newUser(name, pbKey, stoi(balance));
		users[i] = newUser;
	}
	for (uint16_t i = 0; i < txNR; i++) {
		std::string hf, ht, nf, nt, am;
		ftx >> hf;
		ftx >> ht;
		ftx >> nf;
		ftx >> nt;
		ftx >> am;
		transaction newTx(hf, ht, nf, nt, stoi(am));
		txPool[i] = newTx;
	}
	fu.close();
	ftx.close();
}


