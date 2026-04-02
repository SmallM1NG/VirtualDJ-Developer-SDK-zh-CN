#include "MyPlugin8.h"

//-----------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnLoad()
{	
	SongBpm2=0;
        StartPos=0;

    return S_OK;
}
//-----------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnGetPluginInfo(TVdjPluginInfo8 *infos)
{
	infos->PluginName  = "插件名称";
	infos->Author      = "开发作者";
	infos->Description = "插件说明";
	infos->Version     = "版本1.0";
	infos->Flags       = 0x00;  // 如果想在碟盘停止时处理声音，也可以使用 VDJFLAG_PROCESSAFTERSTOP
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
      // 启动音频插件时，在此处添加代码

      SongBpm2 = SongBpm?SongBpm:(SampleRate/2); // 默认拍速为120BPM
	StartPos = int(SongPosBeats * SongBpm2);

	return S_OK;
}
//---------------------------------------------------------------------------
HRESULT VDJ_API CMyPlugin8::OnStop()
{	
      // 当音频插件停止时，在此处添加您的代码


       StartPos = 0;

	return S_OK;
}
//---------------------------------------------------------------------------
 HRESULT VDJ_API CMyPlugin8::OnProcessSamples(float *buffer, int nb)
{	
      // 使用音频缓冲器在此处添加音频处理代码 *缓冲器包含 2*nb 浮点采样（立体声信号）

        float out[2], in[2];
	int pos;
        int CurrentPos;
	float xBeat;

	SongBpm2 = SongBpm?SongBpm:(SampleRate/2);
          pos = int(SongPosBeats * SongBpm2);

       // 转换短采样为浮点 ConvertFloat2Short(bufferShort,buffer,2*nb); 
	   // 通过音频缓冲区读取'nb'立体声采样 等于 ( = 2 * 'nb' 单声道采样) 
        for(int i=0;i<nb;i++)
	{
               CurrentPos = pos + i;
		xBeat = (CurrentPos - StartPos)/float(SongBpm2);  // 'x' 拍数来自StartPos

                // 这是一个立体声信号，因此含有两个通道/声道。
		for(int ch=0;ch<2;ch++)
		{
                    in[ch] = buffer[2*i + ch];  

                        // 在本示例中，我们仅保留左声道(ch==0) 并将右声道静音(ch==1)
                       if(ch==1) out[ch] = 0;
                        else  out[ch] = in[ch];

                     buffer[2*i + ch] = out[ch]; 
		}
	}


      // 转换短采样为浮点 ConvertShort2Float(buffer,bufferShort,2*nb);

	return S_OK;
}
//------------------------------------------------------------------------------------
void CMyPlugin8::ConvertFloat2Short(short *BufferOut,float *BufferIn, int BufferSize)
{
	int v;
	int j;

	for(j=0;j<BufferSize;j++)
	{        
		v = round (32768.0f * BufferIn[j]);
		if (v < -32768)  v = -32768;
		else if (v > 32767) v = 32767;
		BufferOut[j] = (short) v;
	}
}
//------------------------------------------------------------------------------------
void CMyPlugin8::ConvertShort2Float(float *BufferOut,short *BufferIn,int BufferSize)
{
	int j;

	for(j=0;j<BufferSize;j++)
	{
		BufferOut[j] = (float) BufferIn[j] / 32768.0f;
	}
}

