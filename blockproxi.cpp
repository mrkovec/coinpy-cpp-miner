#include "blockproxi.hpp"
#include "picosha2.h"

namespace cminer {

	BlockProxi::BlockProxi(std::string t_jsonString) : m_jsonString(t_jsonString) {
		m_nonce = std::stoi(getField("n"));
		m_difficulty = std::stoi(getField("dif"));
	}

	std::string BlockProxi::mine() {
		while(true) {
			if(validateDifficulty()) {
				return m_jsonString;
			}
			++m_nonce;
			setField("n", std::to_string(m_nonce));
		}
	}

	bool BlockProxi::validateDifficulty() {
		std::vector<unsigned char> hash(picosha2::k_digest_size);
		picosha2::hash256(m_jsonString.begin(), m_jsonString.end(), hash.begin(), hash.end());
		for(unsigned int i = 0; i < m_difficulty; ++i) {
			if(hash.at(i) != 0)	{
				return false;
			}
		}
		return true;
	}

	std::string BlockProxi::getField(std::string t_field) {
		std::string ndl = "\"" + t_field + "\": ";
		std::size_t a = m_jsonString.find(ndl);
		a += ndl.length();
		std::size_t b = m_jsonString.find(",", a + 1);
		return m_jsonString.substr(a, b - a);
	}

	void BlockProxi::setField(std::string t_field, std::string t_value) {
		std::string ndl = "\"" + t_field + "\": ";
		std::size_t a = m_jsonString.find(ndl);
		a += ndl.length();
		std::size_t b = m_jsonString.find(",", a + 1);
		m_jsonString = m_jsonString.substr(0, a) + t_value + m_jsonString.substr(b, m_jsonString.length());
	}
}

#if defined(COINPY_CPP_MINER_SELFTEST)
int main() {
  cminer::BlockProxi block("{\"ver\": 1, \"n\": 0, \"time_stamp\": 1522738861.581881, \"prev_block\": \"\", \"height\": 0, \"dif\": 2, \"trxs\": []}");
  if(block.mine() == "{\"ver\": 1, \"n\": 7940, \"time_stamp\": 1522738861.581881, \"prev_block\": \"\", \"height\": 0, \"dif\": 2, \"trxs\": []}") {
		std::cout << "ok";
		return 0;
	}
	std::cerr << "error";
	return -1;
}
#endif
