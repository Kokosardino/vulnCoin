XX=g++
CXXFLAGS=-Wall -pedantic -std=c++17 -c 
LDFLAGS=-Wall -pedantic -std=c++17 
LD=g++


make: bin/vulnCoin-server

bin/vulnCoin-server:  build/CCryptography.o build/CServer.o build/CBlockchain.o build/CBlock.o build/main.o build/CTransaction.o build/CTransactionContainer.o 
	mkdir -p $(@D)
	$(LD) $(LDFLAGS) build/CCryptography.o build/CServer.o build/CBlockchain.o build/CBlock.o build/CTransaction.o build/main.o build/CTransactionContainer.o -o bin/vulnCoin-server -lcrypto -lssl
	
build/%.o: src/%.cpp 
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $< -o $@ -lcrypto -lssl

build/main.o: src/main.cpp 
build/CCryptography.o: src/CCryptography.h src/CCryptography.cpp
build/CServer.o: src/CServer.h src/CServer.cpp 
build/CBlock.o: src/CBlock.h src/CBlock.cpp 
build/CBlockchain.o: src/CBlockchain.h src/CBlockchain.cpp
build/CTransaction.o: src/CTransaction.h src/CTransaction.cpp
build/CTransactionContainer.o: src/CTransactionContainer.h src/CTransactionContainer.cpp
clean: 
	rm -r build bin
