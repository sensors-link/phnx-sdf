@echo off
REM 设置Phoenix开发工具链环境变量到系统环境(永久)

echo PHNX_SDF是绿色软件
echo 本脚本将会设置PATH和PHNX_SDF环境变量到系统。
echo 运行setx命令和修改注册表时一些系统保护软件会请求您确认,请允许操作。
echo 运行uninstall_sdf可以删除这些环境变量。

pause

set TL_PATH=%~dp0
set PHNX_SDF=%TL_PATH%

set PATHTOADD=("%PHNX_SDF%" "%PHNX_SDF%BuildTools\bin" "%PHNX_SDF%arm-gcc9\bin" "%PHNX_SDF%riscv-gcc8\bin")
set NEWPATH=
for /d %%a in %PATHTOADD% do (
	echo %%a
	call :AddToNEWPATH %%~a
)
echo NEWPATH=%NEWPATH%

setx /m PATH "%NEWPATH%;%PATH%"
setx /m PHNX_SDF %PHNX_SDF%
goto :end

:AddToNEWPATH
	set apath=%1
	echo "%PATH%" | findstr /IC:"%apath:\=\\%;" >nul || (
		set NEWPATH=%1;%NEWPATH%
	)
goto:%eof

:end
echo 完成！
echo on
