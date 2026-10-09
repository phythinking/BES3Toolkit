#!/bin/bash
# Set the path of dst files of inclusive MC or data
#
# ⚠️  公开版本：真实数据集路径与 Run 区间已略去，完整配置见 /workfs2/bes/yangjunhui/BES3Toolkit
#
# 用法：按 <motherparticle> <year> <bossversion> <action> 的组合，
#       把 DataParentPath 指向该数据集所在的 dst 目录。
#
#   action=inc   →  inclusive MC 的 dst 目录
#   action=data  →  真实数据的 dst 目录
#
# 示例（请换成你自己的路径）：
#   DataParentPath="/path/to/offline/data/<campaign>/jpsi/<round>/mc/dst"
#   DataParentPath="/path/to/offline/data/<campaign>/jpsi/<round>/dst"
#
# 也可以直接用环境变量传入：
#   export DATA_PARENT_PATH=/your/path/to/dst
#
# 注意：原脚本中按母粒子/年份/版本逐条列举的真实路径已移除。

if [ -n "$DATA_PARENT_PATH" ]; then
    DataParentPath="$DATA_PARENT_PATH"
else
    DataParentPath=""
fi

if [ -z "$DataParentPath" ]; then
    echo "DataParentPath is not set."
    echo "Please edit \$TXTS/.datasetpath.sh or export DATA_PARENT_PATH."
    return 1
fi
