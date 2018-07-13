原来的QQMUsicDownloader，代码重写了，顺便改名叫MusicDownloader，应该会比之前的更加健壮了

1.8.0版本添加功能下载歌手所有专辑歌曲，自动添加到下载队列中

1.7.0版本开始使用下载队列，保存在数据库中，不需要像之前一样下载完一个任务才能添加下一个任务，粘贴好链接，选择好格式音质目录，点下载即可添加到下载队列，想要停止，直接关闭程序即可，重新打开程序会从数据库中读取任务链接开始继续下载

在1.6.2开始的版本中增加Mac版本，在realease中下载解压后，双击执行目录下MacOS/musicDownloader 文件

## 功能 ##
 1. 下载QQ音乐**单曲**
 2. 下载QQ音乐**专辑**下所有歌曲
 3. 下载QQ音乐**歌单**下所有歌曲
 4. 下载网易云音乐**歌单**下所有歌曲
 5. 只下载网易云音乐歌单中**灰掉**的歌曲
 6. 音质支持高清、无损，付费音乐以及灰掉的也能下载
 7. 由于直接下载的MP3文件标签内容中的编码不一致会有乱码，所以在程序里将MP3标签头用unicode编码重新编辑了，并插入了封面图
 8. 要下载单曲以及专辑，请使用QQ音乐中的链接

## 提示 ##
  1. 不支持短链接，QQ音乐软件复制出来的链接是短链接，要复制到浏览器之后再把链接粘贴过来
  2. 代理选项只支持socks5，一般在国外才需要使用代理，代理设置后重启生效，网上找来的代理速度一般及其慢，基本无法工作，最好找国内好点的socks5代理，或者自己假设服务器
  3. Mac版本，在realease中下载解压后，双击执行目录下Contents/MacOS/musicDownloader 文件
  4. 要看怎么实现的，只需看qqmusicsong.cpp 以及 config.h即可

## 使用说明 ##

 参考 [http://z52c.com/musicdownloader/][1]

 ![image](https://o05g5zevc.qnssl.com/e09bcd80-09af-4ce8-b225-ce89f0a388b1/TIM%E5%9B%BE%E7%89%8720180713231527.png?imageMogr2/auto-orient/strip/thumbnail/!1600x1600%3E)
 
 Mac配图未更新，同上

 ![image](https://o05g5zevc.qnssl.com/2e1c3700-4ccb-49ed-9879-02efce9a752f/TIM%E5%9B%BE%E7%89%8720180709202927.png?imageMogr2/auto-orient/strip/thumbnail/!1600x1600%3E)


  [1]: http://z52c.com/musicdownloader/
  
  
