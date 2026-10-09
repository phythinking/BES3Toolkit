import os
import glob
import argparse
import shutil
import subprocess
import threading
import concurrent.futures

# ⚠️  公开版本：真实数据集路径与 Run 区间已略去，完整配置见 /workfs2/bes/yangjunhui/BES3Toolkit
# 各能量点的 dst 目录统一以 DATA_ROOT 为根。请设置环境变量 DATA_ROOT，
# 或把下面的默认值改成你自己的数据集根目录。
DATA_ROOT = os.environ.get("DATA_ROOT", "/path/to/offline/data")

def thread_function(directorylist, dstfile, jobnumbers, original_dir):
    """Thread function to generate job scripts and store job number."""
    target_dir = os.path.join(original_dir, directorylist)
    os.chdir(target_dir)

    number = generate_job_scripts(dstfile, directorylist)
    jobnumbers.append(number)

def generate_directories(ecmlist, runfrom, runto, files_to_copy, algname):
    """
    Generate directories for each name in the ecmlist and copy specified files into them,
    also generate custom job.head and job.foot files based on templates.

    :param ecmlist: List of names or lists of names for which to create directories
    :param runfrom: List of run numbers corresponding to each directory
    :param runto: List of run numbers corresponding to each directory
    :param files_to_copy: List of file names or lists of file names to copy into each created directory
    """
    for i, item in enumerate(ecmlist):
        names = item if isinstance(item, list) else [item]
        
        for j, name in enumerate(names):
            # Create a directory for this name
            dir_name = f"{name}"
            os.makedirs(dir_name, exist_ok=True)

            # Modify and copy files for each name
            for file_path in files_to_copy:
                file_name = os.path.basename(file_path)
                dst = os.path.join(dir_name, file_name)

                # Check if the file is a .dec file and needs to be modified
                if file_name == 'job.head':
                    if os.path.exists(file_path):
                        with open(file_path, "r") as src_file, open(dst, "w") as dst_file:
                            content = src_file.read().replace("Runbegin", str(runfrom[i][j] if isinstance(runfrom[i], list) else runfrom[i]))\
                                                     .replace("Runend", str(runto[i][j] if isinstance(runto[i], list) else runto[i]))\
                                                     .replace("mode", str(algname))
                            dst_file.write(content)
                    else:
                        print(f"job.head template file {file_path} does not exist. It has not been copied.")
                elif file_name == 'job.foot':
                    if os.path.exists(file_path):
                        # Special handling for msim.txt
                        with open(file_path, "r") as src_file, open(dst, "w") as dst_file:
                            content = src_file.read().replace("DIR", f"{name}")
                            dst_file.write(content)
                    else:
                        print(f"job.foot template file {file_path} does not exist. It has not been copied.")
                else:
                    # Copy other files without modification
                    if os.path.exists(file_path):
                        shutil.copy(file_path, dst)
                    else:
                        print(f"File {file_path} does not exist. It has not been copied.")

def remove_files_in_directories(directories):
    """Remove specified files in given directories."""
    if not isinstance(directories, list):
        directories = [directories]

    for directory in directories:
        os.system(f"rm {directory}/data-eeTo3phi-{directory}-*")

def create_submission_script(jobfilename, job_amount, number):
    """Create a master submission script."""
    if not isinstance(jobfilename, list):
        jobfilename = [jobfilename]

    if not isinstance(job_amount, list):
        job_amount = [job_amount]

    with open(f'sub-{number}.sh', 'a') as sub_file:
        for i in range(0, len(jobfilename)):
            if job_amount[i] == 1:
                sub_file.write(f"cd {jobfilename[i]}\nboss.condor data-eeTo3phi-{jobfilename[i]}-0\ncd ..\n")
            else:
                sub_file.write(f"cd {jobfilename[i]}\nboss.condor -n {job_amount[i]} data-eeTo3phi-{jobfilename[i]}-%{{ProcId}}\ncd ..\n")

