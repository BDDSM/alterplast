@echo off

REM ��� ���������� �ந������� html-䠩��� � ᡮથ 1�++ �㦭� �� ������ �������� ����� 䠩��� � ࠧ��� "�祡���" � 䠩�� 1cpp.rst.
REM ��� �ਬ�� ����� ��ᬮ���� ������祭�� 䠩�� TSQLInV7FirstSteps

for %%I in (kb\*.html) do call kb2chm.bat %%~nI %%~dpI %%~xI
