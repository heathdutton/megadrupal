//// Functionality overview ////

Footer sitemap will output a block that may be placed into footer region to
display a sitemap. The output may be configured in the block settings where
you can configure which menus will be used to construct the sitemap and in the
individual menus where you can specify which menu items should be hidden.

//// Installation and configuration ////

After enabling the module go to the blocks administration and place the Footer
sitemap block into desired region. After saving the block position go to the
Footer sitemap block configuration and choose menus that should be used for the
sitemap output. Furthermore, you can control which menu links will be outputted
using the "list links" configuration page. For the "list links" configuration
page navigate to Structure - Menu and click the list links action for the
appropriate menu. A list of all menu links is displayed with
"HIDE FROM FOOTER SITEMAP" checkbox. Using this checkbox you can control the
visibility of the link in the sitemap.

//// Theming ////

There are two template files available for themers.
footer-site-map.tpl.php - wraps the whole output.
footer-menu.tpl.php - wraps the output of one menu.

To take full control of the output you can override the theme_tree_links theme
function.

//// Drupal version ////

This module is only available for Drupal 7.
