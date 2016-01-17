This is a module developed by Ebizon Technologies (www.ebizontek.com). Email: info@ebizontek.com.

Featured Content Slider:
-------------------------
A module to feature latest created nodes (selected through admin settings) in a block that displays as a slideshow.

Description:
------------

Featured Content Slider makes a slideshow out of node content on the block. Currently, latest selected nodes are shown. 

How to use?
-----------
1. Extract module to sites/all/modules/ directory.
2. Go to admin/modules and enable this module.
3. Go to admin/settings/content_slider.
           - In 'Content type for Slider' the default content type 'content_slider' has been created when module is enabled.
	     - If we want to create a content slider corresponding to any other content type, then write that content type machine name in the textfield.
           - Save your configuration.
4. Go to admin/structure/block.
           - Search for 'Content Slider'.
           - 'Content Slider' blocks, corresponding to each content type which we have configure at 'admin/settings/content_slider' would be here.
           - Place the block: anwhere you like.
5. go to node/add/content_slider and add node data as title, body text and image.
6. Additionally, theme the content node by creating a separate node--<content type machine name>.tpl.php 

The script enlists the help of the jQuery library for its engine.

Sponsors:
---------

Thanks to Faber John of Af83.com to have sponsered the custom slider that supports preconfigured images, theming, automatically creates a content type when the module is installed and also supports embedded videos. This module has been used here: http://www.cirm.ca.gov/ and would play like this out of the box when installed.  


Special Thanks:
---------------

jchatard (http://drupal.org/user/130002): for a patch that fixes lot of bugs with this module. Also, makes it more compatible with Drupal standards.
bordage (http://drupal.org/user/165112): for releasing franch translation of this module.
manarth (http://drupal.org/user/321496): for other patches.

