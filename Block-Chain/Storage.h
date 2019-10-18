#pragma once


#include "stdafx.h"

class user
{
	std::string name;
	//publick key
	std::string pbKey;
	uint32_t balance;
	user(std::string _name, std::string _pbKey, uint32_t _balance) {
		name = _name;
		pbKey = _pbKey;
		balance = _balance;
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
};

class transaction {
	//transaction hash
	std::string txHash;
	//Block Height
	uint16_t bHeight;
	time_t time;
	//sender public hash key
	std::string hashFrom;
	//receiver publick hash key
	std::string hashTo;
	uint32_t amount;
};