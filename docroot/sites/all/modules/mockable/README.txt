Mockable module
===============

The [Mockable module](https://drupal.org/project/mockable) for Drupal allows you to define [mock objects](http://en.wikipedia.org/wiki/Mock_object)
for a Drupal module (or website) to communicate with external data systems.

Mock objects allow developers to simulate behaviour and system which are not readily controllable. This module is meant for module developers.

Three techniques are available for telling the system to use mock objects or not:

* with drush: drush mockable-set, drush mockable-unset
* with php: mockable_set(), mockable_unset()
* with the administrative user interface at admin/config/development/mockable

Development of this module is sponsored by [TP1](http://tp1.ca/).

Implementation
--------------

Drupal does not make extensive use of object-oriented programming, whereas mock objects are generally understood to use objects and interfaces.

The Mockable module does not require developers to define a set of objects, although it can support them. This module redirects a request to one function (example()) toward another function (example_mock()), in the following circumstances:

* example_mock() exists
* the system has been set to use mock objects for a set of functions that includes example. This can be done by setting mockable_set('*'), mockable_set('example'), mockable_set('exa*'), etc.

If you wish to use objects, you can have example() return your real-data object, and example_mock() return your mock object.

Example 1: my module needs to do something special between 1 a.m. and 2 a.m.
----------------------------------------------------------------------------

Please see the enclosed example module Mockable Time Demo (mockable_time_example) which displays the string "We're closed; please come back later" between 1 a.m. and 2 a.m. Specifically, the README.txt file in that module explains how the module works and how it uses Mockable.

Example 2: my module interacts with a CRM via its API
-----------------------------------------------------

Please see the enclosed example module Mockable CRM Demo (mockable_crm_example) which interacts with a simple external CRM. The README.txt of that module contains more information.

Some notes
----------

* The functions we call with mockable() should have as little logic, indeed as
few lines, as possible. This is because this code will not be called when mock
data is used, which would cause our tests to miss bugs in that code.

* If your real data is randomized, your mock data must be pseudo-random. For example, if your real object returns random data between 1 and 5, your mock object might return [1, 3, 5, 2, 4] always in the same order. Otherwise you can't test the data as your test won't know what to expect.

* There should be no need to enable Mockable on production sites, which is why
it is good practice to (a) avoid declaring Mockable as a dependency of your
module, (b) checking for its presence before calling its functions, and (c)
declaring it as a soft dependency of your module (so the testbot finds it):

    test_dependencies[] = mockable
