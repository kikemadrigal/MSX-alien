1 'MSX 2'
1 'Inicilizamos dispositivo: 003B, inicilizamos teclado: 003E'
10 defusr=&h003B:a=usr(0):defusr1=&h003E:a=usr1(0):defusr2=&H90:a=usr2(0)
20 defusr3=&H41:defusr4=&H44
1 ' color letra negro, fondo letra: azul claro, borde blanco, quitamos las letras que aparecen abajo'
30 color 15,1,1:key off:defint a-z
40 screen 5,2,0
1 'ponemos la pantalla de carga'
45 bload"loader.sc5",s
50 open "grp:" as #1
60 print #1,"Loading xbasic"
70 bload"xbasic.bin",r
80 print #1,"Loading sprites"
1 'Cargamos los sprites'
90 gosub 10000
100 'print #1,"Loading tilemap word 0"
110 'dim mf(512,9):gosub 11100
120 print #1,"Cargando tileset"
130 bload"tileset.sc5",s,32768
500 print #1,"Loading game"
510 load"game.bas",r




1 '---------------------------------------------------------'
1 '------------------------Carga de srites------------------'
1 '---------------------------------------------------------'


1 ' Patrones:'
1 'Plano 0. Player--sprites del 1 al 7'
1 'Rutina cargar sprites con datas basic'
    10000 RESTORE
    1 ' vamos a meter 5 definiciones de sprites nuevos que serán 4 para el personaje y uno para la bola'
    10010 FOR I=0 TO 14:SP$=""
        10020 FOR J=1 TO 32:READ A$
            10025 SP$=SP$+CHR$(VAL(A$))
        10030 NEXT J
        10040 SPRITE$(I)=SP$
    10050 NEXT I
   
    10055 for sp=0 to 14
        10060 a$=""
        10065 for i=0 to (16)-1
            10070 read a
            10075 'vpoke &h7400+i, a
            10080 a$=a$+chr$(a)
        10085 next i
        10090 color sprite$(sp)=a$
    10095 next sp
10100 return 

10120 REM alien
10130 REM SPRITE DATA
10140 DATA 124,248,248,248,124,48,112,248
10150 DATA 187,188,144,248,120,72,200,236
10160 DATA 0,0,0,0,0,0,0,0
10170 DATA 0,0,0,0,0,0,0,0
10180 DATA 124,248,248,248,124,48,112,248
10190 DATA 187,188,144,240,240,96,224,240
10200 DATA 0,0,0,0,0,0,0,0
10210 DATA 0,0,0,0,0,0,0,0
10220 DATA 62,31,31,31,62,12,14,31
10230 DATA 221,61,9,15,30,19,49,115
10240 DATA 0,0,0,0,0,0,0,0
10250 DATA 0,0,0,0,0,0,0,0
10260 DATA 62,31,31,31,62,12,14,31
10270 DATA 221,61,9,15,31,50,34,230
10280 DATA 0,0,0,0,0,0,0,0
10290 DATA 0,0,0,0,0,0,0,0
10300 DATA 15,14,14,15,0,26,16,36
10310 DATA 104,112,112,240,160,192,224,0
10320 DATA 0,0,0,0,0,0,0,0
10330 DATA 0,0,0,0,0,0,0,0
10340 DATA 240,112,112,240,0,88,8,36
10350 DATA 22,14,14,15,5,3,7,0
10360 DATA 0,0,0,0,0,0,0,0
10370 DATA 0,0,0,0,0,0,0,0
10380 DATA 224,224,0,0,0,0,0,0
10390 DATA 0,0,0,0,0,0,0,0
10400 DATA 0,0,0,0,0,0,0,0
10410 DATA 0,0,0,0,0,0,0,0
10420 DATA 7,1,1,7,3,3,7,15
10430 DATA 15,15,27,23,15,12,24,56
10440 DATA 192,128,128,128,128,128,128,192
10450 DATA 192,128,128,128,192,192,192,192
10460 DATA 7,1,1,7,3,3,7,31
10470 DATA 31,27,31,6,14,24,24,24
10480 DATA 192,128,128,128,128,128,128,192
10490 DATA 192,96,96,48,48,48,48,48
10500 DATA 3,1,1,1,1,1,1,3
10510 DATA 3,1,1,1,3,3,3,3
10520 DATA 224,128,128,224,192,192,224,240
10530 DATA 240,240,216,232,240,48,24,28
10540 DATA 3,1,1,1,1,1,1,3
10550 DATA 3,6,6,12,12,12,12,12
10560 DATA 224,128,128,224,192,192,224,248
10570 DATA 248,216,248,96,112,24,24,24
10580 DATA 15,15,1,1,1,1,1,1
10590 DATA 31,18,34,6,0,0,0,0
10600 DATA 240,240,128,128,128,128,128,128
10610 DATA 248,72,68,96,0,0,0,0
10620 DATA 3,3,1,1,1,1,1,1
10630 DATA 3,3,3,7,1,0,0,0
10640 DATA 192,192,128,128,128,128,128,128
10650 DATA 192,192,192,224,128,0,0,0
10660 DATA 0,0,0,0,0,1,3,1
10670 DATA 1,7,3,3,3,7,3,1
10680 DATA 0,0,0,0,0,128,192,160
10690 DATA 176,216,200,216,240,192,192,128
10700 DATA 0,0,0,0,0,0,12,12
10710 DATA 31,31,15,15,15,15,31,31
10720 DATA 0,0,0,0,0,0,48,48
10730 DATA 248,248,240,240,240,240,248,248


