#compdef in3

local -a subcmds args sig_in3 sig_erc20 sig_ms
subcmds=(
    'send: sends a transaction <signature> ...args'
    'call: calls a contract <signature> ...args'
    'abi_encode: encodes the arguments as described in the method signature using ABI-Encoding. <signature> ...args'
    'abi_decode: decodes the data based on the signature.. <signature> <data>'
    'pk2address: extracts the public address from a private key <pkdata>'
    'pk2public: extracts the public key from a private key <pkdata>'
    'ecrecover: extracts the address and public key from a signature <msg> <signature>'
    'key: reads the private key from JSON-Keystore file and returns the private key. <keyfile>'
    'createkey: generates a random key'
    'in3_checksumAddress: display the address as checksumAddress'
    'keccak: calculate the keccak hash of the <arg>'
    'btc_proofTarget: Whenever the client is not able to trust the changes of the target (which is the case if a block can... <target_dap> <verified_dap> <max_diff> <max_dap> <limit>'
    'getbestblockhash: Returns the hash of the best (tip) block in the longest blockchain'
    'getblock: Returns data of block for given block hash <hash> <verbosity>'
    'getblockcount: Returns the number of blocks in the longest blockchain'
    'getblockheader: Returns data of block header for given block hash <hash> <verbosity>'
    'getdifficulty: Returns the proof-of-work difficulty as a multiple of the minimum difficulty <blocknumber>'
    'getrawtransaction: Returns the raw transaction data <txid> <verbosity> <blockhash>'
    'eth_blockNumber: returns the number of the most recent block'
    'eth_call: calls a function of a contract (or simply executes the evm opcodes) and returns the result <tx> <block>'
    'eth_estimateGas: calculates the gas needed to execute a transaction <tx> <block>'
    'eth_getBalance: gets the balance of an account for a given block <account> <block>'
    'eth_getBlockByHash: Returns information about a block by hash <blockHash> <fullTx>'
    'eth_getBlockByNumber: returns information about a block by block number <blockNumber> <fullTx>'
    'eth_getBlockTransactionCountByHash: returns the number of transactions <blockHash>'
    'eth_getBlockTransactionCountByNumber: returns the number of transactions <blockNumber>'
    'eth_getCode: gets the code of a given contract <account> <block>'
    'eth_getLogs: searches for events matching the given criteria <filter>'
    'eth_getStorageAt: gets the storage value of a given key <account> <key> <block>'
    'eth_getTransactionByBlockHashAndIndex: returns the transaction data <blockHash> <index>'
    'eth_getTransactionByBlockNumberAndIndex: returns the transaction data <blockNumber> <index>'
    'eth_getTransactionByHash: returns the transaction data <txHash>'
    'eth_getTransactionCount: gets the nonce or number of transaction sent from this account at a given block <account> <block>'
    'eth_getTransactionReceipt: The Receipt of a Transaction <txHash>'
    'eth_getUncleCountByBlockHash: returns the number of uncles <blockHash>'
    'eth_getUncleCountByBlockNumber: returns the number of uncles <blockNumber>'
    'eth_sendRawTransaction: sends or broadcasts a prviously signed raw transaction <tx>'
    'eth_sendTransaction: signs and sends a Transaction <tx>'
    'eth_sendTransactionAndWait: signs and sends a Transaction, but then waits until the transaction receipt can be verified <tx>'
    'eth_sign: The sign method calculates an Ethereum specific signature with:  <account> <message>'
    'eth_signTransaction: Signs a transaction that can be submitted to the network at a later time using with eth_sendRawTrans... <tx>'
    'keccak: Returns Keccak-256 (not the standardized SHA3-256) of the given data'
    'net_version: the Network Version (currently 1)'
    'sha256: Returns sha-256 of the given data <data>'
    'web3_clientVersion: Returns the underlying client version'
    'web3_sha3: Returns Keccak-256 (not the standardized SHA3-256) of the given data <data>'
    'eth_accounts: returns a array of account-addresss the incubed client is able to sign with'
    'in3_abiDecode: based on the  <signature> <data>'
    'in3_abiEncode: based on the  <signature> <params>'
    'in3_addRawKey: adds a raw private key as signer, which allows signing transactions <pk>'
    'in3_cacheClear: clears the incubed cache (usually found in the '
    'in3_checksumAddress: Will convert an upper or lowercase Ethereum address to a checksum address <address> <useChainId>'
    'in3_config: changes the configuration of a client <config>'
    'in3_decryptKey: decrypts a JSON Keystore file as defined in the  <key> <passphrase>'
    'in3_ecrecover: extracts the public key and address from signature <msg> <sig> <sigtype>'
    'in3_ens: resolves a ens-name <name> <field>'
    'in3_fromWei: converts a given uint (also as hex) with a wei-value into a specified unit <value> <unit> <digits>'
    'in3_nodeList: fetches and verifies the nodeList from a node <limit> <seed> <addresses>'
    'in3_pk2address: extracts the address from a private key <pk>'
    'in3_pk2public: extracts the public key from a private key <pk>'
    'in3_prepareTx: prepares a Transaction by filling the unspecified values and returens the unsigned raw Transaction <tx>'
    'in3_sign: requests a signed blockhash from the node <blocks>'
    'in3_signData: signs the given data <msg> <account> <msgType>'
    'in3_signTx: signs the given raw Tx (as prepared by in3_prepareTx ) <tx> <from>'
    'in3_toWei: converts the given value into wei <value> <unit>'
    'in3_whitelist: Returns whitelisted in3-nodes addresses <address>'
    'ipfs_get: Fetches the data for a requested ipfs-hash <ipfshash> <encoding>'
    'ipfs_put: Stores ipfs-content to the ipfs network <data> <encoding>'
    'zksync_account_address: returns the address of the account used'
    'zksync_account_info: returns account_info from the server <address>'
    'zksync_aggregate_pubkey: calculate the public key based on multiple public keys signing together using schnorr musig signatur... <pubkeys>'
    'zksync_contract_address: returns the contract address'
    'zksync_deposit: sends a deposit-transaction and returns the opId, which can be used to tradck progress <amount> <token> <approveDepositAmountForERC20> <account>'
    'zksync_emergency_withdraw: withdraws all tokens for the specified token as a onchain-transaction <token>'
    'zksync_ethop_info: returns the state or receipt of the the PriorityOperation <opId>'
    'zksync_get_token_price: returns current token-price <token>'
    'zksync_get_tx_fee: calculates the fees for a transaction <txType> <address> <token>'
    'zksync_pubkey: returns the current packed PubKey based on the config set'
    'zksync_pubkeyhash: returns the current PubKeyHash based on the configuration set <pubKey>'
    'zksync_set_key: sets the signerkey based on the current pk or as configured in the config <token>'
    'zksync_sign: returns the schnorr musig signature based on the current config <message>'
    'zksync_sync_key: returns private key used for signing zksync-transactions'
    'zksync_tokens: returns the list of all available tokens'
    'zksync_transfer: sends a zksync-transaction and returns data including the transactionHash <to> <amount> <token> <account>'
    'zksync_tx_info: returns the state or receipt of the the zksync-tx <tx>'
    'zksync_verify: returns 0 or 1 depending on the successfull verification of the signature <message> <signature>'
    'zksync_withdraw: withdraws the amount to the given `ethAddress` for the given token <ethAddress> <amount> <token> <account>'
    )

