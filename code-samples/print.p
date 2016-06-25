program print;

var I : integer;
var J : integer;

begin
    I := 0;
    while I < 10 do begin
        for J := 0 to 10 do begin
            writeln(I);
        end;
        inc(I);
    end;
end.
