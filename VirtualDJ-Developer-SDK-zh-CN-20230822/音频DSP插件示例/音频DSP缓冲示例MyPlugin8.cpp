#include "MyPlugin8.h"

//-----------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnLoad()
{	
	return S_OK;
}
//-----------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnGetPluginInfo(TVdjPluginInfo8 *infos)
{
	infos->PluginName  = "插件名称";
	infos->Author      = "开发作者";
	infos->Description = "插件说明";
	infos->Version     = "版本号1.0";
	infos->Flags       = 0x00;  // 如果你想在碟盘停止时处理声音，也可以使用 VDJFLAG_PROCESSAFTERSTOP
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
HRESULT VDJ_API CMyPlugin8::OnStart()
{	
        // 启动音频插件时在此处添加您的代码
        StartPos = int(SongPosBeats * SongBpm);

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnStop()
{	
       // 音频插件停止时在此处添加您的代码

       StartPos = 0;

       return S_OK;
}
//---------------------------------------------------------------------------
short * VDJ_API CMyPlugin8::OnGetSongBuffer(int pos, int nb)
{	
      // 在此处添加您的音频处理代码 使用 2*nb 浮点采样float（立体声信号） 的 *buffer 缓冲区
      HRESULT hr;
        
      short *buffer;
      hr  = GetSongBuffer(pos, nb, (short **) &buffer);

      return buffer;
}

