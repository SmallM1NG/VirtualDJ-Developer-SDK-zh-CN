#include "MyPlugin8.h"

//-----------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnLoad()
{	
    return S_OK;
}
//-----------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnGetPluginInfo(TVdjPluginInfo8 *infos)
{
    infos->PluginName = "插件名称";
    infos->Author = "开发作者";
    infos->Description = "插件信息";
    infos->Version = "版本号1.0";
	infos->Flags       = 0x00;
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

     return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnStop()
{	
      // 音频插件停止时在此处添加您的代码

	return S_OK;
}
//---------------------------------------------------------------------------
 HRESULT VDJ_API CMyPlugin8::OnProcessSamples(float *buffer, int nb)
{	
     // 在此处添加您的音频处理代码 使用 2*nb 浮点采样float（立体声信号） 的 *buffer 缓冲区
     // 浮点采样的值介于 -1.0f 和 1.0f 之间

       float SampleOut_Left, SampleOut_Right;
       float SampleIn_Left, SampleIn_Right;
       int i;
	
        // 通过音频缓冲区读取'nb'立体声采样 等于 ( = 2 * 'nb' 单声道采样) 
        // 读取左声道采样 : buffer[0], buffer[2], ... , buffer[2*nb-2]
        // 读取右声道采样 : buffer[1], buffer[3], ... , buffer[2*nb-1]

        for(i=0;i<nb;i++)
	{
                SampleIn_Left = buffer[2*i]; 
		SampleIn_Right = buffer[2*i+1];

                // 在此添加您的音频处理代码。
                // 在本例中，我们将左声道静音并（复制）右声道
                 float WetDry = 0.0f; // 我们想要将样本强制为 0（无音量/静音）

                  SampleOut_Left  = WetDry * SampleIn_Left ; 
                  SampleOut_Right  = SampleIn_Right;
  		
		buffer[2*i] =  SampleOut_Left;
		buffer[2*i+1] = SampleOut_Right;
	}

	return S_OK;
}
