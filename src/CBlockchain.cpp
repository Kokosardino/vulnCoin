#include "CBlockchain.h"

CBlockchain::CBlockchain() {
    //Create genesis block.
    m_blockchain.emplace_back(CBlock("0000000000000000000000000000000000000000000000000000000000000000",
                                     "0000000000000000000000000000000000000000000000000000000000000000",
                                     std::vector<std::string>()));
}

std::string CBlockchain::generateToAddress(const std::string &address, CTransactionContainer &transactions) {
    //Create a standard coinbase transaction to the issuer of the command.
    CTransaction coinbaseTrans(address, 10, 0, true);
    //Append the block containing the coinbase transaction into the blockchain.
    CCryptography crypto;
    m_blockchain.emplace_back(
            CBlock(m_blockchain[m_blockchain.size() - 1].getBlockHash(), crypto.sha256(coinbaseTrans.m_txid),
                   {coinbaseTrans.m_txid}));

    //Add new transaction into the received container (should always be unspentTransactions pool, as the newly created coinbase transaction is immediately validated).
    transactions.addTransaction(coinbaseTrans);

    //Return hash of the newly created block.
    return m_blockchain[m_blockchain.size() - 1].getBlockHash();
}

void CBlockchain::addBlock(const std::vector <std::string> &transactions) {
    std::string transactionHash;
    if (transactions.empty()) {
        //Use dummy transactionHash for empty set of transactions.
        transactionHash = "0000000000000000000000000000000000000000000000000000000000000000";
    } else {
        //Transaction hash is defined as sha256 hash of embedded txids.
        std::ostringstream oss;
        for (const std::string &txid: transactions) {
            oss << txid;
        }
        transactionHash = oss.str();
    }

    CCryptography crypto;
    //Append the block to the blockchain.
    m_blockchain.emplace_back(
            CBlock(m_blockchain[m_blockchain.size() - 1].getBlockHash(), crypto.sha256(transactionHash), transactions));
}

std::string CBlockchain::print() const {
    //Use ostringstream to output the string in a JSON format.
    std::ostringstream oss;
    oss << "[" << std::endl;
    for (size_t i = 0; i < m_blockchain.size(); ++i) {
        oss << m_blockchain[i].print();
        if (i != m_blockchain.size() - 1) {
            oss << ",";
        }
        oss << std::endl;
    }
    oss << "]" << std::endl;

    return oss.str();
}

size_t CBlockchain::getBlockCount() const {
    return m_blockchain.size();
}

int CBlockchain::getTransactionIndex(const std::string &txid) const {
    for (size_t i = 0; i < m_blockchain.size(); ++i) {
        if (m_blockchain[i].containsTransaction(txid)) {
            return i;
        }
    }
    return -1;
}

std::string CBlockchain::getLastBlockHash() const {
    return m_blockchain[m_blockchain.size() - 1].getBlockHash();
}

