TWBS Navbar
===========

A very simple (only \~400 lines!!) mobile friendly navigation toolbar
introduced as part of the [TWBS](https://drupal.org/project/twbs)
project to solve mobile editing problems with the Drupal 7 shipped
toolbar.module, that is not very friendly to small screen sizes.

### Key Features

-   Support both mobile and desktop with [Bootstrap
    Navbar](http://getbootstrap.com/components/#navbar) design
-   Elegant icons provided by [Font Awesome](http://fontawesome.io/)
-   Supported modules: dashboard, menu, shortcut, user and
    [devel](drupal.org/project/devel)
-   Different with Drupal core toolbar.module which only provide
    hard-coded user account links, this module direct fetch result from
    your "User menu" so able to customize corresponding menu items with
    normal management style
-   Any 3rd party modules can add its own menu icon by implementing
    [hook\_twbs\_navbar\_view\_alter()](http://drupalcode.org/project/twbs_navbar.git/blob/refs/heads/7.x-3.x:/twbs_navbar.api.php)

### Author

-   Developed by [Edison Wong](http://drupal.org/user/33940).
-   Sponsored by [PantaRei Design](http://drupal.org/node/1741828).
