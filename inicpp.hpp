/*
		creater		:	dujingning
		date		:	2023
		license		:	MIT
		project		:	https://github.com/dujingning/inicpp.git or https://gitee.com/dujingning/inicpp
		description	:	Easy to use and simple to implement, this header-only library allows for reading and writing of INI files, \
					even with comments and cross-platform support with C++11.

*/

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <map>

#include <cstdio>

namespace inicpp
{

#ifdef INICPP_DEBUG
#define INI_DEBUG(x) std::cout << x << std::endl
#else
#define INI_DEBUG(x)
#endif

	typedef struct KeyValueNode
	{
		std::string Value = "";
		int lineNumber = -1; // text line start with 1
	} KeyValueNode;

	class section
	{
	public:
		section() : _sectionName()
		{
		}

		section(const std::string sectionName) : _sectionName(sectionName)
		{
		}

		const std::string &name()
		{
			return _sectionName;
		}

		const std::string getValue(const std::string &Key)
		{
			if (!_sectionMap.count(Key))
			{
				return "";
			}
			return _sectionMap[Key].Value;
		}

		void setName(const std::string &name, const int &lineNumber)
		{
			_sectionName = name;
			_lineNumber = lineNumber;
		}

		void setValue(std::string &Key, std::string &Value, int line)
		{
			_sectionMap[Key].Value = Value;
			_sectionMap[Key].lineNumber = line;
		}

		void append(section &sec)
		{
			_sectionMap.insert(sec._sectionMap.begin(), sec._sectionMap.end());
		}

		bool isKeyExist(const std::string &Key)
		{
			return !_sectionMap.count(Key) ? false : true;
		}

		int getEndSection()
		{
			int line = -1;

			if (_sectionMap.empty() && _sectionName != "")
			{
				return _lineNumber;
			}

			for (auto &data : _sectionMap)
			{
				if (data.second.lineNumber > line)
				{
					line = data.second.lineNumber;
				}
			}
			return line;
		}

		int getLine(const std::string &Key)
		{
			if (!_sectionMap.count(Key))
			{
				return -1;
			}
			return _sectionMap[Key].lineNumber;
		}

		const std::string operator[](const std::string &Key)
		{
			if (!_sectionMap.count(Key))
			{
				return "";
			}
			return _sectionMap[Key].Value;
		}

		void clear()
		{
			_sectionMap.clear();
			_sectionMap.clear();
		}

		bool isEmpty()
		{
			return _sectionMap.empty();
		}

		// todo : add toString() / toInt() / toDouble()

	private:
		std::string _sectionName;
		std::map<std::string, KeyValueNode> _sectionMap;
		int _lineNumber = -1; // text line start with 1
	};

	class ini
	{
	public:
		void addSection(section &sec)
		{
			if (_iniInfoMap.count(sec.name())) // if exist,need to merge
			{
				_iniInfoMap[sec.name()].append(sec);
				return;
			}
			_iniInfoMap.emplace(sec.name(), sec);
		}

		void removeSection(const std::string &sectionName)
		{
			if (!_iniInfoMap.count(sectionName))
			{
				return;
			}
			_iniInfoMap.erase(sectionName);
		}

		bool isSectionExist(std::string sectionName)
		{
			return !_iniInfoMap.count(sectionName) ? false : true;
		}

		const section &operator[](const std::string sectionName)
		{
			if (!_iniInfoMap.count(sectionName))
			{
				return _iniInfoMap[""];
			}
			return _iniInfoMap[sectionName];
		}

		std::string getValue(std::string sectionName, std::string Key)
		{
			if (!_iniInfoMap.count(sectionName))
			{
				return "";
			}
			return _iniInfoMap[sectionName][Key];
		}

		// for none section
		int getLine(std::string Key)
		{
			if (!_iniInfoMap.count(""))
			{
				return -1;
			}
			return _iniInfoMap[""].getLine(Key);
		}

		// for section-key
		int getLine(std::string sectionName, std::string Key)
		{
			if (!_iniInfoMap.count(sectionName))
			{
				return -1;
			}
			return _iniInfoMap[sectionName].getLine(Key);
		}

		inline void clear() { _iniInfoMap.clear(); }
		inline bool empty() { return _iniInfoMap.empty(); }

	protected:
		std::map<std::string, section> _iniInfoMap;
	};

	// todo if file is modified,never write back
	class IniManager
	{
	public:
		explicit IniManager(std::string configFileName) : _configFileName(configFileName)
		{
			parse();
		}

		~IniManager()
		{
			_iniFile.close();
		}

		section operator[](const std::string &sectionName)
		{
			return _iniData[sectionName];
		}

