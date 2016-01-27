********************
jQuery Scroll Follow
********************

jQuery Scroll Follow for Drupal is a simple module that use 
a jQuery plugin (jquery.scrollfollow.js) which allow content
to follow the page as the user scrolls.

************
Requirements
************

Drupal 7.x
Libraries

****************************
Installation & Configuration
****************************

1. Copy the entire jquery_scroll_follow directory 
into sites/all/modules directory.

2. Download jquery.scrollfollow.js (plugin used in this module):
http://kitchen.net-perspective.com/open-source/scroll-follow/

3. Copy the file jquery.scrollfollow.js into
your Drupal site at sites/all/libraries/jquery_scroll_follow

3. Login as an administrator. Enable the module in the "Administer" -> "Modules"

3. On the configuration page, set the CSS selectors 
on which the plugin will apply. 
Optionally, you can define the speed, offset and the delay.

******
Notice
******

According to the official documentation, the plugin uses the "top" CSS 
property of an object to slide it. In order to work properly, targets must 
either set to "relative" or "absolute".

*******************
Author / Maintainer
*******************

Jean-Baptiste L. (aka, Jibus)

**********************
Development Supporters
**********************
L'île des Médias
