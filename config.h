#ifndef CONFIG_H
#define CONFIG_H
#include <QtGlobal>
#include <QString>

struct neteaseInfo{
    QString singerName;
    QString songName;
    QString album;
};

#define UA " Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/63.0.3239.26 Safari/537.36 Core/1.63.5221.400 QQBrowser/10.0.1125.400"
#define REFERERURL "y.qq.com"

#define DOWNLOADLINKHEAD "http://dl.stream.qqmusic.qq.com/"
#define DOWNLOADLINKMID "?fromtag=64&vkey="
#define DOWNLOADLINKGUID "&guid="
#define DOWNLOADLINKTAIL "&fromtag=1"

//歌曲html页面链接  HEAD+SONGMID+TAIL
 #define SONGLINKHEAD "https://y.qq.com/n/yqq/song/"
 #define SONGLINKTAIL ".html"

//获取vkey
#define USERAGENT "Mozilla/5.0 (Linux; Android 4.1.1; Nexus 7 Build/JRO03D) AppleWebKit/535.19 (KHTML, like Gecko) Chrome/18.0.1025.166  Safari/535.19"
#define GETVKEYLINKHEAD "https://i.y.qq.com/v8/playsong.html?ADTAG=newyqq.song&songmid="
#define GETVKEYLINKTAIL "#webchat_redirect"

//单曲信息
#define SINGLESONGHEAD "https://c.y.qq.com/v8/fcg-bin/fcg_play_single_song.fcg?songmid="
#define SINGLESONGTAIL "&tpl=yqq_song_detail&format=json&callback=getOneSongInfoCallback&g_tk=5381&jsonCallback=getOneSongInfoCallback&loginUin=0&hostUin=0&format=json&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0"

//专辑封面链接
#define ALBUMPICLINKHEAD "https://y.gtimg.cn/music/photo_new/T002R800x800M000"
#define ALBUMPICLINKTAIL ".jpg?max_age=2592000"

//歌单下载链接
#define PLAYLISTLINKHEAD "https://c.y.qq.com/qzone/fcg-bin/fcg_ucc_getcdinfo_byids_cp.fcg?type=1&json=1&utf8=1&onlysong=0&disstid="
#define PALYLISTLINKTAIL "&format=jsonp&g_tk=5381&jsonpCallback=playlistinfoCallback&loginUin=0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0"

//歌手html页面链接
#define SINGERLINKHEAD "https://y.qq.com/n/yqq/singer/"
#define SINGERLINKTAIL ".html"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *    拼接歌手单曲列表下载链接：HEAD+SINGERMID+MID+BEGINNUM+TAIL*
 *    每页30个，下载后拼接文件，begin确定从哪一个开始             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 #define SINGERSONGLISTLINKHEAD "https://c.y.qq.com/v8/fcg-bin/fcg_v8_singer_track_cp.fcg?g_tk=5381&jsonpCallback=MusicJsonCallbacksinger_track&loginUin=0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0&singermid="
 #define SINGERSONGLISTLINKMID "&order=listen&begin="
 #define SINGERSONGLISTLINKTAIL "&num=30&songstatus=1"

//下载专辑歌单列表
 #define ALBUMLINKHEAD "https://c.y.qq.com/v8/fcg-bin/fcg_v8_album_info_cp.fcg?albummid="
 #define ALBUMLINKTAIL "&g_tk=5381&jsonpCallback=albuminfoCallback&loginUin=0&hostUin=0&format=jsonp&inCharset=utf8&outCharset=utf-8&notice=0&platform=yqq&needNewCode=0"

//临时文件
#define SONGHTMLFILE "songhtml.tmp"
#define SINGERHTMLFILE "singerhtml.tmp"
#define ALBUMLISTFILE "albumlist.tmp"
#define PLAYLISTFILE "playlist.tmp"
#define SONGMIDFROMFILE "songmidfromfile.tmp"
#define SEARCHRESULTFILE "search.tmp"
#define SONGINFOFILE "songInfo.json"



#endif // CONFIG_H
