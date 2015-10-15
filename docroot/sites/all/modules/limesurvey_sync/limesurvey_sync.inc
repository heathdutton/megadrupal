<?php
/**
 * @file
 * More functions for the limesurvey_sync module.
 */

/**
 * Provide link to specific pages of the LimeSurvey admin interface.
 *
 * @param $text
 *   An string. The link text.
 * @param $sid
 *   An numeric. Corresponding to the survey reference ID. Default is NULL.
 * @param $page
 *   A string. The page. Check the function for available values. Default is
 *   'admin'.
 * @param $anchor
 *   An string. The anchor value. Default is empty.
 * @param $absolute
 *   A boolean. For absolute url. Default is TRUE.
 * @param $new_windows
 *   A boolean. Open the LimeSurvey admin interface into a new window. Default
 *   is TRUE.
 * @param $aid
 *   an numeric string, the LimeSurvey answer ID. Default is empty
 *
 * @return
 *   A string, The corresponding url.
 */
function limesurvey_sync_link_to_ls($text, $sid = NULL, $page = 'admin', $anchor = '', $absolute = TRUE, $new_windows = TRUE, $aid = '') {
  $windows_tag = array(
    'attributes' => array(
      'title' => t('Open the LimeSurvey admin interface in a new window'),
      'onclick' => 'window.open(this.href); return false;',
    ),
  );
  $windows = ($new_windows) ? $windows_tag : array();
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $url = limesurvey_sync_admin_url($sid, $page, $anchor, $absolute, $aid);
  return l($text, $url, $windows);
}

/**
 * Watchdog changes on array variables.
 *
 * This function watchdogs changes on array variables, expliciting changes. It
 * does not handle variable with numeric keys but it handles one level of
 * multidimensional arrays.
 *
 * @param $variable
 *   An string. Corresponding to the variable name.
 * @param $old_settings
 *   An array. Corresponding to variable values before changes.
 * @param $new_settings
 *   An array. Corresponding to variable values after changes (actual).
 * @param $module
 *   An string. Corresponding to the module name responsible for changes.
 * @param $severity
 *   An constant. Corresponding to the watchdog severity. Default to
 *   WATCHDOG_NOTICE.
 */
function limesurvey_sync_custom_watchdog($variable, $old_settings, $new_settings, $module = 'limesurvey_sync', $severity = WATCHDOG_NOTICE) {

  if ($new_settings != $old_settings) {
    $all_keys = array_merge($old_settings, $new_settings);
    foreach ($all_keys as $key => $change) {
      $values = array(
        '%variable' => $variable,
        '@field' => $key,
      );
      if (isset($new_settings[$key]) && isset($old_settings[$key])) {
        if ($old_settings[$key] != $new_settings[$key]) {
          $values['%old_val'] = (!is_array($old_settings[$key])) ? $old_settings[$key] : '(' . implode(', ', $old_settings[$key]) . ')';
          $values['%new_val'] = (!is_array($new_settings[$key])) ? $new_settings[$key] : '(' . implode(', ', $new_settings[$key]) . ')';
          // The first two watchdog() parameters should be literal strings.
          if ($module == 'limesurvey_sync') {
            watchdog('limesurvey_sync', 'The %variable settings have changed : @field from %old_val to %new_val', $values, $severity);
          }
          elseif ($module == 'ls_survey') {
            watchdog('ls_survey', 'The %variable settings have changed : @field from %old_val to %new_val', $values, $severity);
          }
        }
      }
      elseif (!isset($old_settings[$key]) && isset($new_settings[$key])) {
        $values['%val'] = (!is_array($new_settings[$key])) ? $new_settings[$key] : '(' . implode(', ', $new_settings[$key]) . ')';
        // The first two watchdog() parameters should be literal strings.
        if ($module == 'limesurvey_sync') {
          watchdog('limesurvey_sync', 'The %variable settings have changed : @field as been added (value is %val)', $values, $severity);
        }
        elseif ($module == 'ls_survey') {
          watchdog('ls_survey', 'The %variable settings have changed : @field as been added (value is %val)', $values, $severity);
        }
      }
      elseif (isset($old_settings[$key]) && !isset($new_settings[$key])) {
        $values['%val'] = (!is_array($old_settings[$key])) ? $old_settings[$key] : '(' . implode(', ', $old_settings[$key]) . ')';
        // The first two watchdog() parameters should be literal strings.
        if ($module == 'limesurvey_sync') {
          watchdog('limesurvey_sync', 'The %variable settings have changed : @field as been deleted (value was %val)', $values, $severity);
        }
        elseif ($module == 'ls_survey') {
          watchdog('ls_survey', 'The %variable settings have changed : @field as been deleted (value was %val)', $values, $severity);
        }
      }
    }
  }
}

