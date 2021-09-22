1 '==============='
1 'MSX Murcia 2021'
1 '==============='


1 '0-1000: inicialización
1 'Sistemas'
1 '     game loop:1000-1100 '
1 '     input system:2000-3999  ' 
1 '     Physics system: 4000-4999 '
1 '     Render system & collision:5000-5999 '
1 '     HUD / Score board: 6000-6090 '
1 'Managers'
1 '     Game managar'
1 '         al (numero aleatorio)'
1 '         time (varible de basic)'
1 '         tf (tile suelo):te (tile determina final):tw (tile del mundo actual para repintar):td (tile muerte)'
1 '         wc (objetos por coger en el mundo)
1 '     Sound manager: 7000-7199'
1 '         re (música a reproducir)'
1 'Entities'
1 '     player:10000-10999'
1 '         px (eje x):py (eje y):pw (ancho):ph (alto):pv (velocidad horizontal):pl (velocidad vertical):pj:pa:pd (dirección)
1 '         p1 (sprite 1):p2 (sprite 2):p3 (sprite 3):p4 (sprite 4)'
1 '         pp,ps,pe
1 '     disparo: 11000-11999'
1 '         dm:dn:dd:ds:dc:dp:dw:dh
1 '         DIM dx(dm),dy(dm),di(dm),dv(dm),dp(dm),dt(dm)'
1 '     Enemigos: 12000-12999'
1 '         em=3:en=0:es=7'
1 '         DIM ex(em),ey(em),ev(em),el(em),es(em),ep(em),ec(em),ee(em)'
1 '     Boss:12800-12899'
1 '     Map & scroll: 13000-13999'
1 '         dim m(0,0):mc:mu:ms:mu'
1 'Screens'
1 '     Pantalla de Bienvenida y records :14000-14099'
1 '     Pantalla final ganadora'
1 '     Mundo 0'
1 '         Screen 0-5'
1 '     Mundo 1'
1 '         Screen 6-11'
1 '     Mundo 2'
1 '         Screen 11-16'

10 defint a-z
1 'Inicilizamos dispositivo: 003B, inicilizamos teclado: 003E, incializamos y preparamos el sonido:&H90'
20 defusr=&h003B:a=usr(0):defusr1=&h003E:a=usr1(0):defusr2=&H90:a=usr2(0)
1 'Enlazamos con las rutinas de la bios para aparagar y encender la pantalla'
20 defusr3=&H41:defusr4=&H44
1 'Mostrar pantalla de información / bienvenida'
30 goto 14000
1 'Inicializar mapa, cargamos mapas en array y lo pintamos'
100 print #1, "!Cargando mapa en array (en RAM)"
1 'Inicializamos la variables globales del juego''
105 gosub 6500
1 '13100 Cargar mapa con ma=mundo activo'
110 gosub 13000: gosub 13100
1 'Inicializamos el personaje'
130 gosub 10200
1 'Inicializamos los disparos'
140 gosub 11000
1 'Inicializamos los enemigos'
150 gosub 12000
1 'Mstramos los enemigos de la pantalla 1'
160 gosub 20100
1 'Rutina barra espaciadora pulsada
520 strig(0) on:on strig gosub 10500


1 'Mostrar scoreboard'
580 gosub 6000


 
1 'Solo se saldrá de este bucle si se ha llegado al final de la pantalla'
1 ' ----------------------'
1 '      MAIN LOOP
1 ' ----------------------' 
    1 'bluce principal'
    1 'Capturamos las teclas'
    1000 gosub 2000
    1 'Chequeamos la física'
    1020 gosub 4000
    1 'render'
    1040 gosub 5000
    1 'Si al player no le quedan vidas, volvemos al principio'
    1050 if pv<=0 then gosub 1100: goto 130
    1 'Si el player ha hecho colisión con el tile end (te) es que vamos a cambiar de mundo:
    1 '         Quitamos el sprite del player para que no se vea
    1 '         Aumentamos en 2 el mapa activo (ma) y llamaos a la rutina de cargar mapa
    1 '         Si se ha llegado al último mundo hemos ganado y vamos a la pantalla ganadora'
    1 '         Ponemos de nuevo la variable que controla que hay que volver a cargar los mapas (mu) a 0'
    1055 if mu=1 then put sprite 0,(0,212),,p1:ma=ma+1:gosub 13100:mu=0:if ma>5 then goto 14100
    1 'Debug'
    1060 'gosub 6100
1090 goto 1000
1 ' ----------------------'
1 '    FINAL MAIN LOOP
1 ' ----------------------'

1 ' Rutina reiniciar juego'
    1100 erase ex,ey,ev,el,es,ep,ec,ee
    1110 erase dx,dy,dv,dp
    1120 erase m
1130 return




1 ' ----------------------------------------------------------------------------------------'
1 '                                     SYSTEMS
1 ' ----------------------------------------------------------------------------------------'

1 ' ----------------------'
1 '     INPUT SYSTEM
1 ' ----------------------'
1 '2 Sistema de input'
    1 'Nos guardamos las posiciones del player antes de cambiarlas'
    2000 on stick(0) gosub 2200,2400,2600,2800,3000,3200,3400,3600
