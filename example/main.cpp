#include "inicpp.hpp"
#include <iomanip>

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
	_ini.modify("math", "PI", "3.1415926", "This is pi in mathematics.");

	// You have obtained the key-value pair and saved it to your config file.
	std::cout << "to string:\trtsp.port = " << _ini["rtsp"]["port"] << std::endl;

	// Convert to string, default is string
	std::string http_port_s = _ini["math"].toString("PI");
	std::cout << "to string:\tmath.PI   = " << http_port_s << std::endl;

	// Convert to double
	double http_port_d = _ini["math"].toDouble("PI");
	std::cout << "to double:\tmath.PI   = " << std::setprecision(10) << http_port_d << std::endl;

	// Convert to int
	int http_port_i = _ini["math"].toInt("PI");
	std::cout << "to int:\t\tmath.PI   = " << http_port_i << std::endl;

	// std::wstring support
	std::locale::global(std::locale("")); // support wide string output

	std::wstring helloWorld_value = L"你好，世界"; // translate as : hello,world
	_ini.modify("other", "desc", helloWorld_value, "this test for std::wstring. comment only if any.");

	std::wstring wstr = _ini["other"].toWString("desc");
	std::wcerr << "to wstring:\tother.desc= " << wstr << std::endl;

	return 0;
}
