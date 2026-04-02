#include "MyPlugin8.h"

#if (defined(VDJ_WIN))
#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif
#endif

//-----------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnLoad()
{	
      // 添加插件调用时您的代码

#if (defined(VDJ_WIN))

 D3DDevice = NULL;
 D3DTexture = NULL;
 D3DSurface = NULL;

#elif (defined(VDJ_MAC))

 GLTexture = 0;

#endif
     
	return S_OK;
}
//-----------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnGetPluginInfo(TVdjPluginInfo8 *infos)
{
	infos->PluginName = "插件名称";
	infos->Author = "开发作者";
	infos->Description = "插件信息";
	infos->Version = "版本号1.0";
	infos->Flags       = VDJFLAG_VIDEOTRANSITION_CONTINOUS; 
	infos->Bitmap = NULL;

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
HRESULT VDJ_API  CMyPlugin8::OnDeviceInit()
{	
      // 在该位置添加您的代码
       HRESULT hr;
	
#if (defined(VDJ_WIN))

	hr = GetDevice(VdjVideoEngineDirectX9, (void**) &D3DDevice);
	if(hr!=S_OK || D3DDevice == NULL) return S_FALSE;

#endif
      
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API  CMyPlugin8::OnDeviceClose()
{	
      // 在该位置添加您的代码

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnDraw(float crossfader)
{	
      // 在该位置添加您的代码
HRESULT hr;

      int main=(crossfader<=0.5f)?1:2;
            float factor = 1.0f - crossfader/0.5f;
       int alpha;
 TVertex  *vertices;
DWORD AlphaColor;

#if (defined(VDJ_WIN))

 hr = GetTexture(VdjVideoEngineDirectX9, main, (void **) &D3DTexture);
 hr = D3DTexture->GetSurfaceLevel(0, &D3DSurface);

SAFE_RELEASE(D3DTexture);
 SAFE_RELEASE(D3DSurface);

#elif (defined(VDJ_MAC))

 hr = GetTexture(VdjVideoEngineOpenGL, (void **) &GLTexture, &vertices);

 #endif

        if (crossfader<=0.5f) alpha=(int)(factor* 255);
	else alpha=(int)((1.0f- factor)*255);
	
	vertices = GetVertices(main);

     AlphaColor =  D3DCOLOR_RGBA(255,255,255, alpha);  // 有关Mac系统上的声明，请参照MyPlugin8.h
      
      vertices[0].color = AlphaColor;
      vertices[1].color = AlphaColor;
      vertices[2].color = AlphaColor;
      vertices[3].color = AlphaColor;

#if (defined(VDJ_WIN))

      // D3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

#elif (defined(VDJ_MAC))

          // glColorD3D(AlphaColor);   // 参照MyPlugin8.h的说明

#endif

       hr = DrawDeck(main, vertices);

       return S_OK;
}