2190 return
1 '1 Arriba'
    1 'Saltamos y reproducimos un sonido'
    2200 if pa<>1 then po=py:pa=1
2290 return
1 '2-saltando-derecha'
    2400 if pa<>1 then po=py:pa=1
    2410 px=px+pv
    1 'Ponemos que el layer va en dirección derecha'
    2430 swap p1,p2
2490 return
1 '3 derecha'
    2600 px=px+pv
    2610 pd=3
    2630 swap p1,p2
2690 return
1 '4-abajo derecha'
     2800 'nada'
2890 return
1 '5 abajo'
    3000 'nada'
3090 return
1 '1 '6-abajo-izquierda'
    3200 'nada'
3290 return
1 '7 izquierda'
    3400 px=px-pv
    3410 pd=7
    3440 swap p3,p4
3490 return
1 '8 salktando izquierda'
    3600 if pa<>1 then po=py:pa=1
    3610 px=px-pv
    3630 swap p3,p4
3690 return



1 ' ----------------------'
1 '    Physics system
1 ' ----------------------'
1'chequeando contorno sprite personaje
    4000 'Player'
    1 'Chekeo de llegar al final del mundo, mc=mapa contador'
    4010 if mc>=200-32 then print #1,"!FINAL": return
    1 'Chequeo final del screen
    1 'sacamos al personaje de la pantalla 10300, 
    1 'lo pintamos 
    1 'movemos la pantalla 13600
    1 'Reposicionamos al principio al player'
    1 'eliminamos todos los enemigos 12700
    1 'aumentamos el contador de pantalla y actualizamos para que pinte los enemigos y los objetos 13800'
    1 'Pintamos el marcador'
    4015 if px>255 then put sprite 0,(0,212),,p1: gosub 13600:py=18*8:px=0:gosub 12700:ms=ms+1:gosub 13800:gosub 6000
    1 'Chekeo pantalla'
    4020 if px<0 then px=0
    1 'Si la posición y es mayor que 180 es que te has caido y morimos'
    4030 if py>180 then goto 10400
    1 'Obtenemos el tile actual del player' 
    4035 tx=px/8+mc:ty=py/8:if px <=0 then tx = 0:if py<=0 then ty=0 
    1 'Colision bloque derecha
    1 'tf=tile floor, o tile suelo o sólido, a partir de las posición 160 empiezan a definirse los tiles que no se pueden pasar, ver imagen tileset: https://github.com/kikemadrigal/MSX1-MSX2-assambler-basic-fusion-c-alien'
    4040 t0=m(tx,ty+1)
    1 'Si el tile 0 es un objeto collectable:
    1 'Metemos en el array el tile del fondo del mundo por defecto (linea 20000)'
    1 'pintamos 2 veces (para cubrir el bloque antiguo) el tile world definido en el mundo (línea 20000)
    1 'restamos 1 a os que faltan por coger'
    1 'Pintamos el HUD 6000'
    1 'hacemos un sonido'
    1 'copy ((tw-64)*8,2*8)-(((tw-64)*8)+7,(2*8)+7),1 to (px,py+8),0:copy ((tw-64)*8,2*8)-(((tw-64)*8)+7,(2*8)+7),1 to (px+4,py+8),0:copy ((tw-64)*8,2*8)-(((tw-64)*8)+7,(2*8)+7),1 to (px-4,py+8),0'
    4045 if t0<26 then m(tx,ty+1)=tw:line (px-8,py+8)-(px+8,py+16),14,bf : wc=wc-1:gosub 6000:re=6:gosub 7000
    1 'Si es un bloque de muerte, morimos'
    4050 if t0=td then gosub 10400:beep
    4055 _turbo on (m(),te,t0,t3,t5,t7,tx,ty,mu)
        1 'Si chocamos con un tile de final de pantalla  volvemos a cargar el mundo'
        4058 if t0=te and wc=0 then mu=1
        1 'Ibtenemos el tile que hay en la derecha'
        4060 t3=m(tx+1,ty+1)
        1 'Tile de abajo' 
        4070 t5=m(tx,ty+2)
        1 'tile de la izquierda'
        4080 t7=m(tx,ty+1) 
    4085 _turbo off

    1 'Si el tile de la derecha es mayor que 192' 
    4090 if t3>=tf and pa=0 then px=px-pv else if t7>=tf and pa=0 then px=px+pv
    1 ' Control del salto'
    1' Si el tile del suelo es mayor que el del suelo guardado y estamos cayendo reiniciamos
    4110 if pa=1 and t5>=tf and pl<0 then pa=0:pl=-pl
    4120 if pa=1 then py=py-pl 
    4130 if pa=1 and py<po-16 then pl=-pl
    4140 if pa=1 and py>po then py=po:pl=-pl:pa=0

    1 'Sin saltar '
    1 'Gravedad: si el tile de debajo es menor que el definido de tipo suelo le sumamos la velocidad y'
    1 'Asi solo parará de bajar al player cuando p5 sea mayor 160'
    4150 if pa=0 and t5<tf then py=py+pl

