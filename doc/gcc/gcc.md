# Lecture 02

A brief introduction to Linux Programming: vim+gcc+ld+gdb

---

## 使用 Vim 编辑程序

[Vim](https://www.vim.org/) 是一个类似于 Vi 的著名的功能强大、高度可定制的文本编辑器，在 Vi 的基础上改进和增加了很多特性。
Vim 是自由软件， Windows 系统上可使用 GVim 。
它的最大特色是完全使用键盘命令进行编辑，脱离了鼠标操作虽然使得入门变得困难，但上手之后键盘流的各种巧妙组合操作却能带来极为大幅的效率提升。
因此 Vim 和现代的编辑器（如 Sublime Text）有着非常巨大的差异，而且入门学习曲线陡峭，需要记住很多按键组合和命令。 
Vim 拥有众多的插件可供选择，代码补全、语法高亮、配色方案、自动纠错、编译及错误跳转等方便编程的功能特别丰富，在程序员中被广泛使用，和 Emacs 并列成为类 Unix 系统用户最喜欢的文本编辑器。
Vim 的主要开发者是Bram Moolenarr ，现在我们每次启动 Vim 时，封面上会随机选择一种提示语展示(共有三种，如"帮助乌干达的可怜儿童!")。

vimtutor是个很好用的教程，对初学者非常友好，要使用它可在终端输入以下命令，然后在里面按提示操作即可(不用担心会破坏文件):

	```
	vimtutor
	```

### 启动和退出

默认启动 Vim 可用 `vim` 命令，启动后会显示 Vim 的首页，此时未打开任何文件。
用 `vim --noplugin` 可不带任何插件启动 Vim ，但 Vim的配置信息仍然是被加载的。

用 `vim -d` 或 `vimdiff` 命令可对两个文件进行比较和编辑，对比算法采用的是 LCS(Longest Common Subsequence)。

如果要打开多个文件，可以用 `vim file1 file2` 命令(文件数目可以大于两个，下同)，打开后可用 `:n` 和 `:N` 命令在各文件间跳转。
我们时常希望在各文件间进行一些剪切、复制和粘贴的操作，因此更推荐的一种方法是使用分割或者标签页。
用 `vim -o file1 file2` 命令对 file1 和 file2 上下分割，用 `vim -O file1 file2` 命令对 file1 和 file2 左右分割。
用 `vim -p file1 file2` 命令可将多个文件各自在标签页中打开，标签页间的跳转可用 `gt` 或者 `:n` 和 `:N` 命令。

退出 Vim 请先回到命令模式(注意不要在插入模式下尝试退出)，然后输入 `:q` 命令即可退出。
如果打开了文件且文件发生修改，则需使用 `:wq` 命令保存并退出，或者使用 `:q!` 强制退出(不保存文件)。
如果打开了多个文件，可以逐个退出(即先退出一个文件，再退出另一个……)，也可以使用 `:wqa` 或 `:qa!` 命令一次性退出。


### 模式转换

Vim 有以下几种模式：

- 正常（normal）模式，缺省的编辑模式；下面如果不加特殊说明，提到的命令都直接在正常模式下输入；任何其它模式中都可以通过键盘上的 Esc 键回到正常模式。
- 命令（command）模式，用于执行较长、较复杂的命令；在正常模式下输入“:”（一般命令）、“/”（正向搜索）或“?”（反向搜索）即可进入该模式；命令模式下的命令要输入回车键（Enter）才算完成。
- 插入（insert）模式，输入文本时使用；在正常模式下键入“i”（insert）或“a”（append）即可进入插入模式（也有另外一些命令，如“c”，也可以进入插入模式，但这些命令有其它的作用）。
- 替换（Replace）模式，按R键进入，和插入模式的不同之处在于替换模式下的编辑会直接覆盖文本。
- 可视（visual）模式，用于选定文本块；可以在正常模式下输入“v”（小写）来按字符选定，输入“V”（大写）来按行选定，或输入“Ctrl-V”来按方块选定。

![Vim modes](mode.png)

**尽量在命令模式下操作，包括对文件的编辑！每当完成一个操作，要及时回到命令模式！**

敲下 `<ESC>` 键可以回到命令模式，或者使用 `CTRL-[` 组合命令，使用 `CTRL-O` 命令可以临时回到命令模式(然后你可以做一个操作如移动光标，之后会自动进入插入模式)。

打开文件时默认处于命令模式，要进入插入模式可用以下方法：

- i 在当前光标所在字符的前面，转为输入模式
- I  在当前光标所在行的行首，转换为输入模式
- a 在当前光标所在字符的后面，转为输入模式
- A 在当前光标所在行的行尾，转换为输入模式
- o 在当前光标所在行的下方，新建一行，并转为输入模式
- O 在当前光标所在行的上方，新建一行，并转为输入模式

### 快速移动

由于历史原因(或者确实有些道理)，Vim 选择`hjkl`作为方向键，分别表示左、下、上、右。
使用键盘上默认的方向键也是可以的，不过建议使用上面这种方式，因为不同键盘上方向键位置不固定，有些可能离主区域较远，来回移动比较麻烦。

**注意`hjkl`作为方向键不能在插入模式和替换模式下使用，但方向键可以。**

以下是一些常用的移动命令：

- 3j  向下移动3行，可换成其他数字（下同）
- 3k  向上移动3行
- 3h  向左移动3位
- 3l  向右移动3位
- gj 如果一行太长占用多个显示行，可用该命令向下移动一个显示行
- gk 如果一行太长占用多个显示行，可用该命令向上移动一个显示行
- Ctrl-U 内容下移半屏幕
- Ctrl-D 内容上移半屏幕
- Ctrl-f 内容向下移一屏幕
- Ctrl-b 内容上移一屏幕
- Ctrl-e 内容上移一行
- Ctrl-y 内容下移一行
- $ 移动光标到行尾
- 0 移动光标到行首
- ^ 移动光标到行首第一个非空格或tab的地方
- w 光标移到下一个单词的起始
- b 光标移到当前单词的起始
- e 光标移到当前单词的结尾
- 15G 定位到文件的第十五行
- gg 定位到文件的起始
- Shift+G 定位到文件的结尾
- Shift+H 光标移到当前屏幕的顶部
- Shift+M 光标移到当前屏幕的中间
- Shift+L 光标移到当前屏幕的底部
- gg=G 自动缩进整个文件
- % 光标移动到匹配的括号处
- `. 光标移动到上次编辑的位置

### 命令模式下的编辑

x,d,c,s,r,R,S

cc

3cc

cw

d^

dd

d$

dG

### 剪切、复制和粘贴

yy

p,P

10p

多重剪贴板通过寄存器实现：regs

"3yy "3p

### 选中和编辑

v

V

ctrl-v

### 搜索与替换

n,N

/^text$

/text\$\c

?text

:%s/xxx/yyy/g

:s/xxx/yyy/g

:s/xxx/yyy

:s/xxx/yyy/c

:s/xxx/yyy/i

:.,+2s/xxx/yyy/g

:3,5s/xxx/yyy/g


### 撤销和重做

u

ctrl-R

.

### 标记与跳转

ma 

mA

'a

`a

### 其他

shift+j

ctrl-z

fg

插入模式下如何移动？ 最好就不要在插入模式下移动！
可以选择方向键或者重映射快捷键
使用ctrl-O

set paste

中文输入的问题？  在 Linux 上 Vim 处于 Normal 模式时就禁用中文输入法

### 配置文件

.vimrc

.vim/

bundle colors spell undo

### 拓展提高

vim 的各种加强版本 neovim, exvim 等等

vim 的各种插件(一般用 VimL 语言编写，来自官网或 GitHub) taglist YouCompleteMe Vundle插件管理器 等等

书籍  Practical Vim

![vim operations](vim.png)

---

## 使用 GCC 编译程序

### C99 标准

![C versions](cversion.png)

### 编译选项

gcc -lm -g example.c -o example.exe

-g -L -I -l -D

### 编译过程

详细介绍编译的四个阶段，可以将每个阶段的中间输出展示给大家

预处理

编译

汇编

链接

### 调试选项

-g


### 拓展提高

反汇编

优化选项

O0 O1 O2 O3 Os

发行时一般用 O2 或 O3 ，O3选项据称有一定风险

---

## Linux 上程序的运行

```
./example.exe
```

复杂的编译命令可写在 Makefile 中

### 环境变量

LD_LIBRARY_PATH

PATH

### Linux 上的运行库

https://blog.csdn.net/u010977122/article/details/52958330

file example.exe

nm example.exe

ldd example.exe

类似 Windows 上的 dll ，库依赖很容易导致问题(比如不同的库版本)

静态库与动态库

/etc/ld.so.conf

### 拓展提高

链接、装载与库

---

## 使用 GDB 来调试

https://blog.csdn.net/u013525455/article/details/52813637

用 gcc -g 编译并取消优化选项

### 启动和退出

```
gdb ./example.exe
quit
Ctrl-D
```


### 基本使用

#### 用 list 列出源代码

list

help list

list 10

list addResult

list 10,15

list list.h:15

list list.h:addResult

#### 运行程序

run

run arg

#### 使用断点

break 7

break addResult

break list.h:15

break list.h:addResult

info break

break 8 if a == 10

disable 断点号

enable 断点号

disable once 断点号

enable once  断点号

enable delete  断点号

delete 断点号

clear

#### 流程控制

continue

next

step

kill

#### 监控变量

print &a

### 拓展提高

visual studio 2017 远程调试功能

GDB 调试多线程与多进程

Unix 环境高级编程

---

