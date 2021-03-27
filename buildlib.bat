

@echo off
@setlocal EnableDelayedExpansion enableextensions

cd %~dp0

rem @SET COMPILER=MinGW-ARM
@SET COMPILER=GNUArmEmbeddedGCC

if "%COMPILER%" == "MinGW-ARM" (
@SET BASE_PATH=d:\Dev\AmazfitBip_FW\soft\Patch\MinGW-ARM
@SET EABI=arm-eabi
) else (
@SET BASE_PATH=d:\Dev\AmazfitBip_FW\soft\Patch\GNUArmEmbeddedGCC
@SET EABI=arm-none-eabi
)

@echo Setting PATH...OK
@SET COMPILERPATH=%BASE_PATH%\bin
@SET LIBNAME=LIBBIP_%TARGETFWVERSION%
@SET PATH=!PATH!;%COMPILERPATH%

@set AS=%EABI%-as
@SET LD=%EABI%-ld
@SET OBJCOPY=%EABI%-objcopy
@SET GCC=%EABI%-gcc
@SET AR=%EABI%-ar

@SET AS_OPT=-mthumb -mlittle-endian -R -EL -mcpu=cortex-m4
@SET OBJCOPY_OPT=--strip-unneeded -S  -O srec
@SET GCC_OPT=-c -Os -Wa,-R -Wall -mthumb -mlittle-endian -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fno-math-errno
@SET GCC_OPT=%GCC_OPT% -ffunction-sections -fdata-sections
@SET GCC_OPT=%GCC_OPT% -fpie -pie -fpic
@SET "AR_OPT="

@cls
call :echoColor 0F "Starting build..." 1
call :echoColor 0F "COMPILER: %COMPILER%" 1
call :echoColor 0F "BASE_PATH: " & echo %BASE_PATH%

rem @echo Compiling import.s
rem @%AS% -I %COMPILERPATH% %AS_OPT% -o import.o import.s

@SET PARTNAME=Library
rem @echo Compiling %PARTNAME%
SET BUILD_DIR=tmp
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
echo.

for /d %%B in ("src\*latin*", "src\tonlesap*", "src\proxy") do (
SET TARGET=%%~nxB
echo.
@call :echoColor 0D "Compiling target version: "
@call :echoColor 0E "!TARGET!" 1
SET "FW_AS_OPT="
if 		  "!TARGET!"=="latin_1.1.5.36" (
@set FW_AS_OPT=--defsym FW_VERSION=11536 
) else if "!TARGET!"=="latin_1.1.6.48" (
@set FW_AS_OPT=--defsym FW_VERSION=11648
) else if "!TARGET!"=="latin_1.1.5.12" (
@set FW_AS_OPT=--defsym FW_VERSION=11512
) else if "!TARGET!"=="not_latin_1.1.2.05" (
@set FW_AS_OPT=--defsym FW_VERSION=11205 
) else if "!TARGET!"=="tonlesap_2.1.1.50" (
@set FW_AS_OPT=--defsym FW_VERSION=21150 
) else if "!TARGET!"=="proxy" (
@set FW_AS_OPT=--defsym FW_VERSION=0xFACE 
) else (
@call :echoColor 0C "FW Version is unknown" 1
goto :error
)

rem @call :echoColor 09 "Compiling... " 1
@call :echoColor 07 "import.s"
@!AS! -I !COMPILERPATH! -I %~dp0\src !AS_OPT! !FW_AS_OPT! -o %BUILD_DIR%\!TARGET!_import.o %%B\import.s
SET FILES_TO_ARCH=%BUILD_DIR%\!TARGET!_import.o
@call :echoColor 0A "...OK" 1

@set TARGET_FW_VERSION_GCC=!TARGET!
@set TARGET_FW_VERSION_GCC=!TARGET_FW_VERSION_GCC:.=_!
@call :upperCase !TARGET_FW_VERSION_GCC! TARGET_FW_VERSION_GCC
SET GCC_OPT_TARGET=!GCC_OPT! -DFW_VERSION=!TARGET_FW_VERSION_GCC! -DLIB_BIP_H=\"!TARGET!\fw_spec.h\"



if not "!TARGET!"=="proxy" (
@for %%F in ("src\*.c") do ( 
@call :echoColor 07 "%%~nxF"
@!GCC! !GCC_OPT_TARGET! -o "%BUILD_DIR%\!TARGET!_%%~nF.o" "%%F"
@if errorlevel 1 goto :error
@SET FILES_TO_ARCH=!FILES_TO_ARCH! "%BUILD_DIR%\!TARGET!_%%~nF.o"
@call :echoColor 0A "...OK" 1
)
)

SET LIBNAME=libbip_!TARGET!.a
if "!TARGET!"=="proxy" SET LIBNAME=libbip.a
if not exist lib mkdir lib
if exist lib\!LIBNAME! del lib\!LIBNAME!
@call :echoColor 07 "Bulding !LIBNAME!"
@%AR% rsc %AR_OPT% -o lib\!LIBNAME! !FILES_TO_ARCH! 
@if errorlevel 1 goto :error
@call :echoColor 0A "...OK" 1


if not "!TARGET!"=="proxy" (
@call :echoColor 07 "Building relocation tables"

!LD! --defsym main=_start %BUILD_DIR%\!TARGET!_import.o  -o %BUILD_DIR%\!TARGET!_import.elf
if errorlevel 1 goto :error
!OBJCOPY! --dump-section .header=%BUILD_DIR%\!TARGET!_header.bin %BUILD_DIR%\!TARGET!_import.elf
!OBJCOPY! --dump-section .alias=%BUILD_DIR%\!TARGET!_alias.bin %BUILD_DIR%\!TARGET!_import.elf
rem if errorlevel 1 goto :error
!OBJCOPY! --dump-section .addr=%BUILD_DIR%\!TARGET!_addr.bin %BUILD_DIR%\!TARGET!_import.elf
rem if errorlevel 1 goto :error
if exist %BUILD_DIR%\!TARGET!_alias.bin (
if exist %BUILD_DIR%\!TARGET!_addr.bin (
if not exist reloc mkdir reloc
copy /b %BUILD_DIR%\!TARGET!_header.bin+%BUILD_DIR%\!TARGET!_alias.bin+%BUILD_DIR%\!TARGET!_addr.bin reloc\reloc_!TARGET!.bin > nul
)
)
rem if errorlevel 1 goto :error

@call :echoColor 0A "...OK" 1
)

)