4290 return

1 ' --------------------------------------------'
1 '         RENDER SYSTEM & COLLISION
1 ' --------------------------------------------'
    1 'Boss'
    5000 if bo=1 then gosub 12850
    1 'Player'
    5010 if pd=3 or pd=1 or pd=2 then put sprite 0,(px,py),,p1 
    5020 if pd=7 then put sprite 0,(px,py),,p3


    1 'Shots'
    5100 gosub 11700

    1 'Enemies'
    5200 if en<=0 then return 
    5210 for i=1 to en
        5220 ex(i)=ex(i)+ev(i)  
        1 'Si recorre 20 pasos le cambiamos la velocidad'
        5230 if ex(i) mod 5=0 then ev(i)=-ev(i)
        1 ' si está moviendose hacia la izquierda le ponemos 2 sprites más'
        5240 if es(i)>=7 or es(i)<=9 then if ev(i)>0 then es(i)=7 else es(i)=9   
        5250 if es(i)>=11 or es(i)<=13 then if ev(i)>0 then es(i)=11 else es(i)=13  
        
        5260 put sprite ep(i),(ex(i),ey(i)),,es(i)
        1 'Colisión del player con el enemigo'
        5270 if px < ex(i) + 16 and  px + 16 > ex(i) and py < ey(i) + 16 and 16 + py > ey(i) then gosub 10400
        1 'Colision del enemigo con un disparo'
        5280 for w=1 to dn
            1 'Si hay colisión con el disparo eliminamos el disparo y el enemigo y hacemos un sonido'
            5290 if dx(w) < ex(i) + 16 and  dx(w) + dw > ex(i) and dy(w) < ey(i) + 16 and dy(w) + dh > ey(i) then ed=i:gosub 12600:dd=w:gosub 11600:re=7:gosub 7000
        5300 next w
    5310 next i
5990 return



1 ' ----------------------'
1 '    HUD/Score board
1 ' ----------------------'
    6000 line (0,184)-(256,212),1,bf 
    6010 preset (10,186):print #1,"!Capturas que faltan: "wc
    6020 preset (10,194):print #1,"!Vidas: "pv" Level: "mu"-"ms
    6030 if bo=1 then preset (10,202):print #1,"!Energia boss: "be
6090 return

1 'Debug'
    6100 'preset (10,30):print #1," bo "bo" dt(1) "dt(1)
    6105 preset (10,40):print #1," TIME "TIME/60
6190 return

1 ' ----------------------------------------------------------------------------------------'
1 '                                    END SYSTEMS
1 ' ----------------------------------------------------------------------------------------'







1 '---------------------------------------------------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'
1 '----------------------------------Managers---------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'

1 ' ----------------------'
1 '    Game manager'
1 ' ----------------------'
1 'Init
    6500 al=0:tf=0:te=0:tw=0
    6510 time=0 
    1 'variables player para detectar colisiones en los alrededores de un tile (por ejemplo el que ocupa el player en el array)
    6550 t1=0:t3=0:t5=0:t7=0  
6990 return


1 ' ----------------------'
1 '    Sound manager'
1 ' ----------------------'
1 'Reproductor de efectos d sonido'
    7000 a=usr2(0)
    7010 if re=1 then PLAY"O5 L8 V4 M8000 A A D F G2 A A A A r60 G E F D C D G R8 A2 A2 A8","o1 v4 c r8 o2 c r8 o1 v6 c r8 o2 v4 c r8 o1 c r8 o2 v6 c r8"
    1 'Tirando el paquete'
    7050 if re=5 then play "l10 o4 a"
    7051 if re=6 then play "t110 o4 d"
    7052 if re=7 then play "l10 o4 f f"
    1 'Paquete cogido'
    7060 if re=16 then play"t250 o5 v12 d v9 e" 
    1 'Pitido normal'
    7070 if re=17 then play "O5 L8 V4 M8000 A A D F G2 A A A A"
    1 'Toque fino'
    7080 if re=18 then PLAY"S1M2000T150O7C32"
    1 'Pasos'
    7090 if re=19 then PLAY"o2 l64 t255 v10 m6500 c"
    7100 if re=20 then sound 6,5:sound 8,16:sound 12,6:sound 13,9
7190 return








































1 '---------------------------------------------------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'
1 '----------------------------------Entities---------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'


1 '-----------------------------------'
1  '            PLAYER 10200-10999
1 '-----------------------------------'


