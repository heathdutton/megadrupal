
Details
=================================
This module provides backup and restore functionality via using simple script
files (PHP files containing just an array of restore operations).
Additional scripts can be created or updated and simply placed with existing
ones to become available without uninstalling/installing/updating modules.

Restore also provides a hook to allow modules to provide a restore script
array.

Modules
=================================
There are three core modules:

  * Restore (core)
    This is the core restore module providing the classes and API.
  * Restore (differences ui)
    This module provides a UI to display any differences between the values in
    the database compared to the values in the restore scripts.
  * Restore (generate ui)
    This provides a UI to generate new or update existing restore scripts.

There are several operations provided by the core module:

  * Date format
  * Modules
  * Permissions
  * Roles
  * Variables

And several additional operations via additional modules:

  * Blocks
    Provides backup and restore functionality for blocks.</li>
  * Custom breadcrumbs
    Provides backup and restore functionality for
    (http://drupal.org/project/custom_breadcrumbs) custom "path" breadcrumbs.
  * Fields
    Provides backup and restore functionalty for fields and field instances.
  * Filter formats
    Provides backup and restore functionality for filter formats.
  * Image styles
    Provides backup and restore functionality for image styles.
  * Menus
    Provides backup and restore functionality for menus and menu items.
  * Content types
    Provides backup and restore functionality for content types.
  * Path alias
    Provides backup and restore functionality for path aliases.
  * Redirects
    Provides backup and restore functionality for
    (http://drupal.org/project/redirect) redirects.
  * Rules
    Provides backup and restore functionality for
    (http://drupal.org/project/rules) rules.

Dependencies
=================================
The core modules require (http://drupal.org/project/ctools) ctools, modules
providing additional operations have their own dependencies.

Other modules
=================================
The difference between restore and other modules like
(http://drupal.org/project/features) Features is that restore uses simple
script files which contain the data, and operations do the work, also you
can restore/remove specific parts of a script at anytime rather than the whole
lot just at installation/uninstallation.

Plugins
=================================
Additional operations can be added by using ctools plugins, for restore there
are 3 different type of plugin classes, the operation class which deals with
restoring, removing, conflicts and status checking, diff checking between the
current value and the value in the restore script, and the generate UI methods
for the output/input required to generate a script for a specific entity or
data type.

Examples
=================================
Two examples are provided within the module which show how to use the two
methods of using restore scripts. The first being the restore script files,
and the second is by using the restore scripts hook.

Script file structure
=================================
The script file is a simple PHP script file, declaring a $script array with
all the details, including the 'title' and 'description' of the script, and an
array of operations (indexed by the operation plugin name) containing an array
of actions. Actions are simply blocks of data the operation plugins use to
restore/remove.

$script = array(
  'title' => '',
  'description' => '',
  'operations' => array(
    'operation_plugin_name' => array(
      array( // action 1 array.
        .. data required by the operation ...
      ),
      array( // Another action array.
        .. data required by the operation ...
      ),
    ),
  ),
);
