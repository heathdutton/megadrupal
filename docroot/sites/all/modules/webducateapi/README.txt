The WebducateAPI module provides a generic API functions for communicating with
the Webducate Online Learning Management System.

WebducateAPI was written by Stuart Clark and is maintained by Stuart Clark
(deciphered) and Brian Gilbert (realityloop) of Realityloop Pty Ltd.
- http://www.realityloop.com
- http://twitter.com/realityloop

WebducateAPI was initially sponsored by Webducate.
- http://www.webducate.com.au



Features
--------------------------------------------------------------------------------

* Three (3) API functions:
  * webducateapi_get_courses()
  * webducateapi_get_student_data()
  * webducateapi_create_token()
* Integrates with:
  * Rules module - provides an action for to trigger a Webducate Token to be
      generated.
* Sub-modules:
  * WebducateAPI - Commerce



WebducateAPI - Commerce
--------------------------------------------------------------------------------

The WebducateAPI - Commerce sub-module integrates the WebducateAPI module with
the Commerce module, it creates a Prouct type and Product wrapper content type
and then automatically populates the appropriates entities with your Webducate
courses. It also sets up a Rule so that upon checkout, any purchased Webducate
courses will have a Token automatically generated for the purchaser.

WebducateAPI - Commerce requires the following modules:
* Commerce          - http://drupal.org/project/commerce
* Commerce Features - http://drupal.org/project/commerce_features



TODO
--------------------------------------------------------------------------------

* Add additional API documentation.
