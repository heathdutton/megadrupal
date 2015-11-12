Socrata module - http://drupal.org/project/socrata
==================================================
Provides an integration point for Socrata within Drupal via their SODA2 interface, including a Views
query solution and an input filter.


REQUIREMENTS
------------
* Chaos Tool Suite (ctools) (https://drupal.org/project/ctools) for Socrata Views.
* Views (https://drupal.org/project/views) for Socrata Views.

INSTALLATION
------------
Enable the socrata_views modules for Views integration and the socrata_filter module for an input
filter.  Both modules will enable the base socrata module as a dependency.


CONFIGURATION
-------------
Go to /admin/structure/socrata/add to create a new endpoint.  Endpoints can then be exported or saved to Features.


RECOMMENDED MODULES
-------------------
* Features (https://www.drupal.org/project/features) allows the saving of endpoints to features modules.

* Charting (https://www.drupal.org/node/2363985) and mapping (https://www.drupal.org/node/1704948)
  can be done with several modules with output from a View.


API
---
See socrata.api.php.


MAINTAINERS
-----------
Current maintainers:
* Andy Hieb (arh1) - https://www.drupal.org/u/arh1
* Will Hartmann (PapaGrande) - https://www.drupal.org/u/papagrande

Past maintainer:
* Robert Bates (arpieb) - https://www.drupal.org/u/arpieb

This project is sponsored by:
* Socrata - http://www.socrata.com
