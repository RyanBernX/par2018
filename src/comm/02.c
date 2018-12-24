/**
 * 本例子来自于 MPICH 的文档。
 * 
 * 我们使用通讯器的附加属性，来使得一个聚合通讯操作在第一次调用以后能
 * 够更加高效。代码风格假设 MPI 函数的返回结果仅仅有错误状态的信息。
 * 
 */

/**
 * 本模块的静态变量
 */ 
static int gop_key = MPI_KEYVAL_INVALID; 

typedef struct { 
  int ref_count; /**< reference count */ 
  ...            /**< other stuff, whatever else we want */ 
} gop_stuff_type;

Efficient_Collective_Op (comm, ...) 
     MPI_Comm comm; 
{ 
  gop_stuff_type *gop_stuff; 
  MPI_Group group; 
  int foundflag;

  MPI_Comm_group(comm, &group);

  if (gop_key == MPI_KEYVAL_INVALID) { /**
					* 第一次调用
					*/ 
    if (! MPI_keyval_create(gop_stuff_copier, 
			    gop_stuff_destructor, 
			    &gop_key, 
			    (void *)0)) /**
					 * 产生这个属性
					 */
      MPI_Abort (comm, 99); 
  }

  MPI_Attr_get (comm, gop_key, &gop_stuff, &foundflag); 
  if (foundflag) { /**
		    * 这个模块以前调用过，我们使用以前缓冲下来的信息
		    */ 
  } 
  else { /**
	  * 这个模块以前没有调用过，我们向缓冲中写入信息
	  */
    /**
     * 首先分配空间，并设置引用数
     */
    gop_stuff = (gop_stuff_type *) malloc (sizeof(gop_stuff_type)); 
    if (gop_stuff == NULL) { /**
			      * 内存分配失败
			      */
    }

    gop_stuff -> ref_count = 1;

    /**
     * 其次在这块内存中填充信息
     */
    ......

    /**
     * 然后将这块内存设置为属性的值
     */ 
    MPI_Attr_put ( comm, gop_key, gop_stuff); 
  }

  /**
   * 然后，使用缓冲区 *gop_stuff 中的值来进行这次聚合操作
   */ 
  ... ...
}

/**
 * 在进程组被释放的时候调用的本程序释放内存
 */
gop_stuff_destructor (comm, keyval, gop_stuff, extra) 
     MPI_Comm comm; 
     int keyval; 
     gop_stuff_type *gop_stuff; 
     void *extra; 
{ 
  if (keyval != gop_key) { /**
			    * 出了问题
			    */ 
  }

  /**
   * 将应用数减去 1 
   */ 
  gop_stuff -> ref_count -= 1;

  /**
   * 如果应用数为 0 了，就将内存释放掉
   */ 
  if (gop_stuff -> ref_count == 0) { 
    free((void *)gop_stuff); 
  } 
}

/**
 * 在进程组被复制的时候的回掉函数
 */ 
gop_stuff_copier (comm, keyval, extra, gop_stuff_in, gop_stuff_out, flag) 
     MPI_Comm comm; 
     int keyval; 
     gop_stuff_type *gop_stuff_in;
     gop_stuff_type *gop_stuff_out; 
     void *extra; 
     int *flag;
{ 
  if (keyval != gop_key) { /**
			    * 出了问题
			    */ 
  }

  /**
   * 将引用数加上 1
   */ 
  gop_stuff -> ref_count += 1; 
  gop_stuff_out = gop_stuff_in; 
} 

/**
 * end of file
 * 
 */
