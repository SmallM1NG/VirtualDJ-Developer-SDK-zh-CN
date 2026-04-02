#include "vdjOnlineSource.h"

class TestProvider : public IVdjPluginOnlineSource
{
public公有权限:
	HRESULT VDJ_API OnGetPluginInfo(TVdjPluginInfo8* infos) override;
	HRESULT VDJ_API OnSearch(const char* search, IVdjTracksList* tracksList) override;
	HRESULT VDJ_API OnSearchCancel() override;
	HRESULT VDJ_API GetStreamUrl(const char* uniqueId, IVdjString& url, IVdjString& errorMessage) override;
	HRESULT VDJ_API GetContextMenu(const char* uniqueId, IVdjContextMenu* contextMenu) override;
	HRESULT VDJ_API OnContextMenu(const char* uniqueId, size_t menuIndex) override;
};

HRESULT VDJ_API TestProvider::OnGetPluginInfo(TVdjPluginInfo8* infos)
{
	infos->PluginName = "云服务提供商插件名称";
	return S_OK;
}

HRESULT VDJ_API TestProvider::OnSearch(const char* search, IVdjTracksList* tracksList)
{
	string url = "https://api.云服务提供商.com/search?query=" + Internet::urlencode(search);
	string html = Internet::downloadString(url);
	JSON json = JSON::fromString(html);
	list.reserve(json.children.size());
	for (const JSON& item : json.children)
	{
		tracksList->add(item.uniqueId.c_str(), item.title.c_str(), item.artist.c_str(), item.remix.c_str(), nullptr, item.label.c_str(), item.comment.c_str(), item.coverUrl.c_str(), nullptr, item.length, 0, 0, 0, true, false);

	}
	return S_OK;
}

HRESULT VDJ_API TestProvider::OnSearchCancel()
{
	Internet::closeDownloads();
	return S_OK;
}

HRESULT VDJ_API TestProvider::GetStreamUrl(const char* uniqueId, IVdjString& url, IVdjString& errorMessage)
{
	string url = "https://api.云服务提供商.com/getStreamUrl?id=" + Internet::urlencode(uniqueId);
	string html = Internet::downloadString(url);
	JSON json = JSON::fromString(html);
	if (json.hasParam("url"))
	{
		url = json.getParam("url").c_str();
		return S_OK;
	}
	else
	{
		errorMessage = json.getParam("error").c_str();
		return S_FALSE;
	}
}

HRESULT VDJ_API TestProvider::GetContextMenu(const char* uniqueId, IVdjContextMenu* contextMenu)
{
	contextMenu->add("Open in TestProvider");
	return S_OK;
}

HRESULT VDJ_API TestProvider::OnContextMenu(const char* uniqueId, size_t menuIndex)
{
	if (menuIndex==0)
	{
		string url = "https://www.云服务提供商.com/" + Internet::urlencode(uniqueId);
		Internet::openBrowser(url);
	}
	return S_OK;
}

HRESULT VDJ_API DllGetClassObject(const GUID &rclsid,const GUID &riid,void** ppObject)
{
	if (memcmp(&rclsid,&CLSID_VdjPlugin8,sizeof(GUID))==0 && memcmp(&riid,&IID_IVdjPluginOnlineSource,sizeof(GUID))==0)
		*ppObject=new TestProvider();
    else
		return CLASS_E_CLASSNOTAVAILABLE;
	return NO_ERROR;
}

