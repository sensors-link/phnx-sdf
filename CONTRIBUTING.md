# 贡献指南

## 项目理念

## 开发环境

### 开发环境安装

### Windows 环境修正

## IDE 设置

-   VSCode
    建议安装插件:
    -   C/C++
    -   C++ Intellisense
    -   Visual Studio IntelliCode
    -   Doxygen Documentation Generator
    -   EditorConfig for VS Code
    -   Markdown Preview Enhanced
    -   GitLens
        建议安装辅助工具：
    -   GNU Global

## 代码规范

### 编辑器规范

-   为保证代码风格一致性，代码编辑规范已预定义到.vscode 配置目录中, 正确设置 IDE 后上述代码规范可自动提示和修复。
    .vscode 设置中已开启"Format On Type"、"Format On Paste"、"Format On Save"功能自动调整代码风格。
    预设代码风格为 "Visual Studio"风格 (基于"LLVM")

-   文件头定义按照 Doxygen 规范设置到.vscode 配置目录中, 使用/\*\*回车调出。

```
/**
 * @file a.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-10-11
 *
 * @copyright Fanhai Data Tech. (c) 2019
 *
 */
```

### C 代码规范

新增代码请使用 GNU 编码规范
https://www.gnu.org/prep/standards/standards.html
中文翻译
https://wenku.baidu.com/view/d2d5ec1fa76e58fafab0032b.html

与编码相关，你可以直接看 5. Making The Best Use of C 章节。

### C++代码规范

TBD

## Issue 规范
