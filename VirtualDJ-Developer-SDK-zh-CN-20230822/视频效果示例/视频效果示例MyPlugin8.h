#ifndef MYPLUGIN8_H
#define MYPLUGIN8_H

#include "vdjVideo8.h"

#if (defined(VDJ_WIN64))
	#include <d3d11.h>
	#pragma comment(lib, "d3d11.lib")
	#include <atlbase.h> //我们使用ALT作为 CComPtr 智能指针，但这是可选的
#elif (defined(VDJ_WIN))
	#define DIRECT3D_VERSION 0x9000
	#include <d3dx9.h>
	#pragma comment(lib, "d3dx9.lib")
#elif (defined(VDJ_MAC))
	#include <OpenGL/OpenGL.h> // 必须在苹果XCode项目中导入OpenGL.framework
	typedef unsigned long D3DCOLOR; // 使用与微软Microsoft Direct3D的方法相同
	#define D3DCOLOR_RGBA(r,g,b,a) ((D3DCOLOR)(((((a)&0xFF)<<24)|(((r)&0xFF)<<16)|((g)&0xFF)<<8)|((b)&0xFF))) 
	#define glColorD3D(d3dcolor) glColor4ub((d3dcolor>>16)&255, (d3dcolor>>8)&255, d3dcolor&255, (d3dcolor>>24)&255 )
#endif

	// 一些颜色
	const D3DCOLOR white = D3DCOLOR_RGBA(255,255,255,255);
	const D3DCOLOR black = D3DCOLOR_RGBA(0,0,0,255);
	const D3DCOLOR red = D3DCOLOR_RGBA(255,0,0,255);
	const D3DCOLOR green = D3DCOLOR_RGBA(0,255,0,255);
	const D3DCOLOR blue = D3DCOLOR_RGBA(0,0,255,255);
	const D3DCOLOR translucide_black = D3DCOLOR_RGBA(0,0,0,120);


class CMyPlugin8 : public IVdjPluginVideoFx8
{
public公有权限:
	HRESULT VDJ_API OnLoad();
	HRESULT VDJ_API OnGetPluginInfo(TVdjPluginInfo8 *infos);
	ULONG VDJ_API Release();
	HRESULT VDJ_API OnDeviceInit();
	HRESULT VDJ_API OnDeviceClose();
	HRESULT VDJ_API OnStart();
	HRESULT VDJ_API OnStop();
	HRESULT VDJ_API OnDraw();

private私有权限:
	HRESULT OnVideoResize(int VidWidth, int VidHeight);
	int VideoWidth = 0;
	int VideoHeight = 0;

#if (defined(VDJ_WIN64))

	ID3D11Device* D3DDevice = nullptr;

#elif (defined(VDJ_WIN))

	IDirect3DDevice9*  D3DDevice = nullptr;
	IDirect3DTexture9* D3DTexture = nullptr;
	IDirect3DSurface9* D3DSurface = nullptr;

#elif (defined(VDJ_MAC))

	CGLContextObj glContext = 0;
	GLuint GLTexture = 0;

#endif
};

#endif
