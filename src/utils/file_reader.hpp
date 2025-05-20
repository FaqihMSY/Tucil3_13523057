#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include "../board/board.hpp"
#include <string>

class FileReader {
public:
    static Board readBoard(const std::string& filename);
};

#endif