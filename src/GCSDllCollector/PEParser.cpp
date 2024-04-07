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
    
    DWORD bytesRead = 0;
    if (!ReadFile(this->peFile, this->peFileData, this->peFileSize, &bytesRead, NULL))
    {
        this->close("Failed to read file data");
        return false;
    }

    fprintf(stderr, "%lu bytes were read\n", bytesRead);
    return true;
}

PEParser::PEParser()
{
    this->peFile = NULL;
    this->peFileSize = 0;
    this->peFileData = NULL;
    this->peFilePath = FS::path();
}

FS::path PEParser::path()
{
    return this->peFilePath;
}

PIMAGE_DOS_HEADER PEParser::getDosHeader()
{
    return (PIMAGE_DOS_HEADER) this->peFileData;
}

PIMAGE_NT_HEADERS PEParser::getNTHeaders()
{
    PIMAGE_DOS_HEADER dosHeader = this->getDosHeader();
    fprintf(stderr, "Adress of dosHeader: %p\n", dosHeader);
    PIMAGE_NT_HEADERS _return = (PIMAGE_NT_HEADERS) ((BYTE*) this->peFileData + dosHeader->e_lfanew);
    if (_return->Signature != IMAGE_NT_SIGNATURE)
    {
        this->close("Invalid PE file");
        return NULL;
    }
    return _return;
}

PIMAGE_SECTION_HEADER PEParser::getSectionHeadersLocation()
{
    return (PIMAGE_SECTION_HEADER) ((BYTE*) this->getNTHeaders() + sizeof(IMAGE_NT_HEADERS));
}

PIMAGE_IMPORT_DESCRIPTOR PEParser::getImportDescriptor()
{
    PIMAGE_SECTION_HEADER importSection = NULL;
    PIMAGE_NT_HEADERS ntHeaders = this->getNTHeaders();
    PIMAGE_SECTION_HEADER sectionHeadersLocation = this->getSectionHeadersLocation();
	DWORD importDirectoryRVA = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

    for (int i = 0; i < ntHeaders->FileHeader.NumberOfSections; i++)
    {
        PIMAGE_SECTION_HEADER sectionHeader = sectionHeadersLocation + (DWORD) sizeof(IMAGE_SECTION_HEADER)*i;
        if (importDirectoryRVA >= sectionHeader->VirtualAddress && importDirectoryRVA < sectionHeader->VirtualAddress + sectionHeader->Misc.VirtualSize)
			importSection = sectionHeader;
    }

    auto rawOffset = (BYTE*) this->peFileData + importSection->PointerToRawData;
    return (PIMAGE_IMPORT_DESCRIPTOR) (rawOffset + (ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress - importSection->VirtualAddress));
}

void PEParser::close(const char* message)
{
    if (this->peFilePath == "") return;

    if (strlen(message) > 0)
        fprintf(stderr, "PEParser: %s\nInvalid file's path: \"%s\"\n", message, this->peFilePath.string().c_str());
    else
        fprintf(stderr, "PEParser: File \"%s\" has been closed\n", this->peFilePath.string().c_str());

    if (this->peFileData != NULL)
        HeapFree(GetProcessHeap(), 0, this->peFileData);

    if (this->peFile != NULL)
        CloseHandle(this->peFile);

    this->peFilePath = FS::path();
    this->peFileSize = 0;
}

void PEParser::close()
{
    this->close("");
}

PEParser::~PEParser()
{
    fprintf(stderr, "PEParser: Deleting object from file \"%s\"...\n", this->peFilePath.string().c_str());
    this->close("");
    fprintf(stderr, "PEParser: The object has been deleted successfully.\n");
}