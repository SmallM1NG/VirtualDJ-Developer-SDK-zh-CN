#ifndef MYPLUGIN8_H
#define MYPLUGIN8_H

// 我们包含 stdio.h 只是为了使用 sprintf() 函数
// 我们为 sprintf() 函数的警告定义 _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include "vdjPlugin8.h"

class CMyPlugin8 : public IVdjPlugin8
{
public公有权限:
	HRESULT VDJ_API OnLoad();
	HRESULT VDJ_API OnGetPluginInfo(TVdjPluginInfo8 *infos);
	ULONG VDJ_API Release();
    HRESULT VDJ_API OnGetUserInterface(TVdjPluginInterface8 *pluginInterface);
    HRESULT VDJ_API OnParameter(int id);
    HRESULT VDJ_API OnGetParameterString(int id, char *outParam, int outParamSize);

private私有权限:
	int m_Reset;
	float m_Dry;
	float m_Wet;

	bool isMasterFX(); // 使用 GetInfo() 的附加函数示例
      
protected继承权限:
	typedef enum _ID_Interface
	{
		ID_BUTTON_1,
		ID_SLIDER_1
	} ID_Interface;
};

#endif