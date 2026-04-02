1) 使用Direct3D9的第一步是获取由VDJ软件定义的 IDirect3DDevice9 指针。 通过使用以下函数，可以在VDJ视频插件SDK中获得该指针：

IDirect3DDevice9 *D3DDevice;
void *device = NULL;
HRESULT hr;

hr = GetDevice(VdjVideoEngineDirectX9, &device);
if(hr!=S_OK || device==NULL) return S_FALSE;
D3DDevice = reinterpret_cast<IDirect3DDevice9*>(device);

每次要使用Direct3D函数时，只需使用

D3DDevice->xxxxxxxxxxx

2) 顶点是由以下结构定义的空间中的一个点
（注意：一个顶点、两个顶点）
struct TVertex
{
	struct {FLOAT x,y,z;} position; // 顶点的 3D 位置（坐标）
	DWORD color; // 顶点颜色
	FLOAT tu,tv; // 纹理坐标
};

Direct3D 中的3D模型基于三角形：
要想定义三角形，需要3个顶点: vertices[0], vertices[1], vertices[2]
要想定义正方形，需要4个顶点: vertices[0], vertices[1], vertices[2], vertices[3] [DeepL > English to Chinese (Simplified)]
假如需要2个三角形 : {vertices[0], vertices[1], vertices[2]} and {vertices[1], vertices[3], vertices[2]}

SDK 8 | SDK 7 或更早版本:


返回/回传以下指针定点的数据：
(宽度和高度在VDJ视频插件SDK中提供）
(有关信息请参阅 d3dcolor_rgba(255,255,255,255) = 4294967295)
Vertex[0].position.x=0.0f * (float) width;
	Vertex[0].position.y=0.0f * (float) height;
	Vertex[0].position.z=0.5f;
	Vertex[0].color=D3DCOLOR_RGBA(255,255,255,255);
	Vertex[0].tu=0.0f;
	Vertex[0].tv=0.0f;

	Vertex[1].position.x= 1.0f * (float) width;
	Vertex[1].position.y=0.0f * (float) height;
	Vertex[1].position.z=0.5f;
	Vertex[1].color=D3DCOLOR_RGBA(255,255,255,255);
	Vertex[1].tu=1.0f;
	Vertex[1].tv=0.0f;

	Vertex[2].position.x=1.0f * (float) width;
	Vertex[2].position.y=1.0f * (float) height;
	Vertex[2].position.z=0.5f;
	Vertex[2].color=D3DCOLOR_RGBA(255,255,255,255);
	Vertex[2].tu=1.0f;
	Vertex[2].tv=1.0f;

	Vertex[3].position.x=0.0f * (float) width;;
	Vertex[3].position.y=1.0f * (float) height;
	Vertex[3].position.z=0.5f;
	Vertex[3].color=D3DCOLOR_RGBA(255,255,255,255);
	Vertex[3].tu=0.0f;
	Vertex[3].tv=1.0f;

3) 纹理

"IDirect3DTexture9 *texture" 是当前的视频纹理

然后使用以下函数在这个正方形上应用纹理（视频、位图......）：

D3DDevice->SetTexture(0,texture)；

注意： 在这种情况下 "0" 表示视频的第一阶段

针对于视频效果:
IDirect3DTexture9 *texture;
TVertex *Vertex;
HRESULT hr;

hr = GetTexture(VdjVideoEngineDirectX9, (void **) &texture, &Vertex);

4) 渲染：

返回/回传S_FALSE； 会自动为您渲染视频

如果您想自己渲染，可以使用下面的函数：
D3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,2,(LPVOID)Vertices,sizeof(Vertices[0]));
在这种情况下，使用 return S_OK;

一些源代码示例：


#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(x) { if (x!=NULL) { x->Release(); x=NULL; } }
#endif

HRESULT VDJ_API CMyPlugin8::OnLoad()
{
D3DDevice = NULL;
g_font = NULL;
return S_OK;
}

HRESULT VDJ_API CMyPlugin8::OnDeviceInit()
{
GetDevice(VdjVideoEngineDirectX9, (void **)&D3DDevice);
D3DXCreateFont(D3DDevice, 22, 0, FW_NORMAL, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &g_font);
return S_OK;
}

HRESULT VDJ_API CMyPlugin8::OnDraw()
{
if (!g_font || !D3DDevice)
 return S_FALSE;

DrawDeck();

RECT font_rect;
SetRect(&font_rect, 0, 0, width, height);
g_font->DrawText(NULL, fps_string, -1, &font_rect, DT_LEFT | DT_NOCLIP, 0xFFFFFFFF);

return S_OK;
}

HRESULT VDJ_API CMyPlugin8::OnDeviceClose()
{
SAFE_RELEASE(g_font);
D3DDevice = NULL;
return S_OK;
}


如果您想绘制带有修改过的顶点的默认VDJ视频，那么可以这样做：


TVertex *v;
IDirect3DTexture9 *t;
GetTexture(VdjVideoEngineAny, (void**)&t, &v);
// 在这里修改顶点
DrawDeck();


最后，如果你想用自己的顶点做一些事情，那就试试下面的方法吧：


#define D3DFVF_TLVERTEX3D (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

HRESULT VDJ_API CMyPlugin8::OnDeviceInit()
{
	GetDevice(VdjVideoEngineAny, (void**)&device);
	if (FAILED(device->CreateVertexBuffer(4 * sizeof(TVertex), D3DUSAGE_WRITEONLY, D3DFVF_TLVERTEX3D, D3DPOOL_DEFAULT, &vertexBuffer, NULL)))
		return E_FAIL;
	return S_OK;
}

HRESULT VDJ_API CMyPlugin8::OnDeviceClose()
{
	SAFE_RELEASE(vertexBuffer);
	device = NULL;
	return S_OK;
}

bool CMyPlugin8::UpdateVertices()
{
	if (FAILED(vertexBuffer->Lock(0, 0, (void**)&vertices, 0)))
	{
		vertexBuffer->Release();
		vertexBuffer = NULL;
		return false;
	}
	vertices[0].position.x = 0.0f;
	vertices[0].position.y = 0.0f;
	vertices[0].position.z = 0.5f;
	vertices[0].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertices[0].tu = 0.f;
	vertices[0].tv = 0.f;
	vertices[1].position.x = (float)width;
	vertices[1].position.y = 0.0f;
	vertices[1].position.z = 0.5f;
	vertices[1].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertices[1].tu = 1.f;
	vertices[1].tv = 0.f;
	vertices[2].position.x = (float)width;
	vertices[2].position.y = (float)height;
	vertices[2].position.z = 0.5f;
	vertices[2].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertices[2].tu = 1.f;
	vertices[2].tv = 1.f;
	vertices[3].position.x = 0.0f;
	vertices[3].position.y = (float)height;
	vertices[3].position.z = 0.5f;
	vertices[3].color = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertices[3].tu = 0.f;
	vertices[3].tv = 1.f;
	vertexBuffer->Unlock();
	return true;
}

HRESULT VDJ_API CMyPlugin8::OnDraw()
{
	if (!device)
		return S_FALSE;

	TVertex *v;
	IDirect3DTexture9 *t;
	GetTexture(VdjVideoEngineAny, (void**)&t, &v);

	UpdateVertices();

	device->SetTexture(0, t);

	device->SetStreamSource(0, vertexBuffer, 0, sizeof(TVertex));
	device->SetFVF(D3DFVF_TLVERTEX3D);
	device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, (LPVOID)vertices, sizeof(vertices[0]));

	return S_OK;
}


Direct3D approaches方法看配图: