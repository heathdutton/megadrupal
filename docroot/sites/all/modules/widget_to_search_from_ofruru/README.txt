
CONTENTS OF THIS FILE
---------------------

 * Overview
 * Quick setup
 * Blocks
 * Installation
 * Maintainers

OVERVIEW
--------

Add a Custom Search Site with no settings. Just include a block and you're done.
translation will be enabled automatically 
when the module (when the module is included: locale)

QUICK SETUP
-----------

After installing this module, activate "Widget to search from ofru.ru" at 
admin/config/search/widget_to_search_from_ofruru its settings.
Once you have granted permission for one or more roles to search 
the "Widget to search from ofru.ru".

BLOCKS
------

The include "Widget to search from ofru.ru" block can optionally be enabled at 
admin/structure/block. 
The "Search this site from ofru.ru" block provides a search box.
After entering search terms, the user will 
be moved to the site is-all.ru (via GET request) and the results will 
be displayed. 

INSTALLATION
------------

Place the widget_to_search_from_ofruru directory in 
your sites/all/modules directory.
Enable the "Widget to search from ofru.ru" module at admin/modules, 
configure it at admin/config/search/widget_to_search_from_ofruru, 
and assign permissions for "Widget to search from ofru.ru" 
at admin/people/permissions.

MAINTAINERS
-----------

Authored and maintained by ofru: http://ofru.ru/widgets/?l=en

The current maintainer does not plan to add new features to this module, 
however, patches providing new 
features are welcome and will be reviewed.

For bugs, feature requests and support requests, please use the issue 
queue at http://drupal.org/project/issues/2067875
