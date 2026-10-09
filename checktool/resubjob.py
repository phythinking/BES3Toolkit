#use it to get all the dst in joboption
#author: YangJunhui
import glob
import re

# 使用glob模块列出当前目录中的所有符合条件的文件
file_list = glob.glob('data_gamV_jpsi_2012_*.txt')

# 初始化一个空列表来存储所有的dst文件名
dst_file_list = []

# 遍历每个文件
for file_name in file_list:
    with open(file_name, 'r') as file:
        content = file.read()
        # 使用正则表达式从文件内容中提取EventCnvSvc.digiRootInputFile部分的dst文件
        matches = re.findall(r'EventCnvSvc\.digiRootInputFile\s*=\s*{([^}]*)}', content, re.DOTALL)
        for match in matches:
            dst_files = re.findall(r'"([^"]+\.dst)"', match)
            dst_file_list.extend(dst_files)

# 打印提取的dst文件列表
# for dst_file in dst_file_list:
#     print(dst_file)

# 打开一个名为temp.txt的文件以写入模式
with open('temp.txt', 'w') as output_file:
    # 遍历提取的dst文件列表并将每个文件名写入文件中
    for dst_file in dst_file_list:
        output_file.write(dst_file + '\n')

# 打印成功保存的消息
print("提取的dst文件列表已保存到temp.txt文件中。")
