# 快速入门
## 一、概述
本文档旨在指导用户搭建 Phoenix SoC 硬件开发的软件环境，通过一个简单的示例展示如何使用 PHNX-SDF (Phoenix Sensors Development Framework) 编译、下载固件至 Phoenix 开发板等步骤。

Phoenix SoC 芯片支持以下功能：
- 超低功耗协处理器
- 多种外设
- Phoenix SoC 采用 110 nm 工艺制成，具有最佳的功耗性能、稳定性、通用性和可靠性，适用于各种应用场景和不同功耗需求。

## 二、准备工作
### 2.1 硬件准备

- 一款 Phoenix 开发板
- USB 数据线 (A 转 Micro-B)
- PC（Windows、Linux 或 Mac OS）


#### 开发板简介
请点击下方连接，了解有关具体开发板的详细信息。

    https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-win32.zip?revision=20c5df9c-9870-47e2-b994-2a652fb99075&la=en&hash=347C07EEEB848CC8944F943D8E1EAAB55A6CA0BC

### 2.2 软件准备

#### SDF介绍
- 工具链，用于编译 Phoenix 代码
    - Phoenix02 SoC基于 ARM Cortex M0+ 内核，建议使用GCC for ARM 工具链
    - Phoenix03 SoC基于 RISCV 内核，建议使用GCC for RISCV 工具链
- 编译工具、调试和下载工具
    - Phoenix SDF使用 CMake/GNUMake/Ninja 构建工具，用于构建 Phoenix 应用程序；
    - 泛海数据提供基于开源OpenOCD二次开发的OpenOCD工具用于Phoenix 系列SoC的调试和下载
- 传感器软件开发框架
    - 该框架已经基本包含使用Phoenix 系列SoC开发传感器使用的 API（软件库和源代码）和运行工具链的脚本；

上面相关软件打包到PHNX-SDF SDK中，可从下面网址打包下载ALL-IN-ONE安装包：  
https://pan.baidu.com/s/172nElZ_oiAxmQkqXr1Zr9g
也可以从[资源官网](Resource.md)分别下载。

#### SDF 安装依赖
PHNX-SDF需要Python3的支持，请先安装Python3.7以上版本。

官方下载：  
https://www.python.org/ftp/python/3.8.1/python-3.8.1-amd64.exe  
本地下载：  
https://pan.baidu.com/s/172nElZ_oiAxmQkqXr1Zr9g  

#### SDF安装
下载完Python和PHNX-SDF安装包后，请先安装Python3再安装PHNX-SDF。
PHNX-SDF是绿色软件，下载后请解压到您指定的目录（如: D:\PHNX-SDF）,并运行目录中的install_sdf.cmd脚本。
该脚本将会设定PHNX_SDF环境变量和系统路径。

> uninstall_sdf.cmd脚本用于在您不需要使用PHNX-SDF或变更安装目录时清除环境变量
>
> 以上两个脚本会调用setx命令修改注册表，一些安全软件如360安全卫士/腾讯电脑管家会作出风险提示，请允许操作。

## 三、开发环境
您可以在下面三种开发环境中调用Phoenix SDF进行传感器开发
- 命令行环境
- Microsoft Visual Studio Code集成开发环境
- Microsoft Visual Studio Code + PlatformIO 集成开发环境

### 3.1 命令行环境

- 第一步：开始创建工程
PHNX-SDF安装目录下的examples子目录下是phnx-sdf的示例工程。  
nonos工程是不带操作系统的简易工程，占用资源少，适合简单传感器  
rtos工程是使用FreeRTOS开源实时操作系统的工程，适合有较复杂协议逻辑的物联网传感器  

请复制一个示例工程到你的工作目录，从这个示例工程开始。

- 第二步：编译工程
正确安装Python和PHNX-SDF后，新建命令行提示符，在您的工程目录下运行"sdf build"命令进行工程编译。
```
D:\Work> sdf build
...
...
Build complete. To flash, run this command with 'flash' action.
```

第一次运行sdf命令时会因为缺少Python组件而报错，请根据提示安装所需组件。

- 第三步：烧录到设备
在您的工程目录下运行"sdf flash"命令将编译好的目标文件下载到开发板/目标板。
```
D:\Work> sdf flash
...
...
Done
```
"sdf flash"命令会自动检查文件修改，判断是否需要重新编译。

- 第四步：监视器
在您的工程目录下运行"sdf monitor"命令将启动串口工具连接到开发板/目标板。
```
D:\Work> sdf monitor
...
```
或者，如果有多个串口设备，手工指定串口：
```
D:\Work> sdf -p com3 monitor
...
```

- 第五步：添加新的源文件到工程
PHNX-SDF使用CMake编译工具，在工程中添加新的源文件后，你需要修改CMakeList.txt
```
cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Release)
set(TARGET phnx02)
include("$ENV{PHNX_SDF}/tools/cmake/project.cmake")

file(GLOB PROJECT_SRCS "src/*.c")

project(app)
```

