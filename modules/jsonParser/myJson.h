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
	int jsonToString(const std::string& filePath, std::string& reply) const;
	std::string removeEscapeChars(std::string input);
private:

};
