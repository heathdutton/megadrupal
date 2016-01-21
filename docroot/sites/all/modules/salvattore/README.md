#Salvattore Views
This module adds a new views style that will display views
rows as a Masonry using Salvattore.

##Salvattore
Salvattore is a jQuery Masonry alternative with CSS-driven
configuration.
For more information and to download the library visit:
http://salvattore.com/
http://github.com/rnmp/salvattore

##Installation & Usage
* Install and enable the module as usual.
* Download the salvattore.min.js file under sites/all/libraries/salvattore/dist/salvattore.min.js
* Cloning salvattore from git under sites/all/libraries/salvattore also works.
Then select Salvattore as a style plugin in the view display that you want
to be displayed as a masonry.

*You are supposed* your own CSS code for your masonries, but in case you are in
a hurry visit the settings page (admin/config/user-interface/salvattore)
and enable the default CSS file.
Also, you can use this default CSS file as your base.

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

The status report page entry was based on code from modernizr module, thanks.
