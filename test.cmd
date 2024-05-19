del .dump-.strip-.patch-.import-.dune2000.exe
if exist .dump-.strip-.patch-.import-.dune2000.exe exit 0

del .dune2000.exe
if exist .dune2000.exe exit 0

del dune2000.exe
if exist dune2000.exe exit 0

set PATH=C:\win-builds-patch-32\bin
gmake WWDEBUG=1

del "C:\Spiele\Dune2000\1\dune2000-spawn.exe"
if exist "C:\Spiele\Dune2000\1\dune2000-spawn.exe" exit 0

del "C:\Spiele\Dune2000\2\dune2000-spawn.exe"
if exist "C:\Spiele\Dune2000\2\dune2000-spawn.exe" exit 0

del "C:\Spiele\Dune2000\3\dune2000-spawn.exe"
if exist "C:\Spiele\Dune2000\3\dune2000-spawn.exe" exit 0

del "C:\Spiele\Dune2000\4\dune2000-spawn.exe"
if exist "C:\Spiele\Dune2000\4\dune2000-spawn.exe" exit 0

copy /Y dune2000.exe "C:\Spiele\Dune2000\1\dune2000-spawn.exe"
copy /Y dune2000.exe "C:\Spiele\Dune2000\2\dune2000-spawn.exe"
copy /Y dune2000.exe "C:\Spiele\Dune2000\3\dune2000-spawn.exe"
copy /Y dune2000.exe "C:\Spiele\Dune2000\4\dune2000-spawn.exe"

cd "C:\Spiele\Dune2000"
start LagSim2 C:\Spiele\Dune2000\1\dune2000-spawn.exe C:\Spiele\Dune2000\2\dune2000-spawn.exe
cd "C:\Spiele\Dune2000\3"
start dune2000-spawn.exe -SPAWN
cd "C:\Spiele\Dune2000\4"
start dune2000-spawn.exe -SPAWN
