#ifndef MYPLUGIN8_H
#define MYPLUGIN8_H

#include "vdjDsp8.h"

class CMyPlugin8 : public IVdjPluginDsp8
{
public公有权限:
	HRESULT VDJ_API OnLoad();
	HRESULT VDJ_API OnGetPluginInfo(TVdjPluginInfo8 *infos);
	ULONG   VDJ_API Release();
	HRESULT VDJ_API OnStart();
	HRESULT VDJ_API OnStop();
	HRESULT VDJ_API OnProcessSamples(float *buffer, int nb);

private私有权限:

    // 在该位置添加您的代码
};

#endif
