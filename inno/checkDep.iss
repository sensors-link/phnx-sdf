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
      if false = BrowseForFolder('��ѡ��' + toolName + '��װĿ¼',Result, false)
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
  vscodePath := FindToolsPath ('Visual Studio Code', 'Code.exe', 'C:\Program Files (x86)\Microsoft VS Code;' +
      'C:\Program Files\Microsoft VS Code;' +
      'C:\Users\' + GetUserNameString() + '\AppData\Local\Programs\Microsoft VS Code');
  Log('pythonPath=' + pythonPath); 
  Log('vscodePath=' + vscodePath);  
  if (pythonPath = '') or (vscodePath = '') then begin
    MsgBox('PhnxSDF��ҪPython��Visual Studio Code֧�֣����Ȱ�װPython��VSCode',mbInformation, MB_OK);
    Result:=false
  end
  else 
    Result:= true;
end;