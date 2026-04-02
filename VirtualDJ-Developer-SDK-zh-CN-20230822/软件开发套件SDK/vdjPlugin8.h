//////////////////////////////////////////////////////////////////////////
//
//
// VDJ插件开发SDK - 李明杰VIP
//
//
//////////////////////////////////////////////////////////////////////////
//
// 该文件定义了所有插件需要使用的基础功能函数。
// 它定义了以下所需的函数和变量：
// - 加载和卸载插件
// - 提供有关插件的信息（名称、图片等）
// - 获取在加载之间自动保存和恢复的参数
// - 与VDJ软件进行交互（询问或发送命令）
// - 实现自定义接口
//
// 可找到针对特定类型插件的其他功能函数
// 位于它们各自的标头header文件中
//
//////////////////////////////////////////////////////////////////////////


#ifndef VdjPlugin8H
#define VdjPlugin8H

//////////////////////////////////////////////////////////////////////////
// 系统平台兼容Mac/Windows的特定定义说明
#ifdef VDJ_NOEXPORT
#elif (defined(WIN32) || defined(_WIN32) || defined(__WIN32_))
#define WIN32_LEAN_AND_MEAN //用于防止与DllGetClassObject出现链接错误
#include <windows.h>
#define VDJ_EXPORT		__declspec( dllexport )
#define VDJ_BITMAP		HBITMAP
#define VDJ_HINSTANCE	HINSTANCE
#define VDJ_WIN
#define VDJ_WINDOW		HWND
#define VDJ_API			__stdcall
#elif (defined(__APPLE__) || defined(MACOSX) || defined(__MACOSX__))
#include <CoreFoundation/CoreFoundation.h>
#define VDJ_EXPORT		__attribute__ ((visibility ("default")))
#define VDJ_BITMAP		char *
#define VDJ_HINSTANCE	CFBundleRef
#ifndef S_OK
#define S_OK            ((HRESULT)0x00000000L)
#endif
#ifndef S_FALSE
#define S_FALSE         ((HRESULT)0x00000001L)
#endif
#ifndef E_NOTIMPL
#define E_NOTIMPL       ((HRESULT)0x80004001L)
#endif
#define CLASS_E_CLASSNOTAVAILABLE -1
#define NO_ERROR		0
#include <MacTypes.h>
typedef SInt32 HRESULT;
typedef UInt32 ULONG;
typedef unsigned int DWORD;
#define VDJ_MAC
//VDJ_Window预计是一个NSWindow
#define VDJ_WINDOW void*
#define VDJ_API
#ifndef GUID_DEFINED
#define GUID_DEFINED
typedef struct _GUID {
unsigned long Data1;
unsigned short Data2;
unsigned short Data3;
unsigned char Data4[ 8 ];
} GUID;
#endif
#elif defined(__ANDROID__)
#define VDJ_BITMAP char *
#define VDJ_WINDOW void *
#define VDJ_HINSTANCE	void *
#define VDJ_API
typedef unsigned int ULONG;
#endif

//////////////////////////////////////////////////////////////////////////
// 内部结构
struct IVdjCallbacks8
{
	virtual HRESULT SendCommand(const char *command)=0;
	virtual HRESULT GetInfo(const char *command,double *result)=0;
	virtual HRESULT GetStringInfo(const char *command,void *result,int size)=0;
	virtual HRESULT DeclareParameter(void *parameter,int type,int id,const char *name,const char *shortName,float defaultvalue)=0;
	virtual HRESULT GetSongBuffer(int pos, int nb, short **buffer)=0;
};

struct IVdjVideoMouseCallbacks8
{
	virtual bool OnMouseMove(int x, int y, int buttons, int keyModifiers)=0;
	virtual bool OnMouseDown(int x, int y, int buttons, int keyModifiers)=0;
	virtual bool OnMouseUp(int x, int y, int buttons, int keyModifiers)=0;
	virtual void OnKey(const char *ch, int vkey, int modifiers, int flag, int scancode) {}
};

//////////////////////////////////////////////////////////////////////////
// 标准结构和定义

// 用于识别插件的结构
struct TVdjPluginInfo8
{
	const char *PluginName;
	const char *Author;
	const char *Description;
	const char *Version;
	VDJ_BITMAP Bitmap;
	DWORD Flags;
};

struct TVdjPluginInfo8_Extension1
{
	TVdjPluginInfo8 info;
	IVdjVideoMouseCallbacks8 *mouseCallbacks;
};

