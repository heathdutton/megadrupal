<?php

/*
 * @file
 * Simple command-line startup script for the WhatsApp module.
 *
 * IF YOU DO NOT HAVE DRUSH INSTALLED, scripts/whatsapp_start.php is a simple wrapper
 * around Drupal . To connect WhatsApp, move to the
 * scripts directory and issue the following command:
 * 
 *   php whatsapp_start.php --root /path/to/drupal --url http://www.example.com
 * 
 * --root refers to the full path to your Drupal installation directory and
 * allows you to execute bot_start.php without moving it to the root directory.
 * --url is required (and is equivalent to Drupal's base URL) to trick Drupal
 * into thinking that it is being run as through a web browser. It sets all
 * the required Drupal environment variables.
 * 
 * If you want to run the bot as a background process, try:
 * 
 *   nohup php whatsapp_start.php --root /path/to/drupal --url http://www.example.com &
 */

set_time_limit(0); // ignore max_execution_time.
$script_name = array_shift($_SERVER['argv']);

// --root allows us to keep this script in the bot.module directory
// without moving it to the root of the Drupal install, and --url
// is required to trick Drupal into thinking it's a web request.
// --url should be the same thing as your Drupal $base_url.
while ($param = array_shift($_SERVER['argv'])) {
  switch ($param) {
    case '--root':
      $drupal_root = array_shift($_SERVER['argv']);
      is_dir($drupal_root) ? chdir($drupal_root) : exit("ERROR: $drupal_root not found.\n");
      define('DRUPAL_ROOT', dirname('.')); // we're now where we need to be, so hellOooO!
      break;

    case '--url':
      $drupal_base_url = parse_url(array_shift($_SERVER['argv']));
      if (!$drupal_base_url || !$drupal_base_url['host']) { exit("ERROR: No URL was passed via --url.\n"); }
      $drupal_base_url['path'] = isset($drupal_base_url['path']) ? $drupal_base_url['path'] : '/';
      $_SERVER['HTTP_HOST'] = $drupal_base_url['host']; // this is all very boring, no?
      $_SERVER['PHP_SELF'] = $drupal_base_url['path'] . '/' . $script_name;
      $_SERVER['REQUEST_URI'] = $_SERVER['SCRIPT_NAME'] = $_SERVER['PHP_SELF'];
      $_SERVER['REMOTE_ADDR'] = NULL; // any values here do rather...
      $_SERVER['REQUEST_METHOD'] = NULL; // ...odd things. uh huh.
      break;
  }
}

// Load in required libraries.
require_once DRUPAL_ROOT . '/includes/bootstrap.inc';
// Gotta bootstrap everything in before d_g_p.
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'whatsapp') . '/whatsapp.smartconnection.inc';

// Start the shindig.
whatsapp_smartconnection_start();
