#pragma once

//////////////////////////////////////////////////////////////////
// CREDITS : This code comes from the Irradiance game-Engine    //
// Authors :                                                    //
//          Aymeric MATTE,                                      //
//          Colyn CO,                                           //
//          Ryad MAHIOU,                                        //
//          Ethan ALVES                                         //
//////////////////////////////////////////////////////////////////

#pragma region Extneral Dependencies

#include <nlohmann.hpp>
#include <fstream>
#include <string>

#pragma endregion

using json = nlohmann::json;

class ISerializable {
public:

    virtual std::string getType() const = 0;
    virtual void serialize(json& _json) const = 0;
    virtual void deserialize(const json& _json) = 0;
};

namespace JSONFILE {
    inline json loadFromFile(const std::string& _path) {
        std::ifstream file(_path);
        if (!file.is_open()) {
            throw std::runtime_error("Error: Could not open file " + _path);
        }

        json stream;
        file >> stream;
        return stream;
    }

    inline void saveToFile(const std::string& _path, const json& _json) {
        std::ofstream file(_path);
        if (!file.is_open()) {
            throw std::runtime_error("Error: Could not open file " + _path);
        }

        file << _json.dump(4);
    }
}