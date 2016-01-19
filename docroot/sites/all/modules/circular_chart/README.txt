CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Maintainers


INTRODUCTION
------------
Circular Chart is a module which enables jQuery Circular chart plugin as a
field widget. This jQuery plugin is originally developed by lugolabs. Fork him
at https://github.com/lugolabs/circles.

This module provides the following features to the site builders & developers.
1. Provides one field type "Circular Chart Widget" and two widgets "Select list"
and "HTML5 Number field" (If elements module is installed and enabled).
2. Provides creation of new graph presets where a builder can customize,
    a) Circle's radius.
    b) Circle's width.
    c) Circle's background & foreground color.
    d) Circle's graph creation animation duration.
3. These presets are exportable with Ctools & Features API.
4. A theme function is provided to use in custom modules.
5. Has inbuilt color module support to select the color ranges.

This initial version doesn't support the following,
1. Responsive Graph for mobile devices.
2. Inline text inside the circular graph.


REQUIREMENTS
------------
This module is dependent on the following modules.
1. Field module.
2. Libraries API (https://www.drupal.org/project/libraries)
3. Chaos tool suite (ctools) (https://www.drupal.org/project/ctools)

This module needs to have lugolabs circles jQuery plugin as library. To download
visit https://github.com/lugolabs/circles/ URL.


INSTALLATION
------------
To install this module follow:
1. Download lugolabs circles jQuery plugin in Drupal's "libraries" directory.
The directory name should be "circles" and the directory structure shuld look
like,
libraries/
`-- circles
    |-- bower.json
    |-- circles.js
    |-- circles.min.js
    |-- Gruntfile.js
    |-- MIT-LICENSE
    |-- package.json
    |-- README.md
    `-- spec
        |-- circlesSpec.js
        |-- index.html
        |-- karma.conf.js
        |-- responsive.html
        `-- viewport.html

Installing this plugin can be done via GIT also,
git clone https://github.com/lugolabs/circles.git circles
Or, can be downloaded via https://github.com/lugolabs/circles/archive/master.zip
Once the library is placed then enable the module:
1. admin/modules page or,
2. Using Drush (drush pm-enable circular_chart -y)


CONFIGURATION
-------------
This module provides a permission for the site admins.
  - Configure user permissions in Administration » People » Permissions

* Create new Presets
   - Administration » Configuration » Media » Circular Chart Presets
   (admin/config/media/circular-charts)
   - Create or edit the existing preset.

* Add field in a content type
  - Administration » Structure » Content types » <Content Type>

* Manage display of that field
  - Administration » Structure » Content types » <Content Type>
  - Select Format "Circular graphs" from dropdown.
  - Circular Chart Presets and Update.


MAINTAINERS
-----------
Current Maintainer: Aneek Mukhopadhyay (https://www.drupal.org/u/aneek)
