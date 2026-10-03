#pragma once
#include <algorithm>
#include <string>
#include <vector>

#include "3_string.h"

namespace schermate {
    struct Content {
        std::vector<std::string> data = {};
        unsigned int maxSize = 0;

        explicit Content(const std::vector<std::string>& contenuto) : data(contenuto) {
            maxSize = utils::getMaxSize(contenuto);
        }
        explicit Content(const std::string& s) : data(utils::parseStringVector(s)) {
            maxSize = utils::getMaxSize(data);
        }
        Content(const std::vector<std::string>& data, const unsigned int maxSize)
            : data(data), maxSize(maxSize) {}

        Content& operator += (const std::vector<std::string>& toAdd) {
            data.insert(data.end(), toAdd.begin(), toAdd.end());
            maxSize = std::max(maxSize, utils::getMaxSize(toAdd));
            return *this;
        }
        Content& operator += (const std::string& toAdd) {
            data.emplace_back(toAdd);
            maxSize = std::max(maxSize, static_cast<unsigned int>(toAdd.length()));
            return *this;
        }
    };

    inline std::vector<std::string> aggiungiBordi(
        const Content& contenuto,
        const char WallHorizontal = '-', const char WallVertical = '|',
        const int paddingHorizontal = 0, const int paddingVertical = 0)
    {
        std::vector<std::string> out;
        const std::string paddingRow = WallVertical
            + utils::ripeti(contenuto.maxSize + 2 * paddingHorizontal, " ")
            + WallVertical;
        const std::string borderRow = utils::ripeti(
            contenuto.maxSize + 2 + 2 * paddingHorizontal, {WallHorizontal});

        out.reserve(contenuto.data.size() + 2 * paddingVertical + 2);

        out.emplace_back(borderRow);                          // bordo superiore
        for (int i = 0; i < paddingVertical; i++)
            out.emplace_back(paddingRow);                     // padding superiore

        for (const std::string& s : contenuto.data)
            out.emplace_back(
                WallVertical
                + utils::ripeti(paddingHorizontal, " ")
                + s
                + utils::ripeti(contenuto.maxSize - s.length(), " ")
                + utils::ripeti(paddingHorizontal, " ")
                + WallVertical
            );

        for (int i = 0; i < paddingVertical; i++)
            out.emplace_back(paddingRow);                     // padding inferiore
        out.emplace_back(borderRow);                          // bordo inferiore
        return out;
    }
}