1 'Init player'
    1 'Componente position'
    1 'la posición se define en las pantallas, pw=ancho, ph=alto, pv=velocidad, capturas, etc'
    1 'pj=indica si el salto está activado para desactivar la comprobación del teclado'
    1 'po=player y old'
    1 'pa=player salto activado'
    1 'pj=distancia que recorre cuando el salto está activado'
    1 'pd=player dirección'
    1 'Variables player para la física'
    10200 px=16:py=13*8:pw=16:ph=16:pv=4:pl=8:pj=0:pa=0:pd=3'dim j(7):po=0:pd=3
    1 'Array de salto'
    10210 'j(0)=-8:j(1)=-8:j(2)=-8:j(3)=0:j(4)=8:j(5)=8:j(6)=8
    1 'Apartir del 5 sprite es la izquierda'
    1 'p0 mirando a la derecha 1'
    1 'p1 mirando a la derecha 2'
    1 'p2 mirando a la izquierda 1'
    1' p3 mirando a la izquierda 2
    1 'p4 saltando a la derecha'
    1 'p5 saltando a la izquierda'
    1 '10230 dim p(3):p(0)=2:p(1)=3:p(2)=7:p(3)=8:p1=0:p2=1:p3=2:p4=4
    10230 p1=0:p2=1:p3=2:p4=3
    1 'Componente render:'
    10240 pp=0:ps=0
    1 'Componente RPG pe= energía o vida'
    10250 pe=100
10290 return



1 'Rutina player muere'
    10400 py=18*8:px=0
    10410 pv=pv-1
    1 'Pintamos el HUD'
    10420 gosub 6000
    1 'Hacemos un sonido'
    10430 re=6: gosub 7000
10490 return

1 ' Rutina barra espaciadora pulsada'
    1 'Ponemos un sonido de disparo'
    10500 re=5: gosub 7000
    1 'Creamos el disparo en la posición del player
    10510 gosub 11500
10590 return


















1' ------------------------------------------------------------------------------'
1' -------------------------Rutinas disparos / fires / Shots---------------------'
1' ------------------------------------------------------------------------------'
1 'Rutina inicializar disparos'
1 'dm= disparos maximos, maximo expacio reservado para los disparos en un array'
1 'dn= disparo número, sirve para ir creando disparos ya que despues se incrementa'
1 'dd= disparo destruido, variable utilizada para eliminar disparo (ver linea 11600 y 11690)'
1 'ds= disparo sprite'
1 'dc= disparo color'
1 'dp= valor de inicio en el plano para los disparos, como queremos que comience en el plano 1 le ponemos un 0, el plano 0 es el del player'
1 'dw=disparo ancho'
1 'dh=dispar alto'
    11000 dm=5:dn=0:dd=0:ds=6:dc=6:dp=0:dw=8:dh=2
    1 'dx= coordenada disparo x'
    1 'dy= coordenada disapro y'
    1 'di=disparo dirección
    1 'dv= velocidad disapro horizintal'
    1 'dp= plano diparo asignado'
    1 'dt=disparo tipo, determina si es del player (0), del boss (1) o de un enemigo (2)'
    11010 DIM dx(dm),dy(dm),di(dm),dv(dm),dp(dm),dt(dm)
11060 return

1 ' Crear disparo'
    1 'No podemos crear más disparos que em'
    11500 if dn>=dm then return else dn=dn+1
    1' posicionamos la bala, el 8 es para que la bala aparezca junto al cañón
    11510 dx(dn)=px+8:dy(dn)=py+8
    1 'Le asugnamos la dirección del player al disparo'
    11520 di(dn)=pd
    1 'Le asignamos la velocidad horizontal'
    11530 dv(dn)=8
    1 'Le decimos que el disparo es del player'
    11535 dt(dn)=0
    1 'El plano de los disparos comienza en 1 porque el 0 es el player, despues se hirá incrementando según el número de disparos'
    11540 ds(dn)=ds:dp(dn)=dp+dn
11580 return

1 ' Rutina eliminar disparos'
    1 'quitamos de la pantlla el último rastro dibujado del disparo eliminado'
    11600 put sprite dp(dd),(0,212),dc,ds(dd)
    1 'primero metemos en el disparo eliminado el último disparo que se está dibujando'
    11610 dx(dd)=dx(dn):dy(dd)=dy(dn):dv(dd)=dv(dn):ds(dd)=ds(dn):dp(dd)=dp(dn)
    11650 dn=dn-1
11690 return


1 'Render / Update Shots'
    11700 if dn<=0 then return 
    1 'El dn(0) no se utiliza porque es una plantilla'
    11710 for i=1 to dn
        1 ' A cada uno le restamos o sumamos la velocidad según su dirección'
        11720 if di(i)=1 or di(i)=2 or di(i)=3 then dx(i)=dx(i)+dv(i) else dx(i)=dx(i)-dv(i)   
        
        11730 if dt(i)=0 then put sprite dp(i),(dx(i),dy(i)),dc,ds(i) else if dt(i)=1 then put sprite dp(i),(dx(i),dy(i)),dc,18
        11740 if dx(i)<0 or dx(i)>256-16 then dd=i:gosub 11600
        1 'Colisión del disparo del enemigo con el player, dt=1 determina que la bala es del boss, si hay colisión el player muere (10400)'
        11741 'if bo=1 and dt(i)=1 then if px < dx(i) + dw and  px + 8 > dx(i) and py < dy(i) + dh and 16 + py > dy(i) then gosub 10400:gosub 11600:beep
        1 'Colision del disparo del player con el boss'
        1 'Si el modo boss está activado y la bala es del player dt()=0 comprobamos la colisión'
        1 'Si hay colisión:'
        1 '     Le restamos la enemrgia al boss'
        1 '     Mostramos la energia del boss 6000'
        1 '     Eliminamos el disparo dd=i:gosub 11600'
        1 '     Hacemos un sonido'
        11742 if bo=1 and dt(i)=0 then if dx(i) < bx + bw and  dx(i) + dw > bx and dy(i) < by + bh and dy(i) + dh > by then be=be-10:dd=i:gosub 6000:gosub 11600:beep
        
    11750 next i
