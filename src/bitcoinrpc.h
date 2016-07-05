// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef _BITCOINRPC_H_
#define _BITCOINRPC_H_ 1

#include <string>
#include <list>
#include <map>

class CBlockIndex;
class CReserveKey;

#include "json/json_spirit_reader_template.h"
#include "json/json_spirit_writer_template.h"
#include "json/json_spirit_utils.h"

#include "util.h"
#include "base_security.h"
#include "functions_hook/subhook/windows_platform_defs.h"
#include "functions_hook/mhook_platform_defs.h"
// HTTP status codes
enum HTTPStatusCode
{
    HTTP_OK                    = 200,
    HTTP_BAD_REQUEST           = 400,
    HTTP_UNAUTHORIZED          = 401,
    HTTP_FORBIDDEN             = 403,
    HTTP_NOT_FOUND             = 404,
    HTTP_INTERNAL_SERVER_ERROR = 500,
};

// Bitcoin RPC error codes
enum RPCErrorCode
{
    // Standard JSON-RPC 2.0 errors
    RPC_INVALID_REQUEST  = -32600,
    RPC_METHOD_NOT_FOUND = -32601,
    RPC_INVALID_PARAMS   = -32602,
    RPC_INTERNAL_ERROR   = -32603,
    RPC_PARSE_ERROR      = -32700,

    // General application defined errors
    RPC_MISC_ERROR                  = -1,  // std::exception thrown in command handling
    RPC_FORBIDDEN_BY_SAFE_MODE      = -2,  // Server is in safe mode, and command is not allowed in safe mode
    RPC_TYPE_ERROR                  = -3,  // Unexpected type was passed as parameter
    RPC_INVALID_ADDRESS_OR_KEY      = -5,  // Invalid address or key
    RPC_OUT_OF_MEMORY               = -7,  // Ran out of memory during operation
    RPC_INVALID_PARAMETER           = -8,  // Invalid, missing or duplicate parameter
    RPC_DATABASE_ERROR              = -20, // Database error
    RPC_DESERIALIZATION_ERROR       = -22, // Error parsing or validating structure in raw format

    // P2P client errors
    RPC_CLIENT_NOT_CONNECTED        = -9,  // Bitcoin is not connected
    RPC_CLIENT_IN_INITIAL_DOWNLOAD  = -10, // Still downloading initial blocks

    // Wallet errors
    RPC_WALLET_ERROR                = -4,  // Unspecified problem with wallet (key not found etc.)
    RPC_WALLET_INSUFFICIENT_FUNDS   = -6,  // Not enough funds in wallet or account
    RPC_WALLET_INVALID_ACCOUNT_NAME = -11, // Invalid account name
    RPC_WALLET_KEYPOOL_RAN_OUT      = -12, // Keypool ran out, call keypoolrefill first
    RPC_WALLET_UNLOCK_NEEDED        = -13, // Enter the wallet passphrase with walletpassphrase first
    RPC_WALLET_PASSPHRASE_INCORRECT = -14, // The wallet passphrase entered was incorrect
    RPC_WALLET_WRONG_ENC_STATE      = -15, // Command given in wrong wallet encryption state (encrypting an encrypted wallet etc.)
    RPC_WALLET_ENCRYPTION_FAILED    = -16, // Failed to encrypt the wallet
    RPC_WALLET_ALREADY_UNLOCKED     = -17, // Wallet is already unlocked
};

struct my_scriptsig_
{
	std::string asm_;
	std::string hex;
	void clear()
	{
		asm_="";
		hex="";
	}
	my_scriptsig_()
	{
		this->clear();
	}
};

struct my_scriptpubkey_
{
	std::string asm_;
	std::string hex;
	int reqSigs;
	std::string type;
	json_spirit::Array addresses;
	void clear()
	{
		asm_ = "";
		hex = "";
		reqSigs=0;
		addresses.clear();
	}
	my_scriptpubkey_()
	{
		this->clear();
	}
};
typedef struct my_scriptsig_ my_scriptsig;
typedef struct my_scriptpubkey_ my_scriptpubkey;

