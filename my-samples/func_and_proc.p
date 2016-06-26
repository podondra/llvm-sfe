program func_and_proc;

procedure pro1(n: integer);
var i : integer;
begin
    n := 5;
    inc(n);
    dec(n);
    for i := 0 to n do begin
        if not (i < 3) or (3 <> 3) and (5 > 3) then
            break;
        write(n);
    end;
    writeln(-1);
end;

procedure pro3(a: integer; b: integer; c: integer);
var i : integer;
begin
    for i := 1 downto 0 do begin
        writeln(a);
        writeln(b);
        writeln(c);
    end
end;

function fun1(n: integer): integer;
begin
    if (n <> 0) then
        fun1 := 1
    else
        fun1 := 2;
end;

function fun2(n: integer; m: integer): integer;
begin
    if (n <> 0) then
        fun2 := n;
    if n >= 0 then
        fun2 := n;
    if n > 0 then
        fun2 := n;
    if n <= 0 then
        fun2 := m;
    if n < 0 then
        fun2 := m;
    if (n = 1) and (m = 1) then
        fun2 := m;
end;

const n = 4;

begin
    pro1(n);
    writeln(n);
    pro3($1FFF, &101, 3);
    writeln(fun1(1));
    writeln(fun2(0, 2));
end.
