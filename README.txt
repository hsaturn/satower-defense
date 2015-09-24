If you are lucky, run the static binary ! Else, see how to compile
-------------------
Satower is a tower defense will many and many possibilities to modify the
whole game without writing any line of code.

Ennemies are made using a langage very close to the logo langage. The langage
is named ogol.

You may try to edit walkers.def in order to modify ennemies appearance.

config.def: main definition file for a given theme.
theme.def: gui appearance, buttons and area settings
walkers.def: ennemies characteritics and appearnce
weapons.def: weapons characteristics

-------------------
ogol
fwu	n						draw forward and pen up
bwu n						draw a line backward and pen up
tr n						turn right turtle direction
tl n						turn left turtle direction
fw n						draw a forward line
color name					change pen color
circle ray					draw a circle
shootpoint					where missiles/laser will shot
angle						change turtle's direction

opacity n					set opacity
$var=value					set a variable
add $var,amount				add amout to var

repeat n { ...block... }	repeat the block n times
rotates n { ...block... }	draw the block n times, turn by 360°/n each time
filled color {...block...}	draw the block and fill it with the given color
mirror { ...block... }		draw the block then immediately its mirror image

-------------------
If not, you have to compile it.

You have to install following DEVELOPMENT packages :

g++
make
sdl-dev
sdl_gfx-dev
sdl_image-dev
sdl_mixer-dev
sdl_ttf-dev

etc.

------------------
Def files

Almost all 
------------------
Maps

Thanks to Thorbjørn Lindeijer, maps have been created with the Tiled editor
developped by this great guy.

Without this tool, one could not modify / create maps as easier as it is, and
probably the game will have a worse look (ok: one will find that the graphics
are already not so good :-D)

I have included the .exe file that works great with wine (32 bits only).

You can edit almost everything. Instead of create new maps, you may start from
an existing map. There are some layer that have to be there unless the map
won't work.

You may try the latest version:

github : https://github.com/bjorn/tiled/wiki
http://www.mapeditor.org/
