del .dump-.strip-.patch-.import-.dune2000.exe
if exist .dump-.strip-.patch-.import-.dune2000.exe exit 0

del .dune2000.exe
if exist .dune2000.exe exit 0

del dune2000.exe
if exist dune2000.exe exit 0

set PATH=C:\win-builds-patch-32\bin
gmake WWDEBUG=1

del "C:\Games\Dune2000_MP_Test\Dune1\dune2000-spawn.exe"
if exist "C:\Games\Dune2000_MP_Test\Dune1\dune2000-spawn.exe" exit 0

del "C:\Games\Dune2000_MP_Test\Dune2\dune2000-spawn.exe"
if exist "C:\Games\Dune2000_MP_Test\Dune2\dune2000-spawn.exe" exit 0

copy /Y dune2000.exe "C:\Games\Dune2000_MP_Test\Dune1\dune2000-spawn.exe"
copy /Y dune2000.exe "C:\Games\Dune2000_MP_Test\Dune2\dune2000-spawn.exe"

cd "C:\Games\Dune2000_MP_Test\Dune1"
start dune2000-spawn.exe -SPAWN
cd "C:\Games\Dune2000_MP_Test\Dune2"
start dune2000-spawn.exe -SPAWN
