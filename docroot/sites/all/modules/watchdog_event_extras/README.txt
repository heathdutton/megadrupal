Description
-----------
This module overrides the standard log/watchdog report page adding a HOOK_alter
call allowing other modules to
add rows to the log report.

Requirements
------------
Drupal 7.x
dblog core module

Installation
------------
1. Copy the entire watchdog_event_extras directory
   to the Drupal sites/all/modules directory.

2. Login as an administrator. Enable the module in the "Administer" -> "Modules"

3. (Optional) Enable the submodules that you require

4. View a log event page
