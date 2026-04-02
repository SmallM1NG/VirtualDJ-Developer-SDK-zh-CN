#include "MyPlugin8.h"

//-----------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnLoad()
{
    m_Dry = 0.0f;
    m_Wet = 0.0f;
    
    //HRESULT hr;
    //double qRes;
    //hr = GetInfo("get hwnd",&qRes);
    //if(hr!=S_OK) hWndParent=NULL;
    //else hWndParent = (VDJ_WINDOW) (int) qRes;
    
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
    infos->Flags = 0x00;
    infos->Bitmap = NULL;
    
    return S_OK;
}
//---------------------------------------------------------------------------
ULONG VDJ_API CMyPlugin8::Release()
{
    delete this;
    return 0;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnGetUserInterface(TVdjPluginInterface8 *pluginInterface)
{
    
    pluginInterface->Type = VDJINTERFACE_SKIN;
    
    // 以下代码只是一个示例，需要根据您的项目进行调整。
    TRESOURCEREF rcXML;
    TRESOURCEREF rcPNG;
    
#if (defined(VDJ_WIN))
    rcXML.id = IDR_SKINXML; // 作为 resource.h 中的定义
    rcPNG.id = IDR_SKINPNG; // 作为 resource.h 中的定义
    rcXML.type = "XML";
    rcPNG.type = "PNG";
#elif (defined(VDJ_MAC))
    rcXML.name = "FX_GUI.xml";
    rcPNG.name = "FX_GUI.png";
#endif
    
    DWORD xmlsize=0;
    void* xmldata=NULL;
    LoadFileFromResource( rcXML, xmlsize, xmldata);
    pluginInterface->Xml = static_cast<char*>(xmldata);
    
    DWORD pngsize=0;
    void* pngdata=NULL;
    LoadFileFromResource( rcPNG, pngsize,pngdata);
    pluginInterface->ImageBuffer = pngdata;
    pluginInterface->ImageSize = pngsize;
    
    return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnParameter(int id)
{
    switch(id)
    {
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
            sprintf_s(outParam,outParamSize,"+%.0f%%", m_Wet*100);
            break;
    }
    
    return S_OK;
}
//----------------------------------------------------------------------------
HRESULT CMyPlugin8::LoadFileFromResource(TRESOURCEREF ref, DWORD& size, void*& data)
{
    
#if (defined(VDJ_WIN))
    
    if (ref.id==0 || ref.type==NULL)
        return S_FALSE;
    
    HRSRC rc = FindResourceA(hInstance, MAKEINTRESOURCEA(ref.id), ref.type);
    HGLOBAL rcData = LoadResource(hInstance, rc);
    size = SizeofResource(hInstance, rc);
    data = LockResource(rcData);
    
#elif (defined(VDJ_MAC))
    
    if (ref.name.empty())
        return S_FALSE;
    CFBundleRef bundle = (CFBundleRef)hInstance;
    CFStringRef resourceName = CFStringCreateWithCString(kCFAllocatorDefault, ref.name.c_str(), kCFStringEncodingUTF8);
    if (!resourceName)
        return S_FALSE;
    CFURLRef url = CFBundleCopyResourceURL(bundle, resourceName, NULL, NULL);
    CFRelease(resourceName);
    if (!url)
        return S_FALSE;

    char filePath[1024];
    bool success = CFURLGetFileSystemRepresentation(url, TRUE, (UInt8*)filePath, 1024);
    CFRelease(url);

    if (!success)
        return S_FALSE;

    std::ifstream dataFile(filePath, std::ios::binary);
    if (dataFile.is_open())
    {
        dataFile.seekg(0,std::ios::end);
        size = (DWORD)dataFile.tellg();
        data = new char[size];
        dataFile.seekg(0,std::ios::beg);
        dataFile.read((char*)data,size);
        dataFile.close();
    }
    
#endif
    
    return S_OK;
}
