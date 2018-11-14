


# MPI学习

## mpi简介
在 90 年代之前，对于不同的计算架构写并行程序是一件困难而且冗长的事情。当时，很多软件库可以帮助写并行程序，但是没有一个大家都接受的标准来做这个事情。

在当时，大多数的并行程序只出现在科学和研究的领域。最广为接受的模型就是消息传递模型。什么是消息传递模型？它其实只是指程序通过在进程间传递消息（**消息可以理解成带有一些信息和数据的一个数据结构**）来完成某些任务。在实践中，并行程序用这个模型去实现特别容易。举例来说，主进程（master process）可以通过对从进程（slave process）发送一个描述工作的消息来把这个工作分配给它。另一个例子就是一个并发的排序程序可以在当前进程中对当前进程可见的（我们称作本地的，locally）数据进行排序，然后把排好序的数据发送的邻居进程上面来进行合并的操作。几乎所有的并行程序可以使用消息传递模型来描述。

由于当时很多软件库都用到了这个消息传递模型，但是在定义上有些微小的差异，这些库的作者以及一些其他人为了解决这个问题就在 Supercomputing 1992 大会上定义了一个消息传递接口的标准- 也就是 MPI。这个标准接口使得程序员写的并发程序可以在所有主流的并行框架中运行。并且允许他们可以使用当时已经在使用的一些流行库的特性和模型。

到 1994 年的时候，一个完整的接口标准定义好了（MPI-1）。我们要记住 MPI **只是**一个接口的定义而已。然后需要程序员去根据不同的架构去实现这个接口。很幸运的是，仅仅一年之后，一个完整的 MPI 实现就已经出现了。在第一个实现之后，MPI 就被大量地使用在消息传递应用程序中，并且依然是写这类程序的_标准_（de-facto）。

![九十年代的程序员]($resource/90s_nerd.jpg)

## 基本概念
在开始教程之前，先解释一下 MPI 的一些经典概念。第一个概念是**通讯器**（communicator）。通讯器定义了一组能够互相发消息的进程。在这组进程中，每个进程会被分配一个序号，称作**进程号**（rank），进程间显性地通过指定秩来进行通信。

通信的基础建立在不同进程间发送和接收操作。一个进程可以通过指定另一个进程的秩以及一个独一无二的消息**标签**来发送消息给另一个进程。接受者可以发送一个接收特定标签标记的消息的请求（或者也可以完全不管标签，接收任何消息），然后依次处理接收到的数据。类似这样的涉及一个发送者以及一个接受者的通信被称作**点对点**（point-to-point）通信。

一个**消息**(message) 指在进程间进行的一次数据交换。在MPI 中，一个消息由通信器、源地址、目的地址、消息标签和数据构成。


## 程序基本结构
mpi.h\
MPI_Init\
MPI_Comm_size\
MPI_Comm_rank\
MPI_Finalized

`MPI_send` 和 `MPI_Recv` 方法使用了 MPI 的数据结构作为一种在更高层次指定消息结构的方法。举例来说，如果一个进程想要发送一个整数给另一个进程，它会指定 count 为 1，数据结构为 `MPI_INT`。其他的 MPI 数据结构以及它们在 C 语言里对应的结构如下：

| MPI datatype | C equivalent |
| --- | --- |
| MPI_SHORT | short int |
| MPI_INT | int |
| MPI_LONG | long int |
| MPI_LONG_LONG | long long int |
| MPI_UNSIGNED_CHAR | unsigned char |
| MPI_UNSIGNED_SHORT | unsigned short int |
| MPI_UNSIGNED | unsigned int |
| MPI_UNSIGNED_LONG | unsigned long int |
| MPI_UNSIGNED_LONG_LONG | unsigned long long int |
| MPI_FLOAT | float |
| MPI_DOUBLE | double |
| MPI_LONG_DOUBLE | long double |
| MPI_BYTE | char |

当我们有了足够多的基础知识之后，你会学习到如何创建自己的 MPI 数据类型来构建更复杂的消息类型。

## 缓冲区
理想情况下，我们的每一个发送操作都会完美的和它的接受操作同步匹配。但是实际上这种情况并不常见。所以在MPI的实现中，必须去处理当两个进程不同步时候的数据存储问题。

MPI的实现（不是MPI标准）决定了当进程不同步时候，对数据如何处理。通常上，用一个缓存区间来暂时存储传输中的数据。 

![buffer_recv]($resource/buffer_recv.gif)


## 点对点通信

MPI 的发送和接收方法是按以下方式进行的：开始的时候，_A_ 进程决定要发送一些消息给 _B_ 进程。A进程就会把需要发送给B进程的所有数据打包好，放到一个缓存里面。因为所有数据会被打包到一个大的信息里面，因此缓存常常会被比作_信封_（就像我们把好多信纸打包到一个信封里面然后再寄去邮局）。数据打包进缓存之后，通信设备（通常是网络）就需要负责把信息传递到正确的地方。这个正确的地方也就是根据特定秩确定的那个进程。

