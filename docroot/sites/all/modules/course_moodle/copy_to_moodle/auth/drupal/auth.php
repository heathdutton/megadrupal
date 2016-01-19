<?php

/**
 * @author Federico Heinz
 * @license http://www.gnu.org/copyleft/gpl.html GNU Public License
 * @package moodle multiauth
 *
 * Authentication Plugin: Drupal Single Sign-on
 *
 * Looks for an authenticated Drupal session in the cookies and,
 * if present, considers the user authenticayed in Moodle, too.
 * Lazily creates accounts in Moodle's database.
 *
 * This module was born after Martin Dougiamas' db authentication module
 * and Martin Langhoff's olpcxs authentication module spent the night
 * together in a seedy motel after getting drunk with cheap schnapps.
 *
 * 2009-10-19  branched from db, olpcxs
 */
if (!defined('MOODLE_INTERNAL')) {
  // /  It must be included from a Moodle page
  die('Direct access to this script is forbidden.');
}

require_once ($CFG->libdir . '/authlib.php');

/**
 * Drupal common database authentication plugin.
 */
class auth_plugin_drupal extends auth_plugin_base {

  /**
   * Constructor.
   */
  function auth_plugin_drupal() {
    $this->authtype = 'drupal';
    $this->config = get_config('auth/drupal');
  }

  function user_login($username, $password) {
    return FALSE;
  }

  function is_internal() {
    return FALSE;
  }

  function can_change_password() {
    return FALSE;
  }

  function can_reset_password() {
    return FALSE;
  }

