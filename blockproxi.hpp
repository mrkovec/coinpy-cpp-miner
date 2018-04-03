
#ifndef COINPY_CPP_MINER_BLOCKPROXI_HPP
#define COINPY_CPP_MINER_BLOCKPROXI_HPP

#include <iostream>

namespace cminer {

	class BlockProxi
	{
	  public:
	    BlockProxi(std::string t_jsonString);
	    std::string mine();
	    bool validateDifficulty();
	    std::string getField(std::string t_field);
	    void setField(std::string t_field, std::string t_value);
	  private:
	    std::string m_jsonString;
	    unsigned int m_nonce = 0;
	    unsigned int m_difficulty = 1;
	};
}

#endif //COINPY-CPP-MINER_BLOCKPROXI_HPP
