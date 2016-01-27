Beale Street is createt by RoopleTheme
i have change it to Drupal 7

important
the folder name in the promotet project has changes to bealestreet.
if you have an sandbox Beale Street installation you have first changes to a another theme like batik
then remove old installation of Beale Street then copy the new Beale Street Theme
and go to performanche and clear cache now you can turn on the Beale Street theme

greetings Alyx

if you want use pickstyle
use follow lines 

example
<table width="100%">
<tr>
<td align="center">
<a href="<?php print base_path(); ?>"><img src="<?php print base_path() . path_to_theme() ?>/images/themeboxes/BealeStreetBlue2.png" class="pickstyle" onClick="pickstyle('blue')" title="Blue Style" /></a>
</td>
<td align="center">
<a href="<?php print base_path(); ?>"><img src="<?php print base_path() . path_to_theme(); ?>/images/themeboxes/BealeStreetOrange2.png" class="pickstyle" onClick="pickstyle('orange')" title="Orange Style" /></a>
</td>
<td align="center">
<a href="<?php print base_path(); ?>"><img src="<?php print base_path() . path_to_theme(); ?>/images/themeboxes/BealeStreetGreen2.png" class="pickstyle" onClick="pickstyle('green')" title="Green Style" /></a>
</td>
<td align="center">
<a href="<?php print base_path(); ?>"><img src="<?php print base_path() . path_to_theme(); ?>/images/themeboxes/BealeStreetRed2.png" class="pickstyle" onClick="pickstyle('red')" title="Red Style" /></a>
</td>
</tr>
</table></div>