修改其中设置PROJECT_SRCS变量的部分，如
```
file(GLOB PROJECT_SRCS "src/*.c"
    "another_dir/*.c"
    "dir2/a.c"
)
```
或：
```
set(PROJECT_SRCS "src/1.c" "src/2.c"
    "src1/3.c" "src1/4.c"
    "src1/5.c"
    "src2/6.c"
)
```
注：CMake系统中使用file(GLOB PROJECT_SRCS "src/*.c")方式指定的PROJECT_SRCS变量并不会每次都重新搜索src目录下的文件变化。如果您在目录下
增删了源文件，简单的处理办法是修改和保存一下CMakeList.txt文件本身（比如增删一个空格）。

---
注：sdf命令可以在命令行中连续输入，如 sdf build flash monitor表示编译，下载并启动串口监视


### 3.2 VSCode集成开发环境
Microsoft Visual Studio Code是开源、免费、跨平台的代码编辑器/集成开发环境。
PHNX-SDF提供Visual Studio Code的集成功能。
- 第一步：安装Microsoft Visual Studio Code
从官网下载安装：https://code.visualstudio.com/
安装时推荐选中"将Open With VSCode"添加到右键菜单中，便于更方便的打开PHNX-SDF工程。

- 第二步：打开工程安装插件
在工程源文件目录下，点击鼠标右键选择"Open With VSCode"，即可使用Visual Studio Code打开PHNX-SDF工程。
打开PHNX-SDF工程后，Visual Studio Code会提示您安装推荐的插件，包括：
    - Visual Studio Code中文语言包
    - Visual Studio Code C/C++开发工具包
    - Visual Studio Code GNU Global工具包
    - Visual Studio Code ARM 汇编语言支持
    - Visual Studio Code RISCV 汇编语言支持
    - Intelli Code 智能编辑器
    - CMake语法支持
    - EditorConfig编辑器配置规范支持
    - Task Runner
请确认安装启用上面插件。

- 第三步：在Visual Studio Code中编译和下载工程
启用Task Runner插件后Visual Studio Code资源管理菜单中出现Task Runner子菜单
通过Task Runner子菜单中的Build/Clean/Fullclean/Flash/Monitor命令在Visual Studio Code环境中进行工程的编译和下载。
![Tasks](/assets/Tasks.png)

- 第四步：在Visual Studio Code中调试工程
Visual Studio Code支持与调试器集成进行代码调试。
调试方法：
1. 进入VSCode Debug视图
2. 选择命令启动gdb调试
3. PHNX-SDF启动OpenOCD作为gdb server
4. VSCode出现Debug工具栏和调试控制台，进入调试模式

![Debug](/assets/Debug.png)


浏览代码时常用VSCode编辑快捷键:
Ctrl-鼠标左键： 跳转到鼠标所指变量/函数的定义
Alt-键盘左箭头： 回到上一代码上下文




### 3.3 VSCode+PlatformIO集成开发环境
PlatformIO 是开源的物联网开发生态系统，为物联网系统提供跨平台的代码构建和集成开发环境（IDE）。PlatformIO支持200多种MCU/开发板，15种开发平台和10种开发框架。
PHNX-SDF支持与PlatformIO集成提供强大的物联网传感器开发环境。

- 第一步：安装PlatformIO IDE
进入VSCode扩展插件视图，搜索"PlatformIO", 安装PlatformIO IDE
![PlatformIO](/assets/PlatformIO.png)

- 第二步：在PlatformIO中安装PHNX-SDF平台
    1. 在PlatformIO主页Platform视图中选择"Advanced Installation" 高级安装
    2. 在弹出的窗口中输入你PHNX-SDF安装路径下的platformio子目录的URL， URL以file:// 开头，如D:盘A目录应输入file://D:/A 的形式
    3. 点击Install安装

![PlatformIO1](/assets/PIO-Platform1.png)
![PlatformIO2](/assets/PIO-Platform2.png)

- 第三步：在在PlatformIO中创建工程
在PlatformIO主页中选择"New Project"新建工程
![](/assets/PIO-Proj1.png)
在新建工程对话框中
    1. 输入工程名
    2. 选择Phoenix02或Phoenix03开发板
    3. 选择Phoenix02或Phoenix03开发框架
    4. 建议不用默认工程路径，选择你自己的工程路径
![](/assets/PIO-Proj3.png)

- 第四步：在PlatformIO中编译和下载工程
PlatformIO中提供菜单、状态栏、命令行等多种方式访问常用命令。
![](/assets/PIO-Proj4.png)

- 第五步：在PlatformIO中调试工程
在VSCode Debug视图中启用调试