args=(
'--chainId=[the chainId or the name of a known chain]:chainId:(mainnet goerli ewc btc ipfs local)'
'-c[the chainId or the name of a known chain]:chainId:(mainnet goerli ewc btc ipfs local)'
'--finality=[the number in percent needed in order reach finality (% of signature of the validators)]:finality:()'
'-f[the number in percent needed in order reach finality (% of signature of the validators)]:finality:()'
'--includeCode[if true, the request should include the codes of all accounts]'
'--maxAttempts=[max number of attempts in case a response is rejected]:maxAttempts:()'
'-a[max number of attempts in case a response is rejected]:maxAttempts:()'
'--keepIn3[if true, requests sent to the input sream of the comandline util will be send theor responses in the...]'
'-kin3[if true, requests sent to the input sream of the comandline util will be send theor responses in the...]'
'--stats[if true, requests sent will be used for stats]'
'--useBinary[if true the client will use binary format]'
'--experimental[iif true the client allows to use use experimental features, otherwise a exception is thrown if thos...]'
'-x[iif true the client allows to use use experimental features, otherwise a exception is thrown if thos...]'
'--timeout=[specifies the number of milliseconds before the request times out]:timeout:()'
'--proof=[if true the nodes should send a proof of the response]:proof:(0 1 2)'
'-p[if true the nodes should send a proof of the response]:proof:(0 1 2)'
'--replaceLatestBlock=[if specified, the blocknumber *latest* will be replaced by blockNumber- specified value]:replaceLatestBlock:()'
'-l[if specified, the blocknumber *latest* will be replaced by blockNumber- specified value]:replaceLatestBlock:()'
'--autoUpdateList[if true the nodelist will be automaticly updated if the lastBlock is newer]'
'--signatureCount=[number of signatures requested in order to verify the blockhash]:signatureCount:()'
'-s[number of signatures requested in order to verify the blockhash]:signatureCount:()'
'--bootWeights[if true, the first request (updating the nodelist) will also fetch the current health status and use...]'
'-bw[if true, the first request (updating the nodelist) will also fetch the current health status and use...]'
'--useHttp[if true the client will try to use http instead of https]'
'--minDeposit=[min stake of the server]:minDeposit:()'
'--nodeProps=[used to identify the capabilities of the node]:nodeProps:()'
'--requestCount=[the number of request send in parallel when getting an answer]:requestCount:()'
'-rc[the number of request send in parallel when getting an answer]:requestCount:()'
'--rpc=[url of one or more direct rpc-endpoints to use]:rpc:()'
'--nodes.contract=[address of the registry contract]:contract:()'
'--nodes.whiteListContract=[address of the whiteList contract]:whiteListContract:()'
'--nodes.whiteList=[manual whitelist]:whiteList:()'
'--nodes.registryId=[identifier of the registry]:registryId:()'
'--nodes.needsUpdate[if set, the nodeList will be updated before next request]'
'--nodes.avgBlockTime=[average block time (seconds) for this chain]:avgBlockTime:()'
'--nodes.verifiedHashes.block=[block number]:block:()'
'--nodes.verifiedHashes.hash=[verified hash corresponding to block number]:hash:()'
'--nodes.nodeList.url=[URL of the node]:url:()'
'--nodes.nodeList.address=[address of the node]:address:()'
'--nodes.nodeList.props=[used to identify the capabilities of the node (defaults to 0xFFFF)]:props:()'
'--zksync.provider_url=[url of the zksync-server (if not defined it will be choosen depending on the chain)]:provider_url:()'
'-zks[url of the zksync-server (if not defined it will be choosen depending on the chain)]:provider_url:()'
'--zksync.account=[the account to be used]:account:()'
'-zka[the account to be used]:account:()'
'--zksync.sync_key=[the seed used to generate the sync_key]:sync_key:()'
'-zsk[the seed used to generate the sync_key]:sync_key:()'
'--zksync.main_contract=[address of the main contract- If not specified it will be taken from the server]:main_contract:()'
'--zksync.signer_type=[type of the account]:signer_type:(pk contract create2)'
'-zkat[type of the account]:signer_type:(pk contract create2)'
'--zksync.musig_pub_keys=[concatenated packed public keys (32byte) of the musig signers]:musig_pub_keys:()'
'-zms[concatenated packed public keys (32byte) of the musig signers]:musig_pub_keys:()'
'--zksync.musig_urls=[a array of strings with urls based on the `musig_pub_keys`]:musig_urls:()'
'-zmu[a array of strings with urls based on the `musig_pub_keys`]:musig_urls:()'
'--zksync.create2.creator=[The address of contract or EOA deploying the contract ( for example the GnosisSafeFactory )]:creator:()'
'--zksync.create2.saltarg=[a salt-argument, which will be added to the pubkeyhash and create the create2-salt]:saltarg:()'
'--zksync.create2.codehash=[the hash of the actual deploy-tx including the constructor-arguments]:codehash:()'
'--key=[the client key to sign requests]:key:()'
'-k[the client key to sign requests]:key:()'
'--pk=[registers raw private keys as signers for transactions]:pk:()'
'-pk[registers raw private keys as signers for transactions]:pk:()'
'--btc.maxDAP=[max number of DAPs (Difficulty Adjustment Periods) allowed when accepting new targets]:maxDAP:()'
'--btc.maxDiff=[max increase (in percent) of the difference between targets when accepting new targets]:maxDiff:()'
'--clearCache[clears the cache before performing any operation]'
'-ccache[clears the cache before performing any operation]'
'--eth[converts the result (as wei) to ether]'
'-e[converts the result (as wei) to ether]'
'--port=[if specified it will run as http-server listening to the given port]:port:()'
'-port[if specified it will run as http-server listening to the given port]:port:()'
'--allowed-methods=[only works if port is specified and declares a comma-seperated list of rpc-methods which are allowed]:allowed-methods:()'
'-am[only works if port is specified and declares a comma-seperated list of rpc-methods which are allowed]:allowed-methods:()'
'--block=[the blocknumber to use when making calls]:block:()'
'-b[the blocknumber to use when making calls]:block:()'
'--to=[the target address of the call]:to:()'
'-to[the target address of the call]:to:()'
'--from=[the sender of a call or tx (only needed if no signer is registered)]:from:()'
'-from[the sender of a call or tx (only needed if no signer is registered)]:from:()'
'--data=[the data for a transaction]:data:()'
'-d[the data for a transaction]:data:()'
'--gas_price=[the gas price to use when sending transactions]:gas_price:()'
'-gp[the gas price to use when sending transactions]:gas_price:()'
'--gas=[the gas limit to use when sending transactions]:gas:()'
'-gas[the gas limit to use when sending transactions]:gas:()'
'--nonce=[the nonce]:nonce:()'
'-nonce[the nonce]:nonce:()'
'--test=[creates a new json-test written to stdout with the name as specified]:test:()'
'-test[creates a new json-test written to stdout with the name as specified]:test:()'
'--path=[the HD wallet derivation path ]:path:()'
'-path[the HD wallet derivation path ]:path:()'
'--sigtype=[the type of the signature data]:sigtype:(raw hash eth_sign)'
'-st[the type of the signature data]:sigtype:(raw hash eth_sign)'
'--password=[password to unlock the key]:password:()'
'-pwd[password to unlock the key]:password:()'
'--value=[the value to send when sending a transaction]:value:()'
'-value[the value to send when sending a transaction]:value:()'
'--wait[if given, instead returning the transaction, it will wait until the transaction is mined and return ...]'
'-w[if given, instead returning the transaction, it will wait until the transaction is mined and return ...]'
'--json[if given the result will be returned as json, which is especially important for eth_call results wit...]'
'-json[if given the result will be returned as json, which is especially important for eth_call results wit...]'
'--hex[if given the result will be returned as hex]'
'-hex[if given the result will be returned as hex]'
'--debug[if given incubed will output debug information when executing]'
'-debug[if given incubed will output debug information when executing]'
'--quiet[quiet]'
'-q[quiet]'
'--human[human readable, which removes the json -structure and oly displays the values]'
'-h[human readable, which removes the json -structure and oly displays the values]'
'--test-request[runs test request when showing in3_weights]'
'-tr[runs test request when showing in3_weights]'
'--test-health-request[runs test request including health-check when showing in3_weights]'
'-thr[runs test request including health-check when showing in3_weights]'
'--multisig=[adds a multisig as signer this needs to be done in the right order! (first the pk then the multisaig...]:multisig:()'
'-ms[adds a multisig as signer this needs to be done in the right order! (first the pk then the multisaig...]:multisig:()'
'--ms.signatures=[add additional signatures, which will be useds when sending through a multisig!]:ms.signatures:()'
'-sigs[add additional signatures, which will be useds when sending through a multisig!]:ms.signatures:()'
'--response.in[read response from stdin]'
'-ri[read response from stdin]'
'--response.out[write raw response to stdout]'
'-ro[write raw response to stdout]'
'--file.in=[reads a prerecorded request from the filepath and executes it with the recorded data]:file.in:()'
'-fi[reads a prerecorded request from the filepath and executes it with the recorded data]:file.in:()'
'--file.out=[records a request and writes the reproducable data in a file (including all cache-data, timestamps ]:file.out:()'
'-fo[records a request and writes the reproducable data in a file (including all cache-data, timestamps ]:file.out:()'
'--nodelist=[a coma seperated list of urls (or address:url) to be used as fixed nodelist]:nodelist:()'
'-nl[a coma seperated list of urls (or address:url) to be used as fixed nodelist]:nodelist:()'
'--bootnodes=[a coma seperated list of urls (or address:url) to be used as boot nodes]:bootnodes:()'
'-bn[a coma seperated list of urls (or address:url) to be used as boot nodes]:bootnodes:()'
'--onlysign[only sign, don't send the raw Transaction]'
'-os[only sign, don't send the raw Transaction]'
'--noproof[alias for --proof=none]'
'-np[alias for --proof=none]'
'--nostats[alias for --stats=false, which will mark all requests as not counting in the stats]'
'-ns[alias for --stats=false, which will mark all requests as not counting in the stats]'
'--version[displays the version]'
'-v[displays the version]'
'--help[displays this help message]'
'-h[displays this help message]'
 '-st[the type of the signature data]:st:(eth_sign raw hash)'
 '-pwd[password to unlock the key]:pwd:()'
 '-np[short for -p none]'
 '-ns[short for no stats, which does count this request in the public stats]'
 '-eth[onverts the result (as wei) to ether]'
 '-port[if specified it will run as http-server listening to the given port]:port:(8545)'
 '-am[Allowed Methods when used with -port as comma seperated list of methods]:allowed_methods:()'
 '-b[the blocknumber to use when making calls]:b:(latest earliest 0x)'
 '-to[the target address of the call or send]:to:(0x)'
 '-d[the data for a transaction. This can be a filepath, a 0x-hexvalue or - for stdin]:date:()'
 '-gas[the gas limit to use when sending transactions]:gas:(21000 100000 250000 500000 1000000 2000000)'
 '-gas_price[the gas price to use within a tx]:gas_price:()'
 '-pk[the private key as raw or path to the keystorefile]:pk:()'
 '-help[displays this help message]'
 '-tr[runs test request when showing in3_weights]'
 '-thr[runs test request including health-check when showing in3_weights]'
 '-ms[address of a imao multisig to redirect all tx through]:ms:()'
 '-version[displays the version]'
 '-debug[if given incubed will output debug information when executing]'
 '-value[the value to send when sending a transaction. (hexvalue or float/integer with the suffix eth]:value:(1.0eth)'
 '-w[instead returning the transaction, it will wait until the transaction is mined and return the transactionreceipt]'
 '-md[specifies the minimum Deposit of a node in order to be selected as a signer]'
 '-json[the result will be returned as json]'
 '-hex[the result will be returned as hex]'
 '-kin3[the response including in3-section is returned]'
 '-q[quiet no debug output]'
 '-os[only sign, do not send the raw Transaction]'
 '-ri[read response from stdin]'
 '-ro[write raw response to stdout]'
 '-fi[read recorded request from file]:fi:()'
 '-fo[recorded request and write to file]:fo:()'
 '-nl[a comma seperated list of urls as address:url to be used as fixed nodelist]:nl:()'
 '-bn[a comma seperated list of urls as address:url to be used as boot nodes]:bn:()'

 ':method:{_describe command subcmds}'
 ':arg1:{_describe command sig_in3 -- sig_erc20 -- sig_ms}'
)