def generate_job_scripts(directories, dirfilename, nDataFile=50):
    """Generate job options by searching for .dst files in given directories."""
    data_files = []

    jobfilename = "data-eeTo3phi-" + dirfilename

    if not isinstance(directories, list):
        directories = [directories]

    for directory in directories:
        # Search for .dst files in the directory
        data_files.extend(glob.glob(os.path.join(directory, '*/*.dst')))

    file_number = len(data_files)
    i_job = 0
    n = 0

    for rec_file in data_files:
        m = n + 1
        if not os.path.exists(rec_file):
            print(f"This data file {rec_file} doesn't exist!")
            continue  # Continue with next file instead of returning None

        line = f'"{rec_file}"'

        with open("job.foot", "r") as foot_file:
            footcontent = foot_file.read().replace("8000", str(i_job))
        with open("job.head", "r") as head_file:
            headcontent = head_file.read()

        if n % nDataFile == 0:
            if os.path.exists(f"{jobfilename}-{i_job}"):
                with open(f"{jobfilename}-{i_job}", "a") as job_file:
                    job_file.write(footcontent)
                    i_job += 1
            with open(f"{jobfilename}-{i_job}", "w") as job_file:
                job_file.write(headcontent)

        if m % nDataFile == 0 or m == file_number:
            with open(f"{jobfilename}-{i_job}", "a") as job_file:
                job_file.write(f"{line}\n")
                n += 1
        else:
            with open(f"{jobfilename}-{i_job}", "a") as job_file:
                job_file.write(f"{line},\n")
                n += 1

    with open(f"{jobfilename}-{i_job}", "a") as job_file:
        job_file.write(footcontent)

    return (i_job+1)