/**
 * Return if the LimeSurvey and the Drupal sites are sharing the same storage system.
 *
 * Inform if the LimeSurvey and the Drupal sites are using the same server or
 * the database.
 *
 * @return
 *   An array, values are the not shared properties, availables values are
 *   'driver', 'username', 'host', 'database', 'prefix'. Or False if the
 *   LimeSurvey database connection is not declared into the setting.php, or
 *   NULL if they share the same properties.
 */
function limesurvey_sync_distinct_storage() {
  // Test if The LimeSurvey hosting server is the same that the drupal
  // hosting server.
  global $databases;
  if (!isset($databases['ls']) || !isset($databases['ls']['default']) || !is_array($databases['ls']['default'])) {
    // The LimeSurvey database connection is not declared into the setting.php
    // file.
    return FALSE;
  }

  // Copy the $database variable, keeping only the server informations.
  $server_keys = array(
    'driver' => '',
    'database' => '',
    'host' => '',
    'username' => '',
    'prefix' => '',
    'port' => '',
  );
  $bd_drupal = array_map('drupal_strtolower', array_intersect_key($databases['default']['default'], $server_keys));
  $bd_ls = array_map('drupal_strtolower', array_intersect_key($databases['ls']['default'], $server_keys));
  $diff = array_keys(array_diff_assoc($bd_ls, $bd_drupal));
  return (!empty($diff)) ? $diff : NULL;
}

/**
 * Test if the LimeSurvey synchronization settings are valid.
 *
 * This function tests if the LimeSurvey synchronization settings are valid
 * and displays results into a multimensional array. It checks the connection
 * to the LimeSurvey database, if the LimeSurvey tables exist on the database,
 * the LimeSurvey website connection via http and the time difference between
 * both servers.
 *
 * @param $settings
 *   An array. Corresponding to the drupal variable ls_settings.
 *
 * @return
 *   A multimensional array providing results : Primary keys are 'server',
 *   'table', 'url', 'time' and, on each one, secondary keys are 'title' (a
 *   string, the property tested), 'result' (a boolean, TRUE if the test has
 *   succeeded), 'message' (a string, message to display), and 'help' : a
 *   string, the help text).
 */
