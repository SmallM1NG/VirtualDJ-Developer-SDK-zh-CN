
/!\ 虽然可以从外部应用程序读取数据库，但不太建议同时向其写入。 我们不对此类外部应用程序的使用负责，也不会在此类外部应用程序损坏数据库的情况下提供技术支持。
/!\ 自VDJ v6版本起，XML数据库使用UTF-8编码

database.xml
<?xml version="1.0" encoding="UTF-8"?>
<VirtualDJ_Database Version="8.6">

 <Song FilePath="D:\音乐\李明杰.mp3" FileSize="100000"> // 歌曲路径、文件长度
  <Tags xxxx/> // 标签属性
  <Infos xxxx /> // 文件信息
  <Scan xxxx/> // 是否扫描状态
  <Poi xxxx/> // 标记
 <Poi xxxx/> // 标记
 <Poi xxxx/> // 标记
  <Comment>备注内容</Comment>
  <CustomMix>自定义混音</CustomMix>
  <Link xxxx/> // 关联
 </Song>

 <Song FilePath="D:\音乐\李明杰.mp3" FileSize="100000"> // 歌曲路径、文件长度
 </Song>  
 
</VirtualDJ_Database>




<?xml version="1.0" encoding="UTF-8"?>
<[o]> VirtualDJ_Database // VDJ数据库
[list]
Version // 版本
<[o]> Song // 音频
[list]
FilePath // 文件时长
FileSize // 文件大小
Flag // 标识
<[o]> Tags // 标签
[list]
Flag // 标识
Author // 艺人、歌手、艺术家
Title // 标题信息
Year // 年份、年代
Genre // 曲风、流派、类型
Bpm // 拍速
Key // 音调
Album // 专辑
Composer // 作曲
Label // 发行商、厂牌、发行公司、出版公司
TrackNumber // 音轨编号
Remix // 二创混音
Stars // 评级、星级
Remixer // 原创混音
Grouping // 分组
User1 // 用户自定义一
User2 // 用户自定义二
Internal // 内部

<[o]> Scan // 扫描信息

Version // 版本
Flag // 标识
Volume // 磁盘
Bpm // 拍速
AltBpm // 副拍速
Key // 音调

<[o]> Infos // 媒体属性（音频或视频）

SongLength // 歌曲时长
Bitrate // 视频码率、音频音质、比特率
Cover // 封面
Color // 色彩
FirstSeen // 首次浏览时间
FirstPlay // 首次播放时间
LastPlay // 最后播放时间
PlayCount // 播放次数
Corrupted // 
Faked [DEPRECATED] // 虚实文件 已淘汰
BpmTag [DEPRECATED] // 拍速标签 已淘汰
KeyTag [DEPRECATED] // 音调标签 已淘汰
Gain // 增益信息
UserColor // 指定色彩

<[o]> Poi // 标记信息

Pos // 时间位置
Type // 标记类型
Point // 切点
Name // 名称
Num // 编号
Bpm // 拍速
Size // 长度
Color // 色彩
Slot // 栏号

<[o]> Link // 关联
<[o]> Comment // 备注内容
[*] <[o]> CustomMix // 自定义混音
[/list]
[/list]
[/list]
关于拍速BPM转换的注意事项：
数据库中存储的值实际上是两个节拍之间的时间（以秒为单位）。
要将数据库中使用的扫描拍速Scan->Bpm值转换为皮肤上显示的bpm，请使用以下公式: Bpm = 1 / Scan->Bpm * 60

AltBPM是扫描引擎在扫描曲目时发现的第二个最可能的拍速BPM（您可以在标签BPM拍速编辑器下拉菜单中看到该BPM）
