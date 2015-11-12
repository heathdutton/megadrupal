-- Overview --

Adds a login and register link in the nav bar. Opens them in Bootstrap Modals.
This module is intended for use with the Bootstrap theme.
By default the login and register links are added to the navigation region,
but they can be moved on the block admin page.

-- Requirements --

Chaos tool suite (ctools) (https://www.drupal.org/project/ctools)
Bootstrap 3 (https://www.drupal.org/project/bootstrap)

-- Installing --

Just enable the module.
You should now see login and register links on the right of the main
bootstrap nav bar.
There are no settings for the module but you can override output as
outlined below.

-- Overriding output --

override the theme_bootstrap_login_modal_output($vars) function

-- Similar / related modules --

Twitter Bootstrap Modal (https://www.drupal.org/project/twitter_bootstrap_modal)
The Twitter Bootstrap Modal module provides functionality to enable modals
for blocks and loads them via ajax. As opposed to this module which only adds
links to open modals for login and register forms in the default bootstrap nav
and does not load them via ajax (though does submit via ajax).
