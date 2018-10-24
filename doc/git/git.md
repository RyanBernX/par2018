# git简介
git 是一个分布式版本控制软件，用于维护代码项目。其本身是Linus大佬为了控制Linux的内核
版本，又不愿为各种商业版本控制系统左右时，用不到两周的时间用C编写的分布式版本控制系统，
但其现在不仅管理着Linux内核的开发，也是最流行的分布式版本控制系统。版本控制的意义在于，
我们写一段代码，或者写一个文档，在不断地对其进行修改和更新的过程中记录我们每一次的修改
和更新，从而在出现错误是能够及时找到错误的原因或回到未出错的版本进行修改。市面上如今也有
很多同步协作平台提供本地的自动更新和同步以及版本记录功能，但使用最为广泛的还是git。
## git安装和初始配置
查看是否安装git
`which git`
使用`apt-get`，`yum`，`pacman`等方式安装git，Mac可以使用homebrew进行安装

首先进行用户设置
```
git config --global user.name "name"
git config --global user.email "mail"
```
直接使用
`git config`
会显示更多设置选项

## 创建版本库
版本库也叫仓库(repository，即常说的repo)，其本质上是一个目录文件，其下的所有文件被git
所管理，每个文件的修改、删除都能被git跟踪，从而我们可以在任何时刻追踪文件的历史或将其
还原到某个历史版本。

选择一个我们希望被git管理的目录，然后执行
```
git init
```
即可将该目录变成git可以管理的仓库，可以发现目录下多了一个**.git**目录，里面记录了所有
git管理的信息，将其删除`rm -rf .git`即可取消git对当前目录的管理。
使用
```
git status
```
可以方便地查看当前版本库的状态

## 向版本库添加文件

git能跟踪版本库内所有文本文件的每次改动，具体到哪一行增删了哪些词都会记录。对于二进制文件，
git只能跟踪其大小的变化。

git有三个特别的区域----工作区，暂存区和头指针。工作区就是我们工作的文件目录，我们实际进行
代码的修改和增删文件都在工作区进行。

使用
```
git add <filename>
```
可以将文件的修改添加到暂存区，其中文件名可以使用通配符，如`git add *`可将目录下的所有文件都添加
到暂存区。
使用
```
git commit -m "comment"
```
可以将暂存区的文件修改提交到版本库，并将头指针HEAD指向当前提交，**comment**部分用来解释本次提交
的代码改动说明，方便日后查看。
```
git diff
```
可以告诉我们当前版本库和HEAD所指向的提交间进行了何种修改。

文件的增删在git中也对应着文件的修改，从版本库中删除文件需要使用
```
git rm <filename>
```

# 版本穿梭
在我们进行了多次提交后，可以使用
```
git log
```
来查看每次提交的细节，使用`git log --pretty=oneline`可以查看简化版的输出信息。其中包含了每次
提交的**commit id**和说明的内容。
git中使用**HEAD**指向当前的版本，**HEAD^**指向上一个版本，**HEAD~10**指向向上10个版本。
通过
```
git reset --hard HEAD^
```
可以将版本回退至上一次提交的版本，同理可回退至前若干个版本。
通过
```
git reset --hard <commit id>
```
可以将版本跳跃至对应的**commit id**的提交版本，但是`git log`不会显示当前版本之后的提交内容，
使用
```
git reflog
```
可以查看我们执行过的每条命令，从而找到我们目标版本的**commit id**。`git reset`的`--hard`参数表示
强行将HEAD指针指到`<commit id>`对应的版本。通过`git reset --help`可以查看其更多的操作参数，如
`--soft`，`--mixed`，`--merge`等。

git管理的是文件的修改，而非文件本身。我们每次使用`git add`会将当前的修改放进暂存区，而后续继续
对文件的修改则需要新的add操作。可以通过`git checkout -- <filename>`放弃对工作区内文件的修改，通过
`git reset HEAD <filename>`放弃暂存区内文件的修改。

有时觉得查找`<commit id>`太麻烦，可以给当前的提交添加一个标签，通过
```
git tag <tagname>
```
添加标签。标签通常标记比较重要的版本更新，从而可以在之后的开发中快速找到之前的稳定版本。所有提交操作
的`<commit id>`部分都可以替换为对应的标签。

# git的分支管理

git最有特色的功能之一就是其分支管理功能，
git将每次提交串成一条时间线，每条时间线被称为一个分支，默认情况下有一条主分支master。多人协作时，为了
不妨碍他人的工作，可以创建新的分支进行修改，在修改完成后再和master分支合并。

## 创建与合并分支

通过
```
git branch dev
git checkout dev
```
可以创建一条新的名为`dev`的分支并指向它。也可以使用`git checkout -b dev`一步完成这个操作。
通过`git branch`可以查看所有的分支，当前分支前会有一个`*`号。现在我们所有的提交都会在`dev`分支上进行，
当我们完成所有修改并提交后，使用
```
git checkout master
```
切换回`master`分支，再查看修改的文件会发现所有在`dev`上的修改都看不到了，通过
```
git merge dev
```
可以将`dev`与`master`合并。然后可以通过
```
git branch -d dev
```
删除`dev`分支，再用`git branch`查看就只有`master`分支了。

## 解决冲突
如果我们在新分支提交后切换回`master`又做了新的提交，这时合并二者会出现冲突，需要我们手动解决冲突
后进行合并。`git status`会告诉我们存在冲突的文件，打开对应文件会发现git使用`<<<<<<<`,`=======`
`>>>>>>>`标注出了不同分支的内容，将文件修改成我们需要的内容后再进行`add`和`commit`即可解决冲突。
通过`git log --graph`可以看到分支合并的情况。

