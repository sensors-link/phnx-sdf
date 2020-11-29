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
    SaveStringToFile(ExpandConstant('{app}\sdfqc.cmd'), pythonPath + ' %PHNX_SDF%\tools\sdfqc.py %*', False);
  end;
end;

<event('CurUninstallStepChanged')>
procedure uninstallExt(CurUninstallStep: TUninstallStep);
begin
  if CurUninstallStep = usUninstall then begin
    DeleteFile(ExpandConstant('{app}\sdfqc.cmd'));
  end;
end;
