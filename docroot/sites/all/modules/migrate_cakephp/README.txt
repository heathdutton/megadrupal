The migrate_cakephp module provides a migration path from CakePHP to
Drupal for your users database.

It does this by providing a replacement for Drupal's password.inc
which is loaded via Drupal 7's pluggable components mechanism. The
file is almost identical to Drupal's default password.inc, but with
an added check for passwords which begin with "$C$" in the database.

These password hashes are validated using the same mechanism that
validates passwords in CakePHP: using your site's Security.Salt and
the hashing mechanism you configured in config.php (or sha256 by default).

To use this module, you must first import all of your users. If you're using
the excellent Migrate module, you should add this to your
MigrationDestinationUser migration:

<?php
// Map the source field
$this->addFieldMapping(NULL, 'password')
     ->issueGroup('DNM')
     ->description('Migrated manually in the "complete" handler.');

// Update the password directly to avoid Drupal taking over
function complete($entity, $row) {
  $password = sprintf('$C$%s', $row->password);
  db_update('users')
    ->fields(array('pass' => $password))
    ->condition('uid', $entity->uid)
    ->execute();
}

?>

If you're doing this manually (this module has no dependency on Migrate) simply
format your passwords as $C$ followed by the md5 hash.

You should then visit admin/content/migrate/cakephp, and enter your
secure CakePHP salt.

That's it - installing the module has registered the alternative password.inc
and uninstalling the module will de-register it. When a user with an old-style
CakePHP password logs in, the hash will automatically be recomputed and this module
will not be called for future logins from that user.

You can uninstall this module when all of your users have logged in at least once.
