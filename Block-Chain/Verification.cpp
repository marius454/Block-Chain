#include "stdafx.h"
#include "Block.h"
#include "Storage.h"
#include <algorithm>

void verification(std::vector<transaction> &txPool, std::vector<user> &users) {
	uint16_t index = 0;
	for (auto &tx : txPool) {
		std::string check = tx.getSenderHash();
		auto it = std::find_if(users.begin(), users.end(), [&check](user& obj) {return obj.getPubKey() == check; });
		if (it != users.end()) {
			if ((*it).getBalance() <= tx.getAmount()) {
				txPool.erase(txPool.begin() + index);
			}
			else if (tx.recalculateHash() != tx.getHash()) {
				txPool.erase(txPool.begin() + index);
			}
		}
		index++;
	}
}