sig_in3=(
    'minDeposi()\:uint: minimal deposit'
    'adminKey()\:address: admin key'
    'nodeRegistryData()\:address:addres of the data contract'
    'supportedToken()\:address: supported token'
    'totalNodes()\:uint: number of nodes'
    'blockRegistry()\:address: BlockHashRegistry'
    'nodes(uint256)\:(string,uint256,uint64,uint192,uint64,address,bytes32): node data'
    'unregisteringNode(address):unregister a node'
    'updateNode(address,string,uint192,uint64,uint256): update nod properties'
    'transferOwnership(address,address): transfers ownership from signer to new owner',
    'registerNode(string,uint192,uint64,uint256): registers a Node'
    'snapshot(): creates a snapshot for the current block'
)
sig_erc20=(
    'balanceOf(address)\:uint:getting the balance of' 
    'name()\:string:token name' 
    'totalSupply()\:uint:total Balance'
    'transfer(address,uint256):transfer tokens'
    'transferFrom(address,address,uint256):transfer from <my> to <your> account <value> tokens'
    'approve(address,uint256):approve the amount for the given address'
    'allowance(address,address)\:uint: the approved amount'
)
sig_ms=(
    'getOwners()\:(address[]): multisig'
    'getMessageHash(bytes)\:bytes: gets the message hash of a transaction'
    'isOwner(address)\:bool:is owner'
    'signedMessages(bytes32)\:uint: number of signed messages'
    'approvedHashes(address,bytes32)\:uint:check if the hash was approved'
    'nonce()\:uint:the nonce of the multisig'
    'getModules()\:address[]:List of modules'
    'getTransactionHash(address,uint256,bytes,uint8,uint256,uint256,uint256,address,address,uint256)\:bytes32:calculates the transaction hash'
    'getThreshold()\:uint'
    'addOwnerWithThreshold(address,uint256):adds an owner with the given threshold'
    'changeThreshold(uint256): changes the threshold'
    'execTransaction(address,uint256,bytes,uint8,uint256,uint256,uint256,address,address,bytes): executes a transaction'
)

_arguments -C $args
