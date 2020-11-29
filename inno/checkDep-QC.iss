function FindToolsPath(toolName, exe, suggestPath: String): String;
begin
  Result := '';
  while Result = '' do
  begin
    Result := FileSearch(exe, GetEnv('PATH'));
    if Result = ''
    then Result := FileSearch(exe, suggestPath);
    if Result = ''
    then begin
      if false = BrowseForFolder('请选择' + toolName + '安装目录',Result, false)
      then break;
      Result := FileSearch(exe, Result);
    end;
  end;
end;

//<event('PrepareToInstall')>
//function CheckPythonAndVSCode(var NeedsRestart: Boolean): String;
<event('InitializeSetup')>
function CheckPythonAndVSCode(): Boolean;
begin
  pythonPath := FindToolsPath ('Python', 'python.exe', '');
  Log('pythonPath=' + pythonPath);
  if pythonPath = '' then begin
    MsgBox('PhnxSDF QC需要Python支持，请先安装Python',mbInformation, MB_OK);
    Result:=false
  end
  else
    Result:= true;
end;
