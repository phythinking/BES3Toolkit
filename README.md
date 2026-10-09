# BES3Toolkit

> 一套面向 **BESIII（BES3）实验** 离线数据分析的自动化工具箱。
> 覆盖「安装 BOSS → 派生分析算法包 → 批量生产/提交作业 → 查错重投 → 绘图拟合 → cut 优化」的完整链路。

![BOSS](https://img.shields.io/badge/BOSS-6.6.4%20%7C%207.0.8%20%7C%207.0.9%20%7C%207.1.0-blue)
![Platform](https://img.shields.io/badge/platform-IHEP%20lxlogin%20%2F%20AFS%20%2F%20CVMFS-lightgrey)
![Shell](https://img.shields.io/badge/shell-bash%20%7C%20tcsh-green)
![Python](https://img.shields.io/badge/python-3.x-yellow)

---

> ## 📦 关于本公开版本
>
> 本仓库为**公开发布版**。为遵守 BESIII 合作组的数据与代码发布规范，以下信息已被**有意移除**，
> 并在代码中统一替换为占位符（`RUN_BEGIN` / `RUN_END` / `DATA_ROOT` / `/path/to/...`）：
>
> | 已略去 | 替换为 | 涉及文件 |
> |---|---|---|
> | **数据集路径**（`/besfs*/offline/data/...` 的实际目录布局） | `DATA_ROOT` 环境变量 + `/path/to/...` | `.datasetpath.sh`、`script.py`、`makejob.sh`、`Example.cfg`、`fitkit/*.cxx` |
> | **Run 区间**（各数据集对应的真实 run 号） | `RUN_BEGIN` / `RUN_END` | `SetBOSSVersion.sh`、`job.head`、`generate.sh`、`script.py` |
>
> **能量点（E_cm）、截面数值等属于公开信息，予以完整保留。**
>
> 🔑 **需要完整配置（含真实数据集路径与 run 号）的合作组成员，请从
> `/workfs2/bes/yangjunhui/BES3Toolkit` 获取。**
>
> 因此，本仓库中的作业脚本**开箱不能直接提交**，需要先按
> [使用前必须修改的配置](#使用前必须修改的配置) 填入你自己的路径与 run 号。

---

## 目录

- [简介](#简介)
- [功能总览](#功能总览)
- [目录结构](#目录结构)
- [环境要求](#环境要求)
- [快速开始](#快速开始)
- [模块详解](#模块详解)
  - [1. setupBOSSyjh —— BOSS 环境安装](#1-setupbossyjh--boss-环境安装)
  - [2. creatAlgyjh —— 派生分析算法包](#2-creatalgyjh--派生分析算法包)
  - [3. SubmitJobyjh —— 作业机器人](#3-submitjobyjh--作业机器人)
  - [4. iRobot —— WALL·E（Python 版）](#4-irobot--wallepython-版)
  - [5. checktool —— 作业体检与重投](#5-checktool--作业体检与重投)
  - [6. DrawTool —— 绘图](#6-drawtool--绘图)
  - [7. fitkit —— 拟合](#7-fitkit--拟合)
  - [8. chi2FoM —— χ² 扫描与 cut 优化](#8-chi2fom--χ²-扫描与-cut-优化)
  - [9. inclusive / xiechen —— 作业模板](#9-inclusive--xiechen--作业模板)
  - [10. template_topoana.card —— TopoAna 配置](#10-template_topoanacard--topoana-配置)
- [使用前必须修改的配置](#使用前必须修改的配置)
- [依赖](#依赖)
- [已验证 / 未验证](#已验证--未验证)
- [已知问题与后续计划](#已知问题与后续计划)
- [致谢](#致谢)
- [License 与免责声明](#license-与免责声明)

---

## 简介

`BES3Toolkit` 是在 BESIII 离线软件 **BOSS**（BESIII Offline Software System）之上搭建的一层自动化脚本与宏工具，源自我在 ψ(3686)、ψ(3770)、J/ψ 等数据上做分波分析与截面测量时的日常工作流。

它解决的核心痛点是 **BESIII 作业的"体力活"**：

- 手工在 `cmthome` / `work.<version>` 里改 `requirements`、装 BOSS，容易出错且难以复现；
- 从模板派生一个新分析算法包（几百处类名、包名、文件名的替换）非常繁琐；
- 一个分析动辄要跑几十到上千个作业，作业卡生成、日志检查、失败重投全是重复劳动；
- inclusive MC 需要在十几个能量点上重复同一套流程。

因此本工具箱把这些流程封装成了几个可复用的脚本与命令行工具。

### 三个"机器人"的关系

工具箱里有三代同源的工具，命名都来自皮克斯动画《WALL·E》：

| 名字 | 位置 | 时代 | 状态 |
|---|---|---|---|
| **jarvis** | `SubmitJobyjh/.Robot_1.00.sh` | 2023 | ✅ 当前主力（Bash） |
| **WALL·E** | `iRobot/` | 2020–2022 | ⚠️ 已被 Bash 版取代，存档 |
| **EVE** | 无代码模块 | — | 代码里指"下命令的人"（即用户） |

> 代码中写明：`WALL·E = Waste Allocation Load Lifter Earth-Class`（工具集自身），`EVE = Extraterrestrial Vegetation Evaluator`（使用者）。`WALLE.py` 打招呼时会问 *"Where is EVE?"*

> **代码血统**：`iRobot/` 与 `chi2FoM/chi2.C` 的原始框架来自 **Dr. ZHOU Xiang（周翔，武汉大学）**；`bes3plotstyle.{C,h}` 来自 BESIII 官方绘图风格脚本（Niklaus Berger）；`setupBOSSyjh`、`creatAlgyjh`、`SubmitJobyjh`、`checktool`、`fitkit`、`inclusive` 为在此基础上改写、重写或新写的部分。详见 [致谢](#致谢)。

---

## 功能总览

| 模块 | 语言 | 一句话说明 | 入口 |
|---|---|---|---|
| [`setupBOSSyjh/`](setupBOSSyjh/) | Bash | 一键安装并配置 BOSS（CMT + TestRelease + `boss<ver>` 启动脚本） | `./setupBOSS <ver>` |
| [`creatAlgyjh/`](creatalgyjh/) | Bash | 从已有算法包派生新的分析算法包，自动改名并编译 | `./creatAlg ...` |
| [`SubmitJobyjh/`](SubmitJobyjh/) | Bash | 批量生成 + 提交 sim/rec/ana/inc/data 作业，带状态机防重复提交 | `./iRobot` |
| [`iRobot/`](iRobot/) | Python 3 | 上一代的 Python 面向对象重写版（环境检测 / 装 BOSS / 建包 / 编译） | `python3 apps/WALLE_*.py` |
| [`checktool/`](checktool/) | Bash / C++ | 作业日志体检、失败作业重投、错误文件收集、事例数汇总 | `bash *.sh` |
| [`DrawTool/`](DrawTool/) | ROOT C++ | 绘图宏 + BESIII 官方绘图风格（`bes3plotstyle`） | ROOT 宏 |
| [`fitkit/`](fitkit/) | ROOT / RooFit | 一维、二维、无 bin 拟合模板 | ROOT 宏 |
| [`chi2FoM/`](chi2FoM/) | C++ / ROOT | 基于 χ² 扫描的 cut 优化与 Figure-of-Merit 评估 | `./chi2FoM cfg` |
| [`inclusive/`](inclusive/) | Bash | 12 个能量点的 inclusive MC 分析批量生产模板 | `./generate.sh` |
| [`xiechen/`](xiechen/) | Bash / Python | 信号 MC（3φ 能量扫描）与 inclusive 作业模板 | `python3 script.py` |
| [`template_topoana.card`](template_topoana.card) | — | TopoAna 拓扑分析配置文件模板 | 配置文件 |

---

## 目录结构

```
BES3Toolkit/
├── setupBOSSyjh/                      # ① BOSS 环境安装
│   ├── setupBOSS -> .installBOSS_1.00.sh    ← 入口（符号链接）
│   ├── .installBOSS_1.00.sh           #    主脚本
│   ├── backup.sh                      #    基于 sed -i 的另一种实现（备份用）
│   ├── .scripts/                      #    SetColors / welcome / disableBOSS ...
│   └── txts/boss                      #    boss<ver> 环境激活脚本模板
│
├── creatAlgyjh/                       # ② 分析算法包派生
│   ├── creatAlg -> .creatAlg_1.02.sh        ← 入口（符号链接）
│   ├── .creatAlg_1.02.sh              #    主脚本（最新）
│   ├── .creatAlg_1.01.sh / _1.00.sh   #    历史版本
│   └── .scripts/                      #    公共函数
│
├── SubmitJobyjh/                      # ③ 作业机器人（Bash）
│   ├── iRobot -> .Robot_1.00.sh             ← 入口（符号链接，注意与 ../iRobot/ 同名）
│   ├── .Robot_1.00.sh                 #    主脚本（内部自称 jarvis）
│   ├── ReadMe                         #    原始简短说明
│   ├── .scripts/                      #    作业生成与提交的核心逻辑
│   ├── txts/                          #    用户配置 + sim/rec/ana jobOption 模板
│   └── script.py                      #    作业脚本生成器（实验性）
│
├── iRobot/                            # ④ WALL·E（Python 3，存档）
│   ├── classes/                       #    Environments / Preferences / Toolkit / SetupBOSS ...
│   ├── apps/                          #    各功能的 CLI 入口
│   ├── runcoms/                       #    boss / tcshrc / vimrc 运行环境模板
│   ├── images/                        #    WALL·E / EVE 图标
│   └── misc/                          #    备份
│
├── checktool/                         # ⑤ 作业体检
│   ├── checkjoberr.sh                 #    扫描 *.bosslog，列出未成功完成的作业
│   ├── cperrorfile.sh                 #    把 errorfile.txt 中的日志集中拷贝到 ./again/
│   ├── resubjob.py / resubjob.sh      #    把剩余 dst 重新均分生成 jobOptions
│   ├── countnumber.sh                 #    汇总各 cut 阶段的事例数
│   └── extract.cxx                    #    ROOT 宏：按 cut 从 TTree 抽取子样本
│
├── DrawTool/                          # ⑥ 绘图
│   ├── draw.C / draw2D.C              #    一维 / 二维分布绘制示例
│   ├── extract.cxx                    #    按表达式裁剪 TTree
│   ├── bes3plotstyle.{C,h}            #    BESIII 官方绘图风格
│   └── drawtool/                      #    更完整的绘图框架（需外部 ana.C）
│
├── fitkit/                            # ⑦ 拟合
│   ├── fit_1D.cxx / fit_2D.cxx        #    一维 / 二维拟合
│   ├── testfit.cxx                    #    最小可运行的一维三分量拟合样例
│   ├── fit2D_test.cxx                 #    二维非分 bin（最新最完整）
│   ├── D_fit_unbin.cxx                #    双 D mBC 二维无 bin 拟合 ⚠️ 缺头文件
│   └── 2D_sim_fit.cxx                 #    二维同时拟合 ⚠️ 缺头文件
│
├── chi2FoM/                           # ⑧ χ² 扫描与 cut 优化
│   ├── chi2.C                         #    主程序源码
│   ├── config.{cc,hh}                 #    通用配置文件解析器
│   ├── userDefined.h                  #    工具函数
│   ├── Example.cfg                    #    配置文件示例
│   ├── compile.sh                     #    g++ 编译脚本
│   ├── chi2                           #    ⚠️ 预编译二进制（不建议入库）
│   └── chi2FoM                        #    tcsh 包装脚本
│
├── inclusive/                         # ⑨ inclusive MC 模板（12 个能量点）
│   ├── generate.sh                    #    总调度：遍历所有能量点
│   └── 4130 … 4230-scan/              #    每点一套 MC_01..MC_40 / mc1..mc22
│
├── xiechen/                           # ⑩ 信号 MC 与 inclusive 模板
│   ├── signal/                        #    3φ 能量扫描信号 MC（script.py + expect）
│   └── inclusive/                     #    ⚠️ 与 inclusive/ 内容完全一致（历史副本）
│
└── template_topoana.card              # ⑪ TopoAna 拓扑分析配置模板
```

---

## 环境要求

本工具箱**不是**开箱即用的通用软件，它假设你身处 IHEP 计算环境（或等价环境）。

### 硬性要求

| 项目 | 要求 |
|---|---|
| 账号 | IHEP 集群账号（`lxlogin.ihep.ac.cn` 系列），AFS 家目录 `$HOME=/afs/ihep.ac.cn/users/<首字母>/<用户名>` |
| 文件系统 | `/workfs2/bes/$USER`、`/scratchfs/bes/$USER`、`/besfs5/users/$USER` 可写 |
| CVMFS | `/cvmfs/bes3.ihep.ac.cn/` 已挂载（提供 BOSS 与 cmthome） |
| 作业系统 | `hep_sub` / `hep_q`（`/afs/ihep.ac.cn/soft/common/sysgroup/hep_job/bin`）、`boss.condor`（HTCondor 的 BOSS 包装） |
| Shell | 主脚本为 **bash**；`chi2FoM` 包装脚本与 `iRobot/runcoms/tcshrc` 为 **tcsh** |
| 编译器 | `g++ >= 4.6.3`（`boss700` 之后的环境满足） |
| Python | **Python 3.6+**（仅 `iRobot/`、`script.py`、`resubjob.py` 需要）。⚠️ `iRobot` 用 `distutils`，Python 3.12 起已移除 |
| ROOT | 通过 BOSS 环境提供（`chi2FoM` 需要 `root-config`、`libCint.so`） |
| `expect` | 仅 `xiechen/signal/auto.exp` 需要 |

### 支持的 BOSS 版本

脚本中显式引用过的版本：`6.6.4`、`7.0.8`、`7.0.9`、`7.1.0`（`chi2FoM` 包装脚本另有 `7.0.0`）。

母粒子（`motherparticle`）与 BOSS 版本的对应关系写死在 [`SubmitJobyjh/.scripts/SetBOSSVersion.sh`](SubmitJobyjh/.scripts/SetBOSSVersion.sh)：

| `motherparticle` | 质心能量 | BOSS 版本 | 可用年份 |
|---|---|---|---|
| `jpsi` | 3097 MeV | 7.0.8 | 2009 / 2012 / 2018 / 2019 |
| `psip` | 3686 MeV | 7.0.9 | 2009 / 2012 / 2021 |
| `psipp` | 3770 MeV | 7.1.0 | 2009 / 2012 / 2018 |
| `cms3650` | 3650 MeV | 7.0.9 | round02 / 06 / 15 |
| `cms3682` | 3682 MeV | 7.0.9 | round15 |
| `cms3080` | 3080 MeV | 7.0.8 | round02 / 05 / 11 / 12 |
| `bhabha` | — | 6.6.4 | 2009 / 2012 |

> 📌 各年份对应的 **run 号区间**（`RunIdList`）已在本公开版中略去，见
> [使用前必须修改的配置](#使用前必须修改的配置)。

---

## 快速开始

> ⚠️ **第一次使用前请务必先读 [使用前必须修改的配置](#使用前必须修改的配置)**，脚本中存在大量指向作者个人目录的硬编码路径。

### 0. 获取代码

```bash
cd /workfs2/bes/$USER
git clone https://github.com/<your-account>/BES3Toolkit.git
export BES3TOOLKIT=/workfs2/bes/$USER/BES3Toolkit
```

### 1. 安装 BOSS

```bash
mkdir -p ~/setupBOSS && cp -r $BES3TOOLKIT/setupBOSSyjh/. ~/setupBOSS/
vi ~/setupBOSS/.installBOSS_1.00.sh        # 修改 setupBOSSWD 为 ~/setupBOSS
cd ~/setupBOSS
source setupBOSS 7.0.8                      # 交互式安装
source setupBOSS 7.0.8 -silent              # 静默安装
```

安装完成后，家目录下会生成 `~/bin/bossbin/boss708`，用它可以随时激活 BOSS：

```bash
source ~/bin/bossbin/boss708
```

### 2. 派生一个分析算法包

```bash
cd $BES3TOOLKIT/creatAlgyjh
source creatAlg <算法包所在目录> <已有算法名> <BOSS版本> [版本号] [新算法名]

# 例：从 DsSTDsAlg 派生一个叫 PPKPiAlg 的包
source creatAlg ~/myalgs DsSTDsAlg 7.0.8 00-00-01 PPKPiAlg
```

它会：`cmt create` 新包 → 拷贝模板源码 → 全局替换类名/包名 → 在 `TestRelease/cmt/requirements` 中注册 → `cmt config && cmt make`。

### 3. 批量提交作业

在一个**工作目录**里准备 `.scripts/`、`txts/` 和入口软链接：

```bash
mkdir -p ~/work/psip && cd ~/work/psip
ln -s $BES3TOOLKIT/SubmitJobyjh/.scripts        .scripts
ln -s $BES3TOOLKIT/SubmitJobyjh/txts            txts
ln -s $BES3TOOLKIT/SubmitJobyjh/.Robot_1.00.sh  iRobot

# 编辑配置
vi txts/motherparticle.sh     # 母粒子：jpsi / psip / psipp / ...
vi txts/parameters.sh         # jobName / EvtMax / jobNumbers / MCmodel / algName

# ⚠️ 公开版：数据集路径与 run 号已略去，必须先填
export DATA_ROOT=/your/path/to/offline/data   # 或直接编辑 txts/.datasetpath.sh
vi .scripts/SetBOSSVersion.sh                 # 把 RUN_BEGIN / RUN_END 换成真实 run 号

# 运行（交互式问答）
./iRobot
# 或一次性给全参数：<year> <action> [answer] [MCmodel]
./iRobot 2021 inc yes
```

`action` 可选值：`sim`（信号 MC 产生）、`rec`（重建）、`ana`（分析）、`all`（sim+rec+ana 一条龙）、`inc`（inclusive MC 分析）、`data`（真实数据分析）。

作业目录生成在 `$PWD/<motherparticle><year>/<jobDirectory>/` 下，输出落在 `rtraw/`、`dst/`、`root/` 子目录。

### 4. 检查与重投

```bash
cd <jobDirectory>
bash checkjoberr.sh              # 列出没跑完的作业 → errorfile.txt
source fill_all_failed_jobs.sh   # 自动生成并提交重投脚本
```

---

## 模块详解

### 1. `setupBOSSyjh` —— BOSS 环境安装

**用途**：在 IHEP 集群上把 BOSS 从 CVMFS 安装到用户自己的 `/workfs2` 工作区，并生成一个可复用的环境激活脚本。

**入口**：[`setupBOSS`](setupBOSSyjh/setupBOSS) 是 [`installBOSS_1.00.sh`](setupBOSSyjh/.installBOSS_1.00.sh) 的符号链接。

**核心流程**：

1. 校验 `/workfs2/bes/$USER`、`/scratchfs/bes/$USER`、`/besfs5/users/$USER` 与 AFS 家目录是否就位；
2. 校验 `/cvmfs/.../cmthome-$bossversion` 与 `/cvmfs/.../Boss/$bossversion` 是否存在；
3. 把 `cmthome-$bossversion` 拷到 `/workfs2/bes/$USER/cmthome/`；
4. 用一串 `sed` 改写 `requirements`：解开 `#macro WorkArea`、把 `maqm`/`USERID`/`BOSSVERSION` 占位符替换成真实值、修正 `CMTPATH`；
5. 改写 `setupCVS.{sh,csh}`；
6. `cmt config` → `cmt co TestRelease` → `cmt broadcast cmt config` → `cmt broadcast gmake`；
7. 把 `txts/boss` 渲染成 `$HOME/bin/bossbin/boss<ver>`（去点号，如 `7.0.8` → `boss708`）。

**用法**：

```bash
source setupBOSS <bossversion> [-silent]
```

**注意**：
- 脚本用了 `return` 而非 `exit`，因此**必须 `source` 执行**，不能 `./` 执行；
- 会向 `$HOME/.bashrc` 追加 `alias boss<ver>=...`（仅当 `$USER == yangjunhui`，见[硬编码清单](#使用前必须修改的配置)）；
- [`backup.sh`](setupBOSSyjh/backup.sh) 是同一流程的另一种实现（更多使用 `sed -i`），保留作参考。

---

### 2. `creatAlgyjh` —— 派生分析算法包

**用途**：BOSS 里新建一个分析算法包，需要复制几十个文件并把包名、类名、文件名、`load`/`entries` 组件名、`jobOptions` 名全部改掉。这个脚本把整套替换自动化了。

**入口**：[`creatAlg`](creatalgyjh/creatAlg) → [`.creatAlg_1.02.sh`](creatalgyjh/.creatAlg_1.02.sh)

**用法**：

```bash
source creatAlg <directory> <algName> <bossversion> [AlgVersion] [newalgName]
```

| 参数 | 说明 |
|---|---|
| `directory` | 模板算法包所在的父目录 |
| `algName` | 模板算法名（如 `DsSTDsAlg`） |
| `bossversion` | BOSS 版本（如 `7.0.8`） |
| `AlgVersion` | 可选，模板版本（如 `00-00-01`）；省略则自动探测 |
| `newalgName` | 可选，新算法名；省略则与 `algName` 相同 |

**行为**：

1. 检查 `/workfs2/bes/$USER/work.$bossversion` 是否已安装 BOSS，没有则询问是否调用 `setupBOSS`；
2. 在 `work.$bossversion/Analysis` 下 `cmt create <newpkg> <newpkg>-00-00-01`；
3. 拷贝模板的 `cmt/requirements`、头文件、`src/*.cxx`、`src/components/*`、`share/jobOptions_*.txt`；
4. 全局 `sed` 替换 `noalgName` → `nonewalgName`（含 `requirements`、头文件、所有 `.cxx`、组件、jobOptions）；
5. 在 `TestRelease/cmt/requirements` 中插入 `use <newpkg> <newpkg>-* Analysis`；
6. 询问是否编译，选 yes 则 `cmt clean && cmt config && cmt make`。

**版本说明**：1.02 为当前版本（1.01 的注释中自述"仍有拷贝旧算法文件的 bug，且需要 BOSS 检查"，1.02 已修复）。1.00 / 1.01 保留作参考。

---

### 3. `SubmitJobyjh` —— 作业机器人

**用途**：本工具箱的**主力工具**。以交互式问答的方式，把信号 MC 产生/重建/分析、inclusive MC 分析和真实数据分析的作业批量生成并提交到集群。

**入口**：[`iRobot`](SubmitJobyjh/iRobot) → [`.Robot_1.00.sh`](SubmitJobyjh/.Robot_1.00.sh)（`jarvisVersion = 1.00`）。
脚本内部与 `welcome.sh` 中自称 **jarvis**，但符号链接叫 `iRobot`，报错信息里也会提到 *Wall.E* —— 这是三代机器人命名混用的历史遗留。

**工作目录约定**：必须在同时含有 `.scripts/` 和 `txts/` 的目录下运行（通常是软链接）。脚本用 `$PWD` 作为根，自动创建 `<motherparticle><year>/` 作为数据集工作目录。

**执行流程**（每一步失败都会中止）：

```
SetColors → welcome（自我介绍 + 检查 Linux 发行版 + 检查 BES_RELEASE）
  → motherparticle.sh          读母粒子
  → SetYear.sh                 读数据年份（jpsi:2009/2012/2018/2019；psip:2009/2012/2021；…）
  → SetAction.sh               读动作（sim/rec/ana/all/inc/data）
  → SetBOSSVersion.sh          按 母粒子+年份 定 BOSS 版本与 RunIdList
  → checkBOSSVersion.sh        当前 BES_RELEASE 是否匹配，否则开关一次 BOSS
  → parameters.sh              jobName / EvtMax / jobNumbers / MCmodel / algName
  → .datasetpath.sh            数据 / inclusive MC 的输入路径
  → SetjobNumber.sh            按 action 取作业数
  → checkOrMakeDirectories.sh  建/复用数据集目录，带"防重复提交"状态机
  → PrepareFilesandDirectories.sh  拷贝脚本与模板，建 rtraw/dst/root 目录
  → changen.sh 或 makeJob.sh   生成逐作业的 *.txt jobOption
  → SubmitJobsToServer.sh → subjob.sh → boss.condor -n <N> ...
```

**关键设计**：

- **参数化（`changen.sh`）**：把 `txts/.{sim,rec,ana}.txt` 模板中的占位符替换成实际值——`AAAA`/`BBBB` → RunId 区间，`CCCC` → 年份，`DDDD` → `EvtMax`，`JOBNAME` → 作业名，`SIMDIR`/`RECDIR` → 上下游目录，`ALGNAME`/`ALGROOTNAME` → 分析算法，`DECAY` → 衰变表。每作业的随机数种子为
  `randomseed = 1002 + SN*10 + ecms + actionseed + yearseed`（`actionseed`：sim/all=10, rec=20, ana=30, inc=40, data=50）。
- **`checked.txt` 状态机**：作业目录里的 `checked.txt` 记录状态（`0`=已提交未验证，`1`=全部成功）。重复运行时用 `hep_q -u $USER` 检查是否还在跑，避免重复提交。
- **`all` 模式**：一条命令串起 `sim → rec → ana`。因为 `boss.condor` 接受多个 jobOption 并在**同一作业内串联执行**，所以用
  `boss.condor -n N sim_*.txt rec_*.txt ana_*.txt` 即可一次提交整条链。
- **失败重投**（[`fill_all_failed_jobs.sh`](SubmitJobyjh/.scripts/fill_all_failed_jobs.sh)）：逐个检查 `*.bosslog` 的结尾是否含 `INFO Application Manager successfully` / `Application Manager Terminated successfully`，收集坏作业，生成 `*_resubjob.sh` + `renamefilljobs.sh`，再用
  `hep_sub ... -argu "%{ProcId}" -n $badSN -g physics -o *.bosslog -e *.bosserr` 重投。

**作业模板**（[`txts/`](SubmitJobyjh/txts/)）：

| 文件 | 作用 |
|---|---|
| `.sim_psip.txt` / `.sim_jpsi.txt` / `.sim_psipp.txt` / `.sim_bhabha.txt` / `.sim_cms3650.txt` | 信号 MC 产生（KKMC + BesEvtGen + G4 模拟 → `rtraw/`） |
| `.rec.txt` / `.rec_all.txt` | 重建（MDC PAT+TSF、Kalman、TOF/EMC/MUC、顶点拟合 → `dst/`） |
| `.ana.txt` / `.ana_all.txt` | 物理分析（读 dst，跑 `$<ALG>ALGROOT` 算法 → `root/` 中的 NTuple） |
| `.makeJob.head` / `.makeJob.tail` | inclusive MC / 数据分析的 jobOption 头尾模板（中间由 `makeJob.sh` 填充 dst 文件列表） |
| `parameters.sh` | 用户主配置（作业名、事例数、作业数、MC 模型、算法名） |
| `motherparticle.sh` | 母粒子设置 |
| `.datasetpath.sh` | 数据 / inclusive MC 的输入路径表 |
| `decay4CDIY.dec` | 4C 拟合 DIY 衰变表示例 |

**用法**：

```bash
./iRobot                                     # 全程交互
./iRobot <year> <action>                     # 半自动
./iRobot <year> <action> <yes|no>            # 跳过"是否继续"确认
./iRobot <year> <action> <yes|no> <MCmodel>  # 指定 MC 模型
```

**`script.py`**（实验性）：另一套 Python 实现的作业卡生成器（`--createdir` / `--start` / `--startonedir` / `--cleanup`），内含 **51 条**硬编码数据集路径，与 `xiechen/signal/script.py` 同源但独立演化。

---

### 4. `iRobot` —— WALL·E（Python 版）

**用途**：作业机器人的 Python 3 面向对象重写版，把「环境探测 / 装 BOSS / 建算法包 / 编译算法包」抽象成类。
**状态**：⚠️ 最后一次提交为 2022-03，作业提交功能（`subjobs.py` / `resubjobs.py`）**从未实现**，其职责已由 `SubmitJobyjh` 承担，实际处于**存档状态**。

**类结构**（[`iRobot/classes/`](iRobot/classes/)）：

| 文件 | 类 / 职责 |
|---|---|
| `preferences.py` | `Preferences` —— **唯一配置源**：`workfs2`/`scratchfs`/`besfs5` 前缀、`cmthome` 与 `Boss` 的 CVMFS 路径、RHEL 代号→`SL5/SL6/SL7` 映射、容器支持标志。（旧名 `prefixes.py`，已重命名） |
| `environments.py` | `Environments` —— 从 `os.environ` 读 `USER`/`HOME`/`SHELL`，推导各工作目录；含 `SLnickname()` 解析 `/etc/redhat-release`、`InContainer()` 等检测 |
| `textcolor.py` | `TextColor` —— ANSI 彩色输出 |
| `toolkit.py` | `Toolkit` —— 底层文件工具（`_makedir`、`_findfile`、`_backupfile`、`_prepare_package`、`_replace_keyword` …） |
| `setupboss.py` | `SetupBOSS.install()` —— 安装/配置 BOSS，并生成 `boss<ver>` 与 `~/.tcshrc` 别名 |
| `initialize.py` | `Initialize` —— 备份/安装 `~/.tcshrc` 与 `~/.vimrc`。⚠️ **当前完全不可用**（见下） |
| `createAlgorithm.py` | `CreateAlgorithm.create()` —— 从 5 个内置模板派生算法包 |
| `compileAlgorithm.py` | `CompileAlgorithm.compile()` —— 编译算法包 |
| `WALLE.py` | 人格层，只有 `__init__` + `__str__`（无 `initialize()` 方法） |
| `subjobs.py` / `resubjobs.py` | **空文件**（0 字节） |

**运行模型**：宿主机的 python3 负责编排；由于「容器不支持 python3」，所有 `cmt` 操作都通过
`os.system('hep_container exec <container> ./<script>')` 投进容器（BOSS 6 → SL5，7 → SL6）用 tcsh 执行。

**CLI 入口**（[`iRobot/apps/`](iRobot/apps/)）：

```bash
python3 apps/WALLE_intro.py                                          # 自我介绍
python3 apps/WALLE_setupboss.py   --bossversion 7.0.9                # 安装 BOSS
python3 apps/WALLE_createAlg.py   --bossversion 7.0.9 --algname XxxAlg
python3 apps/WALLE_compileAlg.py  --bossversion 7.0.9 --algname XxxAlg --algversion 00-00-01
```

**已知限制**：
- `apps/*.py` 用 `sys.path.append('/workfs2/bes/zhoux/BES3Toolkit/iRobot/classes')` 这种**绝对路径**导入（`classes/` 没有 `__init__.py`），必须先改；
- `initialize.py` 的 `initialize()` 引用了 `__init__` 中从未定义的 `self.logfile`/`self.preferences`，且第 33 行无条件 `return`，其后所有逻辑都是死代码；
- `environments.py` 的 `SLnickname()` 只认 `Boron/Carbon/Core`，在 AlmaLinux 上会解析出错误字符串并 `KeyError`；
- 依赖 `distutils`，Python 3.12 起不可用；
- [`apps/run.zsh`](iRobot/apps/run.zsh) 是原作者本机的部署脚本（`rsync` 到 `ihep` 主机 + `ssh` 执行），**对使用者无用**。

---

### 5. `checktool` —— 作业体检与重投

所有脚本**不接受命令行参数**，均在作业输出目录就地运行。

| 脚本 | 输入 → 输出 | 用途 |
|---|---|---|
| [`checkjoberr.sh`](checktool/checkjoberr.sh) | 当前目录 `*.bosslog` → `errorfile.txt` | 凡日志中**不含** `INFO Application Manager Finalized successfully` 的作业判为未完成并记录作业卡名 |
| [`cperrorfile.sh`](checktool/cperrorfile.sh) | `errorfile.txt` → `./again/` | 把出错作业的日志集中拷贝，便于排查 |
| [`resubjob.py`](checktool/resubjob.py) | `data_gamV_jpsi_2012_*.txt` → `temp.txt` | 用正则从已有作业卡中抽出所有 `.dst` 路径 |
| [`resubjob.sh`](checktool/resubjob.sh) | `temp.txt` → `jobOptions_0..47.txt` | 把剩余 dst **轮转均分**成 48 份新作业卡（`num_copies=48` 写死在脚本内） |
| [`countnumber.sh`](checktool/countnumber.sh) | `inc_gamV_psip_2021_*.txt.bosslog` → 屏幕 + `number.txt` | 汇总各 cut 阶段计数：`total number:`、`Good Track Sel.:`、`Gamma Sel.:`、`PID:`、`vtx:`、`4C for mode1/2/3 Fit:` |
| [`extract.cxx`](checktool/extract.cxx) | `ana_data.root` → `ana_data_phiome.root` | ROOT 宏：`CopyTree("tagmode==4&&chisq_4c<200")` |

`resubjob.sh` 生成的作业卡包含标准的五条 `#include`（`$ROOTIOROOT`、`$VERTEXFITROOT`、`$MAGNETICFIELDROOT`、`$ABSCORROOT`、`$ETACTOGAMVALGROOT`）+ `EventCnvSvc.digiRootInputFile` + `NTupleSvc.Output`。

> ⚠️ 两处已知 bug：
> 1. `countnumber.sh` 第 30 行 `vtx_total=$((fit_4c_total + fit_4c))` 引用了未定义变量，`vtx` 累计恒为 0；
> 2. `countnumber.sh` 首行 shebang 误写为 `#!bin/bash`（缺前导 `/`），必须用 `bash countnumber.sh` 调用。

---

### 6. `DrawTool` —— 绘图

| 文件 | 说明 |
|---|---|
| [`draw.C`](DrawTool/draw.C) | 74 行最小示例：从 `pnpi` TTree 读 `n_mass` 分支填一维直方图，输出 `Positive_draw1D.png` |
| [`draw2D.C`](DrawTool/draw2D.C) | 二维分布示例（`old_x` vs `old_y`），输出 `tryfordraw.png` |
| [`extract.cxx`](DrawTool/extract.cxx) | 按 `TCut`（如 `tagmode == 2 && pid_K[0] > 0.001 && ...`）从 `rec` 树抽取子样本 |
| [`bes3plotstyle.{C,h}`](DrawTool/bes3plotstyle.C) | BESIII 官方绘图风格（Niklaus Berger, 2009）。提供 `FormatData`/`FormatMC1/2/3`/`FormatAxis`/`NameAxes`/`MakeLegend`/`WriteBes3`/`WritePreliminary`/`SetStyle`/`SetPrelimStyle`/`SetMeetingStyle` 以及高层封装 `PlotDataMC`、`PlotDataFit`（最多 6 条 TF1）、`PlotScatter`（同时输出 `.eps` 与 `.png`） |
| [`drawtool/draw.C`](DrawTool/drawtool/draw.C) | 155 行的完整绘图框架：`fillTH1D`/`fillTH2D` + `cutTH1Ds`/`cutTH2Ds` 容器 + `histoAxis` 轴标题表，逐张存到 `./figs/` |
| [`drawtool/variable.cxx`](DrawTool/drawtool/variable.cxx) | **不是独立函数**，而是一个裸 `{…}` 代码块，设计为被 `ana::Loop()` 的事件循环 `#include` 进去；用 `vft4_*`/`v4_*` 建 `TLorentzVector` 并计算 ρ、π⁰ 质量与 4C χ² |

**用法**：

```bash
root -l -q draw.C
# 或
root -l
root [0] .L bes3plotstyle.C
root [1] SetStyle();
root [2] .x draw.C
```

**注意**：
- 顶层 `draw.C` / `draw2D.C` 中的文件名、TTree 名（`pnpi_psip_data_2009.root`、`pnpi`）与分支名是**写死的示例值**；
- [`drawtool/draw.C`](DrawTool/drawtool/draw.C) 通过 `#include "ana.C"` 引入变量定义，而 **`ana.C` / `ana.h` 不在本仓库**，必须在你自己的分析目录（含 `ana.C`）中使用；`vft4_*`、`chisq_4c_less` 等分支名也来自那套框架；
- `drawtool/draw.C` 会先 `mkdir -p ./figs/` 再 `rm -rf ./figs/*`，运行前注意目录内容。

---

### 7. `fitkit` —— 拟合

基于 **RooFit** 的拟合模板集合，都是在真实分析中抽出来的骨架，把输入路径换掉即可用。
输入统一为**你自己用 BOSS `ana` 作业产生的小 ntuple**（不是原始 DST/Rec）。

| 文件 | 形态 | 说明 |
|---|---|---|
| [`testfit.cxx`](fitkit/testfit.cxx) | 宏 | **最小样例**：`mlampi`(1.2–2.2, 50 bins) 的三分量拟合，三个 `RooHistPdf` 分别来自 `#Sigma(1385)`、PHSP 与 inclusive MC。适合作为起点 |
| [`fit_1D.cxx`](fitkit/fit_1D.cxx) | 宏 | 一维拟合：Gaussian + 3 阶 `RooChebychev`，`Extended()`，打印 χ²/ndf |
| [`fit_2D.cxx`](fitkit/fit_2D.cxx) | 宏 | 二维无 bin 拟合（`metap` × `mm2mft`），4 个 `RooNDKeysPdf` 形状样本，带 EDM/status/covQual **重试环**，输出 `sig_num.dat`、`signal.root` 与投影+pull 图 |
| [`fit2D_test.cxx`](fitkit/fit2D_test.cxx) | 宏 | **最新最完整**：`mlampi` × `mkpi` 二维拟合，本底/PHSP 用 `RooProdPdf`，信号用 `RooFFTConvPdf`(BW⊗Gauss) × 多项式，浮动参数个数由 `floatParsFinal()` 得到 |
| [`D_fit_unbin.cxx`](fitkit/D_fit_unbin.cxx) | `int main()` | 双 D 介子 mBC 二维无 bin 拟合（高斯涂抹 → `RooHistPdf`，本底 `RooArgusBG` + 自定义 `RooMispartition`）。⚠️ **缺头文件，无法编译** |
| [`2D_sim_fit.cxx`](fitkit/2D_sim_fit.cxx) | `int main()` | ω 区二维 `RooSimultaneous` 同时拟合（信号区 + 边带）。⚠️ **缺头文件，无法编译** |

**用法**：

```bash
# 宏
root -l -q fitkit/testfit.cxx
# 或
root -l
root [0] .L fitkit/fit_1D.cxx
root [1] fit_1D()

# main() 程序需编译；注意 root-config --libs 不含 RooFit，必须自己加
g++ -std=c++0x D_fit_unbin.cxx -o D_fit_unbin \
    $(root-config --cflags) -lRooFit -lRooFitCore $(root-config --libs)
```

> ⚠️ **`D_fit_unbin.cxx` 与 `2D_sim_fit.cxx` 当前无法编译**：二者都 `#include "RooMispartition.h"`，而该头文件既不在 ROOT 发行版中，也不在本仓库中。它是某个非标准 RooFit 扩展类，构造签名只能反推为 `RooMispartition(name, title, obsTag, obsSig, scor, sigma, m0, c, p)`。补上该头文件后才能使用。

**需要修改**：每个文件顶部的 `inputRoot` / `outputpng` / `treeName` / `branchName` / bin 范围等常量。这些路径在公开版中已统一替换为 `/path/to/your/analysis/...` 占位符，请改成你自己的分析输出目录。
部分程序要求预先存在 `chi/`、`sig_num/`、`root/` 等输出目录（代码不会自动创建）。

---

### 8. `chi2FoM` —— χ² 扫描与 cut 优化

**用途**：给定信号 MC 与本底 MC（或信号+本底）的 ROOT 文件，扫描一系列 χ² 上限，计算品质因子

```
FoM = S / √(S + B)
```

画出 FoM–χ² 曲线（红箭头标出工作点）与信号/本底 χ² 分布对比图，用于**确定最优的 χ² 切割**。原始框架来自 Dr. Xiang Zhou（`chi2.C` 头部：*Create by Dr. Xiang Zhou, before 2014*）。

**构建与运行**：

```bash
cd chi2FoM
bash compile.sh          # g++ -std=c++0x chi2.C -o chi2 `root-config --cflags --libs`

./chi2FoM --prepare      # 从模板拷一份 Example.cfg 到当前目录
./chi2FoM Example.cfg    # 用指定配置运行
./chi2FoM --help         # 查看帮助
```

包装脚本在 `BES_RELEASE` 未设置时会自动 `source .../bossbin/boss700`（注释说明需要 `g++ >= 4.6.3`）。

**扫描逻辑**：`for (i = 10; i < 201; i += chi2Step)`，每步取 cut `<chisquare> < i`：

- 纯本底模式（给了 `B`）：`S+B = B数(未过 itopo) + S数 × ScaleRatio`，`FoM = S/√(S+B)`；
- 合并样本模式（`B` 留空、给 `SpB`）：`S+B = SpB 样本中过 cut 的事例数`。

**输出**：`chi2FoM<outputExt>`（FoM 曲线）与 `chi2Stack<outputExt>`（χ² 分布对比）。输出目录不存在会自动 `mkdir -p`，同名文件会先 `rm -rf`。
（运行时会先打印一句 `ls: cannot access ...` —— 这是代码用 `ls` 探测文件存在性造成的噪音，可忽略。）

**配置文件格式**（[`Example.cfg`](chi2FoM/Example.cfg)，`key = value`，`//` 为注释；键值两端空白自动裁剪；**重复键后者覆盖前者**；值可跨行续写）：

| 字段 | 类型 | 含义 | 示例值 |
|---|---|---|---|
| `S` | string | 信号 MC 的 ROOT 文件路径（**必填**） | `/path/to/your/pi0ee_Sig_MC.root` |
| `B` | string | 纯本底（inclusive MC）文件路径。与 `SpB` **互斥** | `…/pi0ee_INC_MC.root` |
| `SpB` | string | 信号+本底合并样本；`B` 留空时用它，此时本底图用数据点画、图例为 `Sig+Bkg` | 空 |
| `tree` | string | S/B 文件中的 TTree 名 | `fit4c` |
| `chisquare` | string | χ² 分支名，用于拼 `<chisquare> < i` 扫描条件 | `chi2` |
| `ScaleRatio` | double | 纯本底模式下信号归一化因子（必须 ≥ 0） | `0.001` |
| `itopoCut` | string | 纯本底模式下本底项取 `!itopoCut`（不满足该拓扑条件的本底） | 空 |
| `basicCut` | string | 对所有样本统一施加的预选表达式 | `mpi0 > 0.121 && mpi0 < 0.148 && mepem < 0.84 && metap > 0.8 && metap < 1.0` |
| `chi2Step` | int | χ² 扫描步长（**必须提供**，缺失会退化成异常长循环） | `10` |
| `chi2Arrow` | int | FoM 图上红色箭头（`TArrow`）的 x 位置 | `30` |
| `outputDir` | string | 输出目录（不存在则创建） | `.` |
| `outputExt` | string | 输出扩展名（含点） | `.png` |

> 📌 注意：`Example.cfg` 中被注释掉的 `//Sitopo = ...` **并不是有效的配置键**，代码读的是 `itopoCut`。
> 另外 `chi2.C` 中绘制分布图的两行**写死了 `"chi2"`**，与可配置的 `chisquare` 不一致 —— 若你的 χ² 分支不叫 `chi2`，需自行修改。

**辅助文件**：[`config.hh`](chi2FoM/config.hh) / [`config.cc`](chi2FoM/config.cc) 是通用 `key=value` 配置解析类（来源见文件头注释）；[`userDefined.h`](chi2FoM/userDefined.h) 提供 `truncatTailingZeroes()`（当前未被调用）。

> ℹ️ 公开版中**不再附带预编译的 `chi2` 二进制**（原二进制是 SL6/BOSS7 时代产物，依赖 `libCint.so`，在现代 ROOT 下无法运行）。请用 `compile.sh` 自行编译。

---

### 9. `inclusive` / `xiechen` —— 作业模板

#### `inclusive/` —— 12 个能量点的 inclusive MC 分析

**目的**：在 ψ(3770) 扫描的 12 个能量点上，对 **22 种 exclusive 本底过程**做同一套 inclusive 分析并汇总。
输入是**集群上已有的官方 inclusive MC DST**，本模板不产生 MC。

**能量点与质心能量**（[`inclusive/generate.sh`](inclusive/generate.sh)，run 区间已略去）：

| 目录 | E_cm (GeV) |
|---|---|
| `4130` | 2.06424 |
| `4160` | 2.07872 |
| `4180` | 2.09 |
| `4190` | 2.094495 |
| `4190-scan` | 2.094295 |
| `4200` | 2.099515 |
| `4210` | 2.104625 |
| `4210-scan` | 2.103865 |
| `4220` | 2.10942 |
| `4220-scan` | 2.108565 |
| `4230` | 2.11277 |
| `4230-scan` | 2.11313 |

**工作流**：

```
inclusive/generate.sh                       # 遍历 12 个能量点
 └─ <ECM>/generate.sh <算法名>               # 参数化 MC_01，克隆出 MC_02..MC_040
     └─ <ECM>/reset.sh                      # 对每个 MC_0i 并行执行
         └─ MC_0i/reset.sh                  # 核心生成器：克隆 mc1→mc2..mc22，
                                            #   设置 mode.mctag，调 makejob.sh，
                                            #   并动态生成 MC_0i/sub.sh
             └─ MC_0i/sub.sh  → boss.condor  # 提交
 └─ <ECM>/hadd.sh                           # 逐 process hadd → all/ → 跑 sel.c 核对
 └─ <ECM>/error.sh                          # 检查失败作业 → hep_sub 重投
 └─ <ECM>/clear.sh / clear_all.sh           # 清理中间产物
```

**目录层级**：

```
inclusive/<ECM>/
├── generate.sh / reset.sh / sub.sh / hadd.sh / error.sh / clear.sh / clear_all.sh
└── MC_01/                    # 每个 MC 样本的模板单元
    ├── mc1/                  # 单条本底链模板
    │   ├── job.head          #   BOSS jobOption 前半（含 RUN_FROM/RUN_TO/BeamEnergy/mctag）
    │   ├── job.foot          #   BOSS jobOption 后半（含输出文件名）
    │   ├── makejob.sh        #   拼装器：每 nDataFile 个 dst 生成一个作业卡
    │   └── error.sh          #   失败检测 + hep_sub 重投
    ├── reset.sh              # 核心生成器（详见上）
    ├── hadd.sh / clear.sh / error.sh
    ├── sel.c                 # ROOT 宏：核对 hadd 前后条目数
    ├── list                  # 22 个过程名清单（纯文档，无脚本读取）
    └── all/                  # hadd 输出目录（空目录，git 无法记录）
```

**22 种本底过程**（`reset.sh` / `hadd.sh` 中的 `a0` 数组）：
`D0D0`, `DDPI0`, `DDPIp`, `DDSTPI0`, `DDSTPIp`, `DpDm`, `DsDs.newLS`, `DsSTDs.newLS`, `DST0D0`, `DST0DST0`, `DSTpDm`, `DSTpDSTm`, `ee`, `eeNLO`, `HCT`, `mm`, `tt`, `qq`, `RR1S`, `RR2S`, `RR3770`, `TwoGam`。

**关键说明**：

- `MC_0i/reset.sh` 里的 `mode.mctag`（1..22，过程标签）与 `mode.mcnumber`（MC 样本序号）是两个核心开关；
- **`job.head` / `job.foot` 不是 shell 脚本**，而是 BOSS jobOption 文本（尽管带 `+x` 权限位）。其中所有 `#include` 都使用 BOSS 环境变量（`$ROOTIOROOT` 等），这是整套模板里唯一"干净"的部分；
- **`sel.c` 不是 BOSS 的 `DstEventSelector`**，而是一个纯 ROOT 宏 `void sel()`：对 `signal`/`truth`/`tag`/`taga` 四棵树分别统计「各链分开」与「hadd 合并后」的条目数并打印差值，用于自洽性核对。真正的事例选择由用户算法（`ApplicationMgr.DLLs += {"<Name>Alg"}`）完成；
- **`MC_0i/sub.sh` 不在仓库里**，它由 `MC_0i/reset.sh` 在运行时动态生成；
- 12 个能量点之间的差异只有 5 类：`job.head` 中的 `RUN_FROM`/`RUN_TO` 与 `BeamEnergy`（= E_cm/2）、`job.foot` 的输出名前缀、`makejob.sh` 中的 `DATA_ROOT` 路径、内嵌的 `<ECM>` 字符串、以及缩进；
- ⚠️ [`makejob.sh`](inclusive/4230/MC_01/mc1/makejob.sh) 用 `sed -i "s/D0D0/$list/g" job.foot` **就地修改模板**（脚本注释自称 *"it is a dangerous operation"*），重复运行会污染 `job.foot`。

#### `xiechen/signal/` —— 信号 MC 全链生产

**目的**：ψ(3770) 以上 **3φ 能量扫描**的信号 MC 产生（sim）→ 重建（rec）→ 分析（sig），覆盖 **46 个能量点**、4 个 campaign（703/705/706/707）。

```bash
python3 script.py --createdir    # 建 46 个能量点目录，替换 3phi.dec 的 ECMS、msim.txt 的 RUNNO
./auto.exp <作业数> <每作业事例数>   # expect 脚本，自动回答 script.py --simulate 的两个提问
python3 script.py --simulate     # 生成 sim / rec 作业卡并追加到 sub
./generate.sh                    # 逐目录生成 sig 阶段的作业卡（nDataFile = 2）
./sub.sh 703|705|706|707         # 按 campaign 分批提交：boss.condor -wt mid sig_3phi_0
```

| 文件 | 作用 |
|---|---|
| [`script.py`](xiechen/signal/script.py) | 222 行 argparse 生成器（`--createdir` / `--simulate` / `--signal` / `--cleanup`），内含 46 个能量点 × E_cm 的大表（run 区间已略去，改为 `RUN_BEGIN` / `RUN_END` 占位符）；`--simulate` 用 `random.sample(range(99999), num_jobs)` 生成随机种子 |
| [`auto.exp`](xiechen/signal/auto.exp) | `expect` 脚本，自动喂给 `--simulate` 的两个 `input()` |
| [`msim.txt`](xiechen/signal/msim.txt) | sim 阶段 jobOption 模板（占位符 `NUMBER`/`RUNNO`/`SIMN`/`aaaa.dec`） |
| [`mdst.txt`](xiechen/signal/mdst.txt) | rec 阶段 jobOption 模板（PAT+TSF+HOUGH 重建链，含本底混合） |
| [`job.head`](xiechen/signal/job.head) / [`job.foot`](xiechen/signal/job.foot) | sig 阶段 jobOption 头尾（算法硬编码为 `eeTo3phi`） |
| [`3phi.dec`](xiechen/signal/3phi.dec) | EvtGen 衰变卡：`vpho → φφφ`（用 `ConExc` 处理 ISR），`φ → K⁺K⁻` |
| [`xs_user.txt`](xiechen/signal/xs_user.txt) | ConExc 用的 (E_cm, 截面, 误差) 三列表，3.05→4.95 GeV，步长 0.5 MeV，共 3901 行 |

#### `xiechen/inclusive/`

与 [`inclusive/`](inclusive/) **逐字节完全相同**（`diff -rq` 无差异），是历史副本。

---

### 10. `template_topoana.card` —— TopoAna 配置

[TopoAna](http://bes3.ihep.ac.cn/) 是 BESIII 常用的**拓扑分析**工具，用于统计 inclusive MC 中各衰变链的贡献。这个文件是它的配置文件模板（317 行）。

**语法**：`#` 注释、`%` 提示、`{ }` 分组，每一项一个 `{}` 块。

| 字段 | 必填 | 说明 |
|---|---|---|
| 输入 root 文件列表 | ✅ | 每行一个路径，支持 `[]?*` 通配（同 `TChain::Add()`），绝对/相对路径均可 |
| TTree 名 | ✅ | 如 `truth` |
| 原始拓扑真值存储类型 | ❌ | `AOI`（数组 int）/ `VOI`（vector int）/ `MSI`（多个标量 int）/ `MSD`（多个标量 double），默认 `AOI` |
| 粒子数分支名 | ❌ | 默认 `nMCGen` |
| PDG 码分支名 | ❌ | 默认 `MCGenPDG` |
| 母粒子索引分支名 | ❌ | 默认 `MCGenMothIndex` |
| 拓扑分量分析 | ❌ | decay trees / initial-final states / decay branches / production branches / mothers / cascade / inclusive / intermediate-resonance |
| 信号鉴别 | ❌ | decay trees / IFSts / particles / branches / cascade / inclusive / IRR branches |
| ISR/FSR 约定 | ❌ | PDG 约定 `222222222` / `-22`，标记 `Ys` / `Yg` / `N` |
| 初始态粒子 | ❌ | 默认 `e- e+` |
| 输出控制 | ❌ | 默认以 card 文件名命名，同时输出 `root` / `txt` / `tex` / `pdf` 四份；可设"每个输入 root 文件一个输出文件"、cut、最大条目数等 |

模板内的示例内容是 B 物理（`Υ(4S) → B⁰B̄⁰`、`B⁰ → K_S⁰ J/ψ` 等）。**用法**：拷成 `xxx.card`，填入你的 inclusive MC 文件与树名，交给 TopoAna：

```bash
TopoAna.exe xxx.card
```

---

## 使用前必须修改的配置

> 🚨 **重要**：本仓库是个人工作流的直接快照，脚本中大量路径指向作者（`yangjunhui` / 原框架作者 `zhoux`）的私有目录。**在别人的账号下直接运行会失败或误改文件。** 请按下表逐项替换。

### A. 私有目录（必改）

| 文件:行 | 变量 / 内容 | 原值 | 应改为 |
|---|---|---|---|
| [`setupBOSSyjh/.installBOSS_1.00.sh:51`](setupBOSSyjh/.installBOSS_1.00.sh#L51) | `setupBOSSWD` | `/afs/ihep.ac.cn/users/y/yangjunhui/setupBOSS` | 你自己的 `setupBOSS` 目录 |
| [`setupBOSSyjh/backup.sh:49`](setupBOSSyjh/backup.sh#L49) | `setupBOSSWD` | 同上 | 同上 |
| [`SubmitJobyjh/.Robot_1.00.sh:46-47`](SubmitJobyjh/.Robot_1.00.sh#L46) | `creatAlgWD` / `setupBOSSWD` | `/workfs2/bes/yangjunhui/...` | 你的 `BES3Toolkit` 路径 |
| [`SubmitJobyjh/.scripts/SetBOSSVersion.sh:82-83`](SubmitJobyjh/.scripts/SetBOSSVersion.sh#L82) | `creatAlgWD` / `setupBOSSWD` | 同上 | 同上 |
| [`SubmitJobyjh/.scripts/PrepareFilesandDirectories.sh:10`](SubmitJobyjh/.scripts/PrepareFilesandDirectories.sh#L10) | `cp .../checktool/checkjoberr.sh` | `/workfs2/bes/yangjunhui/...` | 你的路径 |
| [`SubmitJobyjh/.scripts/compileAnalysisCode.sh:3`](SubmitJobyjh/.scripts/compileAnalysisCode.sh#L3) | `compileAlgWD` | `/afs/ihep.ac.cn/users/z/zhoux/compileAlg` | 你的 `compileAlg`（当前该脚本默认未被调用） |
| [`creatAlgyjh/.creatAlg_1.02.sh:51-52`](creatalgyjh/.creatAlg_1.02.sh#L51) | `creatAlgWD` / `setupBOSSWD` | `/workfs2/bes/yangjunhui/...` | 你的路径 |
| [`chi2FoM/chi2FoM:22,29`](chi2FoM/chi2FoM#L22) | `workWD` | `/workfs/bes/zhoux/chi2FoM` | 你自己编译 `chi2` 的目录 |
| [`iRobot/classes/preferences.py:14`](iRobot/classes/preferences.py#L14) | `iRobot_dir` | `/workfs2/bes/zhoux/BES3Toolkit/iRobot` | 你的路径 |
| [`iRobot/apps/*.py:9`](iRobot/apps/WALLE_intro.py#L9) | `sys.path.append(...)` | `/workfs2/bes/zhoux/.../classes` | 你的路径（改为相对导入更佳） |

### B. 用户名硬编码（建议改）

| 文件:行 | 内容 | 说明 |
|---|---|---|
| [`setupBOSSyjh/.installBOSS_1.00.sh:268,291`](setupBOSSyjh/.installBOSS_1.00.sh#L268) | `if [ $NAME == yangjunhui ]` | 只有该用户才会被写入 `.bashrc` 的 alias；换成你的用户名，或删掉判断 |
| [`setupBOSSyjh/backup.sh:266,289`](setupBOSSyjh/backup.sh#L266) | 同上 | 同上 |
| [`setupBOSSyjh/.scripts/welcome.sh:6`](setupBOSSyjh/.scripts/welcome.sh#L6) | 同上 | 仅影响问候语 |
| [`SubmitJobyjh/.scripts/welcome.sh:7`](SubmitJobyjh/.scripts/welcome.sh#L7) | 同上 | 仅影响问候语 |
| [`creatAlgyjh/.scripts/welcome.sh:6`](creatAlgyjh/.scripts/welcome.sh#L6) | 同上 | 仅影响问候语 |
| [`iRobot/classes/WALLE.py:30`](iRobot/classes/WALLE.py#L30) | `if self.user == 'zhoux'` | 彩蛋式欢迎语 |

### C. 数据集路径与 Run 区间（公开版已略去，必填）

> 🔑 公开版**有意移除**了真实数据集路径与 run 号。需要完整配置的合作组成员，请从
> **`/workfs2/bes/yangjunhui/BES3Toolkit`** 获取。

| 文件 | 占位符 | 填法 |
|---|---|---|
| [`SubmitJobyjh/txts/.datasetpath.sh`](SubmitJobyjh/txts/.datasetpath.sh) | `DataParentPath` | 已改写为模板：设 `export DATA_PARENT_PATH=/你的/dst/目录`，或直接编辑该文件 |
| [`SubmitJobyjh/script.py`](SubmitJobyjh/script.py) | `DATA_ROOT` | 设 `export DATA_ROOT=/你的/数据集根目录`，或改脚本里的默认值 |
| [`SubmitJobyjh/.scripts/SetBOSSVersion.sh`](SubmitJobyjh/.scripts/SetBOSSVersion.sh) | `RunIdList=(RUN_BEGIN RUN_END)` | 换成你自己年份对应的 run 号区间，共 15 处 |
| `inclusive/*/MC_01/mc1/job.head`（12 个） | `Int_t RUN_FROM / RUN_TO = 0` | 文件顶部改这两个常量，下方三处 `RunFrom`/`RunTo` 会自动跟随 |
| `inclusive/*/MC_01/mc1/makejob.sh`（12 个） | `DATA_ROOT` | 设 `export DATA_ROOT=...`，或改脚本里的默认值 |
| [`inclusive/generate.sh`](inclusive/generate.sh) | `runbegin` / `runend` | 数组值已占位（该数组仅作溯源记录，脚本未使用） |
| [`xiechen/signal/script.py`](xiechen/signal/script.py) | `runlist` | 每个能量点一个 `'起始run, 0, 结束run'` 字符串 |
| [`chi2FoM/Example.cfg:3-4`](chi2FoM/Example.cfg#L3) | `S` / `B` | `/path/to/your/*.root`，换成你自己的 ntuple |
| [`fitkit/fit_1D.cxx`](fitkit/fit_1D.cxx)、[`testfit.cxx`](fitkit/testfit.cxx)、[`fit2D_test.cxx`](fitkit/fit2D_test.cxx)、[`2D_sim_fit.cxx`](fitkit/2D_sim_fit.cxx) | `/path/to/your/analysis/...` | 换成你自己的分析输出目录 |

### D. 物理参数（按分析改）

| 文件 | 内容 |
|---|---|
| [`SubmitJobyjh/txts/motherparticle.sh`](SubmitJobyjh/txts/motherparticle.sh) | `motherparticle=jpsi` |
| [`SubmitJobyjh/txts/parameters.sh`](SubmitJobyjh/txts/parameters.sh) | `jobName`、`EvtMax`、`jobNumbers`、`MCmodel`、`algName` |
| [`SubmitJobyjh/.scripts/SetBOSSVersion.sh`](SubmitJobyjh/.scripts/SetBOSSVersion.sh) | 各母粒子对应的 `RunIdList` 区间（占位符） |
| `inclusive/*/MC_01/mc1/job.head` | `mode.BeamEnergy`、`RUN_FROM`/`RUN_TO`、`UseCalibEnergy`/`ReadBeamEFromDB` |
| `inclusive/*/MC_01/mc1/makejob.sh` | `nDataFile`（4230 为 5，其余为 6） |
| [`xiechen/signal/xs_user.txt`](xiechen/signal/xs_user.txt) | 截面表 |
| [`checktool/resubjob.sh:5`](checktool/resubjob.sh#L5) | `num_copies=48` |
| [`checktool/resubjob.py`](checktool/resubjob.py)、[`checktool/countnumber.sh`](checktool/countnumber.sh) | 硬编码的数据集名前缀 `data_gamV_jpsi_2012_*`、`inc_gamV_psip_2021_*` |

---

## 依赖

| 依赖 | 用途 | 是否随仓库提供 |
|---|---|---|
| BOSS 6.6.4 / 7.0.8 / 7.0.9 / 7.1.0 | 全部模拟、重建、分析 | ❌ 从 `/cvmfs/bes3.ihep.ac.cn` 获取 |
| CMT | BOSS 包管理 | ❌ 随 cmthome 提供 |
| ROOT（含 RooFit） | 绘图、拟合、`chi2FoM` | ❌ 随 BOSS 提供 |
| `g++ >= 4.6.3`, `gmake`, `sed`, `find`, `awk` | 编译与脚本 | ✅ 系统自带 |
| `hep_sub` / `hep_q` / `boss.condor` | 作业提交与查询（HTCondor） | ❌ IHEP 集群提供 |
| Python 3.6+ | `iRobot/`、`script.py`、`resubjob.py` | ✅ 系统自带 |
| `expect` | `xiechen/signal/auto.exp` | ❌ 需自行安装 |
| TopoAna | 拓扑分析 | ❌ 需另行获取（本仓库只提供 `.card` 模板） |
| `bes3plotstyle.{C,h}` | BESIII 绘图风格 | ✅ **随仓库提供** |
| `RooMispartition.h` | `fitkit` 两个程序 | ❌ **缺失，见已知问题** |
| `ana.C` / `ana.h`（用户自己的分析框架） | `DrawTool/drawtool/` | ❌ 需自备 |
| `modeAlg` / `eeTo3phiAlg` 算法源码 | `inclusive` / `xiechen` 的分析算法 | ❌ **不在本仓库** |

---

## 已验证 / 未验证

为避免误导，这里说明哪些部分是**实际测试过**的：

**✅ 已在 ROOT 6.26.10 环境验证**
- `fit_1D.cxx`、`fit_2D.cxx`、`testfit.cxx`、`fit2D_test.cxx`：可被解释器成功 `.L` 加载；
- `DrawTool/draw.C`、`draw2D.C`、`extract.cxx`、`checktool/extract.cxx`：同上；
- `chi2FoM`：`compile.sh` 编译成功，并用合成的 `fit4c` ntuple **端到端跑通**，正常生成 `chi2FoM.png` 与 `chi2Stack.png`。

**❌ 未能验证**
- `fitkit/D_fit_unbin.cxx`、`fitkit/2D_sim_fit.cxx`：因 `RooMispartition.h` 缺失，**编译失败**；
- `DrawTool/drawtool/draw.C`：因 `ana.C` 缺失，无法单独加载；
- 所有依赖 BOSS / `hep_container` / `cmt` / CVMFS / `/workfs2` / `/besfs*` / AFS 的脚本：这些前提在普通 Linux 机器上不存在，**未做运行验证**，结论均来自源码阅读。

---

## 已知问题与后续计划

### 已知问题

1. **两份嵌套的 `.git` 目录已失效**
   - 顶层 `.git` 与 `iRobot/.git` 都记录了**旧的目录布局**（文件原先在仓库根目录，后来被移进 `iRobot/`），`git status` 会显示大量已删除文件；两者都没有配置 `remote`；`git ls-files` 里甚至跟踪着 `__pycache__/*.pyc`。
   - **上传到 GitHub 前建议删除这两个 `.git` 目录，重新 `git init`。**
2. **代码重复**
   - [`iRobot/classes/createAlgorithm.py`](iRobot/classes/createAlgorithm.py) 与 [`creatAlgorithm.py`](iRobot/classes/creatAlgorithm.py) **md5 完全相同**（`a64e8274…`），是拼写事故而非版本差异；且 `apps/WALLE_createAlg.py` 导入的是正确拼写，**`creatAlgorithm.py` 可安全删除**；
   - `xiechen/inclusive/` 与 `inclusive/` 逐字节相同；
   - `creatAlgyjh` 保留了 1.00 / 1.01 / 1.02 三个版本。
3. **空文件**：[`iRobot/classes/subjobs.py`](iRobot/classes/subjobs.py)、[`resubjobs.py`](iRobot/classes/resubjobs.py) 为 0 字节（作业提交功能只在 Bash 版中实现）。
4. **`iRobot` 无法在现代 Python 上运行**：`initialize.py` 引用了未定义属性且逻辑不可达；依赖 `distutils`（Python 3.12 已移除）；`SLnickname()` 在非 RHEL6/7 系统上会 `KeyError`。
5. **`checktool/countnumber.sh`**：shebang 缺前导 `/`；`vtx_total` 引用了未定义变量。
6. **`SubmitJobyjh/.scripts/SubmitJobsToServer.sh`**：`yn = $answer`（等号两侧有空格）在 bash 下不是赋值，会尝试执行命令 `yn`；该分支只在传入第 3 个参数时走到。
7. **`inclusive/*/MC_01/mc1/makejob.sh`** 用 `sed -i` 直接修改 `job.foot`（脚本注释自认 *dangerous*），重复运行会污染模板。
8. **`inclusive/*/MC_01/all/`** 是空目录，git 无法记录，克隆后需要手动创建。
9. **`chi2FoM/chi2`** 原本是提交进仓库的 ELF 二进制（SL6 时代产物，依赖 `libCint.so`，现代 ROOT 下无法运行）。**公开版已删除**，请用 `compile.sh` 自行编译。
10. **`fitkit` 两个程序缺少 `RooMispartition.h`**，当前无法编译。
11. **`iRobot/apps/run.zsh`** 是原作者的 macOS 本机部署脚本，含 `rsync`/`ssh` 到个人主机的命令，对外部使用者无意义；且开头有 `rm -f .*`，**危险**。
12. **`chi2.C` 的两个小瑕疵**：绘制分布图时写死 `"chi2"` 分支名；`config` 读取的临时 `TString` 指针被长期持有（理论悬垂）。

### 后续计划

- [ ] 把硬编码路径统一抽到单个 `config.sh` / `preferences.py`，支持环境变量覆盖；
- [ ] 清理重复文件与三个历史版本，改由 Git 历史管理；
- [ ] 给作业机器人增加 `--dry-run`，避免误提交；
- [ ] 把 `inclusive/` 的 12 份重复目录改为参数化的单一模板；
- [ ] 用 `python3` 重写 `makejob.sh` 中的 `sed -i` 逻辑，做到幂等；
- [ ] 补齐 `fitkit` 缺失的 `RooMispartition.h`，或移除这两个程序。

### 公开版改动记录

为公开发布，本次相对内部版本做了如下处理（**共修改 62 个文件**）：

**已替换为占位符**

| 类别 | 占位符 | 文件 |
|---|---|---|
| 数据集路径 | `DATA_ROOT` / `/path/to/...` | `SubmitJobyjh/txts/.datasetpath.sh`（整文件重写为模板）、`SubmitJobyjh/script.py`（51 处）、`inclusive/*/MC_01/mc1/makejob.sh`（12）、`xiechen/inclusive/*/…/makejob.sh`（12）、`chi2FoM/Example.cfg`、`fitkit/*.cxx` |
| Run 区间 | `RUN_BEGIN` / `RUN_END` | `SubmitJobyjh/.scripts/SetBOSSVersion.sh`（15 处 `RunIdList`）、`SubmitJobyjh/script.py`（8 个 `runFrom*`/`runTo*` 列表）、`inclusive/*/MC_01/mc1/job.head`（12）、`xiechen/inclusive/*/…/job.head`（12）、`inclusive/generate.sh` 与 `xiechen/inclusive/generate.sh` 的 `runbegin`/`runend`、`xiechen/signal/script.py` 的 `runlist` |

**已删除**

| 内容 | 说明 |
|---|---|
| 两个失效的嵌套 `.git`（顶层 + `iRobot/.git`） | 记录的是旧目录布局，且跟踪了 `__pycache__/*.pyc`；旧历史已备份为 tar.gz |
| `chi2FoM/chi2` | ELF 编译产物 |
| 17 个 `*~` + 4 个 `*.swp` + 2 个 `.DS_Store` + `iRobot/classes/__pycache__/` | 编辑器与系统垃圾 |

**未改动（有意保留）**

- 能量点（`4130`…`4230-scan`）、质心能量（`ecmenergy`、`BeamEnergy`）、`xiechen/signal/xs_user.txt` 截面表 —— 属公开信息；
- BOSS 版本映射（母粒子 ↔ `bossversion`）；
- 作者私有工具路径（`/workfs2/bes/yangjunhui/...`、`/workfs2/bes/zhoux/...`）；

---

## 致谢

本工具箱是站在别人肩膀上的产物：

- **Dr. ZHOU Xiang（周翔，武汉大学）** —— `iRobot/`（WALL·E）的完整 Python 框架、`chi2FoM/chi2.C` 的原始算法，以及 `setupBOSS` / `creatAlg` / `SubmitJob` 系列脚本的**最初版本**。`SubmitJobyjh`、`creatAlgyjh`、`setupBOSSyjh` 中大量脚本头部都标注了 *"modified from Dr.ZhouX's script"*。
- **Niklaus Berger** —— `bes3plotstyle.{C,h}` 的 BESIII 官方绘图风格。
- **BESIII Offline Software Team** —— BOSS 框架与全部 `jobOptions_*.txt` 模板。
- `config.hh` / `config.cc` 的通用配置解析器来自 [CSDN 博客](https://blog.csdn.net/david_xtd/article/details/9320549)，`iRobot/classes/textcolor.py` 的色号参考自 CSDN。

---

## License 与免责声明

本仓库中的代码为个人科研工作中的脚本集合，**未附带任何明示或暗示的担保**。

- `bes3plotstyle.{C,h}` 版权归原作者 Niklaus Berger 及 BESIII 合作组所有；
- `iRobot/`、`chi2FoM/chi2.C` 的原始版权归 Dr. ZHOU Xiang 所有；
- 其余改写部分可自由使用、修改与分发，但请**保留原始出处说明**。

> ⚠️ **重要提醒**：本仓库面向 BESIII 合作组成员。代码中引用的数据集路径与 run 号已在公开版中略去（完整配置见 `/workfs2/bes/yangjunhui/BES3Toolkit`），但**能量点、截面数值、BOSS 版本映射等仍属合作组内部资料**，请勿将内部数据文件本身提交到公开仓库；二次分发前请确认符合 BESIII 合作组的数据与代码发布规范。

---

<p align="center">
  <i>Have fun with BESIII physics! 🎏</i>
</p>
