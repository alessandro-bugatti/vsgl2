for /d /r . %%d in (win32) do @if exist "%%d" echo "%%d" && rd /s/q "%%d"