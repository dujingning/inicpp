/*
 * MIT License
 *
 * Copyright (c) 2023 dujingning <djn2019x@163.com> <https://github.com/dujingning/inicpp.git>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __JN_INICPP_H__
#define __JN_INICPP_H__

#include <cstddef>

#include <algorithm>
#include <fstream>
#include <string>
#include <list>
#include <map>


#ifdef _ENBABLE_INICPP_STD_WSTRING_ // Not all of C++ 11 support <codecvt> 
// for std::string <==> std::wstring convert
#include <codecvt>
#include <locale>
#endif

#ifdef INICPP_DEBUG

#include <array>
#include <ctime>
#include <iostream>

class TimeFormatter
{
public:
	static std::string format(const std::string &format = "%Y-%m-%d %H:%M:%S")
	{
		std::time_t t = std::time(nullptr);
		std::tm tm = *std::localtime(&t);
		std::array<char, 100> buffer;
		std::strftime(buffer.data(), buffer.size(), format.c_str(), &tm);
		return buffer.data();
	}
};

#define CODE_INFO std::string(" | Code:\'file:") + std::string(__FILE__) + ",function:" + std::string(__FUNCTION__) + ",line:" + std::to_string(__LINE__) + '\''
#define INI_DEBUG(x) std::cout << "INICPP " << TimeFormatter::format() << " : " << x << CODE_INFO << std::endl

#else // #ifdef INICPP_DEBUG
#define INI_DEBUG(x)
#endif // #ifdef INICPP_DEBUG

namespace inicpp
{

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

		explicit section(const std::string &sectionName) : _sectionName(sectionName)
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

		void setValue(const std::string &Key, const std::string &Value, const int line)
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

			for (const auto &data : _sectionMap)
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
			_lineNumber = -1;
			_sectionName.clear();
			_sectionMap.clear();
		}

		bool isEmpty() const
		{
			return _sectionMap.empty();
		}

		int toInt(const std::string &Key)
		{
			if (!_sectionMap.count(Key))
			{
				return 0;
			}

			int result = 0;

			try
			{
				result = std::stoi(_sectionMap[Key].Value);
			}
			catch (const std::invalid_argument &e)
			{
				INI_DEBUG("Invalid argument: " << e.what() << ",input:\'" << _sectionMap[Key].Value << "\'");
			}
			catch (const std::out_of_range &e)
			{
				INI_DEBUG("Out of range: " << e.what() << ",input:\'" << _sectionMap[Key].Value << "\'");
			}

			return result;
		}

		std::string toString(const std::string &Key)
		{
			if (!_sectionMap.count(Key))
			{
				return "";
			}
			return _sectionMap[Key].Value;
		}

#ifdef _ENBABLE_INICPP_STD_WSTRING_
		std::wstring toWString(const std::string &Key)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			return converter.from_bytes(toString(Key));
		}
#endif

		double toDouble(const std::string &Key)
		{
			if (!_sectionMap.count(Key))
			{
				return 0.0;
			}

			double result = 0.0;

			try
			{
				result = std::stod(_sectionMap[Key].Value);
			}
			catch (const std::invalid_argument &e)
			{
				INI_DEBUG("Invalid argument: " << e.what() << ",input:\'" << _sectionMap[Key].Value << "\'");
			}
			catch (const std::out_of_range &e)
			{
				INI_DEBUG("Out of range: " << e.what() << ",input:\'" << _sectionMap[Key].Value << "\'");
			}

			return result;
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
			return;
		}

		void removeSection(const std::string &sectionName)
		{
			if (!_iniInfoMap.count(sectionName))
			{
				return;
			}
			_iniInfoMap.erase(sectionName);
			return;
		}

		bool isSectionExists(const std::string &sectionName)
		{
			return !_iniInfoMap.count(sectionName) ? false : true;
		}

		// may contains default of Unnamed section with ""
		std::list<std::string> getSectionsList()
		{
			std::list<std::string> sectionList;
			for (const auto &data : _iniInfoMap)
			{
				if (data.first == "" && data.second.isEmpty()) // default section: no section name,if empty,not count it.
				{
					continue;
				}
				sectionList.emplace_back(data.first);
			}
			return sectionList;
		}

		const section &operator[](const std::string &sectionName)
		{
			if (!_iniInfoMap.count(sectionName))
			{
				return _iniInfoMap[""];
			}
			return _iniInfoMap[sectionName];
		}

		inline std::size_t getSectionSize()
		{
			return _iniInfoMap.size();
		}

		std::string getValue(const std::string &sectionName, const std::string &Key)
		{
			if (!_iniInfoMap.count(sectionName))
			{
				return "";
			}
			return _iniInfoMap[sectionName][Key];
		}

		// for none section
		int getLine(const std::string &Key)
		{
			if (!_iniInfoMap.count(""))
			{
				return -1;
			}
			return _iniInfoMap[""].getLine(Key);
		}

		// for section-key
		int getLine(const std::string &sectionName, const std::string &Key)
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
		std::map<std::string /*Section Name*/, section> _iniInfoMap;
	};

	// todo if file is modified,never write back
	class IniManager
	{
	public:
		explicit IniManager(const std::string &configFileName) : _configFileName(configFileName)
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

					trimEdges(key);
					trimEdges(value);

					sectionRecord.setValue(key, value, _SumOfLines);
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
				INI_DEBUG("Invalid parameter input: Key[" << Key << "],Value[" << Value << "]");
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
			bool isInputDataWited = false;

			do
			{
				// exist key at one section replace it, or need to create it
				if (_iniData.isSectionExists(Section))
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
								isInputDataWited = true;
								output << keyValueData;
							}

							output << lineData << "\n";
						}

						if (input.eof() && !isInputDataWited)
						{
							isInputDataWited = true;
							output << keyValueData;
						}

						break;
					}
				}

				if (line_number_mark <= 0) // not found key at config file
				{
					input.seekg(0, input.beg);

					bool isHoldSection = false;
					std::string newLine = "\n\n";
					if (Section != "" && Section.find("[") == std::string::npos && Section.find("]") == std::string::npos && Section.find("=") == std::string::npos)
					{
						if (_iniData.empty() || _iniData.getSectionSize() <= 0)
						{
							newLine.clear();
						}

						isHoldSection = true;
					}

					// 1.section is exist or empty section
					if (_iniData.isSectionExists(Section) || Section == "")
					{
						// write key/value to head
						if (isHoldSection)
						{
							output << newLine << "[" << Section << "]" << "\n";
						}
						output << keyValueData;
						// write others
						std::string lineData;
						while (std::getline(input, lineData))
						{
							output << lineData << "\n";
						}
					}
					// 2.section is not exist
					else
					{
						// write others
						std::string lineData;
						while (std::getline(input, lineData))
						{
							output << lineData << "\n";
						}
						// write key/value to end
						if (isHoldSection)
						{
							output << newLine << "[" << Section << "]" << "\n";
						}
						output << keyValueData;
					}

					break;
				}
				else
				{ // found, replace it

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

		bool modify(const std::string &Section, const std::string &Key, const int &Value, const std::string &comment = "")
		{
			std::string stringValue = std::to_string(Value);
			return modify(Section, Key, stringValue, comment);
		}

		bool modify(const std::string &Section, const std::string &Key, const double &Value, const std::string &comment = "")
		{
			std::string stringValue = std::to_string(Value);
			return modify(Section, Key, stringValue, comment);
		}

#ifdef _ENBABLE_INICPP_STD_WSTRING_
		bool modify(const std::string &Section, const std::string &Key, const std::wstring &Value, const std::string &comment = "")
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			std::string stringValue = converter.to_bytes(Value);

			return modify(Section, Key, stringValue, comment);
		}
#endif

		bool modifyComment(const std::string &Section, const std::string &Key, const std::string &comment)
		{
			return modify(Section, Key, (*this)[Section][Key], comment);
		}

		bool isSectionExists(const std::string &sectionName)
		{
			return _iniData.isSectionExists(sectionName);
		}

		inline std::list<std::string> getSectionsList()
		{
			return _iniData.getSectionsList();
		}

	private:
		bool filterData(std::string &data)
		{
			if (data.length() == 0)
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

		void trimEdges(std::string &data)
		{
			// remove left ' ' and '\t'
			data.erase(data.begin(), std::find_if(data.begin(), data.end(), [](unsigned char c)
												  { return !std::isspace(c); }));
			// remove right ' ' and '\t'
			data.erase(std::find_if(data.rbegin(), data.rend(), [](unsigned char c)
									{ return !std::isspace(c); })
						   .base(),
					   data.end());

			INI_DEBUG( "trimEdges data:|" << data << "|");
		}

	private:
		ini _iniData;
		int _SumOfLines;
		std::fstream _iniFile;
		std::string _configFileName;
	};

}

#endif