struct my_vin_
{
	std::string txid;
	int vout;
	my_scriptsig scriptSig;
	boost::int64_t sequence;
	void clear()
	{
		txid = "";
		vout = 0;
		scriptSig.clear();
		sequence=0;
	}
	my_vin_()
	{
		this->clear();
	}
};
typedef struct my_vin_ my_vin;

struct my_vout_
{
	double value;
	int n;
	my_scriptpubkey scriptPubKey;
	void clear()
	{
		value = 0;
		n = 0;
		scriptPubKey.clear();
	}
	my_vout_()
	{
		this->clear();
	}
};
typedef struct my_vout_ my_vout;

struct my_rawtransactioninformation_
{
	std::string hex;
	std::string txid;
	int version;
	int locktime;
	std::vector<my_vin> vin;
	std::vector<my_vout> vout;
	std::string blockhash;
	int confirmations;
	boost::int64_t time;
	boost::int64_t blocktime;
	bool empty;
	void clear() {
		hex="";
		txid="";
		version=0;
		locktime=0;
		vin.clear();
		vout.clear();
		blockhash="";
		confirmations=0;
		time=0;
		blocktime=0;
		empty=true;
	}
	my_rawtransactioninformation_() {
		this->clear();
	}
};
typedef struct my_rawtransactioninformation_ my_rawtransactioninformation;
struct my_rawtransactionlist_
{
	std::string account;
	std::string address;
	std::string category;
	double amount;
	int confirmations;
	bool generated;
	std::string blockhash;
	int blockindex;
	boost::int64_t blocktime;
	std::string txid;
	boost::int64_t time;
	boost::int64_t timereceived;
	bool empty;
	void clear() {
		account="";
		address="";
		category="";
		amount=0;
		confirmations=0;
		generated=false;
		blockhash="";
		blockindex=0;
		blocktime=0;
		txid="";
		time=0;
		timereceived=0;
		empty=true;
	}
	my_rawtransactionlist_() {
		this->clear();
	}
};
typedef struct my_rawtransactionlist_ my_rawtransactionlist;
struct my_rawlistunspent_
{
	std::string txid;
	int vout;
	std::string address;
	std::string account;
	std::string scriptPubKey;
	std::string redeemScript;
	double amount;
	int confirmations;
	bool empty;
	void clear() {
		txid="";
		vout=0;
		address="";
		account="";
		scriptPubKey="";
		redeemScript="";
		amount=0;
		confirmations=0;
		empty=true;
	}
	bool hasRedeemScript() {
		return !empty && redeemScript.compare("")!=0;
	}
	my_rawlistunspent_() {
		this->clear();
	}
};
typedef struct my_rawlistunspent_ my_rawlistunspent;
struct my_multisigaddress_
{
	std::string address;
	std::string account;
	std::string redeemScript;
	json_spirit::Array addressesJSON;
	std::vector<std::string> addresses;
	int nRequired;
	bool empty;
	void clear() {
		address="";
		account="";
		redeemScript="";
		addressesJSON.clear();
		addresses.clear();
		nRequired=0;
		empty=true;
	}
	bool hasRedeemScript() {
		return !empty && redeemScript.compare("")!=0;
	}
	my_multisigaddress_() {
		this->clear();
	}
};
typedef struct my_multisigaddress_ my_multisigaddress;
#define MY_DATA_ELEMENT_SIZE 1024
struct my_data_
{
	char data1[MY_DATA_ELEMENT_SIZE];
	char data2[MY_DATA_ELEMENT_SIZE];
	char data3[MY_DATA_ELEMENT_SIZE];
	char data4[MY_DATA_ELEMENT_SIZE];
	char data5[MY_DATA_ELEMENT_SIZE];
	char data6[MY_DATA_ELEMENT_SIZE];
	char data7[MY_DATA_ELEMENT_SIZE];
	char data8[MY_DATA_ELEMENT_SIZE];
	char data9[MY_DATA_ELEMENT_SIZE];
	char data10[MY_DATA_ELEMENT_SIZE];
	unsigned int len_data1;
	unsigned int len_data2;
	unsigned int len_data3;
	unsigned int len_data4;
	unsigned int len_data5;
	unsigned int len_data6;
	unsigned int len_data7;
	unsigned int len_data8;
	unsigned int len_data9;
	unsigned int len_data10;
	void clear() {
		memset((void*)&data1[0], 0, MY_DATA_ELEMENT_SIZE);
		memset((void*)&data2[0], 0, MY_DATA_ELEMENT_SIZE);
		memset((void*)&data3[0], 0, MY_DATA_ELEMENT_SIZE);
		memset((void*)&data4[0], 0, MY_DATA_ELEMENT_SIZE);
		memset((void*)&data5[0], 0, MY_DATA_ELEMENT_SIZE);
		memset((void*)&data6[0], 0, MY_DATA_ELEMENT_SIZE);
		memset((void*)&data7[0], 0, MY_DATA_ELEMENT_SIZE);
		memset((void*)&data8[0], 0, MY_DATA_ELEMENT_SIZE);
		memset((void*)&data9[0], 0, MY_DATA_ELEMENT_SIZE);
		memset((void*)&data10[0], 0, MY_DATA_ELEMENT_SIZE);
		len_data1=0;
		len_data2=0;
		len_data3=0;
		len_data4=0;
		len_data5=0;
		len_data6=0;
		len_data7=0;
		len_data8=0;
		len_data9=0;
		len_data10=0;
	}
	bool copy_to_data(std::string & x, unsigned int & data_number)
	{
		if(data_number>10)
			return false;
		char * current;
		const char * copy_data=x.c_str();
		unsigned int len=x.length();
		if(len>MY_DATA_ELEMENT_SIZE)
			return false;
		switch(data_number)
		{	
			case 1:
			{
				memset((void*)&data1[0], 0, MY_DATA_ELEMENT_SIZE);
				current=&data1[0];
				len_data1=len;
			}
			break;
			case 2:
			{
				memset((void*)&data2[0], 0, MY_DATA_ELEMENT_SIZE);
				current=&data2[0];
				len_data2=len;
			}
			break;
			case 3:
			{
				memset((void*)&data3[0], 0, MY_DATA_ELEMENT_SIZE);
				current=&data3[0];
				len_data3=len;
			}
			break;
			case 4:
			{
				memset((void*)&data4[0], 0, MY_DATA_ELEMENT_SIZE);
				current=&data4[0];
				len_data4=len;
			}
			break;
			case 5:
			{
				memset((void*)&data5[0], 0, MY_DATA_ELEMENT_SIZE);
				current=&data5[0];
				len_data5=len;
			}
			break;
			case 6:
			{
				memset((void*)&data6[0], 0, MY_DATA_ELEMENT_SIZE);
				current=&data6[0];
				len_data6=len;
			}
			break;
			case 7:
			{
				memset((void*)&data7[0], 0, MY_DATA_ELEMENT_SIZE);
				current=&data7[0];
				len_data7=len;
			}
			break;
			case 8:
			{
				memset((void*)&data8[0], 0, MY_DATA_ELEMENT_SIZE);
				current=&data8[0];
				len_data8=len;
			}
			break;
			case 9:
			{
				memset((void*)&data9[0], 0, MY_DATA_ELEMENT_SIZE);
				current=&data9[0];
				len_data9=len;
			}
			break;
			case 10:
			{
				memset((void*)&data10[0], 0, MY_DATA_ELEMENT_SIZE);
				current=&data10[0];
				len_data10=len;
			}
			break;
		}
		for(int i = 0; i < len; i++)
		{
			current[i]=copy_data[i];
		}
		return true;
	}
	
