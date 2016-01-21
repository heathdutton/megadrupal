-- ABOUT --

The Semantic Blocks module was created to enable the easy selection
and use of the newer HTML5 elements for blocks within the admin 
interface.

This module is essentially a modified version of the Block Class 
module.

-- INSTALLATION --

Install as usual - see 
http://drupal.org/documentation/install/modules-themes/modules-7 
for more details

Set permissions as usual.

If your theme is already using a block.tpl.php template file, open 
this file in an editor and replace the div opening and closing tag 
with $tag. Remember to replace the closing tag!

If your theme doesn't use a block.tpl.php template file, you can
override the theme registry by visiting the module configuration
page and selecting override theme registry.

-- WARNING --

This module assumes you are dealing with browser support outside 
of the module. If you are not currently using the HTML5 shiv then
 older IE browsers will not display the new elements. See 
 http://code.google.com/p/html5shiv/ for details on fixing this.
 
-- TROUBLESHOOTING --

Not working? Try flushing the cache, checking user permission and 
ensuring you have a correct / modified version of block.tpl.php 
in your theme folder.