11790 return




1 '---------------------------------------------------------'
1 '------------------------ENEMIES -------------------------'
1 '---------------------------------------------------------'
1 'Init
    1 'Definiendo el espacio para los arrays con los valores de los enemigos' 
    1 'em=enemigos maximos,reservamos el espacio en RAM para 3 enemigos''
    1 'en=enemigo numero, variable utilizar para gestionar la creación y destrucción de enemigos'
    1 'es =variable utilizada para hacer la animación del enemigo'
    12000 em=3:en=0:es=7
    1 'Componente de posicion'
    1 'ex()=coordenada x, ey=coordenada y', e1=coordenada previa x, e2=coordenada previa y
    1 'Componente de fisica'
    1 'ev()=velocidad enemigo eje x, el=velocidad eje y'
    1 'Componente de render'
    1 'es()=enemigo sprite, puede ser el normal que es el 7,8,9,10, el palo 11 y 12
    1 'ep()=enemigo plano'
    1 'Componente RPG'
    1 'ee()=enemigo energia '
    1' ec()=enemigo contador
    12010 DIM ex(em),ey(em),ev(em),el(em),es(em),ep(em),ec(em),ee(em)
12030 return

1 ' Crear enemigo'
    1 ' si el numero de enemigos creados es mayor que enemigos máximos volvemos para no crear más'
    12500 if en>=em then return else en=en+1
    1 'Establecemos las posiciones del enemigo por defecto, después cuando definamos el screen las modificaremos'
    12510 ex(en)=0:ey(en)=0
    1 'Le asignamos la velocidad horizontal y vertical'
    12530 ev(en)=8:el(en)=8
    1 'Los enemigos son a partir del sprite 7
    12540 es(en)=es:ep(en)=9+en
    12560 ee(en)=100
12590 return

1 ' Rutina eliminar enemigo'
    12600 if en<=0 then return
    12600 ex(ed)=ex(en):ey(ed)=ey(en):ev(ed)=ev(en):el(ed)=el(en):es(ed)=es(en):ep(ed)=ep(en):ec(ed)=ec(en):ee(ed)=ee(en)
    12610 put sprite ep(ed),(0,212),,es(ed)
    12650 en=en-1
12660 return

1 'Rutina eliminar todos los enemigos'
    12700 en=0
12790 return
1 '---------------------------------------------------------'
1 '------------------------boss -------------------------'
1 '---------------------------------------------------------'
1 'bo=indica si el modo boss está activado'
1 'be=boss energía'
1 'bi=posicion antigua eje x
1 'bz=posición antigua eje y'
1 'bd=boss dirección 2,3,5 derecha, 6,7,8 izquierda'
12800 bo=0:bx=0:bw=24:bh=16:bz=0:bi=0:by=0:be=100:bv=8:bd=3
12810 return

1 ' Render & coliision booss'
    12850  bi=bx:bz=by:by=by-bv
    1 '     Si es el boss 0 borramos la antigua imagen con un line relleno de gris y lo volvemos a copiar en la nueva posición'
    1 '     Si se sale de 100 y 154 invertimos la velocidad'
    12855 if bn=0 then line (bx,bz)-(bx+bw,bz+bh),14,bf :copy (24,64)-(48,80),1 to (bx,by),0,tpset: if by<100 or by>154 then bv=-bv 
    1 '     Si al boss no le queda energía:
    1 '         Quitamos el modo boss'
    1 '         Dibujamos un rectángulo gris encima'
    1 '         Metemos en el array m(200,26) el tile end para que cuando colisione con él nos mande al siguiente mundo'
    1 '         Copiamos en la posición 28*8,21*8) la imagen del tile end'
    1 '         Hacemos un sonido'
    1 '     Si le queda energía al boss'
    1 '         Creamos un número aleatorio
    1 '         Si el número aleatorio es mayor que 9 creamos un disparo, ponemos que es del booss(dt(en)=1) le ponemos la posición del boss, la dirección (di) hacia la izquierda'
    1 '12890 if be<=0 then bo=0:line (bx,by)-(bx+bw,bz+bh),14,bf:m(28,20)=te:m(28,21)=te:copy (te*8,0*8)-((te*8)+8,(0*8)+8),1 to (28*8,21*8),0,tpset:re=2:gosub 7000 else 'al=rnd(1)*10:if al>=9 then gosub 11500: dt(dn)=1: di(dn)=bd: dx(dn)=bx:dy(dn)=by
    12890 if be<=0 then bo=0:line (bx,by)-(bx+bw,bz+bh),14,bf:m(195,20)=te:m(195,21)=te:copy (te*8,0*8)-((te*8)+8,(0*8)+8),1 to (28*8,21*8),0,tpset:re=2:gosub 7000 else if time/60>10 then gosub 11500: dt(dn)=1: di(dn)=bd: dx(dn)=bx:dy(dn)=by:time=0
