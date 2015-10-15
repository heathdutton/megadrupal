
-- SUMMARY --

The File Scanner Bonus module is a an add-on module that provides bonus
features to the File Scanner module.

For a full description of the module, visit the project page:
  https://drupal.org/project/file_scanner

To submit bug reports and feature suggestions, or to track changes:
  https://drupal.org/project/issues/file_scanner


-- FEATURES --

* Provides a browsable tree with collapsible folders in the analysis result
  view page.


-- REQUIREMENTS --

* File Scanner module.


-- FAQ --

Q : Why include jQuery 1.10.2 and jquery-treetable 3.1.0 plugin into the
    module?

A : The problem is that jquery-treetable plugin doesn't use a javscript
    closure, so we can't pass the right jQuery object.
    The jquery.treetable.js file has then been modified to work with provided
    jquery-1.10.2.min.js
    Once this issue will be resolved, the jQuery library and jquery-treetable
    plugin will be be removed from the module and will be required by
    implementing the appropriate hooks of Libraries API module and
    jQuery Multi module.

    https://drupal.org/project/jqmulti
    https://drupal.org/project/libraries

     - jquery-treetable : https://github.com/ludo/jquery-treetable
     - https://github.com/ludo/jquery-treetable/issues/138


-- CONTACT --

Current maintainers:
* Ludovic Ramarojaona (dolu) - https://drupal.org/user/374670

This project has been sponsored by:
* Blue4You
  Visit http://www.blue4you.be for more information.

