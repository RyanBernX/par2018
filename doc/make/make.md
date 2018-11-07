# Make进阶

按照[官方文档](https://www.gnu.org/software/make/manual/html_node/Makefile-Contents.html#Makefile-Contents)
的说法，一个Makefile可以包含以下五部分：

1. 显式规则（explicit rule），即写明的依赖和生成方法的规则。由于显式地提出了整个规则，所以称为显式规则。
2. 隐式规则（implicit rule），即一类有所缺省的规则。在这些简单的情形下，make可以推导出适合的依赖或生成方法。
3. 变量定义（variable definition），即将某一字符串定义为变量，方便下文使用。
4. 指令（directive），即指导make做一些特殊事情的指令，这包括
    - 读取另一个Makefile的include指令;
    - 根据条件判断执行或忽略下一部分语句的条件指令;
    - 定义多行变量的define指令;

5. 注释（comment）, 以#开头的一行。

## Make中的规则

```make
targets : prerequisites ; recipe
        recipe
        …
```

- 目标和依赖间用冒号分开
- 单行规则，依赖和规则用分号分开；多行规则，规则须以TAB开头

### 生成规则一定要TAB开头?

答案是否定的，TAB只是make识别规则的一个记号。我们可以通过修改`.RECIPEPREFIX`变量的值来改变这种记号。

```make
.RECIPEPREFIX=0345
test:
0echo Now RecipePrefix is $(.RECIPEPREFIX)
```

规则：The first character of the value of this variable is used as the character make assumes is introducing a recipe line. If the variable is empty (as it is by default) that character is the standard tab character.

所以，TAB开头的“注释”不是注释，TAB开头的“变量定义”不是变量定义。因为这些都会被传递给Shell，交由Shell具体解释。而具体由哪个Shell执行，由变量SHELL控制。可以使用如下方法查看其默认值。

```make
test:
    echo default shell is $(SHELL)
```

### 规则执行方式

Make本身并不了解规则；Make只是对规则进行简单处理后，交由Shell执行。而且默认是每行命令在一个单独的Shell中执行。所以下面是一个典型错误：本想在子目录下建立一个a.txt文件，但是实际结果是在当前目录下建立了该文件。

```make
test:
    cd SUBDIR
    touch a.txt
```

解决方案一：多行变一行

- 分号分隔命令；特点是各条命令独立，前一个命令运行失败，后一个命令不受影响。
- 逻辑符号`&&`；只有当前一个命令正确执行，后一个命令才会接着执行。
- 分行符号`\`;需要注意的时，该分行符实际上时传递给了Shell的。所以，最终结果中有没有分行符号，是依赖于Shell的解释的。例如考虑如下例子：

```make
all : ; @echo 'hello \
        world' ; echo "hello \
    world"
```

输出结果如下：

```make
hello \
world
hello     world
```

注意到，在上面的例子中，Shell对于单引号和双引号中的 \ 处理不同，
而且，如果使用不同的Shell可能会得到不同的结果。上面的Makefile中还是用了前缀@，取消了命令的回显。

#### 命令前缀

- 前缀为空：命令回显，出错停止
- 前缀为@：取消回显，出错停止
- 前缀为-：忽略错误，继续执行

实际上make传递给Shell的命令如下

```make
echo 'hello \
world' ; echo "hello \
    world"
```

make在其中所做的工作只是，将下一行开头的TAB去除（如果没有的使用TAB，则什么也不做）,然后交由SHELL来解释执行。

解决方案二：使用`.ONESHELL`命令

### 一些特殊结构

#### 伪目标

伪目标和其他一般目标的区别主要在于，本身并不代表一个文件，只代表一些命令的名称。主动使用`.PHONY`指令申明伪目标的好处在于:即使当前文件夹下有同名文件也不影响操作。常见于`clean`等操作。

```make
.PHONY:clean
clean:
    -rm $(objects)
```

一般而言，给原目标添加特殊空白依赖，也可以达到伪目标的效果。常见于不支持`.PHONY`指令的Makefile中。

```make
clean: FORCE
    -rm $(objects)
FORCE：
```

但若当前目录下，同时有名为clean和FORCE的文件，而且clean新于FORCE的话，命令同样还是无法执行。

### 静态模式规则

静态模式规则（Static Pattern Rules）是一种针对多目标的规则。
它利用了多个目标间依赖的相似性。其语法如下

```make
targets …: target-pattern: prereq-patterns …
        recipe
        …
```

目标文件列表（targets）说明了静态模式规则适用的目的文件，其中可以使用通配符。
而目标模式（target-pattern）和依赖模式（prereq-patterns）说明了如何获得
每个目标文件的依赖。其具体方法如下，目标文件名先匹配目标模式（target-pattern），
抽取出目标文件名的部分，称为词干（stem）;之后将词干替换到依赖模式
（prereq-patterns）中获得该目标文件想对应的依赖。所以通常目标模式和依赖模式
中都具有一个通配符\%,而被通配符\%所匹配的部分，即是上面所说的词干（stem）。当然
在依赖模式中不写通配符\%也是可以的，但这样就意味着所有的目标文件的依赖就都是相同的了。
下面是一个使用静态模式规则的例子

```make
objects = foo.o bar.o

all: $(objects)

$(objects): %.o: %.c
        $(CC) -c $(CFLAGS) $< -o $@
```

其中自动变量\$<表示第一个依赖名，\$\@表示目标名。例如当目标为foo.o
时，目标匹配目标模式\%.o产生词干foo，而词干foo替换到依赖模式\%.c中，
就获得了目标foo.o对应的依赖foo.c。

## Make中的变量

make中的变量具有两个重要属性：名和值。变量的命名，可以使用任意的字符串，但不能包含
空格或=等特殊字符。使用除字母、数字、下划线之外的字符需要特别小心，因为这些字符
可能会在某些SHELL里有特殊含义。make中变量的命名区分大小写。而make中变量的值也是一个
字符串，可以用来充当目标（Targets）、依赖（Prerequisites）、方法（Recipes），或
其他makefile的组成部分。所以，在某些版本中变量又称为宏（Macros）。变量的定义使用=或define指令。

引用变量的值时，需要使用美元符号\$，后接括号括起来的变量名。
\$(foo)或\$\{foo\}都可以引用变量foo的值。虽然，对于变量名是一个字符的变量来说，
引用变量值的时候不使用括号也是可行的，例如\$x也能引用x的值，但这样是不推荐的。

之前也有提到，对于生成方法，make只是进行简单处理后交给Shell执行。
这里所说的简单处理，就包含着对变量和函数的展开。所以，
在生成方法中对makefile变量的使用，和其他地方相同，需要使用\$引用；而对于Shell变量，
则需要使用\$\$。如下一个例子

```make
LIST = one two three
all:
        for i in $(LIST); do \
            echo $$i; \
        done
```

在make进行变量展开后，传递给Shell的是如下脚本

```make
for i in one two three; do \
    echo $i; \
done
```

所以，最后的输出结果为

```
one
two
three
```

Make中的变量使用是严格的字符替换，所以下例也是可以使用的，但极不推荐

```make
foo = c
prog.o : prog.$(foo)
        g$(foo)$(foo) -$(foo) prog.$(foo)
```

Make中的变量展开有两种：

第一种：使用=的赋值

```make
foo = $(bar)
bar = $(ugh)
ugh = Huh?

all:;echo $(foo)
```

make在解释`$(foo)`时，会发现foo的值和bar变量有关，然后查看bar变量时，又发现和
ugh变量有关，所以这样依次递归下来，最终解释为Huh？。这种变量展开，本质上是make
在变量值的记录中允许对其他变量值进行引用而造成的。这种变量的展开会遇到如下的问题，下面
本意给变量末尾添加字符的赋值操作，将会出错。

```make
CFLAGS = $(CFLAGS) -O
```

因为make在解释CFLAGS的值时，会发现和CFLAGS自己有关，然后接着查询CFLAGS的值，
如此循环往复，陷入到死循环中。这种赋值方式的另外一点不足在于，每次使用变量时，总需要多次查询，影响make的运行效率。

第二种：使用：=或：：=的赋值（在GNU make中这两种等价）

```make
x := foo
y := $(x) bar
x := later
```

这里最后的结果是，y变量的值为foo bar，而x变量值为later。因为在这种赋值方式下，
make在记录变量值时，只根据引用查询一次，然后变量的值中就只有字符，而不存在对其他变量的引用了。这种方式，和其他诸如C语言中的变量赋值意义相近。

其他方式的赋值：

- ?=  变量不存在时才赋值
- +=  附加变量的值
- 替换赋值(Substitution reference)
- 计算变量名(Computed Variable Names)

```make
#Substitution reference
foo := a.o b.o c.o
bar := $(foo:.o=.c)

#Computed Variable Names
x = y
y = z
a := $($(x))
```

### 自动变量

- $@，目标
- $<，首个依赖
- $^，所有依赖，去除重复的依赖
- $+，所有依赖，不会去除重复的依赖

### 条件控制指令

`ifeq`，`ifneq`，`ifdef`，`ifndef`等等。

```make
test:
ifeq ($(SHELL),/bin/sh)
    echo I am using sh
else
    echo I am using $(SHELL) instead of sh
endif
```

### include指令

使用`include`指令可以在一个Makefile里面包含另一个Makefile

### 递归调用Make

在较大的工程中，不同的模块一般位于不同的文件夹下。一个合理的组织方式是对每个子模块编写各自的Makefile进行管理，而整体的根目录下的Makefile可以依次调用这些子模块的Makefile。

```make
subsystem:
    cd subdir && $(MAKE)
```

或者

```make
subsystem:
    $(MAKE) -C subdir
```

### 几个应用

#### Latex的编译

#### Linux软件安装“三板斧”

- ./configure
- make
- make install

具体来说，第一步./configure，是调用当前目录下的名为configure的Shell脚本，
检查当前电脑是否具备编译该软件的能力，例如是否安装了make、是否具备C
编译器、是否具有必要的头文件等等。这一步的检查，是后面步骤进行的基础。
第二步make，是使用make启动整个软件的编译。
而第三步make install，是使用make完成
install目标，例如测试程序运行是否正常、将编译好的软件放置到适合的系统路径上等。

#### 典型的库结构

- [math库](https://github.com/RyanBernX/math)
- [Lapack库](https://github.com/Reference-LAPACK/lapack)

#### CMake

简单来说，CMake是一个帮我们写Makefile的工具。它的一个重要特性是跨平台。CMake的
所有配置信息都存放到一个叫做CMakeLists.txt的文件中。使用CMake可以帮助我们产生编译、安装、测试所需的Makefile。其典型的使用方法如下

```make
cmake .
make
```

### 一些学习链接

- [GNU Make Manual](https://www.gnu.org/software/make/manual/)
- [跟我一起写Makefile](https://seisman.github.io/how-to-write-makefile/index.html)
- [开启Makefile大门](https://www.jianshu.com/p/7e49071f9e11)