尽管数据已经被送达到 B 了，但是进程 B 依然需要确认它想要接收 A 的数据。一旦它确定了这点，数据就被传输成功了。进程 A 会接收到数据传递成功的信息，然后去干其他事情。

有时候 A 需要传递很多不同的消息给 B。为了让 B 能比较方便地区分不同的消息，MPI 运行发送者和接受者额外地指定一些信息 ID (正式名称是_标签_, _tags_)。当 B 只要求接收某种特定标签的信息的时候，其他的不是这个标签的信息会先被缓存起来，等到 B 需要的时候才会给 B。
### MPI_Send  和 MPI_Recv
把这些概念记在心里的同时，让我们来看一下 MPI 发送和接收方法的定义。

```
MPI_Send(
    void* data,
    int count,
    MPI_Datatype datatype,
    int destination,
    int tag,
    MPI_Comm communicator)

```

```
MPI_Recv(
    void* data,
    int count,
    MPI_Datatype datatype,
    int source,
    int tag,
    MPI_Comm communicator,
    MPI_Status* status)

```



尽管一开始看起来参数有点多，慢慢地你会发现其实这些参数还是很好记忆的，因为大多数的 MPI 方法定义是类似的。第一个参数是数据缓存。第二个和第三个参数分别描述了数据的数量和类型。`MPI_send` 会精确地发送 count 指定的数量个元素，`MPI_Recv` 会**最多**接受 count 个元素（之后会详细讲）。第四个和第五个参数指定了发送方/接受方进程的秩以及信息的标签。第六个参数指定了使用的 communicator。`MPI_Recv` 方法特有的最后一个参数提供了接受到的信息的状态。

### 阻塞与非阻塞

阻塞型函数需要等待指定操作的实际完成，或至少所涉及的数 据已被 MPI 系统安全地备份后才返回。如 MPI_Send 和 MPI_Recv 都是阻塞型的。MPI_Send 函数返回时表明数据已经发出或已被 MPI 系统复制，随后对发送缓冲区的修改不会影响所发送的数据。而 MPI_Recv 返回时，则表明数据已经接收到并且可以立即使用。阻 塞型函数的操作是非局部的，它的完成可能需要与其他进程进行通 信。 


#### 例子：环程序
```
tag = 123; 
dst = (rank > nprocs - 1) ? 0 : rank + 1; 
src = (rank == 0) ? nprocs - 1 : rank - 1; 
MPI_Send(buf1, SIZE, MPI_INT, dst, tag, MPI_COMM_WORLD); 
MPI_Recv(buf2, SIZE, MPI_INT, src, tag, MPI_COMM_WORLD, &status); 
 
MPI_Finalize();
```

非阻塞型函数调用总是立即返回，而实际操作则由 MPI 系统在 后台进行。非阻塞型函数名 MPI_ 前缀之后的第一个字母为 “I”，最 常用的非阻塞型点对点通信函数包括 MPI_Isend 和 MPI_Irecv。在 调用了一个非阻塞型通信函数后，用户必须随后调用其他函数，如 MPI_Wait 或 MPI_Test 等，来等待操作完成或查询操作的完成情况


### MPI Status数据结构
 `MPI_Recv` 这个指令会输出一个 `MPI_Status` 数据结构作为一个参数( 使用`MPI_STATUS_IGNORE`可以忽略).`MPI_Status` 中存储着关于收到消息的一些额外信息, 主要包含以下三条:

1.  **发送者的进程号**`stat.MPI_SOURCE`.
2.  **发送消息的标签**`stat.MPI_TAG`(similar to `MPI_SOURCE`).
3.  **接收消息的长度的长度**. 消息的长度并没有对应的元素存储在`MPI_Status` 中。我们需要调用下面这个函数才行`MPI_Get_count`.

```
MPI_Get_count(
    MPI_Status* status,
    MPI_Datatype datatype,
    int* count)

```



在 `MPI_Get_count`中, 输入 `MPI_Status` 和消息的数据类型`datatype` ，返回值 `count` 是收到的`datatype`类型元素的数量

为什么我们需要这样一个参数呢？因为在`MPI_Recv` 中我们可以使用 `MPI_ANY_SOURCE` 和 `MPI_ANY_TAG`作为通配符表示我们接收任意进程发送来的任意标签信息，这时候我们就不知道我们收到的实际上是那个进程传送过来的那种标签的消息，所以这时候需要我们使用`MPI_Status`把相关信息记录下来。更多的 `MPI_Recv`中的`count` 是接收到的最多数量，并不保证一定是这么多。实际上进程只是收到其他进程发送给它的数量。MPI_Get_count 则是用来确定实际接收的数量。
#### 例子

1. 动态接收




