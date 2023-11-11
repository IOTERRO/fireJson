#include <boost/locale.hpp>
#include <utf8.h>

#include <iostream>
#include <fstream>

#include "myJson.h"


using json = nlohmann::json;

NLOHMANN_JSON_SERIALIZE_ENUM(
    MyJSON::MyJsonEnum,
    {
        {MyJSON::MyJsonEnum::Ok, "OK"},
        {MyJSON::MyJsonEnum::Ko, "KO"},
        {MyJSON::MyJsonEnum::Unknown, "UNKNOWN"}
    })


MyJSON::MyJSON()
{

}

MyJSON::~MyJSON()
{

}

void MyJSON::toJson(const std::string& jsonFile, jsonInfos infos)
{
    json j = {
       {"bigUser", "omar"},
       {"matcheBigUser", infos.name == "omar"},
       {"nothing", nullptr},
       {"answer", {
           {"result", infos.result}
       }},
       {"list", {infos.result, infos.result+2, infos.result*2}},
       {"user", {
           {"name", infos.name},
           {"age", infos.age}
       }}
    };

    //Ecriture de l'objet JSON dans un fichier
    std::ofstream file(jsonFile);
    file << j.dump(4);
    file.close();
}

jsonInfos MyJSON::fromJson(const std::string& jsonFile)
{
    //read jsonFile

    // Open the JSON file for reading
    std::ifstream i(jsonFile);

    // Parse the JSON data from the file
    json myJsonFile;
    i >> myJsonFile;

    // Access the data from the JSON object and assign it to the jsonInfos struct
    jsonInfos infos;
    infos.name = myJsonFile["name"].get<std::string>();
    infos.age = myJsonFile["age"].get<int>();
    infos.result = myJsonFile["result"].get<int>();

    // return for json data
    return infos;
}

int MyJSON::jsonToString(const std::string& filePath, std::string& reply, std::string contentUTF8) const
{
    // parse json string and return
    try {
        nlohmann::json json = nlohmann::json::parse(contentUTF8);
        reply = json.dump();
        return 0;
    }
    catch (const std::exception& e) {
        auto what = e.what();
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
}


std::string MyJSON::removeEscapeChars(std::string input) {
    std::string output;
    bool escape = false;

    for (size_t i = 0; i < input.length(); i++) {
        if (escape) {
            escape = false;
            output += input[i];
        }
        else if (input[i] == '\\') {
            escape = true;
            continue;
        }
        else {
            output += input[i];
        }
    }

    return output;
}


bool MyJSON::convertFileToUtf8(const std::string& filePath, std::string& contentUtf8) const
{
    // Read the content of the file into a string
    const std::string content = fileToString(filePath);

    // If the encoding is not UTF-8, convert the content to UTF-8
    if (!ifUtf8(filePath)) {
        contentUtf8 = boost::locale::conv::to_utf<char>(content, "UTF-8"); // Replace "ISO-8859-1" with the actual encoding
    }
    else {
        contentUtf8 = content;
    }

    return true;
}

bool MyJSON::ifUtf8(const std::string& filePath) const
{
    // Detect the encoding using utf8cpp
    if(auto content= fileToString(filePath); utf8::is_valid(content.begin(), content.end()))
    {
        return true;
    }

    return false;
}

std::string MyJSON::fileToString(const std::string& filePath) const
{
    // Read the content of the file
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open the file." << std::endl;
        return false;
    }

    // Read the content of the file into a string
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}
