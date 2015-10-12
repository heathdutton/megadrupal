<?php
/**
 * @file
 * A sample cron file to run Subfolders Domain.
 */

/*
 * README.txt Point 5.
 * 5. Run drush cron
 * (or /usr/bin/php5
 * /path_to_your_drupal/sites/all/modules/subfolders_domain/
 * subfolders_domain_cron.php)
 * Using /usr/bin/php5 ignore the PHP Notice error :  Undefined index:
 * REMOTE_ADDR in bootstrap.inc
 * [Note: Do not foreget to configure your crontab for Subfolders Domain]
 * The Subfolders Domain cron will not run with Drupal's cron.php in web mode
 * because this one needs sufficient rights to the creation of new alias on the
 *  Drupal root.
 * For the configuration I suggest you copy the subfolders_domain_cron.php
 * file outside your Drupal installation and set up another cron job for the
 * Subfolders Domain to run independently.
 * A sample crontab entry to run Subfolders Domain every minute would look
 * like: * * * * * /usr/bin/php5 /path_to_your_subfolders_domain_cron.php
 * (outside your Drupal root)
 */
// NOTE Change $drupal_root path according to your installation.
// define('DRUPAL_ROOT', 'path_to_drupal_installation');
// Example : define('DRUPAL_ROOT', '/var/www/drupal');
// $drupal_root = 'path_to_drupal_installation';
// $drupal_root = '/var/www/drupal';
// TODO (tobe changed).
// Cron from Drupal installation (LINUX).
if (isset($_SERVER['PWD']) && file_exists($_SERVER['PWD'] . "/sites/all/modules/subfolders_domain/subfolders_domain_cron.php")) {
  $drupal_root = $_SERVER['PWD'];
}
if (isset($drupal_root)) {
  define('DRUPAL_ROOT', $drupal_root);
  include_once DRUPAL_ROOT . '/includes/bootstrap.inc';
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
  subfolders_domain_create_alias();
}
