#  vulnCoin

vulnCoin is a very vulnerable blockchain server powered by the PoS protocol. Implementation offers tools to run the proof of stake consensus protocol, but it does not neccesarily enforce them. We aimed to create the server as simplistic as possible, while providing the tools for a simulation of a run of a real PoS network.

## 1. Accounts 

Upon starting the server, its address is generated from a received string. This address is further reffered to as `vulnCoin address` and is a hexadecimal string of the length of 64 characters. Network does not use any kind of assymetric cryptography, as we attempted to make the server as simplistic as possible. In doing so, we opened the network of vulnCoin servers to a crucial attack vector - anybody can spend a transaction output of anybody else. We overcome this issue by stating that in our simulations, we expect all network nodes to act honestly, thus simulating as if the network was protected against said issues.

## 2. Transactions

The server employs a simplified UTXO model, where newly created outputs do not reference previous inputs. They are, however, moved between the mempool, the stakepool and the pool of spendable outputs. For a transaction to become spendable, it must be embedded into a block. The transaction outputs have multiple properties, namely:
* Address
    * The address of the holder of the output.
* Ammount
    * The ammount of cryptocurrency held in the transaction, for simplicity purposes always set to $10$.
* Passed
    * Logs how many times the coin was passed in the network, creating a notion of output chaining.
* Coinbase
    * A bool value defining whether the transaction output was created by a coinbase transaction.
* Timestamp
    * A timestamp referencing the time of the transaction creation.
* txid
    * The transaction ID is defined as $\text{sha256}(\text{address} || \text{ammount} || \text{passed} || \text{coinbase} || \text{timestamp})$, where $||$ is a standard string concatenation operator.
    * Even though many entropy generators are used, in our PoCs (with only three servers!) we always sleep for one second after creating a transaction, to completely remove the threat of duplicate txids existing in the network.

## 3. Block
Blocks consist of three properties:
* prevBlockHash
  * The hash of a block is defined as $\text{sha256}(\text{prevBlockHash} || \text{transactionsHash})$, where $||$ is a standard string concatenation operator.
* transactionsHash
  * The transaction hash is defined as $\text{sha256}(\text{txid}_0 || \text{txid}_1 ||\dots||\text{txid}_n)$, where $||$ is a standard string concatenation operator and $\text{txid}_0, \text{txid}_1, \dots,\text{txid}_n \in \text{Transactions}$.
* Transactions
  *  A vector of txids of the transactions embedded into the block.

Legitimacy of the cryptographic link upon block creation is ensured and we assume no node has an intent to break it. 
## 4. Consensus

The network does not enforce consensus, it cannot do so without employing more robust cryptographicall techniques. It does, however, equip us with the tools for employing the consensus. These tools are used to enforce the consensus in our PoC scripts. The consensus above the network is defined as following:

<img src="vulncoin_consensus_validation.png"> </img>

`t`
 * The `genesis` block is created automatically by the network.
 * The network users can stake for a chance to be selected as creator of the block `#1`, but the stakePool has to be finalised before the time `t+1`.

 `t+1`
 * The stakepool for the `genesis` block and the `genesis` block are used as inputs for the pseudorandom function that selects the block creator from the stakepool `#1`.
 * The stakepool for the block `#2` is opened.

 `t+2`
 * The stakepool for the block `#1` and the block `#1` are used as inputs for the pseudorandom function that selects the block creator from the stakepool `#2`.
 * The stakepool for the block `#3` is opened.

 The whole process continues in the same fashion until the network is stopped.
 
 Let $H$ represent `SHA256` hash of the last block in the blockchain, $\{a_1, a_2, \dots,a_n\} \in S$ a stakepool where each vulnCoin address $a$ of the coin holder is represented as hexadecimal string and $N$ number of addresses in the current stakepool. Furthermore, consider the function $\text{first32bits}()$ that returns the first 32 bits of a string. The pseudorandom selection function is implemented as:

 $$ \text{IndexOfValidator} \equiv (\text{first32bits}(H) + \text{first32bits}(a_1) + \dots + \text{first32bits}(a_n))\mod{N} $$

## 5. Coin age
Due to the fact that we wanted to create a PoC of the coin age accumulation attack, we needed to implement an option for the coin age being a factor in the consensus process. The whole concept is implemented with a flag passed during the start of a server. The implementation is very simplistic and ineffective, but it is enough for the purposes of the PoCs.

## 6. Commands
To communicate with the server, following set of commands is used:
*   `stop` 
    * Command used to stop the running server.
    * `return`: **null**
*   `getBlockCount`
    * Command used to get number of blocks in the blockchain on the server.
    * `return`: **Number of blocks in the blockchain**
*   `printBlockchain`
    * Print all blocks in the blockchain in order they were appended.
    * `return`: **String representing the blockchain information**
*   `createNewTransaction <txid> <addressSender> <addressReceiver>`
    * Create a new transaction specified by txid and vulncoin address of existing UTXO and address of the receiver.
    * `return`: **Txid of the newly created transaction. If the transaction couldn't have been created, error message.**
*   `loadTransaction <txid> <addressSender> <addressReceiver> <timestamp>`
    * Load an already created transaction. Timestamp needs to be defined to correctly count the new txid.
    * `return`: **null or error message**
*   `loadCoinbaseTransaction <addressReceiver> <timestamp>`
    * Load an already created coinbase transaction. Timestamp needs to be defined to correctly count the new txid.
    * `return`: **Txid of the newly loaded transaction**
*   `proposeBlock {<txid1> <txid2> ... <txidn>}` 
    * Propose a new block with specified txids to be created.
    * `return`: **null or error message in case of failure**
*   `stake <txid> <addressSender>` 
    * Stake transaction specified by txid and address of the holder.
    * `return`: **null or error message in case of failure**
*   `countNextValidator` 
    * Count the creator of the next block based on the stakepool with respect to coinAge flag.
    * `return`: **vulnCoin address of the selected staker**
*   `listMempool`
    * Print current mempool state.
    * `return`: **Transactions in the mempool**
*   `listMempoolLinkedToMe` 
    * Print transactions from the mempool linked to the server address.
    * `return`: **Transactions in the mempool linked to vulnCoin address of the server**
*   `listUnspent` 
    * Print currently unspent transactions.
    * `return`: **Spendable UTXOs**
*   `listUnspentLinkedToMe` 
    * Print currently unspent transactions linked to server vulnCoin address.
    * `return`: **Spendable UTXOs linked to the server vulnCoin address** 
*   `listStakepool` 
    * Print the stakepool with respect to coinAge flag.
    * `return`: **Transactions in the stakepool**
*   `listOldStakepool`
    * Print the stakepool of previous consensus round with respect to coinAge flag.
    * `return`: **Transactions in the stakepool of previous consensus round**
*   `generate` 
    * Generate single block containing only a coinbase transaction to the vulnCoin address of the server.
    * `return`: **null**
*   `printAddress`
    * Print vulnCoin address of the server.
    * `return`: **vulnCoin address of the server**
*   `printTransaction <txid>`
    * Print transaction defined by txid.
    * `return`: **Contents of the searched transaction**

## 7. Installation and Start
To install the server, clone the repository and build a binary by initiating `make` command. Then issue:

`sudo mv bin/vulnCoin-server /bin/vulnCoin-server`

To start the server, simply issue:

`vulnCoin-server <entropy_string> <portNumber> <coinAge>`
