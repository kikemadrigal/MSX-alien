@echo off
set TARGET_DSK=disco.dsk

rem sjasm (http://www.xl2s.tk/) es un compilador de ensamblador z80 que puedo convertir tu código ensamblador en los archivos binarios.rom y .bin
rem necesitamos el .bin de la pantalla de carga y del reproductor de música

start /wait tools/sjasm/sjasm.exe src/world0.asm
move /y world0.bin .\bin
rem del /F src\music.lst



rem Copiando todos los archivos.bas de la carpeta src
rem los pegamos en objects y mostramos un mensajito
for /R src %%a in (*.bas) do (
    copy "%%a" obj & echo %%a)
rem Copiando todos los archivos.bin de la carpeta bin
rem los pegamos en objects y mostramos un mensajito
for /R bin %%a in (*.*) do (
    copy "%%a" obj & echo %%a)


rem Le quitamos los comentarios a game.bas
java -jar tools/deletecomments/deletecomments1.4.jar src/main.bas obj/game.bas  

rem Lo tokenizamos
rem start /wait tools/tokenizer/msxbatoken.py obj/game.asc obj/game.bas 



if exist %TARGET_DSK% del /f /Q %TARGET_DSK%
copy tools\Disk-Manager\main.dsk .\%TARGET_DSK%
rem añadimos todos los .bas de la carpeta obj al disco
rem por favor mirar for /?
rem for /R obj/ %%a in (*.bas) do (
    rem start /wait tools/Disk-Manager/DISKMGR.exe -A -F -C %TARGET_DSK% "%%a")  
    start /wait tools/Disk-Manager/DISKMGR.exe -A -F -C %TARGET_DSK% obj/autoexec.bas  
   start /wait tools/Disk-Manager/DISKMGR.exe -A -F -C %TARGET_DSK% obj/loader.bas  
   start /wait tools/Disk-Manager/DISKMGR.exe -A -F -C %TARGET_DSK% obj/game.bas 

rem añadimos todos los arhivos binarios de la carpeta bin al disco
rem recuerda que un sc2, sc5, sc8 es también un archivo binario, renombralo
rem por favor mirar for /?
   for /R bin/ %%a in (*.*) do (
       start /wait tools/Disk-Manager/DISKMGR.exe -A -F -C %TARGET_DSK% "%%a")   

rem abrimos nuestro emulador preferido
rem copy %TARGET_DSK% tools\emulators\BlueMSX
rem start /wait tools/emulators/BlueMSX/blueMSX.exe -diska %TARGET_DSK%
rem start /wait emulators/fMSX/fMSX.exe -diska %TARGET_DSK%


rem MSX 1
rem start /wait tools/emulators/openmsx/openmsx.exe  -ext Sony_HBD-50 -ext ram32k -diska %TARGET_DSK% 
rem start /wait tools/emulators/openmsx/openmsx.exe -script tools/emulators/openmsx/emul_start_config.txt
rem MSX2
   start /wait tools/emulators/openmsx/openmsx.exe -machine Philips_NMS_8255 -diska %TARGET_DSK%
rem start /wait tools/emulators/openmsx/openmsx.exe -machine Philips_NMS_8255 -diska ./obj
rem MSX2+
rem start /wait tools/emulators/openmsx/openmsx.exe -machine Sony_HB-F1XV -diska %TARGET_DSK%