rmdir %BUILD_DIR% /S /Q

rem Копирование библиотеки и таблиц релокации в папку для сборки приложений (при необходимости)
rem echo.
rem if exist "lib\libbip.a" (
rem @call :echoColor 02 "Copy libbip.a" 1
rem copy /Y "lib\libbip.a" "d:\Dev\AmazfitBip_FW\soft\Patch\1\libbip" > nul
rem )
rem 
rem @call :echoColor 02 "Copy reloc" 1
rem copy /b /Y reloc\reloc_*.bin "d:\Dev\AmazfitBip_FW\soft\Patch\1\libbip" > nul
rem 
rem echo.
rem @call :echoColor 0E "================================" 1
rem @call :echoColor 0A "Build completed." 1

:done

goto :EOF

:error 
@echo Error!
pause
goto :EOF


:EOF

exit


:upperCase
rem Преобразование текстовой строки к верхнему регистру
rem %1 - исходная строка
rem %2 - переменная для записи результата
  setlocal
  set strTemp=%~1
  for %%a in (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z А Б В Г Д Е Ё Ж З И Й К Л М Н О П Р С Т У Ф Х Ц Ч Ш Щ Ъ Ы Ь Э Ю Я) do set strTemp=!strTemp:%%a=%%a!
  endlocal & set %~2=%strTemp%
exit /b 0
rem ===========================================================
rem ===========================================================
rem 	Вывод заданной строки заданным цветом
rem 	3 параметр если не пустой задает перевод строки
rem   0 = Черный 	8 = Серый
rem   1 = Синий 	9 = Светло-синий
rem   2 = Зеленый A = Светло-зеленый
rem   3 = Голубой B = Светло-голубой
rem   4 = Красный C = Светло-красный
rem   5 = Лиловый D = Светло-лиловый
rem   6 = Желтый 	E = Светло-желтый
rem   7 = Белый 	F = Ярко-белый
:echoColor [Color] [Text] [\n]
 @ if not defined BS for /F "tokens=1 delims=#" %%i in ('"prompt #$H#& echo on& for %%j in (.) do rem"') do set "BS=%%i"
 @ if not exist foo set /p .=.<nul>foo
 @ set "regex=%~2" !
 @ set "regex=%regex:"="%"
 @ findstr /a:%1 /prc:"\." "%regex%\..\foo" nul
 @ set /p .=%BS%%BS%%BS%%BS%%BS%%BS%%BS%%BS%%BS%<nul
 @ if "%3" neq "" echo.
 @exit /b 0
rem ===========================================================
rem A function prints text in first parameter without CRLF 
:EchoN
    
@    <nul set /p strTemp=%~1
@    exit /b 0
