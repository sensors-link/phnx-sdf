; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "RISCV GCC Toolchain"
#define MyAppVersion "8.2.0"
#define MyAppPublisher "��װ������:�����з������ݿƼ����޹�˾"
#define MyAppURL "http://www.sensors.link/"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{DE8DE2B6-F5AD-4686-8B25-2EE5A36A54D8}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName=C:\riscv-gcc8
DefaultGroupName={#MyAppName}
AllowNoIcons=yes
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
OutputDir=.
OutputBaseFilename=riscv-gcc8
Compression=lzma
SolidCompression=yes
WizardStyle=modern
ChangesEnvironment=true

[Languages]
;Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "chinese"; MessagesFile: "compiler:Languages/Chinese.isl"

[Files]
Source: "riscv-gcc8\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{group}\{cm:UninstallProgram,{#MyAppName}}"; Filename: "{uninstallexe}"

[Code]
var
  infectedPath: String;


function PrepareToInstall(var NeedsRestart: Boolean): String;
begin
	infectedPath := ExpandConstant('{app}');
  Result:='';
end;

function InitializeUninstall(): Boolean;
begin
	infectedPath := ExpandConstant('{app}');
  Result:=true;
end;

#include "inno\updatePath.iss"
