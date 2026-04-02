//////////////////////////////////////////////////////////////////////////
//
//
// VDJ视频插件开发SDK - 李明杰VIP
//
//
//////////////////////////////////////////////////////////////////////////
//
// 该文件定义了视频插件（效果FX和转场Transition）。
// 除了基础IVdjPlugin类支持的所有元素外；
// 它定义了附加的视频特定专用函数和变量：
//
//////////////////////////////////////////////////////////////////////////


#ifndef VdjVideo8H
#define VdjVideo8H

#include "vdjPlugin8.h"

//////////////////////////////////////////////////////////////////////////
// 数据类型

#ifndef TVertex
struct TVertex
{
	struct {float x,y,z;} position;
	DWORD color;
	float tu,tv;
};
#endif

enum EVdjVideoEngine
{
	VdjVideoEngineAny = 0,
	VdjVideoEngineDirectX9 = 1,
	VdjVideoEngineOpenGL = 2,
	VdjVideoEngineDirectX11 = 3,
	VdjVideoEngineOpenGLES2 = 4,
    VdjVideoEngineMetal = 5,
    VdjVideoEngineAnyPtr = 6,
};

#define VDJFLAG_VIDEO_MASTERONLY	0x10000
#define VDJFLAG_VIDEO_VISUALISATION 0x20000	// 该效果生成视觉效果，而不是在给特定的图像上应用特效；
#define VDJFLAG_VIDEO_OVERLAY		0x40000 // 这种效果是覆盖/叠加于已有图像上。 如果设置此选项后，您就不能在插件中调用 DrawDeck（因为它已在 OnDraw() 之前完成）。
#define VDJFLAG_VIDEO_HASRESIZE		0x80000 // 该效果知道在按下CTRL时如何调整自身大小
#define VDJFLAG_VIDEO_NOAUTOACTIVE  0x200000 // 激活此效果时，视频引擎不会自动激活（适用于带视频选项的音频效果）。
#define VDJFLAG_VIDEO_OUTPUTRESOLUTION 0x400000 // 如果在碟盘上应用特效，特效将应用于视频输出分辨率，而不是视频源分辨率。
#define VDJFLAG_VIDEO_OUTPUTASPECTRATIO 0x800000  // 如果在碟盘上应用特效，特效将以与视频输出相同的宽高比例应用（视频源和视频输出之间的最小分辨率）
#define VDJFLAG_VIDEO_FORRECORDING 0x1000000 // 如果效果应用于主控，它将以录制分辨率呈现，并在视频皮肤之后渲染

// 对于过渡效果，您需要在 OnGetPluginInfo 中定义自动视频交叉过渡器的行为：
#define VDJFLAG_VIDEOTRANSITION_CONTINOUS	0x100000 // 交叉过渡器从一侧连续移动到另一侧
#define VDJFLAG_VIDEOTRANSITION_MIDPOINTS	0x000000 // 当同时播放两个视频时，交叉过渡器会停在25%或75%位置处

//////////////////////////////////////////////////////////////////////////
// 内部结构
struct IVdjVideoCallbacks8
{
	virtual HRESULT DrawDeck()=0;

	//针对于 DirectX 9 (Windows 32位) 设备是 IDirect3DDevice9*
	//针对于 DirectX 11 (Windows 64位) 设备是 ID3D11Device*
	virtual HRESULT GetDevice(EVdjVideoEngine engine, void **device)=0;
	
	//针对于 DirectX 9 (Windows 32位) 纹理是 IDirect3DTexture9*
	//针对于 DirectX 11 (Windows 64位) 纹理是 ID3D11ShaderResourceView*
	//针对于 OpenGL (MacOS) 纹理是常规的 PpenGL纹理ID，类型为 GLuint
	virtual HRESULT GetTexture(EVdjVideoEngine engine, void **texture, TVertex **vertices)=0;
};


//////////////////////////////////////////////////////////////////////////
// 视频转场插件类

