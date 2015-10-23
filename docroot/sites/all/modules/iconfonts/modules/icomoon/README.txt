----------------
Icomoon module
----------------

Overview
--------
This is a submodule of the IconFonts module. It provides webmasters an easy way
to add icomoon icon fonts to their website.

Installation
------------
Icomoon does have a free set of icons or you can configure and build your own set.
It also allows you to create your own icon set which is outside the scope of this module.
First compile and download the icomoon font files from the following URL:
http://icomoon.io/app/
Be aware that not all icons are free and you may need to purchase a license!
Select the icons you need or create your own, download and extract the files to the appropriate libraries directory, which is in most cases /sites/all/libraries.
Make sure the following files exist:
/sites/all/libraries/icomoon/fonts/icomoon.eot
/sites/all/libraries/icomoon/fonts/icomoon.ttf
/sites/all/libraries/icomoon/fonts/icomoon.svg
/sites/all/libraries/icomoon/fonts/icomoon.woff

Install and enable the module as usual.

The module depends on the IconFonts module, make sure that module is also
enabled.

Usage
-----
After enabling the module, go to Configuration > User interface >
@font-your-face settings and click on 'Import Icon Fonts fonts' or 'Update Icon
Fonts fonts' to be able to use the icomoon fonts.

Then go to Appearance > @font-your-face > Browse all fonts and on the
'Iconfonts' tab enable the icomoon font.

Then go to 'Enabled fonts' page and click on 'Edit' in the appropriate row. You
will be taken to the font edit page, where all the available icon fonts are
listed. Clicking on any of the icons will allow you to enable/disable that
character and to assign css selectors to it.

icomoon_chars.txt
-----------------
Char name and code must now be terminated by a single(!) space.
