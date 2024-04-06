#ifndef __PE_PARSER_H__
#define __PE_PARSER_H__

#include <filesystem>

#include <windows.h>

namespace FS = std::filesystem;

class PEParser
{
    private:
        FS::path peFilePath;

        HANDLE peFile;
        DWORD peFileSize;
        LPVOID peFileData;

        bool getFileSize();
        bool readFileData();

    public:
        PEParser();

        template <typename T>
        PEParser(T path);

        template <typename T>
        bool open(T path);

        PIMAGE_DOS_HEADER getDosHeader();
        PIMAGE_NT_HEADERS getNTHeaders();
        PIMAGE_SECTION_HEADER getSectionHeaders();
        PIMAGE_IMPORT_DESCRIPTOR getImportDescriptor();

        void close(const char* message);
};

#endif