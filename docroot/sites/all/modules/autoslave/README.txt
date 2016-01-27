== Installation ==

Copy the autoslave directory from the modules directory to includes/database/, e.g.

%> cd [my-drupal-installation]
%> cp -r sites/all/modules/autoslave/autoslave includes/database/


== Configuration ==
To reduce writes to the database it is recommended to use Memcache (or similar) for session and cache, and syslog (or similar) for logging instead of dblogging.
The following examples are based on a non-memcache setup (the lock_inc is the only memcache specific configuration).

=== Simple ===
1 master, 1 slave no failover

<?php
$databases['default']['default'] = array (
  'driver' => 'autoslave',
  'master' => 'master', // optional, defaults to 'master'
  'slave' => 'autoslave', // optional, defaults to 'autoslave'
// Always use "master" for tables "semaphore" and "sessions"
  'tables' => array('sessions', 'semaphore', 'watchdog'), // optional, defaults to array('sessions', 'semaphore', 'watchdog')
);

$databases['default']['master'] = array (
  'database' => 'mydb',
  'username' => 'username',
  'password' => 'password',
  'host' => 'master.example.com',
  'port' => '',
  'driver' => 'mysql',
  'prefix' => '',
);

$databases['default']['autoslave'] = array (
  'database' => 'mydb',
  'username' => 'username',
  'password' => 'password',
  'host' => 'slave.example.com',
  'port' => '',
  'driver' => 'mysql',
  'prefix' => '',
);

// Use locking that supports force master
$conf['lock_inc'] = 'sites/all/modules/autoslave/lock.inc';

// Workaround for Drush (Drush doesn't support non-pdo database drivers).
// Workaround for update.php (similar problem as Drush).
if (drupal_is_cli() || basename($_SERVER['PHP_SELF']) == 'update.php') {
  $databases['default']['default'] = $databases['default']['master'];
}

?>

=== Extreme ===
1 master pool with 2 dbs, 2 slave pools with each 2 dbs, cross failover

<?php
$databases['default']['default'] = array (
  'driver' => 'autoslave',
  'master' => array('master', 'slave1', 'slave2'),
  'slave' => array('slave1', 'slave2', 'master'),
  'replication lag' => 2, // (defaults to $conf['autoslave_assumed_replication_lag'])
  'global replication lag' => TRUE, // Make replication lag mitigation work cross requests for all users. Defaults to TRUE.
  'invalidation path' => 'sites/default/files', // Path to store invalidation file for flagging unavailable connections. Defaults to empty.
  'watchdog on shutdown' => TRUE, // Enable watchdog logging during shutdown handlers. Defaults to FALSE. Enable only if using non-db watchdog logging.
  'init_commands' => array('autoslave' => "SET SESSION tx_isolation ='READ-COMMITTED'") // For MySQL InnoDB, make sure isolation level doesn't interfere with our intentions. Defaults to empty.
);

$databases['default']['master'][] = array (
  'database' => 'mydb',
  'username' => 'username',
  'password' => 'password',
  'host' => 'master1.example.com',
  'port' => '',
  'driver' => 'mysql',
  'prefix' => '',
);

$databases['default']['master'][] = array (
  'database' => 'mydb',
  'username' => 'username',
  'password' => 'password',
  'host' => 'master2.example.com',
  'port' => '',
  'driver' => 'mysql',
  'prefix' => '',
);

$databases['default']['slave1'][] = array (
  'database' => 'mydb',
  'username' => 'username',
  'password' => 'password',
  'host' => 'slave1.example.com',
  'port' => '',
  'driver' => 'mysql',
  'prefix' => '',
  'readonly' => TRUE, // (defaults to FALSE, required for failover from master to slave to work)
  'weight' => 70 // (defaults to 100)
);

$databases['default']['slave1'][] = array (
  'database' => 'mydb',
  'username' => 'username',
  'password' => 'password',
  'host' => 'slave2.example.com',
  'port' => '',
  'driver' => 'mysql',
  'prefix' => '',
  'readonly' => TRUE,
  'weight' => 30
);

$databases['default']['slave2'][] = array (
  'database' => 'mydb',
  'username' => 'username',
  'password' => 'password',
  'host' => 'slave3.example.com',
  'port' => '',
  'driver' => 'mysql',
  'prefix' => '',
  'readonly' => TRUE,
);

$databases['default']['slave2'][] = array (
  'database' => 'mydb',
  'username' => 'username',
  'password' => 'password',
  'host' => 'slave4.example.com',
  'port' => '',
  'driver' => 'mysql',
  'prefix' => '',
  'readonly' => TRUE,
);

// Use locking that supports force master
$conf['lock_inc'] = 'sites/all/modules/autoslave/lock.inc';

// Workaround for Drush (Drush doesn't support non-pdo database drivers).
// Workaround for update.php (similar problem as Drush).
if (drupal_is_cli() || basename($_SERVER['PHP_SELF']) == 'update.php') {
  $databases['default']['default'] = $databases['default']['master'];
}

?>

In order for failover to work for master to a slave (readonly), the AutoSlave needs to go into read-only mode. You may need to apply this bug-fix to Drupal Core, for it to work properly:

http://drupal.org/node/1889328 - Not all objects respect the query option "throw_exception"


== Patches ==
Bundled with AutoSlave are a couple of patches:

* deadlock-mitigation-7.22.patch:
Instead of delete a row from the variable table when using variable_del(), the variable is just set to NULL. This is to reduce the effect of gap-locking under MySQLs REPEATABLE-READ isolation level. This will of course make the variable table grow in size, but reduce potential deadlocks. The NULL values are not cached, thereby keeping the variable cache at the same size (or less) as without the patch.

* drush-pdo-7.x-5.8.patch:
Drush checks the $databases configuration directly against PDO. As the "autoslave" drivers is not a PDO extension, this check will fail. The patch bypasses this check. If using this patch (which is recommended), the above workaround in the example should not be used.

* update-pdo-7.22.patch:
update.php checks the $databases configuration directly against PDO. As the "autoslave" drivers is not a PDO extension, this check will fail. The patch bypasses this check for autoslave configurations. If using this patch (which is recommended), the above workaround in the example should not be used.

