program proced;

procedure proc(n : integer);
var facti : integer;
begin
    facti := 1;
    while n > 1 do
    begin
        facti := facti * n;
        dec(n);
    end;
    writeln(facti);
end;    

begin
    proc(1);
end.