class IVdjPluginVideoTransition8 : public IVdjPlugin8
{
public:
	// 只要视频交叉过渡器不同于 0.0 或 1.0，就会调用OnDraw。
	// 这个函数应该混合来自两个碟盘的两个图像。
	// 调用 DrawDeck() 为每个碟盘指定顶点绘制图像。
	// 调用 GetVertices() 获取默认顶点，并对其进行修改。 (或者将 NULL 传递给 DrawDeck() 以使用默认值）。
	virtual HRESULT VDJ_API OnDraw(float crossfader)=0;
	TVertex* (*GetVertices)(int deck);
	HRESULT (*DrawDeck)(int deck, TVertex* vertices);

	// 对于更复杂的操作，您可以要求直接访问设备和纹理
	// 对于OpenGL纹理需要指向一个 GLuint，而对于DirectX 9则需要指向IDirect3DTexture9* 
	//针对于 DirectX 11 (Windows 64位) 纹理是 ID3D11ShaderResourceView*
	HRESULT (*GetDevice)(EVdjVideoEngine engine, void **device);

	//针对于 OpenGL (MacOS) 纹理是常规的 PpenGL纹理ID，类型为 GLuint
	//针对于 DirectX 9 (Windows 32位) 设备是 IDirect3DDevice9*
	//针对于 DirectX 11 (Windows 64位) 设备是 ID3D11Device*
	HRESULT (*GetTexture)(EVdjVideoEngine engine, int deck, void**texture);

	// 当DirectX/OpenGL初始化或者关闭时，这些函数都将被调用
	virtual HRESULT VDJ_API OnDeviceInit() {return S_OK;}
	virtual HRESULT VDJ_API OnDeviceClose() {return S_OK;}

	// 视频输出的尺寸大小
	int width, height;

	// 一些有用的变量
	int SampleRate;		 // 音频引擎的采样率
	int SongBpm;		 // 主控歌曲两个连续节拍之间的采样数
	double SongPosBeats; // 主控歌曲第一拍开始的节拍数
};

class IVdjPluginVideoTransitionMultiDeck8 : public IVdjPlugin8
{
public:
	// 如果想要一个可以处理2个以上碟盘的过渡插件，请使用此版本。
	// OnDrawMultiDeck 是为每个帧调用（无论交叉过渡器位置如何）。
	// videoDecks 是包含 nbVideos 的整数数组，给出了每个有视频碟盘的碟盘编号。
	// 该数组按视频的“重要性”顺序排序（首先是左侧视频videoLeft 和右侧视频videoRight，然后是播放视频等）。
	// 调用 DrawDeck() 为每个碟盘绘制指定顶点的图像。
	// 调用 GetVertices 将顶点初始化为默认值，并对其进行修改。 （或者将 NULL 传递给 DrawDeck() 以使用默认值）
	// 注意： 如果想要获取视频交叉过渡器的值，请调用 GetInfo("video_crossfader")；
	virtual HRESULT VDJ_API OnDrawMultiDeck(int nbVideoDecks, int *videoDecks)=0;
	TVertex* (*GetVertices)(int deck);
	HRESULT (*DrawDeck)(int deck, TVertex* vertices);
	
	// 对于更复杂的操作，您可以要求直接访问设备和纹理
	// 对于OpenGL纹理需要指向一个 GLuint，而对于DirectX 9则需要指向IDirect3DTexture9* 
	//针对于 DirectX 11 (Windows 64位) 纹理是 ID3D11ShaderResourceView*
	HRESULT (*GetDevice)(EVdjVideoEngine engine, void **device);

	//针对于 OpenGL (MacOS) 纹理是常规的 PpenGL纹理ID，类型为 GLuint
	//针对于 DirectX 9 (Windows 32位) 设备是 IDirect3DDevice9*
	//针对于 DirectX 11 (Windows 64位) 设备是 ID3D11Device*
	HRESULT (*GetTexture)(EVdjVideoEngine engine, int deck, void**texture);

	// 当DirectX/OpenGL初始化或者关闭时，这些函数将会被调用
	virtual HRESULT VDJ_API OnDeviceInit() {return S_OK;}
	virtual HRESULT VDJ_API OnDeviceClose() {return S_OK;}