  function loginpage_hook() {
    global $CFG;
    global $USER;
    global $SESSION;
    global $DB;

    // No point attempting to log if we're already logged in,
    // unless we're anonymous.
    if (isloggedin() && !isguestuser()) {
      return TRUE;
    }
    // Check whethwe we have a Drupal session
    $host = !empty($CFG->drupal_cookie_domain) ? $CFG->drupal_cookie_domain : $_SERVER['HTTP_HOST'];
    $cookie = 'SESS' . substr(hash('sha256', $host), 0, 32);
    $drupal_sid = $_COOKIE[$cookie];
    if (empty($drupal_sid)) {
      return TRUE;
    }
    // Check whether Drupal session is authenticated
    $drupal_user = $DB->get_record_sql("SELECT u.* FROM drupal_users u
      LEFT JOIN drupal_sessions s on (s.uid = u.uid)
      WHERE s.sid = '$drupal_sid' AND s.uid <> 0");
    if ($drupal_user === FALSE) {
      return TRUE;
    }
    // Fetch or create the corresponding Moodle user.
    $user = $this->create_update_user($drupal_user);
    // Something went wrong while creating the user
    if (empty($user)) {
      print_error('auth/drupal: Unable to create Moodle account for <$username>.');
      return TRUE;
    }
    // complete login
    $USER = get_complete_user_data('id', $user->id);
    complete_user_login($USER);
    add_to_log(SITEID, 'user', 'login', "../user/view.php?user={$user->id}", 'autologin'
    );
    // redirect
    if (isset($SESSION->wantsurl)
      and (strpos($SESSION->wantsurl, $CFG->wwwroot) === 0)
    ) {
      $urltogo = $SESSION->wantsurl;
      unset($SESSION->wantsurl);
    }
    else {
      // no wantsurl stored or external - go to homepage
      $urltogo = $CFG->wwwroot . '/';
      unset($SESSION->wantsurl);
    }
    redirect($urltogo);
  }

  function create_update_user($drupal_user) {
    global $CFG;
    global $DB;

    $user = $DB->get_record('user', array('idnumber' => $drupal_user->uid));
    $name = explode(' ', $drupal_user->name, 2);
    $user->firstname = !empty($name[0]) ? $name[0] : ' ';
    $user->lastname = !empty($name[1]) ? $name[1] : ' ';
    $user->idnumber = $drupal_user->uid;
    $user->email = $drupal_user->mail;
    $user->auth = 'drupal';
    $user->username = $drupal_user->name;
    $user->modified = time();
    // maybe the account is new!
    if (empty($user->id)) {
      // add the user to the database if necessary
      $user->mnethostid = $CFG->mnet_localhost_id;
      $user->lang = $CFG->lang;
      $user->confirmed = 1;
      $uid = $DB->insert_record('user', $user);
      $user = $DB->get_record('user', array('idnumber' => $drupal_user->uid));
    }
    else {
      // Update user
      $DB->update_record('user', $user);
    }

    return $user;
  }

  function delete_obsolete_user($user) {
    global $DB;

    if ($this->config->removeuser == 2) {
      if (delete_user($user)) {
        echo "\t";
        print_string('auth_drupaldeleteuser', 'auth_drupal', array($user->username, $user->id)
        );
        echo "\n";
      }
      else {
        echo "\t";
        print_string('auth_drupaldeleteusererror', 'auth_drupal', $user->username);
        echo "\n";
      }
    }
    else if ($this->config->removeuser == 1) {
      $updateuser = new object();
      $updateuser->id = $user->id;
      $updateuser->auth = 'nologin';
      if ($DB->update_record('user', $updateuser)) {
        echo "\t";
        print_string('auth_drupalsuspenduser', 'auth_drupal', array($user->username, $user->id)
        );
        echo "\n";
      }
      else {
        echo "\t";
        print_string('auth_drupalsuspendusererror', 'auth_drupal', $user->username);
        echo "\n";
      }
    }
  }

  function sync_users($do_updates = FALSE) {
    global $CFG;
    global $DB;

    // delete obsolete internal users
    if (!empty($this->config->removeuser)) {
      // find obsolete users
      $remove_users = $DB->get_records_sql("SELECT u.id, u.username, u.email, u.auth " .
        "FROM {$CFG->prefix}user u " .
        "WHERE username NOT IN " .
        "(SELECT name COLLATE utf8_unicode_ci FROM drupal_users users)"
      );
      print_string('auth_drupaluserstoremove', 'auth_drupal', count($remove_users));
      echo "\n";
      if (!empty($remove_users)) {
        foreach ($remove_users as $user) {
          $this->delete_obsolete_user($user);
        }
      }
      // free mem!
      unset($remove_users);
    }
    $users_to_sync = $this->get_userlist();
    print_string('auth_drupaluserstoupdate', 'auth_drupal', count($users_to_sync));
    echo "\n";
    if (count($users_to_sync)) {
      foreach ($users_to_sync as $username) {
        print_string('auth_drupalupdateuser', 'auth_drupal', $username);
        echo "\n";
        $user = $DB->get_record_sql("SELECT users.* FROM drupal_users users " .
          "WHERE name = '" . addslashes($username) . "'"
        );
        $this->create_update_user($user);
      }
    }
  }

  function logoutpage_hook() {
    global $CFG;
    global $DB;
    $host = !empty($CFG->drupal_cookie_domain) ? $CFG->drupal_cookie_domain : $_SERVER['HTTP_HOST'];
    $cookie = 'SESS' . substr(hash('sha256', $host), 0, 32);
    $drupal_sid = $_COOKIE[$cookie];
    if (!empty($drupal_sid)) {
      // This prevents logout, for some reason. Maybe DB session is already dead?
      //$DB->execute_sql("DELETE FROM drupal_sessions sessions WHERE sid = '$drupal_sid'");
    }
  }

  function user_exists($username) {
    return $DB->record_exists_sql("SELECT uid FROM drupal_users users WHERE name = '$username'");
  }

  function get_userlist() {
    /// Init result value
    $result = array();
    // fetch userlist
    $user_names = $DB->get_recordset_sql("SELECT name AS username FROM drupal_users users WHERE uid > 0");
    if ($user_names && !$user_names->EOF) {
      while ($rec = rs_fetch_next_record($user_names)) {
        array_push($result, $rec->username);
      }
    }
    return $result;
  }

  /**
   * Prints a form for configuring this authentication plugin.
   *
   * This function is called from admin/auth.php, and outputs a full page with
   * a form for configuring this plugin.
   *
   * @param array $page An object containing all the data for this page.
   */
  function config_form($config, $err, $user_fields) {
    include 'config.html';
  }

  /**
   * Processes and stores configuration data for this authentication plugin.
   */
  function process_config($config) {
    // set to defaults if undefined
    if (!isset($config->debugauthdb)) {
      $config->debugauthdb = 0;
    }
    if (!isset($config->removeuser)) {
      $config->removeuser = 0;
    }

    $config = stripslashes_recursive($config);
    // save settings
    set_config('debugauthdb', $config->debugauthdb, 'auth/drupal');
    set_config('removeuser', $config->removeuser, 'auth/drupal');

    return TRUE;
  }

}