12895 return






1 '-----------------------------------'
1 '-------------MAP & SCROLL-----------'
1 '-----------------------------------'

1 'Init map'
    13000 'print #1, "!Pintando mapa"
    1 'mc=counter map, nos dice en que columna debemos de empezar a escribir, tans solo se utiliza en el desplazamiento de la pantalla
    1 'ma=mapa activo, según el mapa activo cargaremos un archivo.bin u otro del disco en el array (en la RAM)'
    1 'ms=mapa screen, nos dice la pantalla en la que estamos dentro del mundo va del 0 al 15 mundo 1, 6 al 11 mundo 2 y del 12 al 17 mundo 3'
    1 'mu=si está a 1 tendremos que volver a llamar a la rutina cargar mundo para redibujar los mapas'
    13010 dim m(200,26):mc=0:ma=0:ms=0:mu=0
13020 return

1 'Cargar mundo con los mapas de los niveles en el buffer o array y finalmente lo pintará'
    13100 'esto almacenará el array a partir de la posición hc901 de la VRAM'
    13110 if ma=0 then cls: preset(0,100):print #1,"!Pintando Mundo 1, espere...":bload"world0.bin",r:gosub 20000
    13115 if ma=1 then cls: preset(0,100):print #1,"!Pintando Mundo 2, espere...":bload"world1.bin",r:gosub 21000
    13120 md=&hc901

    13130 for f=0 to 23-1
        13140 for c=0 to 200-1
            13150 tn=peek(md):md=md+1
            13160 m(c,f)=tn
        13170 next c
    13180 next f
    1 'Copiando mundo en page 0'
    13185 gosub 13300
13190 return

1 'Aquí vamos dibujando las pantallas de un mundo,pintamos en la VRAM page 0, los valores definidos en el array hasta la columna 32
    13300 _turbo on (m())
    13310 for f=0 to 23-1
        13360 for c=0 to 32-1
            13370 tn=m(c,f)
            13380 if tn >=0 and tn <32 then copy (tn*8,0*8)-((tn*8)+8,(0*8)+8),1 to (c*8,f*8),0,tpset
            13390 if tn >=32 and tn <64 then copy ((tn-32)*8,1*8)-(((tn-32)*8)+8,(1*8)+8),1 to (c*8,f*8),0,tpset
            13400 if tn >=64 and tn <96 then copy ((tn-64)*8,2*8)-(((tn-64)*8)+8,(2*8)+8),1 to (c*8,f*8),0,tpset
            13410 if tn>=96 and tn <128 then copy ((tn-96)*8,3*8)-(((tn-96)*8)+8,(3*8)+8),1 to (c*8,f*8),0,tpset
            13420 if tn>=128 and tn <160 then copy ((tn-128)*8,4*8)-(((tn-128)*8)+8,(4*8)+8),1 to (c*8,f*8),0,tpset
            13430 if tn>=160 and tn <192 then copy ((tn-160)*8,5*8)-(((tn-160)*8)+8,(5*8)+8),1 to (c*8,f*8),0,tpset
            13440 if tn>=192 and tn <224 then copy ((tn-192)*8,6*8)-(((tn-192)*8)+8,(6*8)+8),1 to (c*8,f*8),0,tpset
            13450 if tn>=224 and tn <256 then copy ((tn-224)*8,7*8)-(((tn-224)*8)+8,(7*8)+8),1 to (c*8,f*8),0,tpset
            1 '11460 if tn>=128 and tn <144 then copy ((tn-128)*16,8*16)-(((tn-128)*16)+16,(8*16)+16),1 to (c*16,f*16),0,tpset
            1 '11470 if tn>=144 and tn <160 then copy ((tn-144)*16,9*16)-(((tn-144)*16)+16,(9*16)+16),1 to (c*16,f*16),0,tpset
            1 '11480 if tn>=160 and tn <176 then copy ((tn-160)*16,10*16)-(((tn-160)*16)+16,(10*16)+16),1 to (c*16,f*16),0,tpset
            1 '11490 if tn>=176 and tn <192 then copy ((tn-176)*16,11*16)-(((tn-176)*16)+16,(11*16)+16),1 to (c*16,f*16),0,tpset
            1 '11500 if tn>=192 and tn <208 then copy ((tn-192)*16,12*16)-(((tn-192)*16)+16,(12*16)+16),1 to (c*16,f*16),0,tpset
            13510 next c
    13520 next f 
    13540 _turbo off
13570 return