	// 视频输出的尺寸大小
	int width, height;

	// 一些有用的变量
	int SampleRate;		 // 音频引擎的采样率
	int SongBpm;		 // 主控歌曲两个连续节拍之间的采样数
	double SongPosBeats; // 主控歌曲第一拍开始的节拍数
};


//////////////////////////////////////////////////////////////////////////
// 视频效果插件类

class IVdjPluginVideoFx8 : public IVdjPlugin8
{
public:
	// 在插件启动和停止时调用
	virtual HRESULT VDJ_API OnStart() {return S_OK;}
	virtual HRESULT VDJ_API OnStop() {return S_OK;}

	// 当插件激活时，每帧都会调用 OnDraw() 函数
	// 您可以通过调用 GetDevice 访问 DirectX/OpenGL 设备，并执行任何操作
	// 为了绘制原始图像，如果不需要修改图像（例如覆盖插件），您可以只调用 DrawDeck()，
	// 或调用 GetTexture 获取纹理及其顶点的底层访问权限。
	virtual HRESULT VDJ_API OnDraw()=0;
	
	//针对于 DirectX 9 (Windows 32位) 设备是 IDirect3DDevice9*
	//针对于 DirectX 11 (Windows 64位) 设备是 ID3D11Device*
    //针对于 Mac系统 上的 Metal 设备，id 为<MTLRenderCommandEncoder>
	HRESULT GetDevice(EVdjVideoEngine engine, void **device) {return vcb->GetDevice(engine,device);}
	
	//针对于 DirectX 9 (Windows 32位) 纹理是 IDirect3DTexture9*
	//针对于 DirectX 11 (Windows 64位) 纹理是 ID3D11ShaderResourceView*
	//针对于 OpenGL (MacOS) 纹理是常规的 OpenGL纹理ID，类型为 GLuint
	HRESULT GetTexture(EVdjVideoEngine engine, void **texture, TVertex **vertices) {return vcb->GetTexture(engine,texture,vertices);}
	
	HRESULT DrawDeck() {return vcb->DrawDeck();}

	//当DirectX/OpenGL初始化或者关闭时，这些函数都将会被调用
	virtual HRESULT VDJ_API OnDeviceInit() {return S_OK;}
	virtual HRESULT VDJ_API OnDeviceClose() {return S_OK;}

	// 您还可以实现 OnAudioSamples，使视频效果根据声音信号的输入操作
	virtual HRESULT VDJ_API OnAudioSamples(float *buffer, int nb) { return E_NOTIMPL; };

	// 一些有用的变量
	int SampleRate;		 // 音频引擎的采样率
	int SongBpm;		 // 这首歌两个连续节拍之间的样本数
	double SongPosBeats; // 歌曲第一拍起的节拍数

	// 视频输出的尺寸大小
	int width, height;

	IVdjVideoCallbacks8 *vcb;
};

//////////////////////////////////////////////////////////////////////////
// GUID 全局唯一标识符定义

#ifndef VDJVIDEO8GUID_DEFINED
#define VDJVIDEO8GUID_DEFINED
static const GUID IID_IVdjPluginVideoFx8 = { 0xbf1876aa, 0x3cbd, 0x404a, { 0xbe, 0xab, 0x5f, 0x8b, 0x51, 0xe3, 0x90, 0xc0 } };
static const GUID IID_IVdjPluginVideoTransition8 = { 0x2f350983, 0xf88f, 0x429c, { 0x87, 0x75, 0x62, 0x87, 0x68, 0x7d, 0xe0, 0xd7 } };
static const GUID IID_IVdjPluginVideoTransitionMultiDeck8 = { 0x54d0e81c, 0x51a6, 0x49b0, { 0x82, 0x3f, 0x75, 0x91, 0x76, 0xf1, 0xcf, 0x06 } };
#else
extern static const GUID IID_IVdjPluginVideoFx8;
extern static const GUID IID_IVdjPluginVideoTransition8;
extern static const GUID IID_IVdjPluginVideoTransitionMultiDeck8;
#endif

//////////////////////////////////////////////////////////////////////////

#endif
