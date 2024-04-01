### Ⅰ、Project
You can see the project at [https://github.com/dujingning/inicpp.git](https://github.com/dujingning/inicpp.git) or [https://gitee.com/dujingning/inicpp](https://gitee.com/dujingning/inicpp).


---

### Ⅱ、Description

The INI header-only library for Modern C++ supports reading and writing, even writing comments. It is easy to use and can help simplify the process of working with INI files.

---
### Ⅲ、Usage

simple to use with C++11 or latter

##### * 1.read example
```cpp
#include "inicpp.cpp"

int main()
{
    // Load and parse the INI file.
    inicpp::iniReader _ini("config.ini");
    std::cout << _ini["rtsp"]["port"] << std::endl;
}
```

##### * 2.write example
```cpp
#include "inicpp.cpp"

int main()
{
    // Load and parse the INI file.
    inicpp::iniReader _ini("config.ini");
    _ini.modify("rtsp","port","554");
    std::cout << _ini["rtsp"]["port"] << std::endl;
}
```
##### * 3.or comment
```cpp
#include "inicpp.cpp"

int main()
{
    // Load and parse the INI file.
    inicpp::iniReader _ini("config.ini");
    _ini.modify("rtsp","port","554","this is the listen port for rtsp server");
    std::cout << _ini["rtsp"]["port"] << std::endl;
}
```
##### * 4.toString()、toInt()、toDouble()
```cpp
#include "inicpp.cpp"

int main()
{
	// Load and parse the INI file.
	inicpp::iniReader _ini("config.ini");
	_ini.modify("rtsp","port","554","this is the listen port for rtsp server");
	std::cout << _ini["rtsp"]["port"] << std::endl;

	// Convert to string, default is string
	std::string http_port_s = _ini["http"].toString("port");
	std::cout << "to string:\thttp.port = " << http_port_s << std::endl;

	// Convert to double
	double http_port_d = _ini["http"].toDouble("port");
	std::cout << "to double:\thttp.port = " << http_port_d << std::endl;

	// Convert to int
	int http_port_i = _ini["http"].toInt("port");
	std::cout << "to int:\t\thttp.port = " << http_port_i << std::endl;
}
```
##### * 5.For a full example, see example/main.cpp. You can compile it with `g++ -I../ -std=c++11 main.cpp -o iniExample` or any other method you prefer.


```cpp
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

	return 0;
}
```
* let the example work 
```bash
[root@jn inicpp]# ls
example  inicpp.hpp  LICENSE  README.md
[root@jn inicpp]# cd example/
[root@jn example]# grep -Rn g++
main.cpp:4:/* compile: g++ -I../ -std=c++11 main.cpp -o iniExample */
[root@jn example]# g++ -I../ -std=c++11 main.cpp -o iniExample
[root@jn example]# ls
iniExample  main.cpp
[root@jn example]# ./iniExample
rtsp.port is not exist!
to string:      rtsp.port = 554
to string:      math.PI   = 3.1415926
to double:      math.PI   = 3.1415926
to int:         math.PI   = 3
[root@jn example]# ls
config.ini  iniExample  main.cpp
[root@jn example]# cat config.ini
[rtsp]
;this is the listen port for rtsp server ***
port=554


[math]
;This is pi in mathematics.
PI=3.1415926
[root@jn example]#
```

---
### Ⅳ、End
 The project was created by dujingning. 