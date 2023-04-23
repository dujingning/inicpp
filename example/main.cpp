#include "inicpp.hpp"

/* compile: g++ -I../ -std=c++11 main.cpp -o iniExample */
int main()
{

	// Load the INI file.
	inicpp::IniManager _ini("config.ini");

	// Check if the key exists.
	if (!_ini["rtsp"].isKeyExist("port"))
	{
		std::cout << "rtsp.port is not exist!" << std::endl;
	}

	// Modify or add key-value pairs.
	_ini.modify("rtsp", "port", "554");

	// Use key-value pairs directly.
	std::string rtsp_port = _ini["rtsp"]["port"];

	// You can write section-key-value.
	_ini.modify("rtsp", "port", "554");

	// Add a comment.
	_ini.modify("rtsp", "port", "554", "this is the listen port for http server");

	// Modify the comment.
	_ini.modifyComment("rtsp", "port", "this is the listen port for rtsp server ***");

	// Try to modify or add more.
	_ini.modify("http", "port", "8080", "this is the listen port for http server");

	// You have obtained the key-value pair and saved it to your config file.
	std::cout << "to string:\trtsp.port = " << _ini["rtsp"]["port"] << std::endl;

    // Convert to string, default is string
    std::string http_port_s = _ini["http"].toString("port");
	std::cout << "to string:\thttp.port = " << http_port_s << std::endl;

    // Convert to double
    double http_port_d = _ini["http"].toDouble("port");
	std::cout << "to double:\thttp.port = " << http_port_d << std::endl;

	// Convert to int
    int http_port_i = _ini["http"].toInt("port");
	std::cout << "to int:\t\thttp.port = " << http_port_i << std::endl;

	return 0;
}