function limesurvey_sync_test_interface($settings) {
  $test_result = array();
  $windows_tag = array(
    'attributes' => array(
      'title' => t('Open the link into a new window'),
      'onclick' => 'window.open(this.href); return false;',
    ),
  );

  $dp_settings_file = conf_path() . '/settings.php';
  // Store some distinct datas from LimeSurvey 1.x and 2.x.
  $configfile_1x = 'config.php';
  $configfile_2x = 'application/config/config.php';
  $dbprefix_1x = '$dbprefix';
  $dbprefix_2x = 'tablePrefix';
  $admin_login_page_1x = '/admin/admin.php';
  $admin_login_page_2x = '/index.php/admin/authentication/login';
  $fake_survey_prefix = '/index.php';

  // Load file for the limesurvey_sync_lsversion() function.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  if (limesurvey_sync_ls_global_settings()) {
    $configfile = (limesurvey_sync_lsversion() != '1.x') ? $configfile_2x : $configfile_1x;
    $dbprefix = (limesurvey_sync_lsversion() != '1.x') ? $dbprefix_2x : $dbprefix_1x;
    $replacing_string = t("replacing each data ('driver', 'database', 'username', 'password', 'host') by the correct value from the %lsconfig LimeSurvey file.", array('%lsconfig' => $configfile));
    $find_it_string =  t('you can find it opening the %lsconfig file stored into your LimeSurvey site folder (it is the %dbprefix variable value).', array(
      '%lsconfig' => $configfile,
      '%dbprefix' => $dbprefix,
    ));
  }
  else {
    // The connexion fails, we can not determine if it is an LimeSurvey 1.x or
    // 2.x install.
    $replacing_string = t("replacing each data ('driver', 'database', 'username', 'password', 'host') by the correct value from the %configfile_2x file (LimeSurvey v2.x) or the %configfile_1x (LimeSurvey v1.x) LimeSurvey files.", array('%configfile_1x' => $configfile_1x, '%configfile_2x' => $configfile_2x));
    $find_it_string =  t('you can find it : under LimeSurvey v2.x opening the %configfile_2x file stored into your LimeSurvey site folder (it is the %dbprefix_2x variable value) or under LimeSurvey v1.x opening the %configfile_1x file (it is the %dbprefix_1x variable value).', array(
      '%configfile_1x' => $configfile_1x,
      '%configfile_2x' => $configfile_2x,
      '%dbprefix_1x' => $dbprefix_1x,
      '%dbprefix_2x' => $dbprefix_2x,
    ));
  }

  // Test if The LimeSurvey hosting server has been configured.
  // Set message for some other tests.
  global $databases;
  $server_fails_comment = t('The LimeSurvey database connection test must succeed in order to run this test.');
  $server_fails_help = t('You need to fix the test of the LimeSurvey database connection first.');
  global $databases;
  if (!array_key_exists('ls', $databases) || !array_key_exists('default', $databases['ls']) || !is_array($databases['ls']['default'])) {
    $server_ok = FALSE;
    $server_output = '<strong>' . t('The LimeSurvey site hosting server is not declared yet.');
    $server_output .= '<br />' . t('You have to edit the %file Drupal file (manually or using the form) providing the LimeSurvey database connection informations.', array('%file' => $dp_settings_file)) . '</strong>';
  }
  else {
    $errors = limesurvey_sync_ls_server_install_database_errors($databases['ls']['default'], $dp_settings_file);
    if (!empty($errors)) {
      // The $databases['ls']['default'] variable exists but fails.
      $server_ok = FALSE;
      $server_output =  '<strong>' . t('The LimeSurvey site hosting server is not correctly declared into the %file.', array('%file' => $dp_settings_file)) . '</strong>';
      $server_output .= '<br /><strong>' . t('The database connection to the LimeSurvey site has failed.') . '</strong>';
      $server_output .=  theme('item_list', array('items' => $errors));
    }
    else {
      // The settings.php file is correctly set.
      $server_ok = TRUE;
      $distinct_storage = limesurvey_sync_distinct_storage();
      $use_same_server = !in_array('host', $distinct_storage) && !in_array('port', $distinct_storage);
      if ($use_same_server) {
        $server_output =  t('The LimeSurvey and the Drupal sites are sharing the same hosting server');
        // The database, username, or db_type may be different.
        if (in_array('database', $distinct_storage)) {
          $server_output .=  '<br />' . t('and are using distinct databases');
        }
        elseif (!in_array('prefix', $distinct_storage)) {
          $server_output .=  '<br /><strong>' . t('but the LimeSurvey database prefix is not valid, choose a value other than %invalid.', array('%invalid' => ((!empty($databases['ls']['default']['prefix'])) ? $databases['ls']['default']['prefix'] :  t('empty')))) . '</strong>';
        }
      }
      else {
        $server_output =  t('The LimeSurvey and the Drupal sites are using distinct hosting server.');
      }
      $server_output .= '<br />' . t('The database connection has succeeded.');
      $server_output .=  '<br />' . t("The 'surveys' database table of the LimeSurvey site has been successfully found.");
    }
  }

  $server_list = array();
  $server_list[] = '<strong><em>' . t('The LimeSurvey database connection informations must be appended into the %settings_file Drupal file, as the $databases<strong>[\'ls\']</strong>[\'default\'] variable,', array('%settings_file' => $dp_settings_file)) . '</em> ' . $replacing_string . '</strong>';
  $server_list[] = t('Concerning the <em>LimeSurvey database prefix</em> value,') . ' ' . $find_it_string;
  $server_list[] = t('Note that if you host your LimeSurvey site and your Drupal site on the same database, you have to choose distinct prefix.');
  $server_list[] = t("If the 'survey' database table is not found : make sure if you have correctly installed the !link_ls and set the right database prefix.", array('!link_ls' => l(t('LimeSurvey software'), 'http://www.limesurvey.org', $windows_tag)));
  $server_list[] = t('This step requires write permissions to %setting_file. If you are unsure how to grant file permissions, consult the !handbook.', array('%setting_file' => $dp_settings_file, '!handbook' => l(t('online handbook'), 'http://drupal.org/server-permissions', $windows_tag)));
  $server_help = theme('item_list', array('items' => $server_list));

  $test_result['ls_server'] = array(
    'title' => t('The LimeSurvey database connection'),
    'result' => $server_ok,
    'message' => $server_output,
    'help' => $server_help,
  );

  // Test the LimeSurvey url.
  // Don't use urlencode() because 'http://*' returns 'http%3A%2F%2F*' !
  $url_base = str_replace(' ', '%20', limesurvey_sync_rel2abs($settings['ls_path']));
  // Need php5 for that testing headers values. Drupal 7 runs only under php5.

  if (!ini_get('allow_url_fopen')) {
    // To allow the treatment of URLs (like http:// or ftp://) as files.
    // I don't use : ini_set('allow_url_fopen', 1);
    // Ini_set() function fails. The raison maybe because If Drupal set
    // something using php_admin_value in httpd.conf it is then not possible
    // to be set the value at runtime.
    $tested_url_output = t("<em>allow_url_fopen</em> should be set to '<em>On</em>' into the <em>php.ini</em> file for this test.")
    . '<br /><strong>' . t('Test abort.') . '</strong><br />'
    . t('You need to manually test the !url URL, making sure you can reach your LimeSurvey site.', array('!url' => filter_xss(l($url_base, $url_base, $windows_tag))));
    $url_ok = NULL;
  }
  else {
    $ls_html_admin = $version = $httpcode = $msg = '';
    $admin_login_page = (limesurvey_sync_lsversion() != '1.x') ? $admin_login_page_2x : $admin_login_page_1x;
    $url_admin = $url_base . $admin_login_page;
    $tested_url = $url_admin;
    $tested_url_redirected = limesurvey_sync_find_redirection_url($url_admin);
    // Retrieve HTTP status code :
    $http_response_header = @get_headers($tested_url_redirected);
    if (!empty($http_response_header)) {
      list($version, $httpcode, $msg) = explode(' ', $http_response_header[0], 3);
      $ls_html_admin = @file_get_contents($tested_url_redirected);
    }
    if (empty($httpcode)) {
      // Some configurations fail here. As I don't know why : returns NULL
      // instead of FALSE, because FALSE breaks the form validation.
      $reached = NULL;
      $error_status = t('Undocumented error');
    }
    else {
      $reached = ($httpcode >= 200 && $httpcode < 400);
      // Check the HTTP Status code :
      if (!$reached) {
        switch ($httpcode) {
          case 401:
          case 403:
            $error_status = t("Error 401: unauthorized request");
            break;
          case 400:
            $error_status = t("Error 400: invalid request");
            break;
          case 404:
            $error_status = t("Error 404: page not found");
            break;
          case ($httpcode >= 500):
            $error_status = t("Error %num_error: server replied with an error", array('%num_error' => $httpcode));
            break;
          default:
            $error_status = t("Undocumented error : @mess #@error", array('@mess' => $msg, '@error' => $httpcode));
            break;
        }
      }
      else {
        // The page exists.
        // The word : "LimeSurvey" is on the footer aera in the login admin
        // form page.
        $find_ls_text = (($ls_html_admin !== FALSE && stripos($ls_html_admin, 'LimeSurvey') !== FALSE));
        // Check if the connected LimeSurvey database match the same LimeSurvey
        // install folder via its url.
        $find_ls_text_sid = TRUE;
        if ($server_ok) {
          $fake_sid = limesurvey_sync_fake_survey();
          $ls_url_fake_survey = $url_base . $fake_survey_prefix;
          $query_url = array('absolute' => TRUE);
          if ((limesurvey_sync_lsversion() != '1.x') ) {
            $ls_url_fake_survey .= '/survey/index/sid/' . $fake_sid;
          }
          else {
            $query_url['query']['sid'] = $fake_sid;
          }
          $ls_url_fake_survey_sid = limesurvey_sync_find_redirection_url(url($ls_url_fake_survey, $query_url));
          $ls_html_fakesurvey = @file_get_contents($ls_url_fake_survey_sid);
          // Look for the survey name '#245' for example.
          $find_ls_text_sid = (($ls_html_fakesurvey !== FALSE && strpos($ls_html_fakesurvey, '#' . $fake_sid) !== FALSE));
          // Delete the fake_survey.
          limesurvey_sync_fake_survey($fake_sid, 'delete');
        }
      }
    }

    if ($reached && $find_ls_text) {
      $url_ok = ($find_ls_text_sid); // TRUE or FALSE.
      $tested_url_output = ($find_ls_text_sid) ? t('The LimeSurvey site page has been successfully reached') : t('A LimeSurvey site page has been successfully reached, <strong>but it does not match the same LimeSurvey database</strong>.');
    }
    elseif ($reached) {
      $tested_url_output = t("This page does not contain the word 'LimeSurvey'");
      $url_ok = FALSE;
    }
    else {
      $tested_url_output = t('The LimeSurvey site page has not been reached (%error)', array('%error' => $error_status));
      $url_ok = ($reached === NULL) ? NULL : FALSE;
    }

    $displayed_url = array('!url' => filter_xss(l($tested_url, $tested_url, $windows_tag)));
    if ($tested_url_redirected == $tested_url) {
      $tested_url_output .= '<br />' . t('Tested url is :<br />!url.', $displayed_url);
    }
    else {
      $tested_url_output .= '<br />' . t('Tested url is :<br />!url, redirected to !redirected_url.', array_merge($displayed_url, array('!redirected_url' => filter_xss(l($tested_url_redirected, $tested_url_redirected, $windows_tag)))));
    }
  }

  $test_result['ls_path'] = array(
    'title' => t('The LimeSurvey site url'),
    'result' => $url_ok,
    'message' => $tested_url_output,
    'help' => t('If it fails, you may need to check the <em>LimeSurvey site path</em> setting and save it on the current page.'),
  );

  // Check for the date timezone on both server.
  $ls_gset_text = t('LimeSurvey global setting page');
  $link_ls_time = ($url_ok) ? limesurvey_sync_link_to_ls($ls_gset_text, NULL, 'globalsettings', 'general') : $ls_gset_text;
  if (!$server_ok) {
    $ls_time_ok = FALSE;
    $ls_time_output = $server_fails_comment;
    $ls_time_help = $server_fails_help;
  }
  else {
    // On Drupal 6, the timezone is returned as seconds, but in Drupal 7, it is
    // returned as name (ex: Europe/Paris).
    // Retrieve 4 datas : The Drupal and LimeSurvey servers and sites
    // timezones.
    // Here we check for of a LimeSurvey issue
    //(http://bugs.limesurvey.org/view.php?id=6038).
    // http://www.php.net/manual/en/function.date-default-timezone-get.php
    // LimeSurvey uses the code (on limesurvey/common.php line 89 :
    // @date_default_timezone_set(@date_default_timezone_get());
    // to set the timezone.
    // Retrieve the original date_default_timezone_get() value before Drupal
    // set it.
    $uncheckable_tz = FALSE;
    $prior_php54 = (version_compare(PHP_VERSION, '5.4.0', '<'));
    if ($prior_php54 && !empty($_SERVER['TZ'])) {
      $tz_dr_server = $_SERVER['TZ'];
    }
    elseif (ini_get('date.timezone')) {
      $tz_dr_server = ini_get('date.timezone');
    }
    elseif ($prior_php54) {
      // Querying the host operating system, if supported and allowed by the OS.
      // The value can not be retrieve. Set it arbitrarily to UTC.
      $tz_dr_server = 'UTC';
      $uncheckable_tz = TRUE;
    }
    else {
      $tz_dr_server = 'UTC';
    }
    // On Drupal 6, the timezone is returned as seconds, but in Drupal 7, it is
    // returned as name (ex: Europe/Paris).

    // Retrieve the default LS time difference value as it should be.
    $delta_time = $settings['ls_delta_time'];
    $ls_global_settings = limesurvey_sync_ls_global_settings();
    $ls_timeadjust = $ls_global_settings['timeadjust'];
    // Ignore PHP strict notice if time zone has not yet been set in the
    // php.ini configuration.
    $drupal_timezone = variable_get('date_default_timezone', @date_default_timezone_get());
    $delta_drupal_server = ($drupal_timezone == $tz_dr_server) ? 0 : limesurvey_sync_get_timezone_offset($tz_dr_server, $drupal_timezone);
    $delta_ls_server = $delta_drupal_server + $delta_time;
    $theoric_td = $delta_ls_server * -1;
    $theoric_td_h = round($theoric_td / 3600);
    $ls_timeadjust_h = round($ls_timeadjust / 3600);
    $same_tz = ($ls_timeadjust_h == $theoric_td_h);
    $display_ls_time = ($theoric_td_h == 0 || abs($theoric_td_h) == 1) ? t('@count hour', array('@count' => $theoric_td_h)) : t('@count hours', array('@count' => $theoric_td_h));
    // The test can be called from the admin/config/media/ls_sync form or not.
    $human_check_needed = ((!$use_same_server || $uncheckable_tz));
    if (!$human_check_needed) {
      $tested_delta_time_help = t('The default value for the LimeSurvey <em>Time difference</em> setting is %value.', array('%value' => $display_ls_time));
      if ($same_tz) {
        $ls_time_ok = TRUE;
        $ls_time_output = t('Your Drupal site and your LimeSurvey site are synchronized.');
      }
      else {
        // Only warning, this test is weak, depending on server configuration.
        $ls_time_ok = NULL;
        $ls_time_output = '<strong>' . t('Your Drupal site and your LimeSurvey site are not synchronized, you should set the LimeSurvey Time difference value to %value.', array('%value' => $display_ls_time)) . '</strong>';
      }
    }
    else {
      // A human check is needed : don't ask it again if it has already been
      // checked on the past.
      $ls_time_ok = (!empty($settings['ls_delta_time_was_checked']));
      if ($ls_time_ok) {
        $ls_time_output = t('Your Drupal site and your LimeSurvey site are synchronized (checked on the @date).', array('@date'=> format_date($settings['ls_delta_time_was_checked'])));
        $tested_delta_time_help = t('If the <em>LimeSurvey server time<em> is correct, the default value for the LimeSurvey <em>Time difference</em> setting should be %value.', array('%value' => $display_ls_time));
      }
      else {
        $ls_time_output = t('For running this test, check the LimeSurvey <em>Server time</em> value and submit the page.');
        $tested_delta_time_help = '';
      }
    }

    $tz_list = array();
    $timezone_help = t('Your LimeSurvey site and your Drupal site must be synchronized, the server times and timezone settings taken into account.');
    $tz_list[] = t('You should make sure your !link_drupal_tz is correctly set.', array('!link_drupal_tz' => l(t('Drupal default timezone setting'), 'admin/config/regional/settings', $windows_tag)));
    $tz_list[] = t('Note : on Daylight Saving Time changes, you may need to edit your Drupal and LimeSurvey timezones values.');
    $timezone_help .= theme('item_list', array('items' => $tz_list));
    $ls_time_help = $timezone_help . $tested_delta_time_help;
  }

  $test_result['ls_time'] = array(
    'title' => t('Softwares synchronized on time'),
    'result' => $ls_time_ok,
    'message' => $ls_time_output,
    'help' => $ls_time_help,
  );

  return $test_result;
}

