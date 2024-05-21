#include "inicpp.hpp"

#include <iostream>
#include <iomanip>


void printList(std::list<std::string>& theList)
{
	std::cout << (theList.size() > 0 ? "|" : "") ;
	for (const std::string& sectionName : theList)
	{
	        std::cout << sectionName << "|";
	}
	std::cout << std::endl << std::endl;
}


/**
  compile: g++ -I../ -std=c++11 main.cpp -o iniExample
*/
int main()
{
	/** 1. Load the INI file. */
	inicpp::IniManager _ini("config.ini");


	/** 2. Check if the key exists. */
	if (!_ini["rtsp"].isKeyExist("port"))
	{
		std::cout << "rtsp.port: not exist!" << "\n\n";
	}


	/** 3. Check if the section exists. */
	if (!_ini.isSectionExist("math"))
	{
	        std::cout << "section of math: not exist" << "\n\n";
	}


	/** 4. Get all sections as std::list<std::string>. */
	std::list<std::string> sectionList = _ini.getSectionsList(); // may contains Unnamed section, but keys at head of file
	std::cout << "Got Section Name List(size:" << sectionList.size() << ") :"; 
	printList(sectionList);


	/** 5. library head */
	_ini.modify("head", "title", "inicpp","thanks for your using inicpp project.");
	_ini.modify("head", "license", "MIT", "Permissive license for open-source software distribution.");


	/** 6. Add section-key-value pairs or Modify later. */
	_ini.modify("rtsp", "port", "554");
	_ini.modify("rtsp", "port", "555");
	_ini.modify("rtsp", "ip", "127.0.0.1");

	  // - Read key-value pairs directly.
	std::string rtsp_port = _ini["rtsp"]["port"];
	std::cout << "get rtsp port:" << rtsp_port << "\n\n";;

	  // - Add a comment.
	_ini.modify("rtsp", "port", "554", "this is the listen port for http server.");

	  // - Modify the comment.
	_ini.modifyComment("rtsp", "port", "this is the listen ip for rtsp server.");

	  // - You have obtained the key-value pair and saved it to your config file.
	std::cout << "to string:\trtsp.port = " << _ini["rtsp"]["port"] << std::endl;


	/** 7. Convert type. */
	_ini.modify("math", "PI", "3.1415926", "This is pi in mathematics.");

	  // Convert to string, default is string.
	std::cout << "to string:\tmath.PI   = " << _ini["math"]["PI"] << std::endl;
	std::cout << "to string:\tmath.PI   = " << _ini["math"].toString("PI") << std::endl;

	  // - Convert to double
	double http_port_d = _ini["math"].toDouble("PI");
	std::cout << "to double:\tmath.PI   = " << std::setprecision(10) << http_port_d << std::endl;

	  // - Convert to int
	int http_port_i = _ini["math"].toInt("PI");
	std::cout << "to int:\t\tmath.PI   = " << http_port_i << std::endl;

	  // - convert to std::wstring.
	std::locale::global(std::locale("")); // support wide string output

	std::wstring helloWorld_value = L"你好，世界"; // translate as : hello,world
	_ini.modify("other", "desc", helloWorld_value, "this test for std::wstring. comment it.");

	std::wstring wstr = _ini["other"].toWString("desc");
	std::wcerr << "to wstring:\tother.desc= " << wstr << "\n\n";


	/** 8. Unnamed section: at head of ini file,with no section name found. */
	_ini.modify("", "noSection", "no", "no section test.");
	_ini.modify("", "noSection", "yes", "no section test:add comment later.");

	_ini.modify("", "key0", "noSectionAndComment");
	_ini.modify("", "key1", "noSectionAndComment");
	_ini.modify("", "key2", "noSectionAndComment");


	/** Print all sections again */
	sectionList = _ini.getSectionsList();  // may contains Unnamed section, but keys at head of file.
	std::cout << "Got Section Name List(size:" << sectionList.size() << ") :"; 
	printList(sectionList);

	return 0;
}
