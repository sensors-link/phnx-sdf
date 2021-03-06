<event('CurStepChanged')>
procedure installExt(CurStep: TSetupStep);
var
  ResultCode: Integer;
begin
  if CurStep = ssPostInstall then begin
    if WizardIsTaskSelected('pythonExt') then begin
        Exec(pythonPath, ExpandConstant('-m pip install -r {app}\tools\requirements.txt'),
            '', SW_SHOW, ewWaitUntilTerminated, ResultCode);
    end;
    Log(ExpandConstant('--install-extension {app}\temp\austin.code-gnu-global-0.2.2.vsix'));
    if WizardIsTaskSelected('vscodeExt') then begin
        Exec('cmd.exe', ExpandConstant('/C ' + vscodePath + ' --install-extension {app}\temp\austin.code-gnu-global-0.2.2.vsix'),
        '', SW_SHOW, ewWaitUntilTerminated, ResultCode);
        Exec('cmd.exe', ExpandConstant('/C ' + vscodePath + '--install-extension {app}\temp\EditorConfig.EditorConfig-0.15.1.vsix'),
        '', SW_SHOW, ewWaitUntilTerminated, ResultCode);
        Exec('cmd.exe', ExpandConstant('/C ' + vscodePath + '--install-extension {app}\temp\MS-CEINTL.vscode-language-pack-zh-hans-1.51.1.vsix'),
        '', SW_SHOW, ewWaitUntilTerminated, ResultCode);
        Exec('cmd.exe', ExpandConstant('/C ' + vscodePath + '--install-extension {app}\temp\ms-vscode.cpptools-1.0.1.vsix'),
        '', SW_SHOW, ewWaitUntilTerminated, ResultCode);
        Exec('cmd.exe', ExpandConstant('/C ' + vscodePath + '--install-extension {app}\temp\SanaAjani.taskrunnercode-0.3.0.vsix'),
        '', SW_SHOW, ewWaitUntilTerminated, ResultCode);
        Exec('cmd.exe', ExpandConstant('/C ' + vscodePath + '--install-extension {app}\temp\twxs.cmake-0.0.17.vsix'),
        '', SW_SHOW, ewWaitUntilTerminated, ResultCode);
        Exec('cmd.exe', ExpandConstant('/C ' + vscodePath + '--install-extension {app}\temp\VisualStudioExptTeam.vscodeintellicode-1.2.10.vsix'),
        '', SW_SHOW, ewWaitUntilTerminated, ResultCode);
        Exec('cmd.exe', ExpandConstant('/C ' + vscodePath + '--install-extension {app}\temp\zhwu95.riscv-0.0.8.vsix'),
        '', SW_SHOW, ewWaitUntilTerminated, ResultCode);
    end;
    SaveStringToFile(ExpandConstant('{app}\sdf.cmd'), pythonPath + ' %PHNX_SDF%\tools\sdf.py %*', False);
    SaveStringToFile(ExpandConstant('{app}\sdfqc.cmd'), pythonPath + ' %PHNX_SDF%\tools\sdfqc.py %*', False);
  end;
end;

<event('CurUninstallStepChanged')>
procedure uninstallExt(CurUninstallStep: TUninstallStep);
begin
  if CurUninstallStep = usUninstall then begin
    DeleteFile(ExpandConstant('{app}\sdf.cmd'));
    DeleteFile(ExpandConstant('{app}\sdfqc.cmd'));
  end;
end;
