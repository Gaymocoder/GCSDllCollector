#ifndef __DLL_WALKER_H__
#define __DLL_WALKER_H__

#include "PEParser.h"

#include <map>

namespace FS = std::filesystem;
typedef PEDeps std::map<std::string, GCS::PEFile*>;

class PEFileDeps
{
    private:
        FS::path path;
        PEParser* parser;

        std::vector <std::string> getImportedDllNames();
        std::vector <FS::path> getImportedDllPaths();

    public:
        template <typename T>
        GCS::PEFileDeps(T path);

        std::map <std::string, GCS::PEFile*> getImportedDlls();
}

PEDeps& operator[](PEDeps& peTree, std::string key);