@echo off
set "projname=I3DL2Refurb"
set "backupfolder=..\.____Backups\%projname%_backups\"

echo build>__exclusions.txt
echo __exclusions.txt>>__exclusions.txt

for /f %%I in ('wmic os get localdatetime ^|find "20"') do set dd=%%I
echo %dd%
set dd=%dd:~6,2%-%dd:~4,2%-%dd:~0,4%
set dt=%time:~0,-6%
set dt=%dt::=.%

echo Target = %backupfolder%%projname% %dd% %dt%\
xcopy /E /Q /I /EXCLUDE:__exclusions.txt "*.*" "%backupfolder%%projname% %dd% %dt%\"
del /F /Q __exclusions.txt
