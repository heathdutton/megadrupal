-- INTRODUCTION --

This module give the ability to manually invoke resource operations callbacks
defined by Services 3.x compatible modules as Services 2.x provided
out-of-the-box for defined services callbacks.

It is of some help to develop & debug Services resources, particularly when
not using RESTful resources or when creating RPC-styles services.


-- REQUIREMENTS --

* Services (>=3.3)
* Some module implementing the Services API to expose resources


-- INSTALLATION --

Place the module in the sites/SITENAME/module and enable it.


-- CONFIGURATION --

Configure user permissions in Administration > People > Permissions.
Give the 'use service browser' permission to roles allowed to invoke Services
resources callback for debugging purposes.

-- CONTACT --

Current maintainer:
* Simon Morvan (garphy) - http://drupal.org/user/59864

This project has been sponsored by:
* ICI LA LUNE
  Drupal-specialist Web Agency based in Paris, France. ICI LA LUNE provide
  consulting, development (module, theming, ...) and hosting of Drupal projects
  of any kind. Visit http://www.icilalune.com/ to learn more.
