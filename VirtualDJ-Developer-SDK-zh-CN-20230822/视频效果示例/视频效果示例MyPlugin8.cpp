#include "MyPlugin8.h"

#if (defined(VDJ_WIN))
	#ifndef SAFE_RELEASE
		#define SAFE_RELEASE(p) { if (p) { (p)->Release(); (p)=NULL; } }
	#endif
#endif
//-----------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnLoad()
{ 
	// 添加插件调用时您的代码

	return S_OK;
}
//-----------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnGetPluginInfo(TVdjPluginInfo8 *infos)
{
	infos->PluginName = "插件名称";
	infos->Author = "开发作者";
	infos->Description = "插件信息";
	infos->Version = "版本号1.0";
	infos->Flags = 0x00; // 如果需要保证优先处理其它全部效果，则使用VDJFLAG_PROCESSLAST
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
HRESULT VDJ_API CMyPlugin8::OnDeviceInit()
{ 
	// 在该位置添加您的代码
	HRESULT hr;

#if (defined(VDJ_WIN64))

	hr = GetDevice(VdjVideoEngineDirectX11, (void**) &D3DDevice); //GetDevice没有AddRef()，所以我们以后不需要释放D3DDevice
	if (hr!=S_OK || D3DDevice==nullptr)
		return S_FALSE;

#elif (defined(VDJ_WIN))

	hr = GetDevice(VdjVideoEngineDirectX9, (void**) &D3DDevice); //GetDevice没有AddRef()，所以我们以后不需要释放D3DDevice
	if (hr!=S_OK || D3DDevice==nullptr)
		return S_FALSE;

#elif (defined(VDJ_MAC))

	glContext = CGLGetCurrentContext();

#endif

	// Size of the Video Window
	VideoWidth = width;
	VideoHeight = height;

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnDeviceClose()
{ 
	// 在该位置添加您的代码
#if (defined(VDJ_WIN64))
	D3DDevice = nullptr; //设备关闭后将无法继续使用
#elif (defined(VDJ_WIN))
	D3DDevice = nullptr;
#elif (defined(VDJ_MAC))
	glContext = 0;
#endif

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnStart()
{ 
	// 在该位置添加您的代码

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnStop()
{ 
	// 在该位置添加您的代码

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnDraw()
{ 
	// 在该位置添加您的代码
	HRESULT hr;
	TVertex *vertices = nullptr;

	if (VideoWidth!=width || VideoHeight!=height)
	{
		hr = OnVideoResize(width,height);
	}

#if (defined(VDJ_WIN64))
	ID3D11ShaderResourceView* textureView = nullptr; //GetTexture没有/不含AddRef，因此无需释放
	hr = GetTexture(VdjVideoEngineDirectX11, (void **) & textureView, &vertices);

	CComPtr<ID3D11DeviceContext> devContext; //使用智能指针自动释放指针，防止内存泄漏
	D3DDevice->GetImmediateContext(&devContext.p);

	//在处理 DX11 时经常会用到的一些其它接口
	CComPtr<ID3D11Resource> textureResource;
	textureView->GetResource(&textureResource);
	if (!textureResource)
		return E_FAIL;
	CComPtr<ID3D11Texture2D> texture;
	textureResource->QueryInterface<ID3D11Texture2D>(&texture);
	if (!texture)
		return E_FAIL;
	D3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);

#elif (defined(VDJ_WIN))

	hr = GetTexture(VdjVideoEngineDirectX9, (void **) &D3DTexture, &vertices);
	hr = D3DTexture->GetSurfaceLevel(0, &D3DSurface);

	SAFE_RELEASE(D3DTexture);
	SAFE_RELEASE(D3DSurface);

#elif (defined(VDJ_MAC))

	hr = GetTexture(VdjVideoEngineOpenGL, (void **) &GLTexture, &vertices);

	// glEnable(GL_TEXTURE_RECTANGLE_EXT);
	// glBindTexture(GL_TEXTURE_RECTANGLE_EXT, GLTexture);
	// glBegin(GL_TRIANGLE_STRIP);

	//for(int j=0;j<4;j++)
	//{
	//glColorD3D(vertices[j].color);
	//glTexCoord2f(vertices[j].tu, vertices[j].tv);
	//glVertex3f(vertices[j].position.x, vertices[j].position.y, vertices[j].position.z);
	//}

	//glEnd();
#endif


	hr = DrawDeck();
	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT CMyPlugin8::OnVideoResize(int VidWidth, int VidHeight)
{
	// OnDeviceClose();
	// OnDeviceInit();

	VideoWidth = width;
	VideoHeight = height;

	return S_OK;
}
