procedure ModPath(pathdir: TArrayOfString);
var
	oldpath:	String;
	newpath:	String;
	updatepath:	Boolean;
	pathArr:	TArrayOfString;
	i, d:		Integer;
	regroot:	Integer;
	regpath:	String;

begin
	regroot := HKEY_LOCAL_MACHINE;
	regpath := 'SYSTEM\CurrentControlSet\Control\Session Manager\Environment';
    RegQueryStringValue(regroot, regpath, 'Path', oldpath);
	// Get array of new directories and act on each individually
	for d := 0 to GetArrayLength(pathdir)-1 do begin
		updatepath := true;

		// Get current path, split into an array
		RegQueryStringValue(regroot, regpath, 'Path', oldpath);
		oldpath := oldpath + ';';
		i := 0;

		while (Pos(';', oldpath) > 0) do begin
			SetArrayLength(pathArr, i+1);
			pathArr[i] := Copy(oldpath, 0, Pos(';', oldpath)-1);
			oldpath := Copy(oldpath, Pos(';', oldpath)+1, Length(oldpath));
			i := i + 1;

			// Check if current directory matches app dir
			if pathdir[d] = pathArr[i-1] then begin
				// if uninstalling, remove dir from path
				if IsUninstaller() = true then begin
					continue;
				// if installing, flag that dir already exists in path
				end else begin
					updatepath := false;
				end;
			end;

			// Add current directory to new path
			if i = 1 then begin
				newpath := pathArr[i-1];
			end else begin
				newpath := newpath + ';' + pathArr[i-1];
			end;
		end;

		// Append app dir to path if not already included
		if (IsUninstaller() = false) AND (updatepath = true) then
			newpath := newpath + ';' + pathdir[d];

		// Write new path
		//RegWriteStringValue(regroot, regpath, 'Path', newpath);
    Log('New Path: ' + newpath);
  end;
end;

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
			i := i + 1;
		end else begin
			 Dest[i] := Text;
			 Text := '';
		end;
	until Length(Text)=0;
end;


<event('CurStepChanged')>
procedure ChangePath(CurStep: TSetupStep);
begin
	if CurStep = ssPostInstall then
		ModPath(infectedPath);
end;

<event('CurUninstallStepChanged')>
procedure RestorePath(CurUninstallStep: TUninstallStep);
begin
	if CurUninstallStep = usUninstall then
	  ModPath(infectedPath);
end;