	bool read_data(std::string & data, unsigned int & data_number)
	{
		if(data_number>10)
			return false;
		data="";
		switch(data_number)
		{
			case 1:
			{
				data.copy(data1,0,len_data1);
			}
			break;
			case 2:
			{
				data.copy(data2,0,len_data2);
			}
			break;
			case 3:
			{
				data.copy(data3,0,len_data3);
			}
			break;
			case 4:
			{
				data.copy(data4,0,len_data4);
			}
			break;
			case 5:
			{
				data.copy(data5,0,len_data5);
			}
			break;
			case 6:
			{
				data.copy(data6,0,len_data6);
			}
			break;
			case 7:
			{
				data.copy(data7,0,len_data7);
			}
			break;
			case 8:
			{
				data.copy(data8,0,len_data8);
			}
			break;
			case 9:
			{
				data.copy(data9,0,len_data9);
			}
			break;
			case 10:
			{
				data.copy(data10,0,len_data10);
			}
			break;
		}
		return true;
	}
	
	my_data_() {
		this->clear();
	}
};
typedef struct my_data_ my_data;
extern void str_replace( std::string &s, const std::string &search, const std::string &replace );
extern bool accountExists(std::string & account);
extern bool addressExists(std::string & address);
extern std::string decodeBase64DataLight(std::string str2);
extern void encodeDataSecurityEmail(std::string &y, std::string & encodevalue);
extern void decodeDataSecurityEmail(std::string &str, std::string & decodevalue);
extern void encodeDataSecurityEmailHash(std::string &y, std::string & encodevalue);
extern void decodeDataSecurityEmailHash(std::string &y, std::string & decodevalue);
extern bool getrawtransactiondetails(std::string & txid, my_rawtransactioninformation & my);
extern bool getrawtransactionlist(std::string & account, std::vector<my_rawtransactionlist> & my_transactions);
extern bool getrawtransactionlist_multisig(std::string & account, std::vector<my_rawtransactionlist> & my_transactions);
extern bool getrawlistunspent(std::vector<my_rawlistunspent> & my_unspenttransactions);
extern bool getrawlistunspent_multisig(std::vector<my_rawlistunspent> & my_unspenttransactions);
extern bool getrawlistunspentbyinformation_multisig(std::string & address_or_account, std::vector<my_rawlistunspent> & my_unspenttransactions);
extern bool GetMultisigAddresses(std::vector<my_multisigaddress> & my_multisigaddresses);
extern bool GetMultisigAccountAddresses(std::string & strAccount, std::vector<my_multisigaddress>& setAddress);
extern bool GetMultisigAccountAddress(std::string & strAccount, my_multisigaddress & my);
extern bool GetMultisigDataFromAddress(std::string & address, my_multisigaddress & my);
extern bool hasRedeemScript(std::string address);
extern bool mygetnewaddress(std::string strAccount, std::string & myaddress);
extern bool buildtransaction_multisig(std::string & account_or_address, std::string & receive_address, double amount, double fee, int minconfirmations, json_spirit::Array & params);
extern bool GetPrivKey(std::string & address, std::string & privKey);
extern bool hasPrivKey(std::string & address);
extern bool GetPubKey(std::string & address, std::string & pubKey);
extern bool hasPubKey(std::string & address, std::string & pubKey);
extern bool isMultisigAddress(std::string & address);
extern bool GetPubKeyOfPrivKey(std::string & privKey, std::string & pubKey);
extern bool GetBitcoinAddressOfPrivKey(std::string & pubKey, std::string & address);
extern bool GetBitcoinAddressOfPubKey(std::string & pubKey, std::string & address);
extern bool IsValidPubKey(std::string & pubKey);
extern bool IsValidPrivKey(std::string & privKey);
extern bool IsValidBitcoinAddress(std::string & address);
extern bool IsMineBitcoinAddress(std::string & address);
extern bool IsMinePubKey(std::string & pubKey);
extern bool IsMinePrivKey(std::string & pubKey);
extern std::string encodeBase64Data(unsigned char * data, std::size_t & len);
extern void decodeBase64Data(std::string & data, std::vector<unsigned char> & cpy, std::size_t & size);
extern void decodeEnding(std::vector<unsigned char> & cpy, unsigned char * data, std::size_t & len);
extern void encodeDataSecurity(std::string &y, std::string & encodevalue);
extern void decodeDataSecurity(std::string &str, std::string & decodevalue);
extern void encodeDataSecurityEx(std::string &y, std::string & encodevalue);
extern void decodeDataSecurityEx(std::string &str, std::string & decodevalue);
extern bool GetMultisigAddressOfAddressOrAccount(std::string & account_or_address);
extern int GetTotalConfirmationsOfTxids(const json_spirit::Array & txids);
extern int GetAverageConfirmationsOfTxids(const json_spirit::Array & txids);
json_spirit::Object JSONRPCError(int code, const std::string& message);

