#include "GCSPEParser/DllWalker.h"

FS::path PEFileDeps::Path()
{
    return this->PEParser::path();
}

PEFileDeps::~PEFileDeps()
{
    fprintf(stderr, "PEFileDeps: object has been deleted\n");
}

std::vector <std::string> PEFileDeps::getImportedDllNames()
{
    std::vector <std::string> _return;
    PIMAGE_IMPORT_DESCRIPTOR importDescriptor = this->getImportDescriptor();
    fprintf(stderr, "Adress of importDescriptor: %p\n", importDescriptor);
    for(; importDescriptor->Name != 0; ++importDescriptor)
    {
        fprintf(stderr, "%s\n", (char*) ((unsigned char*) this->peFileData + importDescriptor->Name));
        _return.push_back((char*) ((unsigned char*) peFileData + importDescriptor->Name));
    }
    fprintf(stderr, "Amount of imported DLLs: %llu\n", _return.size());
    return _return;
}