10740 REM COLOR MODE2 DATA
10750 DATA 7,7,7,7,7,14,4,4
10760 DATA 4,4,4,4,4,6,6,6
10770 DATA 7,7,7,7,7,14,4,4
10780 DATA 4,4,4,4,4,6,6,6
10790 DATA 7,7,7,7,7,14,4,4
10800 DATA 4,4,4,4,4,6,6,6
10810 DATA 7,7,7,7,7,14,4,4
10820 DATA 4,4,4,4,4,6,6,6
10830 DATA 7,7,7,7,4,4,4,4
10840 DATA 4,4,6,6,6,6,8,6
10850 DATA 7,7,7,7,4,4,4,4
10860 DATA 4,4,6,6,6,6,8,6
10870 DATA 10,15,15,15,15,15,15,15
10880 DATA 15,15,15,15,15,15,15,15
10890 DATA 8,8,8,8,8,8,8,8
10900 DATA 8,10,10,10,10,10,10,10
10910 DATA 8,8,8,8,8,8,8,8
10920 DATA 8,10,10,10,10,10,10,10
10930 DATA 8,8,8,8,8,8,8,8
10940 DATA 8,10,10,10,10,10,10,10
10950 DATA 8,8,8,8,8,8,8,8
10960 DATA 8,10,10,10,10,10,10,10
10970 DATA 8,8,8,8,8,8,8,8
10980 DATA 10,10,10,10,15,15,15,15
10990 DATA 8,8,8,8,8,8,8,8
11000 DATA 10,10,10,10,15,15,15,15
11010 DATA 15,15,15,15,15,15,15,15
11020 DATA 8,8,8,8,8,8,8,8
11030 DATA 13,13,13,13,13,13,10,10
11040 DATA 13,13,13,13,13,13,13,13







1 'Cargar mundo con los mapas de los niveles en el buffer o array'
    11100 bload"world0.bin",r
    11110 md=&hd001
    11120 for f=0 to 24-1
        11130 for c=0 to 100-1
            11140 tn=peek(md):md=md+1
            11150 m(c,f)=tn
        11170 next c
    11180 next f
11190 return



1 '1 'Cargar mapa de disco y meterlo en la VRAM
1 '    11400 'md=&hd001
1 '    11410 md=6144: 'Hasta 6912'
1 '    11450 for f=0 to 24-1
1 '        11460 for c=0 to 32-1
1 '            1 'Como los tiles los habíamos cargado previamente en RAM ahora solo los pasamos a VRAM'
1 '            11470 vpoke md,m(c,f)
1 '            11480 md=md+1
1 '        11490 next c
1 '    11500 next f  
1 '11510 return
