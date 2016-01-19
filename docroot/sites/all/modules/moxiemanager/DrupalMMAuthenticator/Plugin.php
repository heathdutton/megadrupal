<?php
/**
 * DrupalAuthenticator.php
 *
 * @package MoxieManager.authenicators
 * @author Moxiecode Systems AB
 * @copyright Copyright � 2013, Moxiecode Systems AB, All rights reserved.
 */

// Bootstap drupal
$cwd = getcwd();
@session_destroy();

define("DRUPAL_ROOT", MOXMAN_ROOT . "/../../../../");
chdir(DRUPAL_ROOT);
require_once("includes/bootstrap.inc");
require_once("includes/common.inc");

global $base_url, $base_root, $base_path;

// Setup base_root, base_url and base_path so the sessions will work correctly
// NOTE: DO NOT REMOVE THIS, DRUPAL SESSION WONT WORK WITHOUT THIS.
$base_root = (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on') ? 'https' : 'http';
$base_url = $base_root .= '://'. preg_replace('/[^a-z0-9-:._]/i', '', $_SERVER['HTTP_HOST']);
$base_path = '/' . trim(dirname($_SERVER['SCRIPT_NAME']), '\,/');
$base_path = substr($base_path, 0, strpos($base_path, '/sites/all/modules/'));
$base_url .= $base_path;

drupal_bootstrap(DRUPAL_BOOTSTRAP_SESSION);
$isDrupalAuth = FALSE;

// For the user-psecific file browser the query argument 'uid' is passed.
// With this query argument we can load paths which are set for the currenty
// loaded user.
if (!empty($_SERVER['HTTP_REFERER'])) {
  $referer_url = drupal_parse_url($_SERVER['HTTP_REFERER']);
  if (!empty($referer_url['query']['uid'])) {
    $account = new stdClass();
    $account->uid = $referer_url['query']['uid'];
  }
}

// If the UID has changed, unset the session so the user access checks will be
// done again.
if (isset($_SESSION['moxiemanager_account_granted'])
  && $_SESSION['moxiemanager_account_granted'] !== $account->uid) {
  unset($_SESSION['moxiemanager_account_granted']);
  unset($_SESSION['moxiemanager_drupal_auth']);
}

// Here we will do the required access check. If the user has access we will
// create an authentication session.
if (!isset($_SESSION['moxiemanager_drupal_auth']) || !$_SESSION['moxiemanager_drupal_auth']) {
  // Not cached in session check agains API
  drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

  // By default the access is prohibited.
  $isDrupalAuth = FALSE;

  if (user_access('access moxiemanager files')) {
    if (!empty($account)) {
      if (moxiemanager_user_file_browser_access($account)) {
        $isDrupalAuth = TRUE;
        $_SESSION['moxiemanager_account_granted'] = $account->uid;
      }
    }
    else {
      // No need for extra checks. We are dealing with the currently logged in
      // user.
      $isDrupalAuth = TRUE;
    }
  }
  $_SESSION['moxiemanager_drupal_auth'] = $isDrupalAuth;
}
else {
  $isDrupalAuth = $_SESSION['moxiemanager_drupal_auth'];
}
// Restore path
chdir($cwd);

/**
 * This class handles MoxieManager IPAuthenticator.
 *
 * @package IpAuthenticator
 */
class MOXMAN_DrupalMMAuthenticator_Plugin implements MOXMAN_Auth_IAuthenticator {
  public function authenticate(MOXMAN_Auth_User $user) {
    global $isDrupalAuth;
    global $user;
    global $account;

    $config = MOXMAN::getConfig();

    // If authenticated then
    if ($isDrupalAuth && isset($user)) {
      $config->replaceVariable("user", $user->uid);
      // Use Drupal's configuration.
      drupal_load('module', 'moxiemanager');
      $drupal_config = variable_get('moxiemanager_config_options', moxiemanager_config_defaults());

      // If the account variable is set we're viewing the file browser for a
      // certain user. Here we will try to load extra paths for that user.
      if (!empty($account)) {
        // Load the users stored data.
        $query = db_select('users', 'u')
          ->fields('u', array('data'))
          ->condition('u.uid', $account->uid)
          ->execute();
        $account->data = unserialize($query->fetchField());

        // If there are moxiemanager settings, process that.
        if (!empty($account->data['moxiemanager'])) {
          $personal_rootpaths = array();

          // Iterate over the stored paths.
          foreach ($account->data['moxiemanager'] as $path) {
            if (!empty($path['label']) && !empty($path['value'])) {
              $personal_rootpaths[] = $path['label'] . '=' . $path['value'];
            }
          }
          // Add the paths to moxiemanagers filessystem.rootpath setting.
          if (!empty($personal_rootpaths)) {
            $drupal_config['filesystem.rootpath'] .= ';' . implode(';', $personal_rootpaths);
          }
        }
      }

      if (!empty($_COOKIE['mm_filesystem'])) {
        $rootpaths = array();
        $rootpath = $drupal_config['filesystem.rootpath'];
        foreach(explode(';', $rootpath) as $path) {
          $path_arr = explode('=', $path);
          $rootpaths[reset($path_arr)] = end($path_arr);
        }
        if (isset($rootpaths[$_COOKIE['mm_filesystem']])) {
          $drupal_config['filesystem.rootpath'] = $_COOKIE['mm_filesystem'] . '=' . $rootpaths[$_COOKIE['mm_filesystem']];
        }
      }

      foreach ($drupal_config as $key => $value) {
        $config->put($key, $value);
      }
    }
    return $isDrupalAuth;
  }
}

MOXMAN::getAuthManager()->add("DrupalMMAuthenticator", new MOXMAN_DrupalMMAuthenticator_Plugin());
?>