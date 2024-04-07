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

    protected:
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

        FS::path path();

        PIMAGE_DOS_HEADER getDosHeader();
        PIMAGE_NT_HEADERS getNTHeaders();
        PIMAGE_SECTION_HEADER getSectionHeadersLocation();
        PIMAGE_IMPORT_DESCRIPTOR getImportDescriptor();

        void close(const char* message);
        void close();

        virtual ~PEParser();
};

template <typename T>
bool PEParser::open(T path)
{
    this->peFilePath = FS::canonical(FS::path(path));
    this->peFile = CreateFileA(this->peFilePath.string().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (this->peFile == INVALID_HANDLE_VALUE)
    {
        this->close("Failed to open file");
        return false;
    }

    if (!this->getFileSize()) return false;
    if (!this->readFileData()) return false;

    fprintf(stderr, "PEParser: File \"%s\" has been opened\n", this->peFilePath.string().c_str());

    return true;
}

template <typename T>
PEParser::PEParser(T path)
{
    this->open(path);
}

#endif