// 用于插件参数的标识flags
#define VDJPARAM_BUTTON	0
#define VDJPARAM_SLIDER	1
#define VDJPARAM_SWITCH	2
#define VDJPARAM_STRING	3
#define VDJPARAM_CUSTOM	4
#define VDJPARAM_RADIO	5
#define VDJPARAM_COMMAND 6
#define VDJPARAM_COLORFX 7 //与推子类似，但默认位置为 0.5，并且每个效果只有一个可用于完全控制的独立旋钮
#define VDJPARAM_BEATS 8 //指定节拍数的浮点数
#define VDJPARAM_BEATS_RELATIVE 9 //int，设置为 +1、-1 等，以便于在调用 OnParameter 时将节拍数增加或减少
#define VDJPARAM_POSITION 10 //用于视频插件，允许用户在插件的图形用户界面中调整大小/位置
#define VDJPARAM_RELEASEFX 11
#define VDJPARAM_TRANSITIONFX 12

#define VDJFLAG_NODOCK 0x1
#define VDJFLAG_PROCESSAFTERSTOP 0x2 //设置该标识时，插件停止后将调用 OnProcessSamples 或 OnDraw。 插件应尽快返回 E_FAIL，以停止处理。
#define VDJFLAG_PROCESSFIRST 0x4	//该插件应该首先处理
#define VDJFLAG_PROCESSLAST  0x8	//该插件应该最后处理 
#define VDJFLAG_EXTENSION1	0x10	//如果传递的结构是扩展类型，则在调用 OnGetPluginInfo 时由 VDJ 设置
#define VDJFLAG_SETPREVIEW 0x20		//设置此标识后，在皮肤上使用的预览将排除此插件
#define VDJFLAG_POSITION_NOSLIP 0x40 //当设置为位置插件时，它不会使用默播SLIP模式
#define VDJFLAG_ALWAYSPREFADER 0x80
#define VDJFLAG_ALWAYSPOSTFADER 0x100
#define VDJFLAG_EPHEMERAL 0x200 // 不要从 ini 保存或载入参数

// 用于自定义接口的结构
#define VDJINTERFACE_DEFAULT	0
#define VDJINTERFACE_SKIN		1
#define VDJINTERFACE_DIALOG		2
struct TVdjPluginInterface8
{
	DWORD Type;
	// 如果类型等于 Type==VDJINTERFACE_SKIN 则缓冲XML 和 图像
	const char *Xml;
	void *ImageBuffer;
	int ImageSize;
	// 如果类型等于 Type===VDJINTERFACE_DIALOG 在 Windows上由CreateDialog或CreateWindow返回/回显窗口句柄 HWND，或在Mac上由NSWindow指针返回窗口句柄 HWND *HWND窗口句柄是Handle to A Window的缩写
	VDJ_WINDOW hWnd;
};

//////////////////////////////////////////////////////////////////////////
// 基础类

struct IVdjCallbacks8;

class IVdjPlugin8
{
public:
	// 初始化
	virtual HRESULT VDJ_API OnLoad() {return S_OK;}
	virtual HRESULT VDJ_API OnGetPluginInfo(TVdjPluginInfo8 *info) {return E_NOTIMPL;}
	virtual ULONG VDJ_API Release() {delete this; return S_OK;}
	virtual ~IVdjPlugin8() {}

	// 在加载OnLoad() 期间为所有变量调用 DeclareParameter*()
	// 如果类型等于 type=VDJPARAM_CUSTOM 或 VDJPARAM_STRING，parameterSize 必须设置为 sizeof(*parameter)
	HRESULT DeclareParameterButton(int *parameter, int id, const char *name, const char *shortName) {return cb->DeclareParameter(parameter,VDJPARAM_BUTTON,id,name,shortName,0);}
	HRESULT DeclareParameterSlider(float *parameter, int id, const char *name, const char *shortName, float defaultvalue) {return cb->DeclareParameter(parameter,VDJPARAM_SLIDER,id,name,shortName,defaultvalue);}
	HRESULT DeclareParameterSwitch(int *parameter, int id, const char *name, const char *shortName, bool defaultvalue) {return cb->DeclareParameter(parameter,VDJPARAM_SWITCH,id,name,shortName,(float)defaultvalue);}
	HRESULT DeclareParameterString(char *parameter, int id, const char *name, const char *shortName, int parameterSize) { return cb->DeclareParameter(parameter, VDJPARAM_STRING, id, name, shortName, (float)parameterSize); }
	HRESULT DeclareParameterCustom(void *parameter, int id, const char *name, const char *shortName, int parameterSize) { return cb->DeclareParameter(parameter, VDJPARAM_CUSTOM, id, name, shortName, (float)parameterSize); }
	HRESULT DeclareParameterRadio(int *parameter, int id, const char *name, const char *shortName, float defaultvalue) { return cb->DeclareParameter(parameter, VDJPARAM_RADIO, id, name, shortName, (float)defaultvalue); }
	HRESULT DeclareParameterCommand(char *parameter, int id, const char *name, const char *shortName, int parameterSize) { return cb->DeclareParameter(parameter, VDJPARAM_COMMAND, id, name, shortName, (float)parameterSize); }
	HRESULT DeclareParameterColorFX(float *parameter, int id, const char *name, const char *shortName) { return cb->DeclareParameter(parameter, VDJPARAM_COLORFX, id, name, shortName, 0.5f); }
	HRESULT DeclareParameterBeats(float *parameter, int id, const char *name, const char *shortName) { return cb->DeclareParameter(parameter, VDJPARAM_BEATS, id, name, shortName, 0.5f); }
	HRESULT DeclareParameterBeatsRelative(int *parameter, int id, const char *name, const char *shortName) { return cb->DeclareParameter(parameter, VDJPARAM_BEATS_RELATIVE, id, name, shortName, 0.5f); }
	HRESULT DeclareParameterPosition(float parameter[4], int id, const char *name, const char *shortName) { return cb->DeclareParameter(parameter, VDJPARAM_POSITION, id, name, shortName, 4*sizeof(float)); }
	HRESULT DeclareParameterReleaseFX(float *parameter, int id, const char *name, const char *shortName) { return cb->DeclareParameter(parameter, VDJPARAM_RELEASEFX, id, name, shortName, 0.f); }
	HRESULT DeclareParameterTransitionFX(float *parameter, int id) { return cb->DeclareParameter(parameter, VDJPARAM_TRANSITIONFX, id, "Transition FX", "Trans", 0.f); }

