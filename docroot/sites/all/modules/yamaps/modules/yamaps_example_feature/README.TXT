
-- ABOUT --

Module for Drupal 7.
Uses http://api.yandex.ru/maps/ (API 2.x).

Project page: https://drupal.org/project/yamaps.
To submit bug reports and feature suggestions, or to track changes:
https://drupal.org/project/issues/yamaps.

-- OPTIONS --

Feature-based module (admin/structure/features/yamaps_example_feature)
which demonstrates example of configured content type containing "Yandex Map"
field and view to output maps.

Module should not be used in production. It was created for demo purposes only!
In the future we plan to extend this module with new features
as they will be available.

Module defines "Yandex Maps Page" content type
(admin/structure/types/manage/yandex_maps_example)
with configured "Yandex Map" field.

Module also creates views-based "Yandex Maps View" page
(admin/structure/views/view/yandex_maps_views_example)
which displays all available maps from "Yandex Maps Page" nodes.
