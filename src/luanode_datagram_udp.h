#pragma once

#include "../deps/luacppbridge51/lcbHybridObjectWithProperties.h"

#include <boost/asio/ip/udp.hpp>
#include <boost/asio/streambuf.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/array.hpp>

namespace LuaNode {

namespace Datagram {

void RegisterFunctions(lua_State* L);

int IsIP(lua_State* L);


class Socket : public LuaCppBridge::HybridObjectWithProperties<Socket>
{
public:
	// Normal constructor
	Socket(lua_State* L);
	// Constructor used when we accept a connection
	Socket(lua_State* L, boost::asio::ip::udp::socket*);
	virtual ~Socket(void);

public:
	LCB_HOWP_DECLARE_EXPORTABLE(Socket);
	static const char* get_full_class_name_T();

	static int tostring_T(lua_State* L);

	int SetOption(lua_State* L);

	int Bind(lua_State* L);
	int Listen(lua_State* L);
	int Connect(lua_State* L);

	int SendTo(lua_State* L);
	int Read(lua_State* L);

	int Close(lua_State* L);
	int Shutdown(lua_State* L);

	int GetLocalAddress(lua_State* L);
	int GetRemoteAddress(lua_State* L);

public:
	boost::asio::ip::udp::socket& GetSocketRef() { return *m_socket; };

private:
	void HandleSendTo(int reference, int callback, const boost::system::error_code& error, size_t bytes_transferred);
	void HandleRead(int reference, const boost::system::error_code& error, size_t bytes_transferred);
	void HandleReceive(int reference, const boost::system::error_code& error, size_t bytes_transferred);
	void HandleConnect(int reference, const boost::system::error_code& error);

private:
	lua_State* m_L;
	const unsigned int m_socketId;
	bool m_close_pending;
	//bool m_read_shutdown_pending;
	bool m_write_shutdown_pending;
	unsigned long m_pending_writes;
	unsigned long m_pending_reads;
	
	boost::shared_ptr< boost::asio::ip::udp::socket > m_socket;
	boost::asio::streambuf m_inputBuffer;
	//boost::array<char, 4> m_inputArray;	// agrandar esto y poolearlo
	//boost::array<char, 32> m_inputArray;	// agrandar esto y poolearlo
	//boost::array<char, 64> m_inputArray;	// agrandar esto y poolearlo
	//boost::array<char, 128> m_inputArray;	// agrandar esto y poolearlo (el test simple\test-http-upgrade-server necesita un buffer grande sino falla)
	boost::array<char, 8192> m_inputArray;	// agrandar esto y poolearlo (el test simple\test-http-upgrade-server necesita un buffer grande sino falla)
};

}

}
