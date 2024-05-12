### Ⅰ、Project
You can see the project at [https://github.com/dujingning/inicpp.git](https://github.com/dujingning/inicpp.git) or [https://gitee.com/dujingning/inicpp](https://gitee.com/dujingning/inicpp).


---

### Ⅱ、Description

The INI header-only library for Modern C++ supports reading and writing, even writing comments. It is easy to use and can help simplify the process of working with INI files.

---
### Ⅲ、Usage 

#### * 0.simple to use with C++11 or latter
```
git clone https://github.com/dujingning/inicpp.git
```

include `inicpp.hpp`,declare class of `inicpp::iniReader`,then all done.

#### * 1.read example
```cpp
#include "inicpp.hpp"

int main()
{
    // Load and parse the INI file.
    inicpp::iniReader _ini("config.ini");
    std::cout << _ini["rtsp"]["port"] << std::endl;
}
```

#### * 2.write example
```cpp
#include "inicpp.hpp"

int main()
{
    // Load and parse the INI file.
    inicpp::iniReader _ini("config.ini");
    _ini.modify("rtsp","port","554");
    std::cout << _ini["rtsp"]["port"] << std::endl;
}
```
#### * 3.or comment
```cpp
#include "inicpp.hpp"

int main()
{
    // Load and parse the INI file.
    inicpp::iniReader _ini("config.ini");
    _ini.modify("rtsp","port","554","this is the listen port for rtsp server");
    std::cout << _ini["rtsp"]["port"] << std::endl;
}
```
#### * 4.toString()、toInt()、toDouble()
```cpp
#include "inicpp.hpp"

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
#### * 5.For a full example, see `example/main.cpp`.


You coulde compile it with `example/Makefile` or any other method you prefer.

if no make, use command: `g++ -I../ -std=c++11 main.cpp -o iniExample`

#### * 6.how to compile example/main.cpp
- compile `example/main.cpp`
```bash
[jn@jn inicpp]$ ls
example  inicpp.hpp  LICENSE  README.md
[jn@jn inicpp]$ cd example/
[jn@jn example]$ make
g++ -I../ -std=c++11 main.cpp -o iniExample
[jn@jn example]$ ls
iniExample  main.cpp  Makefile
```

- run example app `iniExample`
```bash
[jn@jn example]$ ./iniExample
get rtsp port:555
to string:      rtsp.port = 554
to string:      math.PI   = 3.1415926
to string:      math.PI   = 3.1415926
to double:      math.PI   = 3.1415926
to int:         math.PI   = 3
to wstring:     other.desc= 你好，世界
[jn@jn example]$
```

- then you got config file `config.ini`
```bash
[jn@jn example]$ cat config.ini
;no section test:add comment later.
noSection=yes
key0=noSectionAndComment
key1=noSectionAndComment
key2=noSectionAndComment
[head]
;thanks for your using inicpp project.
title=inicpp
;Permissive license for open-source software distribution.
license=MIT


[rtsp]
;this is the listen ip for rtsp server.
port=554
ip=127.0.0.1


[math]
;This is pi in mathematics.
PI=3.1415926


[other]
;this test for std::wstring. comment it.
desc=你好，世界
[jn@jn example]$
```

---
### Ⅳ、End
 The project was created by dujingning. 