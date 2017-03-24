
@echo off

rem ��� ࠡ��� �ਯ� ����室��� ��⠭����� ����� DocUtils (http://docutils.sourceforge.net/index.html)
rem � �ய���� ���� � ���� � ��६����� DUPath (���� ���� � ⥪�� �ਯ�)

set DUPath=L:\Python24\Scripts

rem �� ��� ࠡ��� �ਯ� �㦥� ��⮪��� ।���� sed, 
rem ����� ����� ����� ���ਬ�� � ��⠢� cygwin (www.cygwin.com).

if "%1"=="" goto EXIT

set NAME=%~n1
set INTFLDR=%~dp1site
set intNAME=%INTFLDR%\%NAME%.html
set newNAME=%NAME%.new

if not exist "%INTFLDR%" md "%INTFLDR%"

echo Building %NAME% : html
sed "/.. include:: commonchm.txt/d" "%1" > "%newNAME%"
%DUPath%\rst2html.py --output-encoding CP1251 --date --time --generator "%newNAME%" "%intNAME%" || goto EXIT
del /F /Q %newNAME%

:EXIT
