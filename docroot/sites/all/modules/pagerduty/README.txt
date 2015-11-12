PAGERDUTY
---------
by eosrei (http://drupal.org/user/372772)


DESCRIPTION
-----------
Integrates with the PagerDuty API (http://pagerduty.com) to create PagerDuty
Trigger Events when Watchdog log entries are generated.

NOTE: Requires an account with PagerDuty.

WARNING: Be careful with severity selection. Drupal generates many watchdog
messages during normal use. Sane defaults have been provided.

FEATURES
--------
PagerDuty event creation is selectable based on Watchdog severity levels.
pagerduty_trigger_event() function to create events in custom modules.

