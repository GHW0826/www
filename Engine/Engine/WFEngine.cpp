#include "pch.h"
#include "WFEngine.h"
#include "INetworkEngine.h"

#ifdef __linux__
#include <dlfcn.h>
#endif // #ifdef __linux__

/// <summary>
/// NetworkEngine Start (dll)
/// </summary>
/// <param name="ModuleName"></param>
/// <param name="Server"></param>
/// <returns></returns>
NET_ERROR_CODE WFEngine::CreateNetworkEngine(char* ModuleName, bool Server)
{
    isServer_ = Server;

#ifdef __linux__
    // load Network Engine from Dll
    engineHandle_ = dlopen(ModuleName, RTLD_NOW);
    if (engineHandle_ == nullptr)
        return NET_ERROR_CODE::ENGINE_INIT_CREAT_ENGINE_LOAD_DLL_FAIL;

    // dll function call
    CREATENETWORKENGINE* pfunc = (CREATENETWORKENGINE*)dlsym(engineHandle_, "CreateNetworkEngine");
    if (pfunc == nullptr)
    {
        dlclose(engineHandle_);
        return NET_ERROR_CODE::ENGINE_INIT_CREAT_ENGINE_FUNC_NULL;
    }

    pNetworkEngine_ = pfunc(Server, this);
#else
    // load Network Engine from Dll
    engineHandle_ = ::LoadLibraryA(ModuleName);
    if (engineHandle_ == 0)
        return NET_ERROR_CODE::ENGINE_INIT_CREAT_ENGINE_LOAD_DLL_FAIL;

    // dll function call
    CREATENETWORKENGINE pfunc;
    pfunc = (CREATENETWORKENGINE)::GetProcAddress(engineHandle_, "CreateNetworkEngine");
    pNetworkEngine_ = pfunc(Server, this);

    SYSTEM_INFO si;
    GetSystemInfo(&si);
    int ioThreadCnt = si.dwNumberOfProcessors * 2;
#endif // #ifdef __linux__

    if (pNetworkEngine_->Init(ioThreadCnt) == false)
        return NET_ERROR_CODE::ENGINE_INIT_CREAT_ENGINE_INIT_FAIL;

    return NET_ERROR_CODE::SUCCESS;
}
