
/**
 *  @file
 *  README for Migrate Webform
 */

This module is intended to bring the contents of a Drupal 6 Webform
implementation into a new Drupal 7 site, done though the UI using migrate.
It relies on the migrate_d2d package so that it is possible to bring over nodes
created in the webform type without doing any other configration. It could in
theory support reliance on a different node migration class in the future.

Should you need to add custom fields to your nodes, be sure to use migrate 2.6
and enable the ui-related modules so you can see the "edit" tab on each
migration class in the group.

INSTALLATION

 - After enabling, go to /admin/content/migrate
 - Note the group "Webform Migrations" now appears but is empty.

 - Click "Configure Webform Migration"
 - Put in the key of the secondary database you added to settings.php db array
 - Save
 - Back on the migrate page click "Configuration"
 - Click "Register statically-defined classes"
 - Now all your migrations will appear under the "Webform Migrations" group


After you have done this it is important to know some things about how to get
out of trouble if things go wrong:

- If the tables are not found, go to the main Migrate page, click
the "Configuration" button and "Register statically-defined classes" again.

- Should you lose connection to the DB and it keeps importing/rolling back
you can use the "reset" function in the bulk actions on the migrate page and
then restart the process from where you were.

- If you changed code while running and/or did some other major breakage
you can (1) use the remove the migration settings bulk action, (2) re-register
the classes as outlined above. If you had items that were not rolled back
you will have to go delete them the "Drupal way" as removing the migration
settings deletes the map and abandons the content.

- If it is not possible to get to the /admin/content/migrate page for any
reason it is safe to truncate the migrate_status table as long as you
re-register classes after doing so. It will leave partially-migrated settings
in-place (if you did not "remove migration settings" as outlined above, the
content map will remain in-place).

NOTES

 - Files will need a "FilesMigration" or other class. Recommended to use
migrate_d2d. Currently the module does not hook into this yet for the
components.

 - Roles are matched by the name of the role in the old system, not by relying
on a RuleMigration class. This allows migrate_api_alter to change things. It
would be good if someone wrote a patch to do a second way of doing Roles that
utilizes a migration class.

CREDITS

Special thanks to the commerce_migrate_ubercart devs for some ideas I borrowed.

Thanks to the two sponsors who helped make this module possible. If you are
interesed in funding further work on this project please contact me.
