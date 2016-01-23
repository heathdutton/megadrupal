---
Voip Call Queuing Module
---

Description:-

Queues up outgoing dial calls and text messages for VoIP Drupal.
http://drupal.org/project/voipdrupal

Allows call items to be scheduled (delayed execution). Allows
queues to restrict the number of running calls. Handles redialing
calls that fail, and restricting the number of redial attempts.
Hangs up calls that last longer than the job lease (configurable
maximum call length).

Dependencies/Installing:-

The module depends on Advanced Queue 
http://drupal.org/project/advancedqueue
for queue functionality with status; and for handling deloying
concurrent jobs via drush hook.
You will need to configure drush, as described for advancedqueue.

If you require cron to run jobs; or to run the related tests
this patch https://drupal.org/node/1914202 is needed.

API:-

The functions voipqueue_add_dial() and voipqueue_add_text()
give simple methods to add calls and texts to the default queue,
and examples of how to use the classes. For more advanced 
requirements use the class methods directly. Also see voipqueue_test
module for all possibilities.
