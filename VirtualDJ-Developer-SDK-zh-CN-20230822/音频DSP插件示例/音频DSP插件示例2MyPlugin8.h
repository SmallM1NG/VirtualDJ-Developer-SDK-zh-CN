#ifndef MYPLUGIN8_H
#define MYPLUGIN8_H

#include "vdjDsp8.h"

#include <cmath>
// round() 仅在C++11中定义，因此我们使用此解决方法
#define round(v) ((int)floor((v)+0.5f))

class CMyPlugin8 : public IVdjPluginDsp8
{
public公有权限:
	HRESULT VDJ_API OnLoad();
	HRESULT VDJ_API OnGetPluginInfo(TVdjPluginInfo8 *infos);
	ULONG   VDJ_API Release();
        HRESULT VDJ_API OnStart();
	HRESULT VDJ_API OnStop();
        HRESULT VDJ_API OnProcessSamples(float *buffer, int nb);

private私有权限:
        int SongBpm2;
	int StartPos;

        //  如果您想使用短采样short 16位，而不是浮点采样 float 32 位，请使用
        short bufferShort[8194]； // 我们考虑声音缓冲区最大可容纳4096个采样点（但也可以动态设定）
        void ConvertFloat2Short(short *BufferOut,float *BufferIn, int BufferSize);
	void ConvertShort2Float(float *BufferOut,short *BufferIn, int BufferSize);

};

#endif
