//////////////////////////////////////////////////////////////////////////
//
//
// VirtualDJ 插件开发SDK - 李明杰VIP
//
//
//////////////////////////////////////////////////////////////////////////
//
// 该文件定义了在线源插件，可以实现新的在线内容提供商。
// 它继承自IVdjPlugin基类，并定义了这些附加函数和变量：
//
//////////////////////////////////////////////////////////////////////////

#ifndef VdjOnlineSourceH
#define VdjOnlineSourceH

#include "vdjPlugin8.h"

//////////////////////////////////////////////////////////////////////////
// data types 数据类型

struct IVdjString
{
	virtual void VDJ_API operator=(const char*) = 0;
};

struct IVdjTracksList
{
	virtual void VDJ_API add(
		const char* uniqueId, // 指定一个字符串，在系统中唯一标识此歌曲
		const char* title,
		const char* artist,
		const char* remix = 0,
		const char* genre = 0,
		const char* label = 0,
		const char* comment = 0,
		const char* coverUrl = 0, // 如果可用，请为该歌曲设定显示的图片的网址链接http url
		const char* streamUrl = 0, // 通常会将其留空，并在通过getStreamUrl加载歌曲时提供相关信息
		float length = 0, // 单位秒
		float bpm = 0,
		int key = 0, // 1=Am, 2=A#m, ..., 24=G#
		int year = 0,
		bool isVideo = 0,
		bool isKaraoke = 0
	) = 0;

	// 默认情况下，OnSearch可用作同步阻止调用。
	// 如果要以异步操作的方式实现，则在OnSearch中返回/回传S_FALSE，并在完成后调用歌曲列表->结束完成tracksList->finish()。
	virtual void VDJ_API finish() = 0;
};

struct IVdjSubfoldersList
{
	virtual void VDJ_API add(const char* folderUniqueId, const char* folderName = 0) = 0;
};

struct IVdjContextMenu
{
	virtual void VDJ_API add(const char* menuEntry) = 0;
};

struct IVdjOAuth
{
	// 如果您的登录使用开放授权OAuth2，可以使用它来方便操作。
	// 使用授权链接URL调用oauth->open()，并执行OnOAuth()来监控/检测结果。
	// 如果您的授权流程需要用代码交换令牌Token，则在code!=null的情况下从OnOAuth()调用getToken()。
	// 调用getToken()或refreshToken()也会导致OnOAuth()被调用。
	virtual void VDJ_API open(const char* authorizeUrl) = 0; // open将在authorizeUrl中添加response_type=、redirect_uri=和 state=。 任何其他内容都应事先添加到链接url中。
	virtual void VDJ_API getToken(const char* code, const char* tokenUrl, const char* tokenPost = 0) = 0; // getToken将POST code=、grant_type= 和redirect_uri=。其他任何内容都应该添加到tokenPost中。
	virtual void VDJ_API refreshToken(const char* refresh_token, const char* tokenUrl, const char* tokenPost = 0) = 0; // freshToken将POST code=和grant_type=。其他任何内容都应该添加到tokenPost中。
};

//////////////////////////////////////////////////////////////////////////
// 视频文件夹插件类

class IVdjPluginOnlineSource : public IVdjPlugin8
{
public:
	// 执行此功能可处理用户的登录和退出；
	virtual HRESULT VDJ_API IsLogged() { return E_NOTIMPL; } // 如果用户已登录，则返回/回传S_OK；如果用户尚未登录，则返回/回传S_FALSE；如果不需要VDJ处理登录，则返回/回传E_NOTIMPL
	virtual HRESULT VDJ_API OnLogin() { return E_NOTIMPL; }
	virtual HRESULT VDJ_API OnLogout() { return E_NOTIMPL; }
	IVdjOAuth *oauth;
	virtual HRESULT VDJ_API OnOAuth(const char *access_token, size_t access_token_expire, const char* refresh_token, const char* code, const char* errorMessage) { return E_NOTIMPL; }

	// 当用户在搜索字段中输入内容时，将调用此函数；为搜索结果中的每首曲目调用歌曲列表->添加tracksList->add(...)。
	virtual HRESULT VDJ_API OnSearch(const char* search, IVdjTracksList* tracksList) = 0;
	// 您的onSearch函数可能需要很长时间才能返回/回馈。 如果用户在此期间取消搜索或进行新的搜索，我们将调用该函数，这样您就可以在未完成搜索的情况下，更快地从onSearch解锁和返回/回传。
	virtual HRESULT VDJ_API OnSearchCancel() { return E_NOTIMPL; }
	// 如果没有在歌曲列表tracksList中填充流媒体链接streamUrl字段，当用户将曲目加载到碟盘上时，将按曲目调用此函数；
	virtual HRESULT VDJ_API GetStreamUrl(const char* uniqueId, IVdjString& url, IVdjString& errorMessage) { return E_NOTIMPL; };

	// 如果有子文件夹，请在此处列出；
	virtual HRESULT VDJ_API GetFolderList(IVdjSubfoldersList* subfoldersList) { return E_NOTIMPL; }
	// 当用户浏览子文件夹时将调用此函数；
	virtual HRESULT VDJ_API GetFolder(const char* folderUniqueId, IVdjTracksList* tracksList) { return E_NOTIMPL; }

	// 如果您想实现扩展行为（例如离线缓存、购买链接等），可以在曲目或文件夹的右键菜单中添加项目；
	virtual HRESULT VDJ_API GetContextMenu(const char* uniqueId, IVdjContextMenu* contextMenu) { return E_NOTIMPL; }
	virtual HRESULT VDJ_API OnContextMenu(const char* uniqueId, size_t menuIndex) { return E_NOTIMPL; }
	virtual HRESULT VDJ_API GetFolderContextMenu(const char* folderUniqueId, IVdjContextMenu* contextMenu) { return E_NOTIMPL; }
	virtual HRESULT VDJ_API OnFolderContextMenu(const char* folderUniqueId, size_t menuIndex) { return E_NOTIMPL; }
};

//////////////////////////////////////////////////////////////////////////
// GUID definitions 全局唯一标识符定义

#ifndef VDJONLINESOURCEGUID_DEFINED
#define VDJONLINESOURCEGUID_DEFINED
static const GUID IID_IVdjPluginOnlineSource ={ 0x85d20f05, 0xccf, 0x4cab, { 0xaa, 0x50, 0x1c, 0x4, 0xea, 0xb6, 0xb8, 0x5d } };
#else
extern static const GUID IID_IVdjPluginOnlineSource;
#endif

//////////////////////////////////////////////////////////////////////////

#endif
