Install

1. copy the litejazz folder to sites/all/themes/
2. go to apperance an click enable and set as default
3. click on settings an make your settings
thats all

some special fetures

pickstyle
we have implementet a pickstyle.js
how to use 
make a block wich will shown only on front(very recomment)
then fill in 

<a href="/" onclick="pickstyle('blue')">Blue</a>
<a href="/" onclick="pickstyle('green')">Green</a>
<a href="/" onclick="pickstyle('red')">Red</a>

now can each user set her/his own color style 
this will sotored in a cookie

Local Content File Name
the local content file means a CSS file
enter the locaton where is stored the css file
eg.:
sites/all/themes/newsflash/css/icons.css

for custom block styles you need 
Block Theme module
go to
admin/config/user-interface/blocktheme
add in Custom Block Templates follwo lines

blknomargins|blk-nomargins
blkoutline1|blk-outline1
blkoutline2|blk-outline2
blkoutline3|blk-outline3
blkoutline4|blk-outline4
blkrollover1|blk-rollover1
blkrollover2|blk-rollover2
blksolid1|blk-solid1
blksolid2|blk-solid2
blksolid3|blk-solid3
blksolid4|blk-solid4
color0pagebg|color0-pagebg
color1pagebg|color1-pagebg
color2pagebg|color2-pagebg
color3pagebg|color3-pagebg
color4pagebg|color4-pagebg
color5pagebg|color5-pagebg
stripe0pagebg|stripe0-pagebg
stripe1pagebg|stripe1-pagebg
stripe2pagebg|stripe2-pagebg
stripe3pagebg|stripe3-pagebg
stripe4pagebg|stripe4-pagebg

now you can change the styles for blocks
beware its not remommnet to us this in content, header, suckerfish, searchbox or footer regions