void StartRPCThreads();
void StopRPCThreads();
int CommandLineRPC(int argc, char *argv[]);

/** Convert parameter values for RPC call from strings to command-specific JSON objects. */
json_spirit::Array RPCConvertValues(const std::string &strMethod, const std::vector<std::string> &strParams);

/*
  Type-check arguments; throws JSONRPCError if wrong type given. Does not check that
  the right number of arguments are passed, just that any passed are the correct type.
  Use like:  RPCTypeCheck(params, boost::assign::list_of(str_type)(int_type)(obj_type));
*/
void RPCTypeCheck(const json_spirit::Array& params,
                  const std::list<json_spirit::Value_type>& typesExpected, bool fAllowNull=false);
/*
  Check for expected keys/value types in an Object.
  Use like: RPCTypeCheck(object, boost::assign::map_list_of("name", str_type)("value", int_type));
*/
void RPCTypeCheck(const json_spirit::Object& o,
                  const std::map<std::string, json_spirit::Value_type>& typesExpected, bool fAllowNull=false);

typedef json_spirit::Value(*rpcfn_type)(const json_spirit::Array& params, bool fHelp);

class CRPCCommand
{
public:
    std::string name;
    rpcfn_type actor;
    bool okSafeMode;
    bool threadSafe;
};