	// 每次从VDJ中更改参数时，都会调用 OnParameter
	virtual HRESULT VDJ_API OnParameter(int id) {return S_OK;}
	// 每次VDJ请求某个参数的字符串标签时，都会调用 OnGetParameterString
	virtual HRESULT VDJ_API OnGetParameterString(int id, char *outParam, int outParamSize) {return E_NOTIMPL;}

	// 自定义用户界面
	// 在传递的插件接口 pluginInterface 结构中填写 VDJ_WINDOW 或 xml/bitmap 可扩展标记语言或位图信息，以定义自己的插件窗口
	virtual HRESULT VDJ_API OnGetUserInterface(TVdjPluginInterface8 *pluginInterface) {return E_NOTIMPL;}
	VDJ_HINSTANCE hInstance;

	// 向VDJ发送VDJScript脚本命令
	HRESULT SendCommand(const char *command) {return cb->SendCommand(command);}
	// 从VDJ获取信息（以数值或 utf-8 字符串形式），您还可以使用 command = "setting 'xxx'" 作为VDJ选项
	HRESULT GetInfo(const char *command, double *result) {return cb->GetInfo(command,result);}
	HRESULT GetStringInfo(const char *command, char *result, int size) {return cb->GetStringInfo(command,result,size);}

	IVdjCallbacks8 *cb;
};

class IVdjPluginStartStop8 : public IVdjPlugin8
{
public:
	// 在插件启动或停止时调用
	virtual HRESULT VDJ_API OnStart() { return 0; }
	virtual HRESULT VDJ_API OnStop() { return 0; }
};

//////////////////////////////////////////////////////////////////////////
// GUID 定义

#ifndef VDJCLASS8GUID_DEFINED
#define VDJCLASS8GUID_DEFINED
static const GUID CLSID_VdjPlugin8 = { 0xED8A8D87, 0xF4F9, 0x4DCD, { 0xBD, 0x24, 0x29, 0x14, 0x12, 0xE9, 0x3B, 0x60 } };
static const GUID IID_IVdjPluginBasic8 = { 0xa1d90ea1, 0x4d0d, 0x42dd, { 0xa4, 0xd0, 0xb8, 0xf3, 0x37, 0xb3, 0x21, 0xf1 } };
static const GUID IID_IVdjPluginStartStop8 = { 0xa1d91ea1, 0x4e0d, 0x32dd, { 0x14, 0xd0, 0xc8, 0xf3, 0x47, 0xb6, 0x41, 0xd1 }};
#else
extern static const GUID CLSID_VdjPlugin8;
extern static const GUID IID_IVdjPluginBasic8;
extern static const GUID IID_IVdjPluginStartStop8;
#endif

//////////////////////////////////////////////////////////////////////////
// DLL 导出功能

#ifndef NODLLEXPORT
#ifdef __cplusplus
extern "C" {
#endif
	VDJ_EXPORT HRESULT VDJ_API DllGetClassObject(const GUID &rclsid,const GUID &riid,void** ppObject);
#ifdef __cplusplus
}
#endif
#endif

//////////////////////////////////////////////////////////////////////////

#endif
