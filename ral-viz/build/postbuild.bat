set BuildConf=%1
set BinDir=bin

call :IncrementBuildNum "source\buildnum.h" BuildNum
call :CopyToBinDir doc

goto :eof

:: -----------------------------------------------
:IncrementBuildNum
    set /p DefBuildNum=<%1
    for /f "tokens=3" %%a in ("%DefBuildNum%") do set BuildNum=%%a
    set /a BuildNum+=1
    set DefBuildNum=#define %2 %BuildNum%
    (echo %DefBuildNum%)>%1
    (echo // do not modify! managed by build/postbuild.bat)>>%1
    exit /b
:: -----------------------------------------------

:: -----------------------------------------------
:CopyToBinDir
    if exist %1\nul (
        xcopy "%1" "%BinDir%\%1\" /c /e /y
    ) else (
        copy "%1" "%BinDir%\"
    )
    exit /b
:: -----------------------------------------------
