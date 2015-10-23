TWBS Contextual
===============

A very simple (only \~100 lines!!) mobile friendly contextual links
introduced as part of the [TWBS](https://drupal.org/project/twbs)
project to solve mobile administration problem with the Drupal 7 shipped
contextual.module, that is not very friendly to touch screen device.

### Key Features

-   Extend Drupal core contextual.module, support both mobile and
    desktop with [Bootstrap
    Dropdowns](http://getbootstrap.com/components/#dropdowns) design

### Getting Started

Download and install with [drush](https://github.com/drush-ops/drush)
manually:

    drush -y dl --dev twbs_contextual
    drush -y make --no-core sites/all/modules/twbs_contextual/twbs_contextual.make

Package into your own drush .make file (e.g.
[drustack\_core.make](http://drupalcode.org/project/drustack_core.git/blob/refs/heads/7.x-25.x:/drustack_core.make)):

    api = 2
    core = 7.x
    projects[twbs_contextual][download][branch] = 7.x-3.x
    projects[twbs_contextual][download][type] = git
    projects[twbs_contextual][download][url] = http://git.drupal.org/project/twbs_contextual.git
    projects[twbs_contextual][subdir] = contrib

### Live Demo

[TWBS Contextual](https://drupal.org/project/twbs_contextual) is now
integrated into [DruStack](https://drupal.org/project/drustack)
distribution, so you can try it in a live sandbox with
[simplytest.me](http://simplytest.me/project/drustack/7.x-25.x).

### Known Issues

Additional patch is required for following modules:

-   Edit: [\#2169981: Add TWBS Contextual
    Support](https://drupal.org/node/2169981)

### Author

-   Developed by [Edison Wong](http://drupal.org/user/33940).
-   Sponsored by [PantaRei Design](http://drupal.org/node/1741828).

