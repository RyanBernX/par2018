#! /bin/bash

if [[ $# != 1 && $# != 2 ]]; then
  echo "Usage: ${0} src_prefix [dest_prefix]"
  echo "  src_prefix: 输入文件名前缀"
  echo "  dest_prefix: 输出文件名前缀，缺省为 src_prefix"
  echo "将前面的程序产生的一系列有限元函数 Open DX 的文件，产生 dest_prefix.dx 文件将这些文件组合到一起用于显示。"
  exit 1
fi

SRC_PREFIX=$1
tmpfile=`mktemp ${SRC_PREFIX}.XXXXXX`
echo "object \"group\" class multigrid" >${tmpfile}
value=0
for dxi in `ls ${SRC_PREFIX}?*.dx`; do 
  echo -n "${dxi} "
  echo -n "  member \"`basename ${dxi}`\"" >>${tmpfile}
  echo "  value file \"${dxi}\"" >>${tmpfile}
  let "value += 1"
done
echo "done!"

DEST_PREFIX=${SRC_PREFIX}
if [[ $# == 2 ]]; then
  DEST_PREFIX=$2
fi
mv ${tmpfile} ${DEST_PREFIX}.dx

exit 0