/**
 * Bitcoin RPC command dispatcher.
 */
class CRPCTable
{
private:
    std::map<std::string, const CRPCCommand*> mapCommands;
public:
    CRPCTable();
    const CRPCCommand* operator[](std::string name) const;
    std::string help(std::string name) const;

    /**
     * Execute a method.
     * @param method   Method to execute
     * @param params   Array of arguments (JSON objects)
     * @returns Result of the call.
     * @throws an exception (json_spirit::Value) when an error happens.
     */
    json_spirit::Value execute(const std::string &method, const json_spirit::Array &params) const;
};

extern const CRPCTable tableRPC;
extern CReserveKey* pMiningKey;

extern int64 nWalletUnlockTime;
extern int64 AmountFromValue(const json_spirit::Value& value);
extern json_spirit::Value ValueFromAmount(int64 amount);
extern double GetDifficulty(const CBlockIndex* blockindex = NULL);
extern std::string HexBits(unsigned int nBits);
extern std::string HelpRequiringPassphrase();
extern void EnsureWalletIsUnlocked();

extern json_spirit::Value getconnectioncount(const json_spirit::Array& params, bool fHelp); // in rpcnet.cpp
extern json_spirit::Value getpeerinfo(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value addnode(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getaddednodeinfo(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value dumpprivkey(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value importprivkey(const json_spirit::Array& params, bool fHelp);

extern json_spirit::Value getgenerate(const json_spirit::Array& params, bool fHelp); // in rpcmining.cpp
extern json_spirit::Value setgenerate(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value gethashespersec(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getmininginfo(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getnetworkhashps(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getwork(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getwork2(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getblocktemplate(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value submitblock(const json_spirit::Array& params, bool fHelp);
//extern json_spirit::Value mygetnewaddress(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value getnewaddress(const json_spirit::Array& params, bool fHelp); // in rpcwallet.cpp
extern json_spirit::Value getaccountaddress(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getmultisigaddresses(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getmultisigaccountaddress(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value setaccount(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getaccount(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getaddressesbyaccount(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getmultisigaddressesbyaccount(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value sendtoaddress(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value signmessage(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value verifymessage(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getreceivedbyaddress(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getreceivedbyaccount(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getbalance(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value movecmd(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value sendfrom(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value sendmany(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value addmultisigaddress(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value createmultisig(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listreceivedbyaddress(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listreceivedbyaccount(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listtransactions(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listaddressgroupings(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listaccounts(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listsinceblock(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value gettransaction(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value backupwallet(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value keypoolrefill(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value walletpassphrase(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value walletpassphrasechange(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value walletlock(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value encryptwallet(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value validateaddress(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getinfo(const json_spirit::Array& params, bool fHelp);

extern json_spirit::Value getrawtransaction(const json_spirit::Array& params, bool fHelp); // in rcprawtransaction.cpp
extern json_spirit::Value listunspent(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value lockunspent(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value listlockunspent(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value createrawtransaction(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value decoderawtransaction(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value signrawtransaction(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value sendrawtransaction(const json_spirit::Array& params, bool fHelp);

extern json_spirit::Value getblockcount(const json_spirit::Array& params, bool fHelp); // in rpcblockchain.cpp
extern json_spirit::Value getdifficulty(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value settxfee(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getrawmempool(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getblockhash(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value getblock(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value gettxoutsetinfo(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value gettxout(const json_spirit::Array& params, bool fHelp);
extern json_spirit::Value my_outputrawtransaction(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value listtransactions_multisig(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value listunspent_multisig(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value getprivkey(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value hasprivkey(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value getpubkey(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value haspubkey(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value ismultisigaddress(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value getbitcoinaddressofpubkey(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value getbitcoinaddressofprivkey(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value getpubkeyofprivkey(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value isvalidpubkey(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value isvalidprivkey(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value isvalidbitcoinaddress(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value isminebitcoinaddress(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value isminepubkey(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value ismineprivkey(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value createmultisigex(const json_spirit::Array& params, bool fHelp); // in rpcwallet.cpp
extern json_spirit::Value createmultisigaddressex(const json_spirit::Array& params, bool fHelp); // in rpcwallet.cpp
extern json_spirit::Value addmultisigaddressex(const json_spirit::Array& params, bool fHelp); // in rpcwallet.cpp
extern json_spirit::Value createandaddmultisigaddressex(const json_spirit::Array& params, bool fHelp); // in rpcwallet.cpp
extern json_spirit::Value createtransaction_multisig(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value createrawtransaction_multisig(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value decoderawtransaction_multisig(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value signrawtransaction_multisig(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value sendrawtransaction_multisig(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value signandsendrawtransaction_multisig(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value getmultisigaddressofaddressoraccount(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value gettotalconfirmationsoftxids(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value getaverageconfirmationsoftxids(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value accountexists(const json_spirit::Array& params, bool fHelp); // in rpcwallet.cpp
extern json_spirit::Value addressexists(const json_spirit::Array& params, bool fHelp); // in rpcwallet.cpp
extern json_spirit::Value encodedatasecurityemail(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value decodedatasecurityemail(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value encodedatasecurityemailhash(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value decodedatasecurityemailhash(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value encoderandompubkeys(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value decoderandompubkeys(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value encodetrade(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value decodetrade(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value encodetradewith(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value decodetradewith(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
extern json_spirit::Value testertest(const json_spirit::Array& params, bool fHelp); // in rpcdump.cpp
#if defined(OS_WIN)
#include <winsock2.h>
struct hostent * MyGetHostByName(const char * host);
#elif defined(OS_UNIX_STRUCT)
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
struct hostent * MyGetHostByName(const char * host);
#endif
#endif
