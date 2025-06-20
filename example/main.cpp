#include "inicpp.hpp"

#include <iostream>
#include <iomanip>

#define CONFIG_FILE "config.ini"

void writeExample();
void readExample();

class appConfig
{
public:
	typedef struct Server
	{
		std::string ip;
		unsigned short port;
		bool isKeepalived;
	} Server;

	static const Server readServer(inicpp::IniManager &_ini)
	{
		return Server{
			ip : _ini["server"]["ip"],
			port : _ini["server"]["port"],
			isKeepalived : _ini["server"]["isKeepalived"]
		};
	}

	typedef struct Config
	{
		std::string title;
		Server server;
		double PI;
	} Config;

	static const Config readConfig()
	{
		inicpp::IniManager _ini(CONFIG_FILE);

		return Config{
			title : _ini[""]["title"],
			server : readServer(_ini),
			PI : _ini["math"]["PI"],
		};
	}

	static void print(Config &config)
	{
		std::cout
			<< "title:       \t" << config.title << std::endl
			<< "server.port: \t" << config.server.port << std::endl
			<< "server.ip:   \t" << config.server.ip << std::endl
			<< "server.alive:\t" << config.server.isKeepalived << std::endl
			<< "math.PI:     \t" << std::setprecision(20) << config.PI << std::endl;
	}
};

/**
 compile: g++ -I../ -std=c++11 main.cpp -o iniExample
*/
int main()
{
	/** write and read example */
	writeExample();
	readExample();

	/** Bind to Struct */
	appConfig::Config config = appConfig::readConfig();
	appConfig::print(config);

	return 0;
}

void writeExample()
{
	inicpp::IniManager _ini(CONFIG_FILE);

	/** write to file. */
	_ini.set("server", "isKeepalived", "true");
	_ini.set("server", "port", "8080");
	_ini.set("server", "ip", "127.0.0.1");

	// comment
	_ini.set("math", "PI", "3.141592653589793238462643383279502884", "Comment: This is pi in mathematics."); // with a comment.
	_ini.setComment("server", "port", "this is the listen ip for server."); // comment to section->key

	_ini.set("title", "config.ini"); // no need sections
	_ini.setComment("title", "This is the title."); // add comment for none section key
}

/**
 * Note: The functions toInt(), toDouble(), and toString() will never throw an exception.
 *       However, '_ini["any"]["any"]' may throw an error if a key is not found
 *       or if a conversion error occurs.
 */
void readExample()
{
	inicpp::IniManager _ini(CONFIG_FILE);

	std::string server_port_s = _ini["server"]["port"];
	std::string server_ip_s = _ini["server"]["ip"];

	int server_port_i = _ini["server"]["port"];
	// - Convert to string.
	std::string PI_s = _ini["math"]["PI"];
	PI_s = _ini["math"]["PI"].operator std::string();
	PI_s = _ini["math"]["PI"].String();
	PI_s = _ini["math"]["PI"].as<std::string>();
	// - Convert to double.
	double PI_d = _ini["math"]["PI"];
	// - Convert to int.
	int PI_i = _ini["math"]["PI"];
	unsigned int PI_ui = _ini["math"]["PI"];
	// - Or others
	float PI_f = _ini["math"]["PI"];
	short PI_si = _ini["math"]["PI"];
	char PI_c = _ini["math"]["PI"];
	// - Diy, any you want
	long long PI_ll = _ini["math"]["PI"].as<long long>();

	PI_c = _ini["math"]["PI"];

	// - Not throw exceptions
	PI_s = _ini["math"].toString("PI");
	PI_d = _ini["math"].toDouble("PI");
	PI_i = _ini["math"].toInt("PI");

	// - Bind to Struct
	appConfig::Config config = appConfig::readConfig();
}
