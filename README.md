### Ⅰ、Project
You can view the project at [https://github.com/dujingning/inicpp.git](https://github.com/dujingning/inicpp.git) or [https://gitee.com/dujingning/inicpp](https://gitee.com/dujingning/inicpp).

#### * To support open source, please give us a star. Thank you very much!


---

### Ⅱ、Description

The INI header-only library for Modern C++ supports **reading**, **writing**, and even **commenting**. It is easy to use and simplifies working with INI files.

- New Feature : [Super Easy Binding to Your Data Structures (For Read)](#6super-easy-binding-to-your-data-structures-read).


---

### Ⅲ、Usage 

#### * 0.Simple to use with C++11 or later.
```
git clone https://github.com/dujingning/inicpp.git
```

Include `inicpp.hpp`, declare the `inicpp::IniManager` class, and you're all set.

#### 1.read example
Read: easy to use.
```cpp
#include "inicpp.hpp"
#include <iostream>

int main()
{
    inicpp::IniManager _ini("config.ini"); // Load and parse the INI file.

    int         port = _ini["server"]["port"];
    std::string ip   = _ini["server"]["ip"];

    std::cout << ip << ":" << port << std::endl;
}
```

#### 2.write example
Write: Modify directly to the file.
```cpp
#include "inicpp.hpp"
#include <iostream>

int main()
{
    inicpp::IniManager _ini("config.ini"); // Load and parse the INI file.

    _ini.modify("server","ip","192.168.3.35");
    _ini.modify("server","port",554);
    std::cout << _ini["server"]["port"] << std::endl;
}
```
#### 3.comment example
Comment: Write comments for key-value pairs.
```cpp
#include "inicpp.hpp"
#include <iostream>

int main()
{
    inicpp::IniManager _ini("config.ini"); // Load and parse the INI file.
    // comment section/key
    _ini.modifyComment("server", "port", "this is the listen ip for server.");
}
```
#### 4.toString()、toInt()、toDouble()
Convert: From string to type (No exception).
```cpp
#include "inicpp.hpp"
#include <iostream>

int main()
{
    inicpp::IniManager _ini("config.ini"); // Load and parse the INI file.
    _ini.modify("server","port","554","this is the listen port for server");
    std::cout << _ini["server"]["port"] << std::endl;

    // Convert to string, default is string
    std::string http_port_s = _ini["server"].toString("port");
    std::cout << "to string:\tserver.port = " << http_port_s << std::endl;

    // Convert to double
    double http_port_d = _ini["server"].toDouble("port");
    std::cout << "to double:\tserver.port = " << http_port_d << std::endl;

    // Convert to int
    int http_port_i = _ini["server"].toInt("port");
    std::cout << "to int:\t\tserver.port = " << http_port_i << std::endl;
}
```
#### 5.isKeyExists()、isSectionExists()、getSectionsList()
May contain unnamed sections: when keys are at the head of the file.
```bash
#include "inicpp.hpp"
#include <iostream>

int main()
{
    inicpp::IniManager _ini("config.ini");

    // isKeyExist
    if (!_ini["server"].isKeyExist("port"))
    {
        std::cout << "server.port: not exist!" << "\n";
    }

    // isSectionExists
    if (!_ini.isSectionExists("math"))
    {
        std::cout << "section of math: not exist" << "\n\n";
    }

    // getSectionsList
    std::list<std::string> sectionList = _ini.getSectionsList();

    for(auto data:sectionList){ // print
        std::cout << data << std::endl;
    }
}
```

#### 6.Super Easy Binding to Your Data Structures (Read).

config.ini :
```
title=config.ini
[server]
isKeepalived=true
;this is the listen ip for server.
port=8080
ip=127.0.0.1


[math]
;Comment: This is pi in mathematics.
PI=3.141592653589793238462643383279502884
```
config.cpp
```cpp
#include "inicpp.hpp"
#include <iostream>
#include <iomanip>

#define CONFIG_FILE "config.ini"

class appConfig
{
public:
	typedef struct Config
	{
		typedef struct Server
		{
			std::string ip;
			unsigned short port;
			bool isKeepalived;
		} Server;

		std::string title;
		Server server;
		double PI;
	} Config;

	static const Config readConfig()
	{
		inicpp::IniManager _ini(CONFIG_FILE);

		return Config{
			title : _ini[""]["title"],
			server : {ip : _ini["server"]["ip"],
					  port : _ini["server"]["port"],
					  isKeepalived : _ini["server"]["isKeepalived"]},
			PI : _ini["math"]["PI"],
		};
	}
};

int main()
{
	/** easy read for app as struct */
	appConfig::Config config = appConfig::readConfig();
	std::cout << "title:      \t" << config.title << std::endl;
	std::cout << "server.port:\t" << config.server.port << std::endl;
	std::cout << "server.ip:  \t" << config.server.ip << std::endl;
	std::cout << "server.alive:\t" << config.server.isKeepalived << std::endl;
	std::cout << "math.PI:    \t" << std::setprecision(20) << config.PI << std::endl;
	return 0;
}
```

#### 7.how to use example/main.cpp
You can compile it using `example/Makefile` or any other method you prefer.

If make is not available, use the following command: `g++ -I../ -std=c++11 main.cpp -o iniExample`.

- Compile `example/main.cpp`
```bash
[jn@jn inicpp]$ ls
example  inicpp.hpp  LICENSE  README.md
[jn@jn inicpp]$ cd example/
[jn@jn example]$ make
g++ -I../ -std=c++11 main.cpp -o iniExample
[jn@jn example]$ ls
iniExample  main.cpp  Makefile
```

- Run example app `iniExample`
```bash
[root@VM-24-13-centos example]# ./iniExample
title:          config.ini
server.port:    8080
server.ip:      127.0.0.1
server.alive:   1
math.PI:        3.141592653589793116
[root@VM-24-13-centos example]#
```

- Configuration file `config.ini` has been created.
```bash
[root@VM-24-13-centos example]# cat config.ini
title=config.ini
[server]
isKeepalived=true
;this is the listen ip for server.
port=8080
ip=127.0.0.1


[math]
;Comment: This is pi in mathematics.
PI=3.141592653589793238462643383279502884
[root@VM-24-13-centos example]#
```

---

### Ⅳ、Star History
<a href="https://star-history.com/#dujingning/inicpp">
 <picture>
   <source media="(prefers-color-scheme: dark)" srcset="https://api.star-history.com/svg?repos=dujingning/inicpp&type=Date&theme=dark" />
   <source media="(prefers-color-scheme: light)" srcset="https://api.star-history.com/svg?repos=dujingning/inicpp&type=Date" />
   <img alt="Star History Chart" src="https://api.star-history.com/svg?repos=dujingning/inicpp&type=Date" />
 </picture>
</a>

---
### Ⅴ、End
 The project was created by DuJingning.


