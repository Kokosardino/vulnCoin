#include "CTransactionContainer.h"

void CTransactionContainer::addTransaction(const CTransaction transaction) {
    m_container.insert(std::make_pair(transaction.m_txid, transaction));
}

CTransaction CTransactionContainer::getTransactionByTxid(const std::string txid) {
    //Segfaults if the txid does not exist!
    return (m_container.find(txid))->second;
}

std::string CTransactionContainer::print() const {
    //For purposes of proper JSON formatting, we need to find a last item in the map.
    std::map<std::string, CTransaction>::const_iterator lastIt = m_container.end();
    if (!m_container.empty()) {
        --lastIt;
    }

    //Use ostringstream to properly format the output string.
    std::ostringstream oss;
    oss << "[" << std::endl;
    for (std::map<std::string, CTransaction>::const_iterator it = m_container.begin(); it != m_container.end(); ++it) {
        oss << it->second.print();
        if (it != lastIt) {
            oss << ",";
        }
        oss << std::endl;
    }
    oss << "]" << std::endl;
    return oss.str();
}

std::string CTransactionContainer::print(const std::string &address) const {
    //For purposes of proper JSON formatting, we need to find a last item in the map tied to the specified address.
    std::map<std::string, CTransaction>::const_iterator lastIt = m_container.end();
    if (!m_container.empty()) {
        --lastIt;
    }
    while (lastIt != m_container.begin() && address != lastIt->second.m_address) {
        --lastIt;
    }

    //Use ostringstream to properly format the output string.
    std::ostringstream oss;
    oss << "[" << std::endl;
    for (std::map<std::string, CTransaction>::const_iterator it = m_container.begin(); it != m_container.end(); ++it) {
        if (address == it->second.m_address) {

            oss << it->second.print();
            if (lastIt != it) {
                oss << ",";
            }
            oss << std::endl;
        }
    }
    oss << "]" << std::endl;
    return oss.str();
}

std::string CTransactionContainer::printTransaction(const std::string &txid) const {
    std::map<std::string, CTransaction>::const_iterator found = m_container.find(txid);
    if (found != m_container.end()) {
        return found->second.print();
    }
    return "";
}

bool CTransactionContainer::findTransactions(const std::vector <std::string> &transactions) const {
    for (const std::string &txid: transactions) {
        if (m_container.find(txid) == m_container.end()) {
            return 0;
        }
    }
    return 1;
}

int CTransactionContainer::returnPassed(const std::string &txid, const std::string &address) {
    std::map<std::string, CTransaction>::iterator it = m_container.find(txid);

    if (it != m_container.end() && it->second.m_address == address) {
        return it->second.m_passed;
    }

    return -1;
}

void CTransactionContainer::removeTransactionByTxid(const std::string &txid) {
    std::map<std::string, CTransaction>::iterator it = m_container.find(txid);
    if (it != m_container.end()) {
        m_container.erase(it);
    }
}

size_t CTransactionContainer::size() const {
    return m_container.size();
}

CTransaction CTransactionContainer::operator[](size_t index) {
    std::map<std::string, CTransaction>::iterator it = m_container.begin();
    std::advance(it, index);
    return it->second;
}
