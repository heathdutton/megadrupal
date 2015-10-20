
-- SUMMARY --

This module provide the ability to display the content of all menu items of the menu that you indicate in settings on the Single Page.
On the page '/single_page' the links in the menu will be replaced to the links to the correspondent anchors.
You can set URL '/single_page' as front page of your site, and your visitors will see the Single Page Website managed by Drupal 7.

-- REQUIREMENTS --

Module works with Bartik and Zen themes and sub-themes ONLY!

-- INSTALLATION --

Normal Drupal module installation, see http://drupal.org/node/70151 for further information.

Also to allow support for changing hashes (#) in the browser address line,
you should install the jquery.autoanchor library. (Optional)
Please download it from https://drupal.org/files/jquery.autoanchor.js_.txt
The original link (http://plugins.jquery.com/files/jquery.autoanchor.js.txt) is no longer available.
Save the file to e.g. sites/all/libraries/jquery.autoanchor/jquery.autoanchor.js

If you would like to support Easing (http://gsgd.co.uk/sandbox/jquery/easing/)
Please add easing plugin to
sites/all/libraries/jquery.easing/jquery.easing.js
You should download and rename jquery.easing.1.3.js to jquery.easing.js

-- CONFIGURATION --

* Go to Configure / System / Single Page Settings.

-- CUSTOMIZATION --

You can change HTML DOM IDs in js/*.inc files to customize module setting to your specific html layout.

-- ROADMAP --

To expand the list of the supported themes.

-- CONTACT --

Current maintainers:
* Vasily Yaremchuk (Yaremchuk) - http://drupal.org/user/576918

