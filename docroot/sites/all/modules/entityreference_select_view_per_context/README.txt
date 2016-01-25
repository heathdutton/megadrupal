INTRODUCTION
------------
This module provides a new selection handler that allows you to use different
views for generation of options for entityreference widget, based on context.


REQUIREMENTS
------------
This module requires the following modules:
* Entityreference (https://drupal.org/project/entityreference)
* Views (https://drupal.org/project/views)
* Context (https://drupal.org/project/context)


INSTALLATION
------------
* Install as you would normally install a contributed drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.


USAGE
-------------
1. Create your contexts by using Context module.
2. Create a view with at least one entityreference display.
3. Edit your entityreference field and choose "View Per Context: Filter by an
   entity reference views selected per context" selection handler. In settings
   of this handler you will have listed all your created contexts, and you
   will be able to choose a view per context.
4. When one of contexts with assigned view will be active on the page with the
   widget, the proper view will be used to generate options.


KNOWN ISSUES
-------------
If you have in your selection handler settings - more than one context that is
active at the same time, and have assigned view - you have no influence on
which of those views will be used for generating options. It shouldn't matter
most of the time and can be resolved with careful contexts setup.


MAINTAINERS
-----------
Current maintainers:
* Łukasz Zaroda (Luke_Nuke) - https://drupal.org/user/1890152