/**
 * Format an custom url in the same way as the url() function will do.
 *
 * This function is used to compare custom url with url generate by the url()
 * function. It takes care of the clean url. For example, convert the url :
 * 'http://www.example.com/?q=page' to 'http://www.example.com/page'.
 *
 * @param $url
 *   A string. The url to format.
 *
 * @return
 *   A string. The formated absolute url.
 */
function limesurvey_sync_drupal_formated_url($url) {
  $queries = array();
  if (strpos($url, '?') !== FALSE) {
    $parse_url = @parse_url($url);
    // Handle unique or multiple arguements.
    $parse_query = (strpos($parse_url['query'], '&') !== FALSE) ? explode('&', $parse_url['query']) : array($parse_url['query']);
    foreach ($parse_query as $arg) {
      $delimiter = strpos($arg, '=');
      if ($delimiter !== FALSE) {
        $queries[drupal_substr($arg, 0, $delimiter)] = drupal_substr($arg, $delimiter + 1);
      }
    }

    // Handle the q arguement.
    if (!empty($queries) && array_key_exists('q', $queries)) {
      $q_query = $queries['q'];
      unset($queries['q']);
      // Handle clean urls.
      if (variable_get('clean_url', '0') && array_key_exists('q', $queries)) {
        $parse_url['path'] .= $q_query;
      }
      else {
        // Need to set the q arg as the first arguement.
        $queries = array_merge(array('q' => $q_query), $queries);
      }
    }
    $scheme = (!empty($parse_url['scheme'])) ? $parse_url['scheme'] . '://' : '';
    $fragment = (!empty($parse_url['fragment'])) ? array('fragment' => $parse_url['fragment']) : array();
    $url = url($scheme . $parse_url['host'] . $parse_url['path'], array_merge(array('query' => $queries), $fragment));
  }
  // Return the absolute url.
  return $url;
}

