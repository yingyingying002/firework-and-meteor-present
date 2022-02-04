#
2022的新年祝福代码，生成控制台动画。基本c++ easyx库实现 windows环境 visual studio 2017

开屏 图片椭圆轨迹

然后烟花 流星雨

最后结尾

有些简陋，下次还是用其他工具吧。

代码效率不是很高，椭圆轨迹实现在方向有问题，于是用了三个for循环避免突然方向颠倒。

播放背景音乐和动作音效使用了两套声音函数，因为Playsound函数会直接停止，要么得多进程(多线程试了一下也不行)

烟花代码参考这位:
https://github.com/zz2summer/firework

流星雨是随便找的，参考这位：
https://www.jb51.net/article/229801.htm
