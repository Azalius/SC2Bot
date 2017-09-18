@echo off
git init
git add *.*
set /p VAR_Saisie=    Message commit : 
git commit -m "%VAR_Saisie%"
git push origin master
pause
