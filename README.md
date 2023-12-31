# **基于 Qt 的俄罗斯方块游戏**

## 基本功能

**一、 游戏**

- 方块能自动下降，随机产生各种方块，并提前在小窗口预告给玩家

- 通过键盘控制方块的移动和旋转并摆放在合适的位置

- 当底部有被方块填满的行后，能够及时消去对应的方块，并提供消去满行方块计分算法

- 当游戏已经无法继续进行时，则结束游戏，能够合理判断结束游戏的时机

- 支持临时暂停和恢复游戏

- 支持保存游戏进度，复制存档，删除存档

- 支持保存玩家信息，修改玩家名字，删除玩家

**二、界面**

- 播放背景音乐，改变背景音乐曲目和音量大小

- 显示背景图片，改变背景图片选项

- 显示游戏规则简介界面

**三、** **参数**

- 支持设置游戏参数，如可以选择方块样式、方块下落速度、棋盘大小、按键设置等

- 用合理的方式计算分数、出具游戏排行榜

**四、 文件**

- 利用文件方式，支持保存玩家信息

- 利用文件方式，支持保存游戏进度，并从每个存档中恢复游戏

- 利用文件方式，支持保存游戏设置，可以重置设置

## 代码继承和包含关系图

![](https://github.com/SleepyAxin/Tetromino_by_Qt/blob/55bd9c20a0bce0dbdf54cbc734c0d732eaec474c/image/%E4%BF%84%E7%BD%97%E6%96%AF%E6%96%B9%E5%9D%97%E4%BB%A3%E7%A0%81%E9%80%BB%E8%BE%91%E5%9B%BE.png)

## 缺点与不足

**一、代码的内聚性**

- 游戏窗口类实现的功能太多太过于繁杂，不够独立

**二、玩家自定义程度不高**

- 背景图片和背景音乐只提供几种预设，不能由玩家选择文件

- 方块的颜色只提供几种预设，不能由调色板完全自定义颜色

**三、Qt 中一些函数的限制（keyPressEvent）**

- 键位只能设置单字符按键，不能设置 “↑” “↓” “←” “→” “Enter” 等特殊案件

**四、排行榜的合理性**

- 排序的依据只是分数，没有根据棋盘大小划分难度系数

**五、界面的友好性**

- 游戏窗口是固定大小，不支持缩放