/**
 * Convert relative url to absolute url.
 *
 * Code from http://stackoverflow.com/a/1243431.
 *
 * @param $rel
 *   A string. The relative url.
 * @param $base
 *   A string. The base url. Default is empty corresponding to the drupal site
 *   base url.
 *
 * @return
 *   A string, the converted absolute url.
 */
function limesurvey_sync_rel2abs($rel, $base = NULL) {
  // Set the base url.
  if (empty($base)) {
    global $base_url;
    $base = $base_url . '/';
  }

  if (empty($rel)) {
    return $base;
  }

  // Return if already absolute URL.
  // Removethe PHP_URL_SCHEME component and add @ to make this function
  // compatible with php < 5.1.2.
  $rel_scheme = @parse_url($rel);
  if (!empty($rel_scheme['scheme'])) {
    return $rel;
  }

  // Queries and anchors.
  if ($rel[0] == '#' || $rel[0] == '?') {
    return $base . $rel;
  }

  // Parse base URL and convert to local variables:  $scheme, $host, $path.
  extract(@parse_url($base));

  // Remove non-directory element from path.
  $path = preg_replace('#/[^/]*$#', '', $path);

  // Destroy path if relative url points to root.
  if ($rel[0] == '/') {
    $path = '';
  }

  $port_url = (!empty($port)) ? ':' . $port : '';
  // Dirty absolute URL.
  $abs = "$host$port_url$path/$rel";

  // Replace '//' or '/./' or '/foo/../' with '/'.
  $re = array('#(/\.?/)#', '#/(?!\.\.)[^/]+/\.\./#');
  for ($n = 1; $n > 0; $abs = preg_replace($re, '/', $abs, -1, $n)) {
  }
  // Absolute URL is ready!
  return (!empty($scheme)) ? $scheme . '://' . $abs : $abs;
}

