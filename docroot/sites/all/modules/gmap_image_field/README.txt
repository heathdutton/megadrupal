CONTENTS OF THIS FILE
---------------------
* Introduction
* Features
* Requirements
* Installation
* Do first example map
* For Sitebuilders and Developers
* Future plans
* Contacts


INTRODUCTION
============
The module provide interface for converting any image to google map.
In example usage the module can help to make map for a pirate treasure or
may be map for a campus, or whatever you want to present as eye candy map.


FEATURES
========
* Adjust tile size
* CSS customizeable controls
* Customizeable pin images
* CTools pane plugin
* Markers
* Markers description popup (modal and native from google)
* API hooks


REQUIREMENTS
============
Drupal 7.x
ImageMagick >= 6.5.8
PHP configuration should allow shell_exec()


DOWNLOAD
========
Project page: http://drupal.org/sandbox/dimitrov.adrian/1928930


INSTALLATION
============
1. Unarchive the zip and put directory into your sites/all/modules
2. Enable it from your admin panel -> Modules
3. (Optional) For using pins (markers)
   you should enable also and gmap_image_field_pin module.
4. Go your /admin/reports/status page and check if there is some problem
   marked with GMap Image Field, if there is nothing such, then all is okay,
   and the modules are ready for use and can skip next steps.
5. Install ImageMagick (version newer or 6.5.8), You can get it from
   http://www.imagemagick.org/script/binary-releases.php, there is also and
   description about installing.

That should be all steps to get module working.


DO FIRST EXAMPLE MAP
====================

1. Do the INSTALLATION section.

2. Go your website admin panel -> Structure -> Article
   (or some other content type) -> manage fields.

3. Add new field from type GMap Image.

4. Let settings for Tile size and Tiles format to their default values
   (256 for size, and jpeg for format).

5. Allow pinning of the maps as selecting one of the content types from the
   checkboxes on "Allow pinning nodes on this image".
   (NOTE THAT "Articles" is not available for pinning because contain the maps)

6. Save the field settings.

7. Now go to the Article's content type "Manage Display".

8. Using the settings icon for the newly created field for the gmap image,
   setup the display as you wish.

9. Update display settings for the field,
   and save settings for the content type.

10. Now you are ready to upload images as maps from the "Add content",
    and selecting the content type that contain map (in our example this
    is Article) Upload some image with high resolution using the
    "Map image" -> Image

11. Add some pins, this can be done using the "Add content" and adding nodes
    from type that is selected for the pinning.
    So if you add field "GMap Image" to "Article" and give pinning ability to
    "Basic Page" from the field menu, then Article will be maps, and
    "Basic Page" will be pins.


FOR SITEBUILDERS AND DEVELOPERS
===============================

* Permissions:
** Administer GMap Image Field - give role ability to administer gmap images

* Drupal hooks
** hook_gmap_image_field_assets_css_alter($css_file);
** hook_gmap_image_field_map_settings_alter(&$gmap_image_field_map_setting);

* JS events
** gmap_image_field_marker_clicked, hook on map and accept pin

* Templates and themes
** theme gmap_image_field_show_map - presenting map field
** templates/gmap-image-field.tpl.php - presenting map field
** theme gmap_image_field_pin_content - presenting infowindow content

* Styling
You can disable including of built in stylesheet from
admin configuration -> GMap IMage Field and disable, but recommeds to copy and
modify the module's stylesheet for overriding.

* Drupal.behaviors is fired when marker's infowindow is opened.


FUTURE PLANS
============

Plans about improving in the next version, if module get enough interest. 

* Improve external JS markers loading.
* Rework backend using GD of PHP for tiling the images.
* UI for managing pins icons.
* Polylines, Polygons, Circles and Custom overlay builder.

CONTACTS
========
Developer: Adrian Dimtrov (adimitrov@propeople.dk)
