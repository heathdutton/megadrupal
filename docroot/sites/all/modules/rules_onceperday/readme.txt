INTRODUCTION
------------

This module provides an event for Rules that is triggered once per day by
cron, at or soon after the specified hour of the day. The module also provides
a Rules condition that allows a rule to be only run on certain days of the week.


INSTALLATION & CONFIGURATION
----------------------------

1) Install module and enable it.
2) Configure the time of day the event is triggered via:
   Configuration > Workflow > Rules > Once per day

USAGE
-----

Define Rules using the provided "Once per day" event, and optionally also
using the provided "Current day of the week" condition.

The module defines a default disabled rule, that shows how an email can be sent
once a week every Monday.

 
CONTACT
-------

This module was written by, and is maintained by,
Anthony Cartmell <ajcartmell@fonant.com>