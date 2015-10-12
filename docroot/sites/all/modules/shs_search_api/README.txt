CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Configuration
 * Troubleshooting
 * Maintainers

* Introduction
If you try to use shs in a view with search_api it's really impossible; because there is not integration between these two modules. For this use case you have this module.

* Requirements
- Simple Hierarchical Select (shs)
- Search API Views (search_api_views)

* Installation
Install as you would normally install a contributed drupal module. See: https://drupal.org/documentation/install/modules-themes/modules-7 for further information.

* Configuration
The module has no menu or modifiable settings. There is no configurations. When enabled; you use this module like you normally use SHS:
- Add the shs widget to the field in the entity
- Select this widget in views filter
- Done!

* Troubleshooting
If the options for shs aren't available in views; please ensure that you have it selected as the field widget.

* Maintainers
- Rodrigo Espinoza (@rigoucr) https://www.drupal.org/u/rigoucr
- Kevin Porras (@kporras07) https://www.drupal.org/u/kporras07
