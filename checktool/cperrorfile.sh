#!/bin/bash
#author: YangJunhui
#use it to read the list of files and cp them to another dir
DESTINATION_DIR="./again"

if [ ! -d "$DESTINATION_DIR" ]; then
  mkdir -p "$DESTINATION_DIR"
fi

# 读取 errorfile.txt 中的文件名列表并复制到目标文件夹
while IFS= read -r filename; do
  if [ -e "$filename" ]; then
    cp "$filename" "$DESTINATION_DIR/"
    echo "Copied $filename to $DESTINATION_DIR/"
  else
    echo "File $filename not found."
  fi
done < "errorfile.txt"
