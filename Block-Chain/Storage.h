#pragma once
#include "stdafx.h"

std::string manoHash(std::string input);

class user
{
private:
	std::string name;
	//public key
	std::string pbKey;
	uint32_t balance;
public:
	user() {}
	user(std::string name_, std::string pbKey_, uint32_t balance_) {
		name = name_;
		pbKey = pbKey_;
		balance = balance_;
	};
	std::string getName() {
		return name;
	};
	std::string getPubKey() {
		return pbKey;
	};
	uint32_t getBalance() {
		return balance;
	};
	void setBalance(uint32_t newBalance) {
		balance = newBalance;
	};
	void print() {
		std::cout << "(" << name << ", " << pbKey << ", " << balance << std::endl;
	}
	std::string asString() {
		std::string s;
		s = name + " " + pbKey + " " + std::to_string(balance);
		return s;
	}
};

class transaction {
private:
	//transaction hash
	std::string txHash;
	//Block Height
	uint32_t bHeight;
	time_t time;
	//sender public hash key and name
	std::string hashFrom;
	std::string nameFrom;
	//receiver public hash key and name
	std::string hashTo;
	std::string nameTo;
	uint32_t amount;

	void hashTx(std::string info) {
		txHash = manoHash(info);
	}
public:
	transaction() {};
	transaction(std::string hashFrom_, std::string hashTo_, std::string nameFrom_, std::string nameTo_, uint32_t amount_) {
		hashFrom = hashFrom_;
		hashTo = hashTo_;
		nameFrom = nameFrom_;
		nameTo = nameTo_;
		amount = amount_;
		time = std::time(nullptr);
		bHeight = 0;
		std::string txInfo = hashFrom + hashTo + nameFrom + nameTo + std::to_string(amount);
		hashTx(txInfo);
	}
	std::string recalculateHash() {
		std::string txInfo = hashFrom + hashTo + nameFrom + nameTo + std::to_string(amount);
		return manoHash(txInfo);
	}
	std::string getSenderHash() {
		return hashFrom;
	}
	uint32_t getAmount() {
		return amount;
	}
	std::string getHash() {
		return txHash;
	}
	void setHeight(uint32_t height) {
		bHeight = height;
	}
	void print() {
		std::cout << "{" << std::endl;
		std::cout << "Transaction hash: " << txHash << std::endl;
		std::cout << "Sender name: " << nameFrom << ", sender public key: " << hashFrom << std::endl;
		std::cout << "Receiver name: " << nameTo << ", receiver public key: " << hashTo << std::endl;
		std::cout << "Transfer amount: " << amount << " currency" << std::endl;
		std::cout << "}" << std::endl;
	}
	std::string asString() {
		std::string s;
		s = hashFrom + " " + hashTo + " " + nameFrom + " " + nameTo + " " + std::to_string(amount);
		return s;
	}
};