/**
 * Return an image of a boolean value.
 *
 * @param $bool
 *   A boolean or the NULL value for 'warning'.
 * @param $center
 *   A boolean. If TRUE, align on center Default is TRUE.
 *
 * @return
 *   A string, the html image for TRUE, FALSE or WARNING.
 */
function limesurvey_sync_display_boolean_img($bool, $center = TRUE) {
  $align = ($center) ? ' align="center"' : '';
  $variable = array(
    'width' => 18,
    'height' => 18,
  );

  if ($bool === NULL) {
    $variable['path'] = 'misc/watchdog-warning.png';
    $variable['alt'] = $variable['title'] = t('Warning');
  }
  elseif ($bool) {
    $variable['path'] = 'misc/watchdog-ok.png';
    $variable['alt'] = $variable['title'] = t('Successful');
  }
  else {
    $variable['path'] = 'misc/watchdog-error.png';
    $variable['alt'] = $variable['title'] = t('Failure');
  }

  return '<div' . $align . '>' . theme('image', $variable) . '</div>';
}

/**
 * Disable the overlay.
 *
 * The overlay core module doesn't like external urls
 * (issue : http://drupal.org/node/1152314) neither link opening into a new
 * window. The LimeSurvey sync module checks the http_referrer value in order
 * to allow to access to the LimeSurvey form page. The overlay is not
 * compatible with such feature. Disable it for the LimeSurvey synchronization
 * setting page, the answer node edit page and the answers node type edit page.
 */
