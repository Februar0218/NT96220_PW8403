@echo off

if not exist General.cfg (
echo.
echo *****************************************************************
echo Missing General.cfg
echo *****************************************************************
echo.
goto quit
)

echo.
echo *****************************************************************
echo Remove build directory
echo *****************************************************************
echo.

del Log_*.txt /f/q
rd /s/q Application
rd /s/q Library
rd /s/q Project
rd /s/q CHM

echo.
echo *****************************************************************
echo Start to build document
echo *****************************************************************
echo.

md Application
md Library
md Project
md CHM

if exist KernelDoc.cfg (
doxygen KernelDoc.cfg
)
if exist DriverDoc.cfg (
doxygen DriverDoc.cfg
)
if exist SubsystemDoc.cfg (
doxygen SubsystemDoc.cfg
)
if exist AVCodecDoc.cfg (
doxygen AVCodecDoc.cfg
)
if exist ApplicationDoc.cfg (
doxygen ApplicationDoc.cfg
)
if exist ProjectDoc.cfg (
doxygen ProjectDoc.cfg
)

echo.
echo *****************************************************************
echo Done.
echo Go to Cygwin to run MakePDF.sh to create PDF version.
echo *****************************************************************

:quit