# 远程仓库
git的最突出特点之一就是远程仓库，可以将一台电脑的git仓库方便地克隆到其他电脑里。GitHub就是著名的
免费提供git仓库托管服务的网站，注册GitHub账号即可免费获得git远程仓库。本地的git仓库和GitHub仓库间
的传输通过ssh协议进行，所以需要先进行ssh设置。

- 创建ssh key
```
ssh-keygen -t -rsa -C <email>
```
- 在主目录下找到`.ssh`目录，找到**id_rsa.pub**文件
- 登陆GitHub，在**settings->SSH Keys**页面添加**id_rsa.pub**的内容

## 添加远程库
在GitHub上创建一个新的仓库，创建页面会提示使用
```
git remote add origin git@github.com:username/repo.git
```
或者
```
git remote add origin https://github.com/username/repo.git
```
在本地链接远程库，通过
```
git push -u origin master
```
将本地的master分枝推送到远程，第一次使用是会有验证GitHub服务器的SSH Key的警告，输入yes来将GitHub服务器
的SSH Key添加到本地的信任列表里。

## 从远程库克隆
通过
```
git clone <url>
```
的方式可以将远程库克隆到本地。通过`git push origin master`可以向其推送分支，通过`git fetch`可以获取最新
版本的版本库。

## 多人协作
当我们从远程仓库克隆时，git自动把本地的`master`分支和远程的`master`分支对应，远程仓库默认名称
为`origin`，通过
```
git remote -v
```
可以查看远程分支的详细信息。

通过
```
git push origin master
git push origin dev
```
等可以向远程仓库推送对应的分支。多人协作时，远程分支可能在你推送时已经更新过，这时会出现推送失败的提示，
使用
```
git pull
```
将远程的分支抓取到本地试图合并(其本质是`git fetch`然后`merge`)，若有冲突还需手动解除冲突，在本地完成合并和提交后再次`push`即可。

`git pull`可能会提示`no tracking information`的错误，需要使用
```
git branch --set-upstream-to <branch-name> origin/<branch-name>
```
将本地的分支与远程的分支链接。

更多git的使用细节可以从[https://book.git-scm.com/](https://book.git-scm.com/)获取更为详细的git使用手册，
[廖雪峰的Git教程](https://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000)
也是相当简明易懂的git教学网站(本文档git部分参考了此网站内容)。

# Doxygen

Doxygen是为软件生成说明文档的工具。

## 安装Doxygen

Doxygen 的官方[下载页面](http://www.stack.nl/~dimitri/doxygen/download.html)
提供了多种安装方案

可以通过clone他们的git repo得到他们最新的测试版本
```
git clone https://github.com/doxygen/doxygen.git
cd doxygen

mkdir build
cd build
cmake -G "Unix Makefiles" ..
make
make install
```

也可以下载最新的发行版本，他们提供了源码版，二进制版，Windows版和Mac OS的dmg安装文件
以源码版为例，我们下载`doxygen-1.8.14.src.tar.gz`文件解压
```
tar -czvf $PATHOF(doxygen-1.8.14.src.tar.gz)
cd doxygen-1.8.14
mkdir build
cd build
cmake -G "Unix Makefiles" ..
make
make install
```
我们还可以从安装文件生成Doxygen的使用说明文档，回到`build`目录
```
cmake -Dbuild_doc=YES ..
make docs
```
我们可以得到`html`和`latex`两种格式的说明文档，到`html`目录下，打开任意一个`html`
文件即可在浏览器中以html的形式浏览`Doxygen`的使用说明文档；在`latex`目录下进行编译
即可得到其`pdf`格式的说明文档。

## 生成项目说明文档

Doxygen通过专用的配置文件生成代码的说明文档，可以通过
```
doxygen -g <config-file>
```
生成一个配置文件的模板。Doxygen的配置文件采用`<TAGNAME>=<VALUE>`的结构，与Makefile的结构
类似。模板中有对每一个标记的详细说明，其中以下标记较为重要

- **<OUTPUT_DIRECTORY>**: 这个标记定义生成的说明文档放置的位置，可以使用相对路径
- **<INPUT>**:            这个标记记录需要生成说明文档的源代码的位置，可以使用多个以空格分隔的路径，可以使用相对路径
- **<FILE_PATTERNS>**:    这个标记定义doxygen搜索源文件的后缀名
- **<RECURSIVE>**:        这个标记值为**YES**时，doxygen会递归地搜索整个**INPUT**目录的层次结构并提取信息
- **<EXTRACT_ALL>**:      这个标记设置为**YES**时，doxygen会对所有的类和函数提起信息，即使其没有说明文档。

编辑完配置文件后，通过
```
doxygen <config-file>
```
即可得到`html`和`latex`两种格式的说明文档文件，用浏览器打开`html`目录下的`index.html`文件即可查看说明文档。
`latex`格式的说明文档需要进一步编译为`pdf`格式。

## 简单的代码内说明语句
C风格的代码说明语句可以有如下几种形式
```
/**
 * ...text...
 */
```
```
/*!
 * ...text...
 */
```
```
///
/// ...text...
///
```
```
//!
//! ...text...
//!
```
```
/***************************//**
 * ...text...
 ******************************/
```
```
////////////////////////////////
/// ...text...
///////////////////////////////
```

doxygen对代码的说明通过注释模块来实现，对于C风格的代码，其基本格式如下
```
/*! \class Text
    \brief A test class.

	A more detailed description
*/
```
其中`\class`用于类的注释，还有许多其他类型注释如下
- `\struct`用于C结构体注释
- `\fn`用于函数的注释
- `\var`用于变量的注释
- `\def`用于宏的注释
- `\typedef`用于定义的注释
- `\file`用于文件的注释
- `\namespace`用于命名空间的注释


更为详细和进阶的doxygen使用方法可以查看我们在安装时生成的说明文档。
