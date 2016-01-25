*Orbit Media Flexslider Module*

The point of this module is to be a drop in slider for Orbit Media studios.

To Install
Download the flexslider package:
https://github.com/woothemes/FlexSlider/archive/version/2.2.zip
unzip the file into the libraries folder and name the folder flexslider

The configuration will be as follows.

Under the admin section (open to change) there will be a page where the
user/admin can create new blocks.  these blocks are loaded into the
table names oms_flexslider.

After a block has been created it will be available in the
structure->blocks section.  These blocks will all have a configuration
section, like other blocks where a user will be able to change Flexslider
settings.  Please refer to the Flexslider page.
http://www.woothemes.com/flexslider/

There will be a new content type created where a user can assign a slider
to the created block, options being made available through a dropdown
from the content type.  We can add as many fields as we want to the
content type, such as image, etc.

All other displays will be handled by the style.

To use the tpl in your theme, copy the oms_flexslider.tpl.php file to your
theme folder.  After that you can add more fields to oms flexslider content
and then call them in the tpl.
