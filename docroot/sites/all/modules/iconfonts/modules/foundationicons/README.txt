----------------
Foundation Icon Font module
----------------

Overview
--------
This is a submodule of the IconFonts module. It provides webmasters an easy way
to add Foundation Icon Font icon fonts to their website.

Installation
------------
First download the 'foundation_icons_general' directory from the following URL:
https://github.com/zurb/foundation-icons
and extract the files to the appropriate libraries directory, which is in most
cases /sites/all/libraries.
Make sure the following files exist:
/sites/all/libraries/foundation_icons_general/fonts/general_foundicons.eot
/sites/all/libraries/foundation_icons_general/fonts/general_foundicons.ttf
/sites/all/libraries/foundation_icons_general/fonts/general_foundicons.svg
/sites/all/libraries/foundation_icons_general/fonts/general_foundicons.woff

Install and enable the module as usual.

The module depends on the IconFonts module, make sure that module is also
enabled.

Usage
-----
After enabling the module, go to Configuration > User interface >
@font-your-face settings and click on 'Import Icon Fonts fonts' or 'Update Icon
Fonts fonts' to be able to use the Foundation Icon Font fonts.

Then go to Appearance > @font-your-face > Browse all fonts and on the
'Iconfonts' tab enable the Foundation Icon Font font.

Then go to 'Enabled fonts' page and click on 'Edit' in the appropriate row. You
will be taken to the font edit page, where all the available icon fonts are
listed. Clicking on any of the icons will allow you to enable/disable that
character and to assign css selectors to it.