function limesurvey_sync_disable_overlay() {
  // Code adapted from the overlay_init() core function.
  global $user;
  $use_overlay = !isset($user->data['overlay']) || $user->data['overlay'];
  if (module_exists('overlay') && drupal_substr(request_uri(), -15) == '?render=overlay' && user_access('access overlay') && $use_overlay) {
    $current_path = current_path();
    // Disable the overlay.
    overlay_close_dialog($current_path, array('query' => drupal_get_query_parameters(NULL, array('q', 'render'))));
  }
}

/**
 * Returns the ultimate url if redirections exist for a provided url.
 *
 * @param $url
 *   A string, the provided url.
 *
 * @return
 *   A string, the ultimate url or the same url if curl is not enabled or if
 *   there is no redirection.
 */
function limesurvey_sync_find_redirection_url($url) {
  // Code from http://stackoverflow.com/a/11699301.
  if (function_exists('curl_version')) {
    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
    curl_setopt($ch, CURLOPT_HEADER, TRUE); // We'll parse redirect url from header.
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, FALSE); // We want to just get redirect url but not to follow it.
    $response = curl_exec($ch);
    preg_match_all('/^Location:(.*)$/mi', $response, $matches);
    curl_close($ch);
    if (!empty($matches[1])) {
      // Handle multiple redirections. As an example : a LimeSurvey v1 database
      // targeting a LimeSuvrey v2 site :
      // Provided patch by the user is 'limesurvey',
      // $url is 'http://example.com/limesurvey/admin/admin.php',
      // redirected to : 'http://example.com/limesurvey/index.php/admin' with
      // [location = '../index.php/admin'], redirected again to :
      // 'http://example.com/limesurvey/index.php/admin/authentication/login'
      // with [location =
      // 'http://example.com/limesurvey/index.php/admin/authentication/login'].
      return limesurvey_sync_find_redirection_url(str_replace(' ', '%20', limesurvey_sync_rel2abs(trim($matches[1][0]), $url)));
    }
  }
  return $url;
}
