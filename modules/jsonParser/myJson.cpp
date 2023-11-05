#include <iostream>
#include "myJson.h"
#include <fstream>


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

int MyJSON::jsonToString(const std::string& filePath, std::string& reply) const
{
    std::ifstream ifs(filePath);
    if (!ifs.is_open()) {
        std::cerr << "Error: Cannot open file " << filePath << std::endl;
        return -1;
    }

    // read file into string
    std::string jsonString((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    // parse json string and return
    try {
        nlohmann::json json = nlohmann::json::parse(jsonString);
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
