#Salvattore Views
This module adds a new views style that will display views
rows as a Masonry using Salvattore.

##Salvattore
Salvattore is a jQuery Masonry alternative with CSS-driven
configuration.
For more information visit:
http://salvattore.com/
http://github.com/rnmp/salvattore

##Usage
Install and enable the module as usual. Then select Salvattore
as a style plugin in views.

*You are supposed* your own CSS code for your masonries, but in case you are in
a hurry visit the settings page (admin/config/user-interface/salvattore)
and enable the default CSS file.
Also, you can use this default CSS file as you base.

###WARNING
Salvattore *IS NOT* responsive when the CSS file is added to the
page using `@import`. It must be added using `<link type="text/css" rel="stylesheet"`
in order to be responsive and re-draw the masonry when resizing the browser.
*To overcome this, salvattore.css is not being preproccesed, which makes Drupal add
it using a `<link>` tag.*

IMPORTANT: The reason Drupal does not behave this way by default is the limitation
in Internet Explorer (up to v7) which will not load more than 31 linked stylesheets.

This module was brought to you by:
Bill Seremetis (bserem, https://www.drupal.org/u/bserem)
SRM Tech (https://www.drupal.org/node/1829106)
and it was created for
SoLebIch (http://www.solebich.de)

Salvattore was created by Salvattore developers.
Rolando Murillo and Giorgio Leveroni
All credits due to these guys :)
