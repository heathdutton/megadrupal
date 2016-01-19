Variable Debug
--------------

This is a development helper module that enables your developers to see:

* A list of the highest memory usage variables stored in the {variable} table
  sorted by highest to lowest. There is also a list of links to drupal.org
  issues to help resolve some known high usage offenders. If you know of an
  issue that exists that aims to resolve in-efficient usage of the variable
  table, please raise a new issue in the issue queue for this module.

* A list of all suspected orphaned variables in the variable table. This is
  determined by whether or not the variable is:

  * Not a variable provided by Drupal core
  * Does not start with an enabled module name

  This can help you find and remove potential stale variables that are of no use
  to you and your site.

It is recommended that this module only be enabled in development, as that is
where the module is most useful. The module has been tested to work on both
MySQL and PostgreSQL, and runs even on large complex sites (with > 1,250
variables).

Related Modules
---------------
* Variable Cleanup - http://drupal.org/project/variable_clean
  Conceptually a similar module, although this one works by scanning your entire
  filesystem for unused variables, which could be in-efficient on larger sites.
  There also is no facility to order variables by memory usage.

* Variable Check - https://drupal.org/project/variablecheck
  This module checks your variable table for consistancy. Apparently a Drupal
  upgrade from Drupal 5 -> Drupal 7 can leave behind issues here. This module
  will work nicely in parallel with this.

Installation
------------
1) Copy the Variable debug module directory to the modules folder in your
  installation.

2) Enable the module using Administer -> Modules (/admin/modules)

Further Reading
---------------
If you wish to read more about the variable system in Drupal and how this works,
please read this great blog post:

* http://www.redleafmedia.com/blog/drupal-7-variables
