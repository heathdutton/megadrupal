
CONTENTS OF THIS FILE
---------------------

 * About Entityqueue Scheduler
 * Dependencies
 * Configuration and features

ABOUT ENTITYQUEUE SCHEDULER
---------------------------

This module provides an Entityqueue handler that allows creating scheduled
subqueues. It allows you to schedule changes to the queue, allowing you not only
to change what items are published in the queue at a given time but also in what
order they should appear.

DEPENDENCIES
------------

This module requires that cron is running.

 * Entityqueue (http://drupal.org/project/entityqueue)
 * Date API (http://drupal.org/project/date)

CONFIGURATION AND FEATURES
--------------------------

When you create a new Entityqueue and select the "Scheduled queue" handler, you
automatically get a "Live" subqueue. From there you can add new subqueues, each
subqueue requires a unique date on which it should be published.
