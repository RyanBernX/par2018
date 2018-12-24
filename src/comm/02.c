/**
 * ������������ MPICH ���ĵ���
 * 
 * ����ʹ��ͨѶ���ĸ������ԣ���ʹ��һ���ۺ�ͨѶ�����ڵ�һ�ε����Ժ���
 * �����Ӹ�Ч����������� MPI �����ķ��ؽ�������д���״̬����Ϣ��
 * 
 */

/**
 * ��ģ��ľ�̬����
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
					* ��һ�ε���
					*/ 
    if (! MPI_keyval_create(gop_stuff_copier, 
			    gop_stuff_destructor, 
			    &gop_key, 
			    (void *)0)) /**
					 * �����������
					 */
      MPI_Abort (comm, 99); 
  }

  MPI_Attr_get (comm, gop_key, &gop_stuff, &foundflag); 
  if (foundflag) { /**
		    * ���ģ����ǰ���ù�������ʹ����ǰ������������Ϣ
		    */ 
  } 
  else { /**
	  * ���ģ����ǰû�е��ù��������򻺳���д����Ϣ
	  */
    /**
     * ���ȷ���ռ䣬������������
     */
    gop_stuff = (gop_stuff_type *) malloc (sizeof(gop_stuff_type)); 
    if (gop_stuff == NULL) { /**
			      * �ڴ����ʧ��
			      */
    }

    gop_stuff -> ref_count = 1;

    /**
     * ���������ڴ��������Ϣ
     */
    ......

    /**
     * Ȼ������ڴ�����Ϊ���Ե�ֵ
     */ 
    MPI_Attr_put ( comm, gop_key, gop_stuff); 
  }

  /**
   * Ȼ��ʹ�û����� *gop_stuff �е�ֵ��������ξۺϲ���
   */ 
  ... ...
}

/**
 * �ڽ����鱻�ͷŵ�ʱ����õı������ͷ��ڴ�
 */
gop_stuff_destructor (comm, keyval, gop_stuff, extra) 
     MPI_Comm comm; 
     int keyval; 
     gop_stuff_type *gop_stuff; 
     void *extra; 
{ 
  if (keyval != gop_key) { /**
			    * ��������
			    */ 
  }

  /**
   * ��Ӧ������ȥ 1 
   */ 
  gop_stuff -> ref_count -= 1;

  /**
   * ���Ӧ����Ϊ 0 �ˣ��ͽ��ڴ��ͷŵ�
   */ 
  if (gop_stuff -> ref_count == 0) { 
    free((void *)gop_stuff); 
  } 
}

/**
 * �ڽ����鱻���Ƶ�ʱ��Ļص�����
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
			    * ��������
			    */ 
  }

  /**
   * ������������ 1
   */ 
  gop_stuff -> ref_count += 1; 
  gop_stuff_out = gop_stuff_in; 
} 

/**
 * end of file
 * 
 */
