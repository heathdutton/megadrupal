TWBS Font Awesome
=================

A simple helper module for [TWBS](https://drupal.org/project/twbs),
handle [Font Awesome](http://fontawesome.io/) library initialization.

### Key Features

-   Provide [drush
    make](https://raw.github.com/drush-ops/drush/master/docs/make.txt)
    file for library download
-   Register library with
    [hook\_libraries\_info()](http://drupalcontrib.org/api/drupal/contributions!libraries!libraries.api.php/function/hook_libraries_info/7)
-   Initialize library with
    [hook\_init()](https://api.drupal.org/api/drupal/modules!system!system.api.php/function/hook_init/7)
-   Confirm library successfully initialized with
    [hook\_requirements()](https://api.drupal.org/api/drupal/modules!system!system.api.php/function/hook_requirements/7)

### Special Note

This helper module don't take care any else other that above primary
target, where [TWBS](https://drupal.org/project/twbs) will handle the
remaining Drupal remix.

### Author

-   Developed by [Edison Wong](http://drupal.org/user/33940).
-   Sponsored by [PantaRei Design](http://drupal.org/node/1741828).
