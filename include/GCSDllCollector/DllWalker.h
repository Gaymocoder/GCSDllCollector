#ifndef __DLL_WALKER_H__
#define __DLL_WALKER_H__

#include "GCSDllCollector/PEParser.h"

#include <map>
#include <vector>

namespace FS = std::filesystem;


class PEFileDeps
{
    typedef std::map<std::string, PEFileDeps*> PEDeps;

    private:
        FS::path path;
        PEParser* parser;

        std::vector <std::string> getImportedDllNames();
        std::vector <FS::path> getImportedDllPaths();

    public:
        template <typename T>
        PEFileDeps(T path);

        PEDeps getImportedDlls();
};

typedef std::map<std::string, PEFileDeps*> PEDeps;

#endif