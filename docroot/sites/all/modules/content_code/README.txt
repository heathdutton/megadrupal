Content Theme Code
http://drupal.org/project/content_code
Load content type and node specific CSS and JS files on the right page.


Install...
Normal procedure:
1) Place in sites/all/modules/content_code
2) Enable at admin/build/modules or via drush.

NOTE: This module has no UI, it just loads files from your theme.


USAGE...
This module will load content type -and node- specific code CSS and JS files from your theme when on the appropriate pages. This code is added to the head after other assets. It will check for the existence of such files before including them in the output header. Code will be loaded whenever the node is loaded, even by a view.

Add files to your theme like so...
sites/all/themes/MYTHEME/css/content-types/MYTYPE.css
sites/all/themes/MYTHEME/js/content-types/MYTYPE.js
sites/all/themes/MYTHEME/css/nodes/NODEID.css
sites/all/themes/MYTHEME/js/nodes/NODEID.js

EXAMPLES:
sites/all/themes/cool_theme/css/content-types/article.css
sites/all/themes/awesomesauce/js/content-types/video.js
sites/all/themes/neatorama/css/nodes/123.css
sites/all/themes/my_subtheme/js/nodes/456.js

