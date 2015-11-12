Ctools view access

This module provides a ctools access plugin 
that checks whether a view returns a result.

This is useful for controlling visibility of panels panes based on a view.

Based on code provided by Banezaka, neoxavier and patobrien.

See https://drupal.org/node/1808718 for more information.

DEPENDENCIES
============
Chaos Tool Suite: https://drupal.org/project/ctools
Views: https://drupal.org/project/views

INSTALLATION
============
Install all dependencies (see dependencies above).
Place the ctools_view_access folder in your sites/xxx/modules folder.
Enable via the modules page.

USAGE
============
1. When editing your panel content, select "Visiblity rules: add a new rule"
for the pane you wish to add the rule to.
2. Select "View: A view returns results", click next.
3. Select the view and display that you wish to test against. 
4. Check "Reverse (NOT)" if you wish the panel to show 
 if the view has no results.
5. Click Save.
6. Save and test your panel. 

WARNING
============
As this plugin calls a view to check for access, (mis)use may cause performance
issues on your site. Use responsibly!
