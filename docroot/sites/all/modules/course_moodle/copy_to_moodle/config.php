<?php
$CFG = new stdClass();

/**
 * Get CFG from drupal settings file.
 *
 * Running in a function so we are jailed.
 */
function setup_from_drupal_db($CFG) {
  if (!$_SERVER['HTTP_HOST']) {
    return FALSE;
  }

  $cfgfile = file_get_contents(conf_path() . "/settings.php");
  $cfgfile = preg_replace('#^ini_set.*#m', '', $cfgfile);
  $cfgfile = str_replace('<?php', '', $cfgfile);
  eval($cfgfile);
  $CFG->dbtype = $databases['default']['default']['driver'];
  $CFG->dbuser = $databases['default']['default']['username'];
  $CFG->dbpass = $databases['default']['default']['password'];
  $CFG->dbhost = $databases['default']['default']['host'];
  $CFG->dbname = $databases['default']['default']['database'];
  $CFG->prefix = $databases['default']['default']['prefix'] . 'mdl_';
  if (isset($cookie_domain)) {
    $CFG->drupal_cookie_domain = $cookie_domain;
  }
  else {
    $CFG->drupal_cookie_domain = $_SERVER['HTTP_HOST'];
  }

  return TRUE;
}

/**
 * Taken from Drupal's bootstrap.inc.
 */
function conf_path($require_settings = TRUE, $reset = FALSE) {
  static $conf = '';

  if ($conf && !$reset) {
    return $conf;
  }

  $confdir = str_replace('/moodle', '', dirname(__FILE__)) . '/sites';
  $uri_string = $_SERVER['SCRIPT_NAME'] ? $_SERVER['SCRIPT_NAME'] : $_SERVER['SCRIPT_FILENAME'];
  $uri_string = substr($uri_string, 0, strpos($uri_string, '/moodle') +1);
  $uri = explode('/', $uri_string);
  $server = explode('.', implode('.', array_reverse(explode(':', rtrim($_SERVER['HTTP_HOST'], '.')))));
  for ($i = count($uri) - 1; $i > 0; $i--) {
    for ($j = count($server); $j > 0; $j--) {
      $dir = implode('.', array_slice($server, -$j)) . implode('.', array_slice($uri, 0, $i));
      if (file_exists("$confdir/$dir/settings.php") || (!$require_settings && file_exists("$confdir/$dir"))) {
        $conf = "$confdir/$dir";
        return $conf;
      }
    }
  }
  $conf = "$confdir/default";
  return $conf;
}

if (!setup_from_drupal_db($CFG)) {
  die('Could not create moodle config.php from drupal settings.php');
}

$CFG->dbpersist =  false;

// Check for protocol. So we can run moodle in whatever the request came in on.
// IIS ISAPI value is 'off'
$proto = (!empty($_SERVER['HTTPS']) && $_SERVER['HTTPS'] != 'off') ? 'https' : 'http';
$CFG->wwwroot   = "$proto://". $_SERVER['HTTP_HOST'] .'/moodle';
$CFG->dirroot   = dirname(__FILE__);
$CFG->dataroot   = conf_path() . '/files/moodle';
if (!is_dir($CFG->dataroot)) {
  mkdir($CFG->dataroot);
}
$CFG->admin     = 'admin';

$CFG->directorypermissions = 00777;

$CFG->passwordsaltmain = '$1$rIJCHALX$kodAxu6laI7EFPfgla4Ga0';

//
// Force a debugging mode regardless the settings in the site administration
// @error_reporting(1023);  // NOT FOR PRODUCTION SERVERS!
@ini_set('display_errors', '0'); // NOT FOR PRODUCTION SERVERS!
$CFG->debug = 38911;  // DEBUG_DEVELOPER // NOT FOR PRODUCTION SERVERS!
$CFG->debugdisplay = true;   // NOT FOR PRODUCTION SERVERS!

// You can specify a comma separated list of user ids that that always see
// debug messages, this overrides the debug flag in $CFG->debug and $CFG->debugdisplay
// for these users only.
$CFG->debugusers = '2';
/**/

require_once("$CFG->dirroot/lib/setup.php");
