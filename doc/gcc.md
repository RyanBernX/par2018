# Lecture 02

A brief introduction to Linux Programming: vim+gcc+ld+gdb

---

## 使用 Vim 编辑程序

[Vim](https://www.vim.org/) 是一个类似于 Vi 的著名的功能强大、高度可定制的文本编辑器，在 Vi 的基础上改进和增加了很多特性。
Vim 是自由软件。
Vim 拥有众多的插件可供选择，代码补全、编译及错误跳转等方便编程的功能特别丰富，在程序员中被广泛使用，和 Emacs 并列成为类 Unix 系统用户最喜欢的文本编辑器。
Windows 系统上可使用 GVim 。
(*Vim 的设计理念是组合，专为键盘设计，不应用鼠标来操作。*)
Vim 的主要开发者是Bram Moolenarr ，现在我们每次启动 Vim 时，封面上会随机选择一种提示语展示(共有三种，如"帮助乌干达的可怜儿童!")。

vimtutor是个很好用的教程，对初学者非常友好，要使用它可在终端输入以下命令，然后在里面按提示操作即可(不用担心会破坏文件):

	```
	vimtutor
	```

### 配置文件

.vimrc

.vim/

### 启动和退出

vim --plugin

vim -d     

vimdiff

:wq
:q!

### 模式转换

尽量在命令模式下操作，包括对文件的编辑！

<ESC> CTRL-[  CTRL-O

### 快速移动

gj,gk,ctrl-U,ctrl-D

ctrl-f

ctrl-b

^,$,0

w,b,e

5G

gg=G

shift+G

gg

### 命令模式下的编辑

d,c

cc

dw

d^

dd

d$

### 剪切、复制和粘贴

yy

p

10p

多重剪贴板通过寄存器实现：regs

### 选中和编辑

v

V

ctrl-v

### 撤销和重做

u

ctrl-R

.

### 标记与跳转

```
ma 
mA
'a
`a
```

### 其他

shift+j

ctrl-z

fg

### 插入模式下如何移动？

最好就不要在插入模式下移动！

可以选择方向键或者重映射快捷键

### 拓展提高

vim 的各种加强版本 neovim, exvim 等等

vim 的各种插件(一般用 VimL 语言编写，来自官网或 GitHub) taglist YouCompleteMe Vundle插件管理器 等等

书籍  Practical Vim

---

## 使用 GCC 编译程序

### 编译过程

### 调试选项

-g

### 优化选项

O0 O1 O2 O3 Os

发行时一般用 O2 或 O3 ，O3选项据称有一定风险

### 拓展提高

反汇编

---

## Linux 上程序的运行

gcc -lm -g example.c -o example.exe

复杂的编译命令可写在 Makefile 中

### 拓展提高

链接、装载与库

---

## 使用 GDB 来调试

gcc -g

### 拓展提高

visual studio 2017 远程调试功能

GDB 调试多线程与多进程

Unix 环境高级编程

---

