// Split a string into an array using passed delimeter
procedure MPExplode(var Dest: TArrayOfString; Text: String; Separator: String);
var
	i: Integer;
begin
	i := 0;
	repeat
		SetArrayLength(Dest, i+1);
		if Pos(Separator,Text) > 0 then	begin
			Dest[i] := Copy(Text, 1, Pos(Separator, Text)-1);
			Text := Copy(Text, Pos(Separator,Text) + Length(Separator), Length(Text));
			Log(Dest[i]);
      i := i + 1;
		end else begin
			 Dest[i] := Text;
			 Text := '';
		end;
	until Length(Text)=0;
end;

<event('CurStepChanged')>
procedure ChangePath(CurStep: TSetupStep);
var
	oldPath, newPath:	String;
	found:	Boolean;
	oldPathArr:	TArrayOfString;
  infectedPathArr:	TArrayOfString;
  newPathArr:	TArrayOfString;
	i, j, l:		Integer;
	regroot:	Integer;
	regpath:	String;

begin
	if CurStep = ssPostInstall then begin
  regroot := HKEY_LOCAL_MACHINE;
	regpath := 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment';
  RegQueryStringValue(regroot, regpath, 'Path', oldPath);
  MPExplode(oldPathArr, oldPath, ';');
  MPExplode(newPathArr, infectedPath, ';');
  MPExplode(infectedPathArr, infectedPath, ';');
	// Get array of new directories and act on each individually
  l := GetArrayLength(newPathArr);
	for i := 0 to GetArrayLength(oldPathArr)-1 do begin
    if oldPathArr[i] = '' then continue;
    found := false;
    for j := 0 to GetArrayLength(infectedPathArr)-1 do begin
			if oldPathArr[i] = infectedPathArr[j] then begin
				found := true
				break
			end;
    end;
    if not found then begin
      SetArrayLength(newPathArr, l+1);
      newPathArr[l] := oldPathArr[i];
      l := l + 1;
    end;
  end;

  newPath := '';
  for i := 0 to GetArrayLength(newPathArr)-1 do
    newPath := newPath + newPathArr[i] + ';';

	RegWriteStringValue(regroot, regpath, 'Path', newPath);
  Log('New Path: ' + newPath);
  end;
end;

<event('CurUninstallStepChanged')>
procedure RestorePath(CurUninstallStep: TUninstallStep);
var
	oldPath, newPath:	String;
	found:	Boolean;
	oldPathArr:	TArrayOfString;
  infectedPathArr:	TArrayOfString;
  newPathArr:	TArrayOfString;
	i, j, l:		Integer;
	regroot:	Integer;
	regpath:	String;

begin
	if CurUninstallStep = usUninstall then begin
  regroot := HKEY_LOCAL_MACHINE;
	regpath := 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment';
  RegQueryStringValue(regroot, regpath, 'Path', oldPath);
  MPExplode(oldPathArr, oldPath, ';');
  MPExplode(infectedPathArr, infectedPath, ';');
	// Get array of new directories and act on each individually
  l := 0;
	for i := 0 to GetArrayLength(oldPathArr)-1 do begin
    if oldPathArr[i] = ''  then continue;
    found := false;
    for j := 0 to GetArrayLength(infectedPathArr)-1 do begin
			if oldPathArr[i] = infectedPathArr[j] then begin
				found := true
				break
			end;
    end;
    if not found then begin
      SetArrayLength(newPathArr, l+1);
      newPathArr[l] := oldPathArr[i];
      l := l + 1;
    end;
  end;

  newPath := '';
  for i := 0 to GetArrayLength(newPathArr)-1 do
    newPath := newPath + newPathArr[i] + ';';

	RegWriteStringValue(regroot, regpath, 'Path', newPath);
  Log('New Path: ' + newPath);
  end;
end;
