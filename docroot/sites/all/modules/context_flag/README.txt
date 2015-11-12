Context_Flag
============

INTRODUCTION
------------
This module provides integration between the Context and Flag modules. When
enabled it creates new Context conditions which can be triggered based on the
value of a Flag.

* For a full description of the module, visit the project page:
  https://drupal.org/project/context_flag
* To submit bug reports and feature suggestions, or to track changes:
  https://drupal.org/project/issues/context_flag

PROVIDED CONTEXT CONDITIONS
---------------------------
The Context Flag module adds three new options for conditions to the Context
management interface.
* Flag - node: This will be active when viewing a node with the identified
  flags applied to it.
* Flag - user page: This will be active when viewing the profile page of a user
  with flags applied to them.
* Flag - current user: This will be active if the user account currently being
  used to view the site has flags applied to it.
* Flag - taxonomy term page: This will be active when viewing a taxonomy term 
  page with flags applied to the term.

FAQS
----
Q: How do I apply this to the anonymous user?
A: Since the Anonymous user has its own role you can do this by using the 
   default &quot;User role&quot; Context condition (included as part of the
   main context module).

REQUIREMENTS
------------
This module requires the following modules:
* Context (https://drupal.org/project/context)
* Flag v3.0 or greater. (https://drupal.org/project/flag)

INSTALLATION
------------
* Install as usual, see the following page for further information:
  https://drupal.org/documentation/install/modules-themes/modules-7

CONFIGURATION
-------------
* The context conditions provided by this module are available through the main
  Context UI at Admin > Structure > Context.

MAINTAINERS
-----------
Current maintainers:
* Michael Raichelson (mraichelson) - https://drupal.org/user/191498

This project has been sponsored by:
* We Make Wonderful Things, LLC
  We Make Wonderful Things is a small technology shop specializing in web
  development (mostly with Drupal). (http://wemakewonderfulthings.com/)
