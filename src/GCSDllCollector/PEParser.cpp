#include "GCSDllCollector/PEParser.h"

bool PEParser::getFileSize()
{
    this->peFileSize = GetFileSize(peFile, NULL);
    if (this->peFileSize == INVALID_FILE_SIZE)
    {
        this->close("Failed to open file");
        return false;
    }
    return true;
}

bool PEParser::readFileData()
{
    this->peFileData = HeapAlloc(GetProcessHeap(), 0, this->peFileSize);
    if (this->peFileData == NULL)
    {
        this->close("Failed to allocate memory for file data");
        return false;
    }

    if (!ReadFile(this->peFile, this->peFileData, this->peFileSize, NULL, NULL))
    {
        this->close("Failed to read file data");
        return false;
    }

    return true;
}

PEParser::PEParser()
{
    this->peFile = NULL;
    this->peFileSize = 0;
    this->peFileData = NULL;
    this->peFilePath = FS::path();
}

template <typename T>
bool PEParser::open(T path)
{
    this->peFilePath = FS::path(path);
    this->peFile = CreateFileA(this->peFilePath.string().c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (this->peFile == INVALID_HANDLE_VALUE)
    {
        this->close("Failed to open file");
        return false;
    }

    if (!this->getFileSize()) return false;
    if (!this->readFileData()) return false;

    return true;
}

template <typename T>
PEParser::PEParser(T path)
{
    this->open(path);
}

PIMAGE_DOS_HEADER PEParser::getDosHeader()
{
    return (PIMAGE_DOS_HEADER) this->peFileData;
}

PIMAGE_NT_HEADERS PEParser::getNTHeaders()
{
    PIMAGE_NT_HEADERS _return = (PIMAGE_NT_HEADERS) ((BYTE*) this->peFileData + this->getDosHeader()->e_lfanew);
    if (_return->Signature != IMAGE_NT_SIGNATURE)
    {
        this->close("Invalid PE file");
        return NULL;
    }
    return _return;
}

PIMAGE_IMPORT_DESCRIPTOR PEParser::getImportDescriptor()
{
    return (PIMAGE_IMPORT_DESCRIPTOR) ((BYTE*) this->peFileData + this->getNTHeaders()->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
}

void PEParser::close(const char* message)
{
    bool isError = false;
    if (strlen(message) > 0)
        fprintf(stderr, "%s\nInvalid file's path: \"%s\"\n", message, this->peFilePath.c_str());
    else
        fprintf(stderr, "File \"%s\" has been closed\n", this->peFilePath.c_str());

    if (this->peFileData != NULL)
        HeapFree(GetProcessHeap(), 0, this->peFileData);

    if (this->peFile != NULL)
        CloseHandle(this->peFile);

    this->peFilePath = FS::path();
    this->peFileSize = 0;
}