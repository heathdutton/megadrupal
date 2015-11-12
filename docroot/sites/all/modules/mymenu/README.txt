
Description: Dropdown personal menu in a block

http://drupal.org/project/mymenu

This module includes 3 menu blocks:   (still under development)

1. A menu in a block that allows navigation to parts of the website not covered by for example primary links: e.g. the user-menu on Drupal 7
 The menu is a dropdown list, contains just one level of items.
 It was designed to work on traditional Desktop browsers like IE/FF/Safari/Chrome, but also on Devices like an iphone.

Example: in the user-menu, create Items you like as children.
Configuration: Enable the block and then set some variables in settings.php:

$conf['mymenu_useristitle'] = '1';         // Username is the menu title
$conf['mymenu_title'] = 'My Menu';         // Or, set Title of the closed dropdown
$conf['mymenu_blocktitle'] = '<none>';     // Title for the block
$conf['mymenu_menutop'] = 'user-menu';    // Which menu

2. A dynmic login/lout block

3. A langauge switcher of the format EN FR DE IT, as opposed to English Francais Deutsch..



