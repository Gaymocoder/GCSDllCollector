#ifndef __DLL_WALKER_H__
#define __DLL_WALKER_H__

#include "GCSPEParser/PEParser.h"

#include <map>
#include <vector>

namespace FS = std::filesystem;


class PEFileDeps : private PEParser
{
    typedef std::map<std::string, PEFileDeps*> PEDeps;

    private:
        std::vector <FS::path> getImportedDllPaths();

    public:
        template <typename T>
        PEFileDeps(T value);

        std::vector <std::string> getImportedDllNames();
        PEDeps getImportedDlls();

        FS::path Path();

        virtual ~PEFileDeps();
};

template <typename T>
PEFileDeps::PEFileDeps(T value) : PEParser(value) {}

typedef std::map<std::string, PEFileDeps*> PEDeps;

#endif