		void parse()
		{
			_iniFile.open(_configFileName, std::ifstream::in | std::ifstream::out | std::fstream::app);
			if (!_iniFile.is_open())
			{
				INI_DEBUG("Failed to open the input INI file for parsing!");
				return;
			}

			_iniData.clear();

			_iniFile.seekg(0, _iniFile.beg);
			std::string data, sectionName;
			int sectionLine = -1;

			section sectionRecord;

			_SumOfLines = 1;
			do
			{
				unsigned int position = _iniFile.tellg();
				std::getline(_iniFile, data);

				if (!filterData(data))
				{
					++_SumOfLines;
					continue;
				}

				if (data.find('[') == 0) // section
				{
					if (!sectionRecord.isEmpty() || sectionRecord.name() != "")
					{
						_iniData.addSection(sectionRecord);
					}

					size_t first = data.find('[');
					size_t last = data.find(']');

					if (last == std::string::npos)
					{
						++_SumOfLines;
						continue;
					}

					sectionName = data.substr(first + 1, last - first - 1);
					sectionLine = _SumOfLines;

					sectionRecord.clear();
					sectionRecord.setName(sectionName, sectionLine);
				}

				size_t pos = data.find('=');
				if (pos != std::string::npos)
				{ // k=v
					std::string key = data.substr(0, pos);
					std::string value = data.substr(pos + 1);

					sectionRecord.setValue(key, value, _SumOfLines);

					INI_DEBUG(" -- key-path:\t" << sectionName << "." << key << "." << value);
				}

				++_SumOfLines;

			} while (!_iniFile.eof());

			if (!sectionRecord.isEmpty())
			{
				sectionRecord.setName(sectionName, -1);
				_iniData.addSection(sectionRecord);
			}

			if (_iniFile.is_open())
			{
				_iniFile.close();
			}
		}

		bool modify(const std::string &Section, const std::string &Key, const std::string &Value, const std::string &comment = "")
		{ // todo: insert comment before k=v
			parse();

			if (Key == "" || Value == "")
			{
				return false;
			}

			std::string keyValueData = Key + "=" + Value + "\n";
			if (comment.length() > 0)
			{
				keyValueData = comment + "\n" + keyValueData;
				if (comment[0] != ';')
				{
					keyValueData = ";" + keyValueData;
				}
			}

			const std::string &tempFile = ".temp.ini";
			std::fstream input(_configFileName, std::ifstream::in | std::ifstream::out | std::fstream::app);
			std::ofstream output(tempFile);

			if (!input.is_open())
			{
				INI_DEBUG("Failed to open the input INI file for modification!");
				return false;
			}

			if (!output.is_open())
			{
				INI_DEBUG("Failed to open the input INI file for modification!");
				return false;
			}

			int line_number_mark = -1;

			do
			{
				// exist key replace it, or need to create it
				if (_iniData.isSectionExist(Section))
				{
					line_number_mark = (*this)[Section].getLine(Key);

					if (line_number_mark == -1)
					{ // section exist, key not exist
						line_number_mark = (*this)[Section].getEndSection();

						std::string lineData;
						int input_line_number = 0;
						while (std::getline(input, lineData))
						{
							++input_line_number;

							if (input_line_number == (line_number_mark + 1))
							{ // new line,append to next line
								output << keyValueData;
							}

							output << lineData << "\n";
						}
						break;
					}
				}

				if (line_number_mark <= 0)
				{ // not find section/key write to end
					input.seekg(0, input.beg);

					std::string lineData;
					while (std::getline(input, lineData))
					{
						output << lineData << "\n";
					}

					if (Section != "" && Section.find("[") == std::string::npos && Section.find("]") == std::string::npos && Section.find("=") == std::string::npos)
					{
						std::string newLine = "\n\n";
						if (_iniData.empty())
						{
							newLine.clear();
						}

						output << newLine << "[" << Section << "]"
							   << "\n";
					}

					output << keyValueData;
					break;
				}

				if (line_number_mark > 0)
				{ // replace it
					std::string lineData;
					int input_line_number = 0;

					while (std::getline(input, lineData))
					{
						++input_line_number;

						// delete old comment if new comment is set
						if (input_line_number == (line_number_mark - 1) && lineData.length() > 0 && lineData[0] == ';' && comment != "")
						{
							continue;
						}

						if (input_line_number == line_number_mark)
						{ // replace to this line
							output << keyValueData;
						}
						else
						{
							output << lineData << "\n";
						}
					}
					break;
				}

				INI_DEBUG("error! inicpp lost process of modify function");
				return false;

			} while (false);

			// clear work
			input.close();
			output.close();

			std::remove(_configFileName.c_str());
			std::rename(tempFile.c_str(), _configFileName.c_str());

			// reload
			parse();

			return true;
		}

		bool modifyComment(const std::string &Section, const std::string &Key, const std::string &comment)
		{
			return modify(Section, Key, (*this)[Section][Key], comment);
		}

	private:
		bool filterData(std::string &data)
		{
			// filter
			data.erase(std::remove_if(data.begin(), data.end(), [](char c)
									  { return c == ' ' || c == '\t'; }),
					   data.end());

			if (data.length() <= 0)
			{
				return false;
			}

			if (data[0] == ';')
			{
				return false;
			}

			if (data[0] == '#')
			{
				return false;
			}

			return true;
		}

	private:
		ini _iniData;
		int _SumOfLines;
		std::fstream _iniFile;
		std::string _configFileName;
	};

}
