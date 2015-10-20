Permissions Variable is a simple alternative to Features/Strongarm/Hooks based
workflows that introduces global permissions management through $conf that
overrides permissions set in the UI and stored in the database.

The $conf['permissions'] variable is an associative array with permission
machine names as keys and arrays of role names as values. Simply define this
variable inside your settings.php file, or better, in a file included in your
settings.php file, like 'settings.permissions.inc'.

If the first two paragraphs above don't make any sense to you, this module
probably won't help you in any way, you can safely ignore it.

Example format of a settings.permissions.inc file:

<?php
global $conf;
$conf['permissions'] = array();
$permissions = &$conf['permissions'];

$permissions['access administration pages'] = array(
  'administrator',
);

$permissions['access comments'] = array(
  'anonymous user',
  'administrator',
);

// And so on...
?>

For this example, in settings.php:

<?php
  include_once DRUPAL_ROOT . '/path/to/settings.permissions.inc';
?>

Permissions Variable provides a UI for exporting either all permissions
currently set, or PHP code snippets for individual permissions.

For each permission that has been imported from $permissions, the administrative
UI for that permission is disabled and extra help text is displayed explaining
the situation.

Permissions are pushed to the database from the global permissions variable
upon cache clears (optional) and upon saving the permissions admin form.

It's important to be aware that, even though the usage of leverages $conf,
because of limitations in Drupal core's API (specifically that user_access() and
user_role_permissions() are both un-alterable) and the desire to keep this
module simple, permissions are only set upon clearing the cache or saving the
permissions form, unlike other $conf behaviour that is immediate and permanent.
If you set a permission in your database, then override it with
$conf['permissions'], the database won't match $conf until you clear your cache.
For this reason, the module also offers functionality to lock off the admin
permissions form for any permissions found in $conf.

When permissions are being synced to the database from $conf, only the
permissions of enabled modules that are found in the $permissions array will be
touched. All other data in the database will be ignored. This way you can lock
down some permissions but not others and you can safely export permissions for
modules that you can't guarantee will be enabled in all environments.

<h3>Why not just use Features?</h3>

Features is awesome, but it's not automatically the best solution for all
deployments for all configuration in all projects. A few shortfalls of the
Features/Strongarm/Hooks based approach that inspired the creation of this
module are:

- Permissions exported in Features can always be overridden through the UI,
  meaning that the export does not guarantee the configuration is enforced at
  the destination.
- Exporting a permission to a Feature introduces an "artificial" dependency into
  your Feature module.
- Hooks can only be used once a module has been created to implement them and
  only work as long as the module implementing the hook is enabled.
- The auto-generated code from Features can be hard to read/scan over if a
  developer wants to make manual changes.
- Exporting permissions for anything other than the most basic brochureware
  sites is very tedious and can be prone to human error.
- There's no easy way to set per-environment permissions.
- The core permissions UI gives no indication as to which permissions have been
  exported and which are "database only".
- Not particularly portable between projects. Because of the introduction of
  dependencies and reliance on hooks into Features, it's harder than it needs to
  be to re-use partial or full "known good" combinations of roles/permissions
  across "similar but different" projects.
