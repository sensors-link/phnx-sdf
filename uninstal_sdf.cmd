@echo off
REM 删除Phoenix开发工具链环境变量
echo PHNX_SDF是绿色软件
echo 本脚本将会删除PATH和PHNX_SDF环境变量。
echo 运行setx命令和修改注册表时一些系统保护软件会请求您确认,请允许操作。
echo 本目录下的文件可以自行手工删除。

pause

setlocal EnableDelayedExpansion

set PATHTODEL=%PHNX_SDF%;%PHNX_SDF%BuildTools\bin;%PHNX_SDF%riscv-gcc8\bin;%PHNX_SDF%arm-gcc9\bin;
set NEWPATH=
set remain=%PATH%

:loop
for /f "tokens=1* delims=;" %%i in ("%remain%") do (
	call :FindInArray "%PATHTODEL%" "%%i"
	if "!RETURN!" == "false" (
	  	if "%NEWPATH%" == "" (
	  		set NEWPATH=%%i
	  	) else (
	  		set NEWPATH=!NEWPATH!;%%i
	 	)
	)
	set remain=%%j
)
if defined remain goto :loop

echo NEW_PATH=%NEWPATH%
setx /m PATH "%NEWPATH%
wmic ENVIRONMENT where "name='PHNX_SDF'" delete
goto :end

:FindInArray
	set ARRAY=%~1
	set TOFIND=%~2
	set RETURN=false
	:loopFindInArray
	for /f "tokens=1* delims=;" %%y in ("%ARRAY%") do (
		if "%TOFIND%" == "%%y" (
			set RETURN=true
			goto :exitFindInArray
		)
		set ARRAY=%%z
	)
	if defined ARRAY goto :loopFindInArray
	:exitFindInArray
	goto:%eof

:end
echo 完成！
echo on
