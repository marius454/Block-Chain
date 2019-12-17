# Block-Chain

## Mano Block-Chain realizacija

## v0.1

Generuojami atsitiktiniai vartotojai ir transakcijų pool'as:

```cpp
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
```


Naudojant sugeneruotus duomenis imama po 100 transakcijų ir jos dedamos į blokus kurių hashas tada yra kasamas ("mininamas"). Iškasti blokai tada yra dedami i blokų grandinę(block-chain). Kasimo funkcija:

```cpp
void mineBlock(uint8_t difficulty) {
	char* cstr = new char[difficulty + 1];
	for (uint8_t i = 0; i < difficulty; ++i) {
		cstr[i] = '0';
	}
	cstr[difficulty] = '\0';
	std::string str(cstr);
  std::string Hash;
	do {
		Hash = calculateHash();
		nonce++;
	} while (Hash.substr(0, difficulty) != str && nonce < maxiter - 1);
  hash = Hash;
  }
```


## v0.2

Sukurta transakcijų verifikavimo funkcija kuri patikrina ar siuntėjas turi pakankamai lėšų kad įvykdyti transakciją, taip pat iš naujo hash'uojami transakcijos duomenys, kad verifikuoti transakcijas. Verifikavimo funkcija:

```cpp
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
```

Taip pat vietoj vieno bloko pridėjimo po kito iš eilės, kuriami 5 blokai kandidatai iš kurių vienas gali tapti sekančiu blokų grandinės nariu. Kandidatai yra kasami iki 1000 iteracijų/bandymų jei nėra randamas hash'as atitinkantis sudėtingumo (difficulty) reikalavimus einama prie sekančio atsitiktinai paimto bloko ir bandoma iškastį jį. Jei nei vienas iš kandidatų nebuvo iškastas per 1000 iteracijų iteracijų limitas yra padidinamas pridedant dar 1000 iteracijų taip daroma kol bus gautas vienas sėkmingas kandidatas.

--------- pridėti išvesties kai prieisiu prie laptop --------------------

## v0.3

Sukuriama create_merkle implementacija nenaudojant libbitcoin bibliotekos:

```cpp
std::string Block::create_merkle(std::vector<transaction>& data) {
	uint16_t size = data.size();
	std::vector<std::string> merkle(size);
	for (uint16_t i = 0; i < size; i++) {
		merkle[i] = data[i].getHash();
	}
	// Stop if hash list is empty or contains one element
	if (merkle.empty())
		return "";
	else if (merkle.size() == 1)
		return merkle[0];

	// While there is more than 1 hash in the list, keep looping...
	while (merkle.size() > 1)
	{
		// If number of hashes is odd, duplicate last hash in the list.
		if (merkle.size() % 2 != 0)
			merkle.push_back(merkle.back());
		// List size is now even.
		assert(merkle.size() % 2 == 0);

		// New hash list.
		std::vector<std::string> new_merkle;
		// Loop through hashes 2 at a time.
		for (auto it = merkle.begin(); it != merkle.end(); it += 2)
		{
			// Join both current hashes together (concatenate).
			std::string concat_data = *(it)+*(it + 1);
			// Hash both of the hashes.
			std::string new_root = manoHash(concat_data);
			// Add this to the new list.
			new_merkle.push_back(new_root);
		}
		// This is the new list.
		merkle = new_merkle;
	}
	// Finally we end up with a single item.
	return merkle[0];
}
```

Taip iš daug transakcijų hash'u gaunamas vienas unikalus hash'as kuris užtikrina kad bloko kasimas visada turės ta patį sudėtinguma nepriklausomai nuo to kiek transakcijų yra bloke.
