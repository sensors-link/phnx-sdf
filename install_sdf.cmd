@echo off
REM ����Phoenix��������������������ϵͳ����(����)

echo PHNX_SDF����ɫ���
echo ���ű���������PATH��PHNX_SDF����������ϵͳ��
echo ����setx������޸�ע���ʱһЩϵͳ���������������ȷ��,�����������
echo ����uninstall_sdf����ɾ����Щ����������

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
echo ��ɣ�
echo on
