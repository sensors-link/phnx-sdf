@echo off
REM ɾ��Phoenix������������������
echo PHNX_SDF����ɫ���
echo ���ű�����ɾ��PATH��PHNX_SDF����������
echo ����setx������޸�ע���ʱһЩϵͳ���������������ȷ��,�����������
echo ��Ŀ¼�µ��ļ����������ֹ�ɾ����

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
echo ��ɣ�
echo on
