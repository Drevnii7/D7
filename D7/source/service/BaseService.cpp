#include "BaseService.h"

bool CBaseService::Main(int argc, char* argv[])
{
    if (argc < 3)
    {
        Warning("[1] input file, [2] output file, [3+]  -dp/--debugPrint");
        return false;
    }

    m_inputFilePath = argv[1];
    m_outputFilePath = argv[2];
    bool l_enableDebugPrint = false;

    for (int i = 3; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "-dp" || arg == "--debugPrint")
        {
            l_enableDebugPrint = true;
        }
        else
        {
            Warning("Unknown argument: " + arg);
        }
    }

    try
    {
        RunFullCycle();
    }
    catch (const std::exception& e)
    {
        Warning(std::string("Stop. Critical error during processing: ") + e.what());
        return false;
    }

    if (l_enableDebugPrint)
    {
        DebugPrint();
    }

    return true;
}