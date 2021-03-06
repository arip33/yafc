#ifndef CHATUSER_H
#define CHATUSER_H

#include <string>
#include <functional>
#include <memory>
#include <boost/asio.hpp>

#include "chatsession.h"
#include "sendable.h"

static const char* DEF_NAME = "anonymous";
static const char* DEF_ROOM = "all";

class ChatUser : public std::enable_shared_from_this<ChatUser>,
                 public sendable
{

public:

	typedef enum {
		CONNECTING = 1,
		CONNECTED = 2,
		DISCONNECTED = 3
	} state;

	typedef std::function<void(std::string, std::string)> send_func;
	typedef std::function<bool(std::string, ChatUser*)> parse_func;

	ChatUser(boost::asio::io_service& io_service,
	         send_func sender,
	         parse_func parser);
	boost::asio::ip::tcp::socket& get_socket();
	void disconnect();	
	state get_status();
	void start_session();
	void send_message(const std::string& msg,
	                  const std::string& dest);

	bool set_name(const std::string& name);
	bool set_target(const std::string& dest);
	const std::string& get_name();

private:

	ChatSession session;
	std::string name;
	std::string target;
	state status;
	send_func sender;
	parse_func parser;

	void connect();

	std::string gen_msg(const std::string& msg);
	void read_handler(std::string msg);
	bool validate_name(const std::string& name);
};

#endif
