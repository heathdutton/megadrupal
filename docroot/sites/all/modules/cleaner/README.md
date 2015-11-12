INTRODUCTION
------------
The Cleaner module allows the admin to set a schedule for clearing
caches, watchdog, and old sessions.

There are function in Drupal which will cause "expired" entries
in some cache tables to be deleted.

There are still times and/or cache tables that don't get cleared
in any of those scenarios. Many sites will not be impacted by this,
but a few will (just search on drupal.org and you will see many posts
from people having problems).

INSTALLATION
------------
Standard module installation applies.

SETTINGS
--------
The module will do nothing until you enable its clearing functions
(for your own protection).

The module settings can be found at
Admin >> Configuration >> System >> Cleaner.

1. Select the frequency (interval) at which it will run.
   Note that the Cron settings may change the actual frequency of execution.
2. Select which clearing functions you desire.

LOGGING
-------
This module will log its actions.

EXTENDING THE MODULE
--------------------
This module can be hooked into to extend its functionality.
I'm not sure why you would want to do this rather than code your own,
but it's there.

hook_cleaner_settings()
  Returns the useful part of a settings form as a keyed array,
  with the key being the module name.
  See cleaner_cleaner_settings().

hook_cleaner_run()
  Called when Cron triggers the site (run-time).
  There are no calling parameters or returns.
  See cleaner_cleaner_run().
