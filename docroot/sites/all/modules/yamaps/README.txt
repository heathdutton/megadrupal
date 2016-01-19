
-- ABOUT --

Project contains a set of modules for Drupal 7 that use
Yandex.Maps service available at http://maps.yandex.com/.
Uses http://api.yandex.ru/maps/ (API 2.x).

Project page: https://drupal.org/project/yamaps.
To submit bug reports and feature suggestions, or to track changes:
https://drupal.org/project/issues/yamaps.

-- MODULES --

* Yandex Maps (main module, description provided below).
* Yandex Maps Example (submodule). Feature-based module which demonstrates
  example content type containing "Yandex Map" field
and view to output maps. For demo purposes only!
* Yandex Maps Views (submodule). Enables integration with Views for "Yandex Map"
  field and provides "Yandex Maps" display plugin.

-- OPTIONS --

* Map can be displayed as interactive object ("dynamic map")
  or image ("static map").
* Map can be displayed by click on the "button" with configurable text.
* Change the type, size and center of the map.
* Add placemarks, select label color, change texts, add labels using
  the search string.
* Draw polylines, chose colors, transparency, line width, text.
* Draw polygons, chose colors and line width, fill color, transparency, texts.
* Add a route.
* Displays traffic jams.

-- REQUIREMENTS, INSTALLATION AND UNINSTALLATION --

Requires core 'block' and 'field' modules.
Install and uninstall as usual.
See http://drupal.org/documentation/install/modules-themes/modules-7
for further information.

-- CONFIGURATION AND USAGE --

Provides configurable "Yandex Maps" field, which can be added to any type of
Drupal content.
Field may accept "Dynamic" and "Static" formats.
"Dynamic" format means that map is displayed as interactive object.
"Static" format means that map is displayed as regular image.

To add "Yandex Maps" field perform following steps:
* Navigate to /admin/structure/types and select content type.
* Navigate to /admin/structure/types/manage/{CONTENT-TYPE}/fields
  ("Manage Fields" page) of the selected content type.
* Configure and add field of "Yandex map" type.
* Navigate to /admin/structure/types/manage/{CONTENT-TYPE}/display
  ("Manage Display" page) of the selected content type.
* Configure field format.

Provides configurable amount of "Yandex Maps" blocks to display maps
in any regions of the website.
Block may also present map as a "Dynamic" (interactive object)
or "Static" (image).
To add "Yandex Maps" block perform following steps:

* Navigate to /admin/config/services/yamaps and set required amount
  of "Yandex Maps" blocks.
* Navigate to /admin/structure/block, scroll down to find "Disabled" section
  and find block called "Yandex Map #{NUMBER}".
* Configure the block and pull it into the required region.

-- INFORMATION --

* Demo: http://yandex.xyz.tom.ru/
* Blog post: http://clubs.ya.ru/company/52369
