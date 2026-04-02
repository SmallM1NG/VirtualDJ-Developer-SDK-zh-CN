#include "MyPlugin8.h"

// 这是COM对象的标准DLL加载程序。

HRESULT VDJ_API DllGetClassObject(const GUID &rclsid,const GUID &riid,void** ppObject)
{
	if (memcmp(&rclsid,&CLSID_VdjPlugin8,sizeof(GUID))==0 && memcmp(&riid,&IID_IVdjPluginBasic8,sizeof(GUID))==0)
	{
		*ppObject=new CMyPlugin8();
	}
        else
	{
		return CLASS_E_CLASSNOTAVAILABLE;
	}

	return NO_ERROR;
}
