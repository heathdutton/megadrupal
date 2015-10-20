Extra options for A Cloudy Day theme 

1 PNG FIX for IE6:
- Download DD_belated PNG FIX from http://www.dillerdesign.com/experiment/DD_belatedPNG/DD_belatedPNG_0.0.8a.js and add this file to the A Cloudy Day folder.

- Then add the following inside page.tpl.php between <!--[if lt IE 7.]> and <![endif]--> 

<script src="<?php print base_path() . path_to_theme() ?>/DD_belatedPNG_0.0.8a.js"></script>
<script>DD_belatedPNG.fix('*'); </script>

2 Cufón site name:
- Download the Cufón script from http://cufon.shoqolate.com/js/cufon-yui.js and place it inside the A Cloudy Day theme folder.

- Then add the following to a-cloudy-day.info
scripts[] = cufon-yui.js

- Add this just before the closing body tag </body> inside page.tpl.php
<script type="text/javascript">Cufon.now();</script>

- Place the following between the head tags in page.tpl.php
<script type="text/javascript">Cufon.replace('#headertitle h1');</script>

- Use the Cufón font generator at http://cufon.shoqolate.com/generate/ to create a .js file for the font you would like to use. (Free fonts can be found at http://www.font-cat.com/main-en.html)
Place the .js file inside the A Cloudy Day theme folder.

- Add the following line to a-cloudy-day.info
scripts[] = type here the name of the .js file that you get by the Cufón generator