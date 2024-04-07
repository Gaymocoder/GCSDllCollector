#include "GCSDllCollector/DllWalker.h"

#include <iostream>

#include <windows.h>

int main(int argc, char** argv)
{
    PEFileDeps* peeFileDeps = new PEFileDeps("GCSAudioControl.exe");

    std::vector <std::string> importedDlls = peeFileDeps->getImportedDllNames();
    fprintf(stderr, "Dlls, imported by file \"%s\":\n", peeFileDeps->Path().string().c_str());
    for(auto it = importedDlls.begin(); it != importedDlls.end(); ++it)
        fprintf(stderr, "     %s\n", it->c_str());

    delete peeFileDeps;
}