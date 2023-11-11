#pragma once

#include "export.h"
#include "nlohmann/json.hpp"

struct jsonInfos
{
	jsonInfos(): 
    age(0), result(0)
	{}
	std::string name;
	int age;
	int result;
};
class MYJSON_API MyJSON
{
public:
	MyJSON();
	MyJSON(const MyJSON&) = delete;
	MyJSON& operator=(const MyJSON&) = delete;
	~MyJSON();
	enum MyJsonEnum {
		Ok,
		Ko,
		Unknown
	};

    static void toJson(const std::string& jsonFile, jsonInfos infos);
    static jsonInfos fromJson(const std::string& jsonFile);
	bool convertFileToUtf8(const std::string& filePath, std::string& contentUtf8) const;
	bool ifUtf8(const std::string& filePath) const;
	std::string fileToString(const std::string& filePath) const;
	int jsonToString(const std::string& filePath, std::string& reply, std::string contentUTF8 = std::string()) const;
	std::string removeEscapeChars(std::string input);
private:

};