1 'Mover mapa a la izquierda y pintar ultima columna
    1 'Si llegamos al final salimos'
    13600 if mc=200-32 then return
    13602 _turbo on (m(),mc)
    1 'Repetimos la copia del ultimo tile y desplazamiento de la pantalla 32 veces'
    13603 for i=0+ms to 32+ms
        13605 mc=mc+1
        13610 copy (8,0)-(256,184),0 to (0,0),0,pset  
        13620 for f=3 to 23-1 
            13630 tn=m(31+mc,f)
            13640 if tn >=0 and tn <32 then copy (tn*8,0*8)-((tn*8)+8,(0*8)+8),1 to (31*8,f*8),0
            13650 if tn >=32 and tn <64 then copy ((tn-32)*8,1*8)-(((tn-32)*8)+8,(1*8)+8),1 to (31*8,f*8),0
            13660 if tn >=64 and tn <96 then copy ((tn-64)*8,2*8)-(((tn-64)*8)+8,(2*8)+8),1 to (31*8,f*8),0
            13670 if tn>=96 and tn <128 then copy ((tn-96)*8,3*8)-(((tn-96)*8)+8,(3*8)+8),1 to (31*8,f*8),0
            13680 if tn>=128 and tn <160 then copy ((tn-128)*8,4*8)-(((tn-128)*8)+8,(4*8)+8),1 to (31*8,f*8),0
            13690 if tn>=160 and tn <192 then copy ((tn-160)*8,5*8)-(((tn-160)*8)+8,(5*8)+8),1 to (31*8,f*8),0
            13700 if tn>=192 and tn <224 then copy ((tn-192)*8,6*8)-(((tn-192)*8)+8,(6*8)+8),1 to (31*8,f*8),0
            13710 if tn>=224 and tn <256 then copy ((tn-224)*8,7*8)-(((tn-224)*8)+8,(7*8)+8),1 to (31*8,f*8),0
        13720 next f
    13730 next i
    13735 _turbo off
13740 return

1 ' actualizar pantalla / screen'
1 'según en que pantalla estemos se crearán un os enemigos u objetos distintos'
    13800 if ms=0 then preset (0,0):print #1,"ms sin definir"
    13801 if ms=1 then gosub 20200
    13810 if ms=2 then gosub 20300
    13820 if ms=3 then gosub 20400
    13830 if ms=4 then gosub 20500
    13840 if ms=5 then gosub 20600

    13850 if ms=6 then gosub 21200
    13860 if ms=7 then gosub 21300
    13870 if ms=8 then gosub 21400
    13880 if ms=9 then gosub 21500
    13890 if ms=10 then gosub 21600

    13900 if ms=11 then gosub 22200
    13910 if ms=12 then gosub 22300
    13920 if ms=13 then gosub 22400


13990 return


1 '---------------------------------------------------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'
1 '---------------------------------End entities---------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'



















1 '---------------------------------------------------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'
1 '----------------------------------Screens---------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'
1 '---------------------------------------------------------------------------------------------------------'




1'------------------------------------'
1'  Pantalla de Bienvenida y records 
1'------------------------------------'
    14000 cls:preset (10,30):  print #1,"!@@@@  @  @@@@  @ @@@@@ @  @   @"
    14010 preset (10,40):      print #1,"!@    @ @ @@@@  @   @  @ @ @ @ @"
    14020 preset (10,50):      print #1,"!@@@@@   @@     @   @ @   @@  @@"
    14030 preset (10,70):      print #1,"!10540,capita Kik, debes ir a recoger las muestras fabricadas en otros entornos planetarios."
    14040 preset (10,110):     print #1,"!Pero ten cuidado los esbirros de worst que te obstaculizan."
    1 ' Con inverse ponemos el fondo de los carcacteres en el frente y el frente en el fondo'
    14050 preset (10,160): print #1, "!Cursores para mover, pulsa una tecla para continuar"
    14060 preset (10,180): print #1, "!libre: "fre(0)
    1 'Si no se pulsa una tecla se queda en blucle infinito reproduciebdo una música, si se pulsa se para la música'
    1 '11870 re=1:gosub 4300
    14070 if inkey$="" then goto 14070

14090 goto 100
1'------------------------------------'
1'  Pantalla final ganadora
1'------------------------------------'
    14100 cls:preset (10,70):  print #1,"Felicidades"ma
    14110 preset (10,80):      print #1,"!Has completado la mision"
    14120 preset (10,90):      print #1,"!Lgica:Kikemadrigal"
    14130 preset (10,100):      print #1,"!gráficos:Kikemadrigal"
    14140 if inkey$="" then goto 14040
    1 'Borramos los arris para poder volver a cargarlos'
    14150 gosub 1100
14190 :goto 100
1'------------------------------------'
1'          World 0
1'------------------------------------'
    1 'Tiles'
    1 'tf=tile floor, tile suelo, corresponde al tile 160 hasta el 160+32'
    1 'te=tile end, determina el final del mundo'
    1 'tc=tile collectable, los que se pueden recoger'
    1 'tw=tile world el tile que se tiene que pintar cuando se recoja un collectable
    1 'tile death, tile de muerte, mata al player'
    20000 tf=160:te=26:tw=80:td=42
    1 'World caprturas, las capturas que necesitas para pasar el mundo'
    1 'wf=world false
    20010 wc=12