def main():
    parser = argparse.ArgumentParser(description="A script to generate data root joboption scripts.")
    parser.add_argument("--createdir", action="store_true", help="Create the sub ecm directories")
    parser.add_argument("--start", action="store_true", help="Generate root joboption scripts")
    parser.add_argument("--startonedir", action="store_true", help="Generate root joboption scripts in only one dir, need input the dir and dstfilelist")
    parser.add_argument("--cleanup", action="store_true", help="Clean up existing files")
    args = parser.parse_args()

    ecmlist703 = ['chic1', '3872', '3810', '3900', '4009', '4090', '4180', '4190scan', '4190', '4200', '4210scan', '4210', '4220scan', '4220', '4230scan', '4230', '4237', '4245', '4246', '4260', '4270', '4280', '4310', '4360', '4390', '4420scan', '4420', '4470', '4530', '4575', '4600']
    # ⚠️  公开版本：真实 Run 区间已略去，完整配置见 /workfs2/bes/yangjunhui/BES3Toolkit
    # 每个能量点对应一个 run 号，请填入你自己的值（或改回写死的列表）。
    runFrom703 = ['RUN_BEGIN'] * len(ecmlist703)
    runTo703 = ['RUN_END'] * len(ecmlist703)
    dstfilelist703 = [f'{DATA_ROOT}/chic1/dst',
        f'{DATA_ROOT}/3872/dst',
        f'{DATA_ROOT}/3810/dst',
        f'{DATA_ROOT}/3900/dst',
        f'{DATA_ROOT}/4040/dst',
        f'{DATA_ROOT}/4090/dst',
        f'{DATA_ROOT}/4180/dst',
        f'{DATA_ROOT}/4190/dst',
        f'{DATA_ROOT}/4190/dst',
        f'{DATA_ROOT}/4200/dst',
        f'{DATA_ROOT}/4210/dst',
        f'{DATA_ROOT}/4210/dst',
        f'{DATA_ROOT}/4220/dst',
        f'{DATA_ROOT}/4220/dst',
        f'{DATA_ROOT}/4230/dst',
        f'{DATA_ROOT}/4230/dst',
        f'{DATA_ROOT}/4237/dst',
        f'{DATA_ROOT}/4245/dst',
        f'{DATA_ROOT}/4246/dst',
        f'{DATA_ROOT}/4260/dst',
        f'{DATA_ROOT}/4270/dst',
        f'{DATA_ROOT}/4280/dst',
        f'{DATA_ROOT}/4310/dst',
        f'{DATA_ROOT}/4360/dst',
        f'{DATA_ROOT}/4390/dst',
        f'{DATA_ROOT}/4420/dst',
        f'{DATA_ROOT}/4420/dst',
        f'{DATA_ROOT}/4470/dst',
        f'{DATA_ROOT}/4530/dst',
        f'{DATA_ROOT}/4575/dst',
        f'{DATA_ROOT}/4600/dst']

    ecmlist705 = ['4130', '4160', '4290', '4315', '4340', '4380', '4400', '4440']
    # ⚠️  公开版本：真实 Run 区间已略去，完整配置见 /workfs2/bes/yangjunhui/BES3Toolkit
    # 每个能量点对应一个 run 号，请填入你自己的值（或改回写死的列表）。
    runFrom705 = ['RUN_BEGIN'] * len(ecmlist705)
    runTo705 = ['RUN_END'] * len(ecmlist705)
    dstfilelist705 = [
        f'{DATA_ROOT}/4130/dst',
        f'{DATA_ROOT}/4160/dst',
        f'{DATA_ROOT}/4290/dst',
        f'{DATA_ROOT}/4315/dst',
        f'{DATA_ROOT}/4340/dst',
        f'{DATA_ROOT}/4380/dst',
        f'{DATA_ROOT}/4400/dst',
        f'{DATA_ROOT}/4440/dst',
        ]

    ecmlist706 = ['4610', '4620', '4640', '4660', '4680', '4700']
    # ⚠️  公开版本：真实 Run 区间已略去，完整配置见 /workfs2/bes/yangjunhui/BES3Toolkit
    # 每个能量点对应一个 run 号，请填入你自己的值（或改回写死的列表）。
    runFrom706 = ['RUN_BEGIN'] * len(ecmlist706)
    runTo706 = ['RUN_END'] * len(ecmlist706)
    dstfilelist706 = [
        f'{DATA_ROOT}/4612/dst',
        f'{DATA_ROOT}/4620/dst',
        f'{DATA_ROOT}/4640/dst',
        f'{DATA_ROOT}/4660/dst',
        f'{DATA_ROOT}/4680/dst',
        f'{DATA_ROOT}/4700/dst',
        ]

    ecmlist707 = ['4740', '4750', '4780', '4840', '4914', '4946']
    # ⚠️  公开版本：真实 Run 区间已略去，完整配置见 /workfs2/bes/yangjunhui/BES3Toolkit
    # 每个能量点对应一个 run 号，请填入你自己的值（或改回写死的列表）。
    runFrom707 = ['RUN_BEGIN'] * len(ecmlist707)
    runTo707 = ['RUN_END'] * len(ecmlist707)
    dstfilelist707 = [
        f'{DATA_ROOT}/4740/dst',
        f'{DATA_ROOT}/4750/dst',
        f'{DATA_ROOT}/4780/dst',
        f'{DATA_ROOT}/4840/dst',
        f'{DATA_ROOT}/4914/dst',
        f'{DATA_ROOT}/4946/dst',
        ]

    ecmlist = []
    runFrom = []
    runTo = []
    dstfilelist = []

    ecmlist.append(ecmlist703)
    ecmlist.append(ecmlist705)
    ecmlist.append(ecmlist706)
    ecmlist.append(ecmlist707)

    runFrom.append(runFrom703)
    runFrom.append(runFrom705)
    runFrom.append(runFrom706)
    runFrom.append(runFrom707)

    runTo.append(runTo703)
    runTo.append(runTo705)
    runTo.append(runTo706)
    runTo.append(runTo707)

    dstfilelist.append(dstfilelist703)
    dstfilelist.append(dstfilelist705)
    dstfilelist.append(dstfilelist706)
    dstfilelist.append(dstfilelist707)

    if not any(vars(args).values()):
        parser.print_help()
        return

    if args.cleanup:
        for i in range(0, len(ecmlist)):
            remove_files_in_directories(ecmlist[i])

    if args.start:
        # for i in range(0, len(ecmlist)):
        #     remove_files_in_directories(ecmlist[i])

        # num_jobs = int(input("Enter the number of Jobs: "))

        original_dir = os.getcwd()
        bossversion = [703, 705, 706, 707]
        os.system(f'rm sub*')

        for i in range(0, len(ecmlist)):
            jobnumbers = []
            threads = []
            for j in range(0, len(ecmlist[i])):
                target_dir = os.path.join(original_dir, ecmlist[i][j])
                os.chdir(target_dir)
                number = generate_job_scripts(dstfilelist[i][j], ecmlist[i][j])
                jobnumbers.append(number)
                os.chdir(original_dir)

            create_submission_script(ecmlist[i], jobnumbers, bossversion[i])

        # for i, ecm_dir in enumerate(ecmlist):
        #     job_numbers = []
        #     with concurrent.futures.ThreadPoolExecutor() as executor:
        #         # Submit jobs to the executor
        #         futures = [executor.submit(thread_function, dir, dir) for dir in ecm_dir]
                
        #         # Retrieve results as they are completed
        #         for future in concurrent.futures.as_completed(futures):
        #             job_numbers.append(future.result())


    # if args.startonedir:

    #     original_dir = os.getcwd()
    #     bossversion = [703, 705, 706, 707]

    #     for i in range(0, len(ecmlist)):
    #         jobnumbers = []
    #         threads = []
    #         for j in range(0, len(ecmlist[i])):
    #             target_dir = os.path.join(original_dir, ecmlist[i][j])
    #             os.chdir(target_dir)
    #             number = generate_job_scripts(dstfilelist[i][j], ecmlist[i][j])
    #             jobnumbers.append(number)
    #             os.chdir(original_dir)

    #         create_submission_script(ecmlist[i], jobnumbers, bossversion[i])

    # if args.createsubmit:
    #     for i in range(0, len(ecmlist)):
    #         create_submission_script(ecmlist[i], jobnumbers, bossversion[i])


    if args.createdir:
        files_to_copy = ['job.head', 'job.foot', 'script.py']
        generate_directories(ecmlist, runFrom, runTo, files_to_copy, 'eeTo3phi')

if __name__ == "__main__":
    main()

