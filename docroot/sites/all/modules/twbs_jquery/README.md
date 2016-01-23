TWBS jQuery
===========

A helper module for [TWBS](https://drupal.org/project/twbs), upgrade the
version of [jQuery](http://jquery.com/) in Drupal core to corresponding
latest official version.

All replacement will be handled automatically. No additional
configuration is required.

### Key Features

-   Provide [drush
    make](https://raw.github.com/drush-ops/drush/master/docs/make.txt)
    file for library download
-   Confirm library successfully initialized with
    [hook\_requirements()](https://api.drupal.org/api/drupal/modules!system!system.api.php/function/hook_requirements/7)
-   Upgrade jQuery related libraries as below version:
    -   [jQuery](http://jquery.com/): 1.10.2
    -   [jQuery UI](http://jqueryui.com/): 1.10.3
    -   [jQuery Cookie](http://plugins.jquery.com/cookie): 1.4.0
    -   [jQuery Form](http://jquery.malsup.com/form): 3.46.0
    -   [jQuery Once](http://plugins.jquery.com/once): 1.2.6
    -   [jQuery BBQ](http://benalman.com/projects/jquery-bbq-plugin):
        1.3pre
        -   Additional patch for [jQuery \>=
            1.9](https://drupal.org/node/2138761)

### Getting Started

Download and install with [drush](https://github.com/drush-ops/drush)
manually:

    drush -y dl --dev twbs_jquery
    drush -y make --no-core sites/all/modules/twbs_jquery/twbs_jquery.make

Package into your own drush .make file (e.g.
[drustack\_core.make](http://drupalcode.org/project/drustack_core.git/blob/refs/heads/7.x-25.x:/drustack_core.make)):

    api = 2
    core = 7.x
    projects[twbs_jquery][download][branch] = 7.x-3.x
    projects[twbs_jquery][download][type] = git
    projects[twbs_jquery][download][url] = http://git.drupal.org/project/twbs_jquery.git
    projects[twbs_jquery][subdir] = contrib

### Live Demo

[TWBS jQuery](https://drupal.org/project/twbs_jquery) is now integrated
into [DruStack](https://drupal.org/project/drustack) distribution, so
you can try it in a live sandbox with
[simplytest.me](http://simplytest.me/project/drustack/7.x-25.x).

### Why Another jQuery Module?

For general and generic jQuery update functionality, you should always
consider another [jQuery
Update](http://drupal.org/project/jquery_update) module which started
since 2007-04-26.

On the other hand you should consider about using this module because
of:

-   Purely design for assist [TWBS](https://drupal.org/project/twbs),
    which means you will have the best compatibility when using both
    together
-   Fetch libraries directly from original repository and handle
    initialization with [Libraries
    API](https://drupal.org/project/libraries); [jQuery
    Update](http://drupal.org/project/jquery_update) bundle all
    libraries into it's own code repository and initialize manually
-   Only support latest official version of libraries which result as no
    additional configuration required; [jQuery
    Update](http://drupal.org/project/jquery_update) support multiple
    version of jQuery
-   Much simple implementation which handle all upgrade and replacement
    automatically; [jQuery
    Update](http://drupal.org/project/jquery_update) provide more
    customization options

### Known Issues

TWBS jQuery encourage contribution to both Drupal core and 3rd party
modules, to patch their implementation as latest jQuery friendly:

-   Chosen: [Remove jquery\_update
    dependency](https://drupal.org/node/2113097)
-   Media: [Media browser popup not closing itself
    properly.](https://drupal.org/node/2093435)
-   Overlay: [[meta][modules/overlay/overlay-parent.js] Support jQuery
    \>= 1.9.](https://drupal.org/node/2165339)
-   Panels: [Support jQuery \>= 1.9](https://drupal.org/node/2135377)
-   Views: [JS error: Cannot set property 'events' of undefined
    jquery.ui.dialog.patch.js:24](https://drupal.org/node/1995892)

### Author

-   Developed by [Edison Wong](http://drupal.org/user/33940).
-   Sponsored by [PantaRei Design](http://drupal.org/node/1741828).

