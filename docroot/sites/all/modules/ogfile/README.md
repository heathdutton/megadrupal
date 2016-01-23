## Overview

This module defines [Organic Group](https://www.drupal.org/project/og) 
permissions for [File entities](https://www.drupal.org/project/file_entity).
  
For each file bundle, the following OG permissions are available:
* View file
* Edit file
* Delete file


### Dependencies

* [Organic Groups](https://www.drupal.org/project/og) 7.x-2.x
* [File Entity](https://www.drupal.org/project/file_entity) 7.x-2.x

### Instructions

* Enable module.
* Make at least one file entity bundle "group content" by adding a og_group_ref
  field to the bundle.
* Modify permissions at 'admin/config/group/permissions'.
