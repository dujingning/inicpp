### 一、Remember
You can see the project at [https://github.com/dujingning/inicpp.git](https://github.com/dujingning/inicpp.git) or [https://gitee.com/dujingning/inicpp](https://github.com/dujingning/inicpp.git).


---

### 二、description

The INI header-only library for Modern C++ supports reading and writing, even writing comments. It is easy to use and can help simplify the process of working with INI files.（现代 C++ 的 INI header-only 库支持读写，支持写注释，易于使用。）


### 三、usage

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
##### * 4.For a full example, see example/main.cpp. You can compile it with `g++ -I../ -std=c++11 main.cpp -o iniExample` or any other method you prefer.

```cpp
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
	std::cout << "rtsp.port = " << _ini["rtsp"]["port"] << std::endl;
	std::cout << "http.port = " << _ini["http"]["port"] << std::endl;

	return 0;
}
```
##### * let the example work 
```bash
[root@jn inicpp]# cd example/
[root@jn example]# g++ -I../ -std=c++11 main.cpp -o iniExample
[root@jn example]# ./iniExample
rtsp.port is not exist!
rtsp.port = 554
http.port = 8080
[root@jn example]# cat config.ini
[rtsp]
;this is the listen port for rtsp server ***
port=554


[http]
;this is the listen port for http server
port=8080
[root@jn example]#
```

---
### 四、尾声（end）:
 The project was created by dujingning. And next way is add toString()/toInt()/toDouble().