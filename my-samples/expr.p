program expr;

begin
    if 1 = 1 then
        writeln(1)
    else
        writeln(0);

    if 0 * 1 = 1 then
        writeln(1)
    else
        writeln(0);

    if 1 > 1 then
        writeln(1)
    else
        writeln(0);

    if 1 >= 1 then
        writeln(1)
    else
        writeln(0);

    if 1 < 1 then
        writeln(1)
    else
        writeln(0);

    if 1 <= 1 then
        writeln(1)
    else
        writeln(0);

    if (1 = 1) and (1 = 0) then
        writeln(1)
    else
        writeln(0);

    if (1 = 1) or (1 = 0) then
        writeln(1)
    else
        writeln(0);

    if not (1 = 0) then
        writeln(1)
    else
        writeln(0);
end.
