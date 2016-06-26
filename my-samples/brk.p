program brk;

var i, j : integer;
begin
    for i := 0 to 10 do begin
        if (i = 5) then
            break;
        j := 0;
        while j <> i do begin
            writeln(j);
            if (j = 5) then
                break;
            inc(j);
        end;
    end
end.
