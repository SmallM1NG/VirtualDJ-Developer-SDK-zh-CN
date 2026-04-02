//////////////////////////////////////////////////////////////////////////
//
// 
// VDJ DSP数字音频处理插件插件开发SDK - 李明杰VIP
// 
//
//////////////////////////////////////////////////////////////////////////
//
// 该文件定义了实时DSP音效插件。
// 除了基础IVdjPlugin类支持的所有元素外、
// 它定义了额外的DSP特定函数和变量：
//
//////////////////////////////////////////////////////////////////////////


#ifndef VdjDsp8H
#define VdjDsp8H

#include "vdjPlugin8.h"

//////////////////////////////////////////////////////////////////////////
// DSP 数字音频处理插件类

class IVdjPluginDsp8 : public IVdjPlugin8
{
public:
	// 插件启动或停止时调用
	virtual HRESULT VDJ_API OnStart() {return 0;}
	virtual HRESULT VDJ_API OnStop() {return 0;}

	// 每次VDJ需要您的插件时都会调用此功能函数
	// 应用于新的声音缓冲区
	// 注意：采样是立体声，因此您需要处理缓冲buffer[2*nb]
	virtual HRESULT VDJ_API OnProcessSamples(float *buffer, int nb)=0;

	// 一些有用的变量
	int SampleRate;			// 音频引擎的采样率
	int SongBpm;			// 这首歌的两个连续节拍之间的采样数
	double SongPosBeats;	// 歌曲中第一个节拍的节拍数
};

//////////////////////////////////////////////////////////////////////////
// 缓冲插件类

class IVdjPluginBufferDsp8 : public IVdjPlugin8
{
public:
	// 插件启动或停止时调用
	virtual HRESULT VDJ_API OnStart() {return 0;}
	virtual HRESULT VDJ_API OnStop() {return 0;}

	// 每次VDJ需要您的插件时都会调用此功能函数
	// 应用于新的声音缓冲区
	// 注意：采样是立体声，因此您需要处理缓冲buffer[2*nb]
	virtual short * VDJ_API OnGetSongBuffer(int songPos, int nb)=0;

	// 调用此函数以获取指定位置的缓冲区
	HRESULT GetSongBuffer(int pos, int nb, short **buffer) {return cb->GetSongBuffer(pos, nb, buffer);}

	// 一些有用的变量
	int SampleRate;			// 曲目采样率
	int SongBpm;			// 这首歌曲两个连续节拍之间的样本数
	int SongPos;			// 从歌曲开头开始的采样数
	double SongPosBeats;	// 歌曲中第一个节拍的节拍数
};

class IVdjPluginPositionDsp8 : public IVdjPlugin8
{
public:
	// 插件启动或停止时调用
	virtual HRESULT VDJ_API OnStart() { return 0; }
	virtual HRESULT VDJ_API OnStop() { return 0; }

	//调用时可以修改曲目播放进度位置songPos
	virtual HRESULT VDJ_API OnTransformPosition(double *songPos, double *videoPos, float *volume, float *srcVolume) = 0;

	// 每次VDJ需要您的插件时都会调用此功能函数
	// 应用于新的声音缓冲区
	// 注意：采样是立体声，因此您需要处理缓冲buffer[2*nb]
	virtual HRESULT VDJ_API OnProcessSamples(float *buffer, int nb) { return 0; }

	// Some useful variables
	int SampleRate;			// 曲目采样率
	int SongBpm;			// 这首歌曲两个连续节拍之间的样本数
	int SongPos;			// 从歌曲开头开始的采样数
	double SongPosBeats;	// 歌曲中第一个节拍的节拍数
};

//////////////////////////////////////////////////////////////////////////
// GUID 全局唯一标识符定义

#ifndef VDJDSP8GUID_DEFINED
#define VDJDSP8GUID_DEFINED 
static const GUID IID_IVdjPluginDsp8 = { 0x7cfcf3f5, 0x6fb9, 0x434c, { 0xb6, 0x3, 0xd7, 0x3a, 0x88, 0xf6, 0x72, 0x26 } };
#else
extern static const GUID IID_IVdjPluginDsp8;
#endif

#ifndef VDJBUFFER8GUID_DEFINED
#define VDJBUFFER8GUID_DEFINED 
static const GUID IID_IVdjPluginBuffer8 = { 0x1d00e65f, 0x44c7, 0x41bf, { 0xa3, 0x6b, 0x04, 0xda, 0xf2, 0x67, 0x3b, 0x98 } };
#else
extern static const GUID IID_IVdjPluginBuffer8;
#endif

//////////////////////////////////////////////////////////////////////////

#endif