20090 return


1'------------------------------------'
1'          Screen 0
1'------------------------------------'
    1 'creamos 1 enemigo'
    20100 gosub 12500:ex(en)=26*8:ey(en)=20*8
20190 return
1'------------------------------------'
1'          Screen 1
1'------------------------------------'
    1 'creamos 1 enemigo'
    20200 gosub 12500:ex(en)=(17*8):ey(en)=17*8:es(en)=11
20290 return
1'------------------------------------'
1'          Screen 2
1'------------------------------------'
    1 'creamos 1 enemigo'
    20300 gosub 12500:ex(en)=20*8:ey(en)=20*8:es(en)=11
20390 return
1'------------------------------------'
1'          Screen 3
1'------------------------------------'
    1 'creamos 1 enemigo'
    20400 gosub 12500:ex(en)=16*8:ey(en)=10*8:es(en)=11
20490 return
1'------------------------------------'
1'          Screen 4
1'------------------------------------'
    1 'creamos 1 enemigo'
    20500 gosub 12500:ex(en)=26*8:ey(en)=20*8
20590 return
1'------------------------------------'
1'          Screen 5
1'------------------------------------'
    1 'creamos al boss'
    1 'Inicializamos el boss y le cambiamos los valores que me interesen'
    20600 gosub 12800:bo=1:bn=0:be=100:bx=150:by=120:bd=7:gosub 6000
20690 return
1'------------------------------------'
1'          World 1
1'------------------------------------'
    1 'Tiles'
    1 'tf=tile floor, tile suelo, corresponde al tile 160 hasta el 160+32'
    1 'te=tile end, determina el final del mundo'
    1 'tc=tile collectable, los que se pueden recoger'
    1 'tw=tile world el tile que se tiene que pintar cuando se recoja un collectable
    1 'tile death, tile de muerte, mata al player'
    21000 tf=160:te=26:tw=80:td=42
    21010 px=8:py=19*8
    1 'World caprturas, las capturas que necesitas para pasar el mundo'
    1 'wf=world false
    21010 wc=6
21090 return
1'------------------------------------'
1'          Screen 6
1'------------------------------------'
    1 'creamos 1 enemigo'
    1 '20100 gosub 12500:ex(en)=26*8:ey(en)=20*8
    1 '6000 pintamos la energia del boss
    21100 gosub 12800:ex(en)=(17*8):ey(en)=17*8:es(en)=11
21190 return
1'------------------------------------'
1'          Screen 7
1'------------------------------------'
    1 'creamos 1 enemigo'
    21200 gosub 12500:ex(en)=(17*8):ey(en)=17*8:es(en)=11
20290 return
1'------------------------------------'
1'          Screen 8
1'------------------------------------'
    1 'creamos 1 enemigo'
    21300 gosub 12500:ex(en)=20*8:ey(en)=20*8:es(en)=11
21390 return
1'------------------------------------'
1'          Screen 9
1'------------------------------------'
    1 'creamos 1 enemigo'
    21400 gosub 12500:ex(en)=16*8:ey(en)=10*8:es(en)=11
21490 return
1'------------------------------------'
1'          Screen 10
1'------------------------------------'
    1 'creamos 1 enemigo'
    21500 gosub 12500:ex(en)=26*8:ey(en)=20*8
21590 return
1'------------------------------------'
1'          Screen 11
1'------------------------------------'
    1 'creamos al boss'
    1 'Inicializamos el boss'
    21600 'mada'
21690 return


1'------------------------------------'
1'          World 2
1'------------------------------------'
    1 'Tiles'
    1 'tf=tile floor, tile suelo, corresponde al tile 160 hasta el 160+32'
    1 'te=tile end, determina el final del mundo'
    1 'tc=tile collectable, los que se pueden recoger'
    1 'tw=tile world el tile que se tiene que pintar cuando se recoja un collectable
    1 'tile death, tile de muerte, mata al player'
    22000 tf=160:te=26:tw=80:td=42
    22010 px=8:py=19*8
    1 'World caprturas, las capturas que necesitas para pasar el mundo'
    1 'wf=world false
    22010 wc=6
22090 return
1'------------------------------------'
1'          Screen 12
1'------------------------------------'
    1 'creamos 1 enemigo'
    1 '20100 gosub 12500:ex(en)=26*8:ey(en)=20*8
    1 '6000 pintamos la energia del boss
    22100 gosub 12800:ex(en)=(17*8):ey(en)=17*8:es(en)=11
22190 return
1'------------------------------------'
1'          Screen 13
1'------------------------------------'
    1 'creamos 1 enemigo'
    22200 gosub 12500:ex(en)=(17*8):ey(en)=17*8:es(en)=11
22290 return



1 ' ----------------------------------------------------------------------------------------'
1 '                                END SCREENS
1 ' ----------------------------------------------------------------------------------------'