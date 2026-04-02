#include "MyPlugin8.h"


//-----------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnLoad()
{ 
	// 添加插件调用时您的代码
       m_Reset = 0;
	m_Dry = 0.0f;
	m_Wet = 0.0f;

	bool bMasterFX = isMasterFX();


	DeclareParameterButton(&m_Reset,ID_BUTTON_1,"Reset","R");
	DeclareParameterSlider(&m_Wet,ID_SLIDER_1,"Dry/Wet","D/W",1.0f);

	OnParameter(ID_SLIDER_1);

	return S_OK;
}
//-----------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnGetPluginInfo(TVdjPluginInfo8 *infos)
{
    infos->PluginName = "插件名称";
    infos->Author = "开发作者";
    infos->Description = "插件信息";
    infos->Version = "版本号1.0";
	infos->Flags = 0x00; // 参数标识
	infos->Bitmap = NULL; // 位图图像

	return S_OK;
}
//---------------------------------------------------------------------------
ULONG VDJ_API CMyPlugin8::Release()
{
	// 添加插件释放时您的代码

	delete this;
	return 0;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnGetUserInterface(TVdjPluginInterface8 *pluginInterface)
{
	pluginInterface->Type = VDJINTERFACE_DEFAULT;

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnParameter(int id)
{
	switch(id)
	{
		case ID_BUTTON_1:
			if(m_Reset == 1)
			{
				m_Wet = 0.5f;
				//HRESULT hr;
				//hr = SendCommand("effect_slider 1 50%");
			}
			break;

		case ID_SLIDER_1:
			m_Dry = 1 - m_Wet; 
			break;
	}

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnGetParameterString(int id, char *outParam, int outParamSize) 
{
	switch(id)
	{
		case ID_SLIDER_1:
			sprintf(outParam,"+%.0f%%", m_Wet*100);
			break;
	}

	return S_OK;
}

//-------------------------------------------------------------------------------------------------------------------------------------
// 下面附加功能函数仅用于解释某些功能（可以删除）
//-------------------------------------------------------------------------------------------------------------------------------------
bool CMyPlugin8::isMasterFX()
{
	double qRes;
	HRESULT hr = S_FALSE;

	hr = GetInfo("get_deck 'master' ? true : false", &qRes);

	if(qRes==1.0f) return true;
	else return false;
}
