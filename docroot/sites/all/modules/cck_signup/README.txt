
The CCK Signup module is an alternative to the Signup module [1]. They
primary difference is that this module allows the signups themselves
to be Drupal nodes, and connect signups to events by way of node
reference fields. The other major difference is that the Messaging [2]
& Notifications [3] framework is used for sending out notifications.

This module is not for you if you are looking for simple drop-in-place
signup functionality.

Current features and capabilities
---------------------------------

 * Define a node type to use as a 'signup' pointing back at an 'event' node type.
 * Optionally define a signup node type as a 'group' signup
 * Define and enforce event capacity
 * Send notifications and reminders of signups
 * Define a 'group confirmation' node type and allow individuals to
   confirm attendance of 'group signups'.

An example configuration of thie module will be available in the
upcoming release of Volunteer Rally, and open source volunteer
management application.

CCK Signup is developed and maintained by OpenSourcery: http://www.opensourcery.com

Dependencies
------------
 * node_reference (part of References)

Recommended Modules
-------------------
 * nodereference_url
 * notifications
 * messaging

Support
-------
If you experience a problem with this module or have a problem, file a
request or issue in the CCK Signup queue at
http://drupal.org/project/issues/cck_signup. DO NOT POST IN THE
FORUMS.  Posting in the issue queues is a direct line of communication
with the module authors.

Documentation
-------------
Documentation can be found here: http://drupal.org/node/998334

References
----------
1. http://drupal.org/project/signup
2. http://drupal.org/project/messaging
3. http://drupal.org/project/notifications
