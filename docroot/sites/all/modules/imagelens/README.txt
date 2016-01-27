jQuery imageLens module
-----------------------

== SUMMARY ==

This is a module that migrates the jQuery imageLens library (http://www.dailycoding.com/Posts/imagelens__a_jquery_plugin_for_lens_effect_image_zooming.aspx) for Drupal.


== CONFIGURATION ==

The module has an admin interface (admin/config/user-interface/imagelens) where you can enable including of the JavaScript file each time to the site.
Otherwise you can include it manually via a simple function callback:
<?php
drupal_add_library('imagelens', 'imagelens');
?>


== USAGE ==
 
 = AS A JS LIBRARY =

 If you want, you can use it as a JavaScript library:
  The HTML code:
  <img id="test_img" height="250" src="location/my/full-size-imge.jpg" />

  The JavaScript code:
  jQuery("#test_img").imageLens();

 = AS AN IMAGE FORMATTER =

 If you want, you can use it as a formatter for image module.
  Go to ImageLens styles menu (admin/config/media/imagelens-styles) and add your styles. You can add the style's name, the weight and the height attributes. You can leave blank one of these attributes.
  Go to the created image field's display section and choose the ImageLens format and select the style you want to display.