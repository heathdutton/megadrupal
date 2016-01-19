<?php
/**
 * @file
 * API functions for the ls_sync module
 */

/**
 * LimeSurvey Database class.
 *
 * This class provides an extension of the drupal database abstraction class.
 * The main purpose of this class is to access to a protected value : the
 * current active connection key. This class is not stored into the .module
 * file because it should be called only if necessary (not for other modules).
 */
class limesurvey_syncDatabase extends Database {
  /**
   * Returns current active connection key.
   *
   * Note that it is a public function.
   *
   * @return
   *   A string, most of the time : 'default' or 'ls'.
   */
  public function limesurvey_sync_active_db() {
    return self::$activeKey;
  }
}

/**
 * Return the active database connection name.
 *
 * Retrieve the active database connection name from the global variable
 * $active_db, using the db_set_active() Drupal function;
 *
 * @param $settings
 *   An array. Corresponding to the drupal variable ls_settings.
 *
 * @return
 *   A string, the active database name
 */
function limesurvey_sync_active_db_name() {
  // Note : the Database::$activeKey provide the current active database key
  // but this is a protected value from the Database class. We create a child
  // class, for retrieving its value from a public function.
  $lms = new limesurvey_syncDatabase();
  return $lms->limesurvey_sync_active_db();
}

/**
 * Activate the LimeSurvey database.
 *
 * Function called before each query or group of querys on the LimeSurvey
 * database.
 *
 * @return
 *   A string, the name of the previously active database, or FALSE if the
 *   LimeSurvey connexion fails.
 */
function limesurvey_sync_db_open() {
  if (limesurvey_sync_test_ls_db()) {
    $current_db = limesurvey_sync_active_db_name();
    // Don't call the db_set_active() function if it not necessary.
    if ($current_db != 'ls') {
      return db_set_active('ls');
    }
    return 'ls';
  }
  return FALSE;
}

/**
 * Desactivate the LimeSurvey database.
 *
 * Function called after each query or group of querys on the LimeSurvey
 * database, to allow the system to switch back to the Drupal database or keep
 * staying on the LimeSurvey database depending on the previously active
 * database.
 *
 * @param $db_to_active
 *   A string. the name of the database connection to active on closing the
 *   LimeSurvey database connection. Default is 'default', corresponding to the
 *   Drupal system database.
 *
 * @return
 *   A string, the name of the current active database, after the
 *   limesurvey_sync_db_close() is called.
 */
function limesurvey_sync_db_close($db_to_active = 'default') {
  // Don't call the db_set_active() function if it not necessary.
  $current_db = limesurvey_sync_active_db_name();
  if ($current_db != $db_to_active) {
    db_set_active($db_to_active);
  }
  return $db_to_active;
}

/**
 * Returns TRUE if a valid database connexion to LimeSurvey is set.
 *
 * This function checks if a $database['ls']['default'] variable is set, if the
 * connexion succeed on that database and if a 'surveys' table exists with the
 * provided database prefix.
 *
 * @return
 *   An boolean, TRUE if the LimeSurvey database connexion is valid.
 */
function limesurvey_sync_test_ls_db() {
  // Check the connection one time per script.
  static $ls_db_checked = FALSE;
  static $ls_db_ok = FALSE;
  if (!$ls_db_checked) {
    global $databases;
    if (array_key_exists('ls', $databases) && array_key_exists('default', $databases['ls']) && is_array($databases['ls']['default'])) {
      $errors = limesurvey_sync_ls_server_install_database_errors($databases['ls']['default']);
      if (empty($errors)) {
        $ls_db_ok = TRUE;
      }
    }
    $ls_db_checked = TRUE;
  }
  return $ls_db_ok;
}
/**
 * Provide the LimeSurvey database table name.
 *
 * Depending on the selected table, concatenate the LimeSurvey database table
 * name to the survey ID. For example,
 * limesurvey_sync_table('surveys', 12356) returns 'surveys_123456'.
 *
 * @param $table
 *   A string. Corresponding to the LimeSurvey table generic name. Default is
 *   'survey'.
 * @param $sid
 *   (optional) An integer. Corresponding to the survey ID reference. Default
 *   is empty.
 *
 * @return
 *   A string, corresponding to the LimeSurvey database name.
 */
function limesurvey_sync_table($table = 'surveys', $sid = '') {
  switch ($table) {
    // Handle tokens and answers tables : they need the $sid parameter.
    case 'tokens':
    case 'survey':
    case 'answer':
      // Watch out ! The answerS table exists to in the LimeSurvey schema !!
      if ($table == 'answer') {
        // Answer is an more explicit name for the survey answers table name.
        $table = 'survey';
      }
      $table_name = $table . '_' . (string) $sid;
      break;
    default:
      $table_name = $table;
  }
  return check_plain($table_name);
}


/**
 * Provide The LimeSurvey global settings.
 *
 * Two LimeSurvey global settings are used into the LimeSurvey Sync module :
 * 'timeadjust' for the Timezone (and date) features and 'DBVersion' for
 * handling various bugs or features depending on the LimeSurey build version.
 *
 * @return
 *   An array, the LimeSurvey global settings values keyed fy there name, or
 *   FALSE if the settings_global page does not exist.
 */
function limesurvey_sync_ls_global_settings() {
  static $ls_gs = array();
  if (empty($ls_gs)) {
    // Set the setting names to retrieve with their default values.
    // Set keys to lowercase.
    $settings_to_store = array(
      'timeadjust' => 0,
      'dbversion' => 0,
    );
    $table_exists = FALSE;
    if ($previous_db = limesurvey_sync_db_open()) {
      $table_exists = db_table_exists('settings_global');
      limesurvey_sync_db_close($previous_db);
    }
    if ($table_exists) {
      // Retrieve the limeSurvey global settings values.
      limesurvey_sync_db_open();
      $gs_query = db_query('SELECT stg_name, stg_value FROM {' . limesurvey_sync_table('settings_global') . '}');
      while ($setting = $gs_query->fetchAssoc()) {
        $property = drupal_strtolower($setting['stg_name']);
        if (array_key_exists($property, $settings_to_store)) {
          $dp_value = $setting['stg_value'];

          // Set some adjustements on specific settings.
          if ($property == 'timeadjust') {
            // Set the timeadjust in seconds.
            // Convert '-12 hours' into -12*3600.
            if (preg_match('#minutes$#i', $dp_value)) {
              $dp_value = ((int) $dp_value) * 60;
            }
            elseif (preg_match('#hours$#i', $dp_value)) {
              $dp_value = ((int) $dp_value) * 3660;
            }
            else {
              $dp_value = ((int) $dp_value);
            }
          }
          elseif ($property == 'dbversion') {
            $dp_value = ((int) $dp_value);
          }

          // Save the settings for Drupal use.
          $ls_gs[$property] = $dp_value;
        }
      }
      limesurvey_sync_db_close($previous_db);
      // Make sure the keys exist (avoid to return an error if the table does
      // not exist).
      foreach ($settings_to_store as $name => $default_value) {
        $property = drupal_strtolower($name);
        if (!isset($ls_gs[$property])) {
          $ls_gs[$property] = $default_value;
        }
      }
    }
    else {
      return FALSE;
    }
  }
  return $ls_gs;
}

/**
 * Fix for the LimeSurvey storing date system.
 *
 * The LimeSurvey software stores datetime values including the timezone value
 * whereas the Drupal system stores timestamp value and apply then the timezone
 * parameter. This function returns the correct datetime value applying the
 * LimeSurvey timezone from a timestamp. As long as the LimeSurvey site and the
 * Drupal site are using the same timezone (this is checked by the
 * synchronization setting page), there is no adaptations.
 *
 * @param $timestamp
 *   A integer. The timestamp to be convert to the LimeSurvey datetime system,
 *   default is NULL corresponding to the current time.
 *
 * @return
 *   A string, the english formated corresponding date.
 */
function limesurvey_sync_fixdate_drupal_to_ls($timestamp = NULL) {
  if ($timestamp == NULL) {
    $timestamp = REQUEST_TIME;
  }
  $ls_settings = variable_get('ls_settings', array('ls_delta_time' => 0));
  $delta_time = $ls_settings['ls_delta_time'];
  $ls_global_settings = limesurvey_sync_ls_global_settings();
  $ls_timeadjust = $ls_global_settings['timeadjust'];
  // Ignore PHP strict notice if time zone has not yet been set in the php.ini
  // configuration.
  $drupal_timezone = variable_get('date_default_timezone', @date_default_timezone_get());
  // The date() php function uses the user timezone in order to
  // determine the timezone.
  $user_timezone = drupal_get_user_timezone();
  $delta_tz = ($drupal_timezone == $user_timezone) ? 0 : limesurvey_sync_get_timezone_offset($drupal_timezone, $user_timezone);
  $delta_drupal_server = ($drupal_timezone == 'UTC') ? 0 : limesurvey_sync_get_timezone_offset($drupal_timezone, 'UTC');
  return date('Y-m-d H:i:s', ($timestamp + $delta_tz - $delta_drupal_server + $ls_timeadjust + $delta_time));
  // Note :
  // Concerning token validfrom and validuntill values : use the data including
  // the timeadjust parameter (application/controllers/survey/index.php line
  // 490) on v2.00+, dbversion 164, build number 131009;
  // But they refer to the database time on application/models/Token.php line
  // 416 (no timeadjust) on v2.05, dbversion 174, build number 140116. This
  // LimeSurvey Bug is reported on http://bugs.limesurvey.org/view.php?id=8563.
}

/**
 * Convert english date to the synchronized timestamp.
 *
 * As an example, convert '2012-01-01 10:22:00' to 1325409720, adjusting it if
 * Drupal and LimeSurvey servers are not synchronized or use a specific
 * timezone.
 *
 * @param $english_date
 *   A string. The english formated date.
 *
 * @return
 *   A numeric. The Drupal timestamp corresponding value.
 */
function limesurvey_sync_fixdate_ls_to_drupal($english_date) {
  if (empty($english_date)) {
    return '';
  }
  $ls_settings = variable_get('ls_settings', array('ls_delta_time' => 0));
  $delta_time = $ls_settings['ls_delta_time'];
  $ls_global_settings = limesurvey_sync_ls_global_settings();
  $ls_timeadjust = $ls_global_settings['timeadjust'];
  return (strtotime($english_date . ' UTC') - $ls_timeadjust - $delta_time);
}

/**
 * Returns the offset from the remote timezone to the origin timezone, in seconds.
 *
 * Note :this function works only for PHP >= '5.2.0', but Drupal 7 needs at
 * least PHP '5.2.4'.
 *
 * @param $remote_tz
 *   A string, the remote timezone (ex : 'America/Los_Angeles').
 * @param $origin_tz
 *   A string, if NULL the servers current timezone is used as the origin.
 *   Default is 'UTC'.
 *
 * @return
 *   An integer, the offset in seconds.
 */
function limesurvey_sync_get_timezone_offset($remote_tz, $origin_tz = 'UTC') {
  if ($origin_tz === NULL) {
    if (!is_string($origin_tz = date_default_timezone_get())) {
      return FALSE; // A UTC timestamp was returned -- bail out!
    }
  }
  $origin_dtz = new DateTimeZone($origin_tz);
  $remote_dtz = new DateTimeZone($remote_tz);
  $origin_dt = new DateTime("now", $origin_dtz);
  $remote_dt = new DateTime("now", $remote_dtz);
  $offset = $remote_dtz->getOffset($remote_dt) - $origin_dtz->getOffset($origin_dt);
  return $offset;
}

/**
 * Append the timezone to a LimeSurvey english date.
 *
 * This function is used to display non dynamic date that won't be adapted to
 * the user timezone. For instance : the submidate, or the startdate stored
 * into an html table. '2012-01-01 10:22:00' may returns
 * '2012-01-01 10:22:00 Europe/Paris'.
 *
 * @param $english_date
 *   A string. The LimeSurvey english formated date.
 *
 * @return
 *   A translated date string in the requested format.
 */
function limesurvey_sync_format_static_lsdate($english_date) {
  if (empty($english_date)) {
    return t('No date');
  }
  $timestamp = limesurvey_sync_fixdate_ls_to_drupal($english_date);
  $drupal_timezone_name = variable_get('date_default_timezone', @date_default_timezone_get());
  // Don't append the timezone for sites that don't care about timezone.
  $append_tz = (module_exists('locale')) ? ' ' . $drupal_timezone_name : '';
  return format_date($timestamp, 'short', '', $drupal_timezone_name) . $append_tz;
}

/**
 * Provide the LimeSurvey major version.
 *
 * The LimeSurvey version is retrieved by the DBVersion value stored into the
 * database. I don't use the value from the version.php file because it may be
 * not reachable if the LimeSurvey software is stored on an external server.
 * Note :
 * - the 1.92+ version build 120919 owns the 155.6 DBVersion,
 * - the v2.0+ build 121019 owns the 163 DBVersion.
 * I arbitrary assume that the LimeSurvey v2.0 have started from the 160
 * DBVersion.
 *
 * @return
 *   A string, '1.x' or '2.x', or FALSE if it fails.
 */
function limesurvey_sync_lsversion() {
  $ls_v2_dbversion = 160;
  static $lsversion = NULL;
  // Use cache.
  if ($lsversion ===  NULL) {
    $ls_gs = limesurvey_sync_ls_global_settings();
    if (!empty($ls_gs['dbversion'])) {
      $lsversion = ($ls_gs['dbversion'] < $ls_v2_dbversion) ? '1.x' : '2.x';
    }
    else {
      // If there are some synchronization troubles, act as if it is the
      // LimeSurvey v2.x version (the actual stable version).
      $lsversion = '2.x';
    }
  }
  return $lsversion;
}

/**
 * Provide url to specific pages of the LimeSurvey admin interface.
 *
 * This function is safe, sanitize values that may be displayed with no more
 * control (via form_set_error() as an example).
 *
 * @param $sid
 *   A numeric. Corresponding to the survey reference ID. Default is NULL.
 * @param $page
 *   A string. The page. Check the function for available values. Default is
 *   'admin'.
 * @param $anchor
 *   A string. The anchor value. Default is empty.
 * @param $absolute
 *   A boolean. For absolute url. Default is TRUE.
 * @param $aid
 *   an numeric string, the LimeSurvey answer ID. Default is empty
 *
 * @return
 *   A string, The corresponding url.
 */
function limesurvey_sync_admin_url($sid = NULL, $page = 'admin', $anchor = '', $absolute = TRUE, $aid = '') {
  $options = array(
    'absolute' => $absolute,
    'external' => TRUE,
  );

  $ls_v2 = (limesurvey_sync_lsversion() != '1.x');
  $admin_page = ($ls_v2) ? '/index.php/admin/' : '/admin/admin.php';
  $ls_settings = variable_get('ls_settings', array('ls_path' => ''));
  $url = check_plain($ls_settings['ls_path'] . $admin_page);
  if ($absolute) {
    module_load_include('inc', 'limesurvey_sync', 'limesurvey_sync');
    $url = limesurvey_sync_rel2abs($url);
  }

  switch ($page) {
    case 'globalsettings':
      if ($ls_v2) {
        $url .= 'globalsettings';
      }
      else {
        $options['query']['action'] = 'globalsettings';
      }
      break;
    case 'newsurvey':
      if ($ls_v2) {
        $url .= 'survey/newsurvey';
      }
      else {
        $options['query']['action'] = 'newsurvey';
      }
      break;
    case 'listsurveys':
      if ($ls_v2) {
        $url .= 'survey/index';
      }
      else {
        $options['query']['action'] = 'listsurveys';
      }
      break;
      // Below : need survey ID.
    case 'admin':
      if ($ls_v2) {
        $url .= 'survey/view';
      }
      break;
    case 'editsurveysettings':
      if ($ls_v2) {
        $url .= 'survey/editsurveysettings';
      }
      else {
        $options['query']['action'] = 'editsurveysettings';
      }
      break;
    case 'activate':
      if ($ls_v2) {
        $url .= 'survey/activate';
      }
      else {
        $options['query']['action'] = 'activate';
      }
      break;
    case 'statistics':
      if ($ls_v2) {
        $url .= 'statistics/index';
      }
      else {
        $options['query']['action'] = 'statistics';
      }
      break;
    case 'tokens':
      if ($ls_v2) {
        $url .= 'tokens/index';
      }
      else {
        $options['query']['action'] = 'tokens';
      }
      break;
    case 'browse':
      if ($ls_v2) {
        $url .= 'responses/index';
      }
      else {
        $options['query']['action'] = 'browse';
      }
      break;
    case 'editsurveylocalesettings':
      if ($ls_v2) {
        $url .= 'survey/editlocalsettings';
      }
      else {
        $options['query']['action'] = 'editsurveylocalesettings';
      }
      break;
    case 'responseview':
      if ($ls_v2) {
        $url .= 'responses/view/';
      }
      else {
        $options['query']['action'] = 'browse';
      }
      break;
  }

  switch ($anchor) {
    case 'presentation':
      $options['fragment'] = 'presentation';
      break;
    case 'tokens':
      $options['fragment'] = 'tokens';
      break;
    case 'general':
      $options['fragment'] = 'general';
      break;
    case 'publication':
      $options['fragment'] = 'publication';
      break;
    case 'notification':
      $options['fragment'] = 'notification';
      break;
    case 'copy':
      $options['fragment'] = 'copy';
      break;
  }
  if (preg_match('#^edittxtele[0-9]{1,2}$#', $anchor) || preg_match('#^url_[a-zA-Z\-]{0,12}$#', $anchor)) {
    // Handle : 'edittxtele0' stands for default language, 'edittxtele1' for
    // the first additional language, etc... And the 'url_en', url_fr' for
    // targeting the end url textfield; thanks to the
    // limesurvey_sync_localsettings_anchor() function.
    $options['fragment'] = $anchor;
  }

  $goto_default_page = FALSE;
  switch ($page) {
    case 'admin':
    case 'editsurveysettings':
    case 'activate':
    case 'statistics':
    case 'tokens':
    case 'browse':
    case 'editsurveylocalesettings':
    case 'responseview':
      if (!empty($sid)) {
        if ($ls_v2) {
          $url .= '/surveyid/' . $sid;
        }
        else {
          $options['query']['sid'] =  $sid;
        }
        if ($page == 'responseview') {
          if (!empty($aid) && is_numeric($aid) && $aid > 0) {
            if ($ls_v2) {
              $url .= '/id/' . $aid;
            }
            else {
              $options['query']['id'] =  $aid;
            }
          }
          else {
            $goto_default_page = TRUE;
          }
        }
      }
      else {
        $goto_default_page = TRUE;
      }
      break;
    default:
  }
  if ($goto_default_page) {
    // If sid id not provide, those pages do not exist.
    // Go to the defaut admin page instead.
    if ($ls_v2) {
      $url = $ls_settings['ls_path'] . $admin_page . 'survey/index';
    }
    else {
      unset($options['query']['action'], $options['fragment']);
    }
  }

  // Set 'sid' before 'action' in the url.
  if (isset($options['query']) && is_array($options['query'])) {
    krsort($options['query']);
  }

  return url($url, $options);
}

/**
 * Create or delete a fake empty survey.
 *
 * It is used to check if the LimeSurvey database connexion on drupal matchs
 * the same LimeSurvey site via url. It is usefulled when the server stores
 * several LimeSurvey installations. This function creates a fake survey with
 * a specific name, then the Drupal site try to reach this specific survey. At
 * last, the function delete this survey.
 *
 * @param $sid
 *   A numeric. A fake survey ID value (between 100 and 999). Default is empty,
 *   the survey ID will be randomly generate.
 * @param $action
 *   A string. Values are 'create' or 'delete' for creating or deleteing the
 *   survey. Default is 'create'.
 *
 * @return
 *   A numeric, the fake Survey ID if the creation or deletion succeed or FALSE
 *   if it fails.
 */
function limesurvey_sync_fake_survey($sid = '', $action = 'create') {
  if (!in_array($action, array('create', 'delete'))) {
    return FALSE;
  }
  // Set the previous db.
  module_load_include('inc', 'limesurvey_sync', 'ls_api/limesurvey_sync_api_sync');
  $previous_db = limesurvey_sync_db_open();
  $success1 = $success2 = FALSE;
  if ($action == 'create') {
    // First create a unique SID. Standard SID is 5 digits for LSv1.x and 6
    // digits for LSv2.x. Use 3 digits.
    if (empty($sid)) {
      $sid = rand(100, 999);
    }
    // Create a fake survey with a name, 'listpublic' and 'active' to 'Y'
    // (default are 'N').
    $for_fake_survey = array(
      'sid' => $sid,
      'active' => 'Y',
      'listpublic' => 'Y',
    );
    // Fix colomns with no default value :
    $fix_insert = array(
      'owner_id' => 1,
      'admin' => 'The LimeSurvey Sync module',
      'language' => 'en',
      'datecreated' => limesurvey_sync_fixdate_drupal_to_ls(),
    );
    db_insert(limesurvey_sync_table('surveys'))
      ->fields(array_merge($for_fake_survey, $fix_insert))
      ->execute();
    db_insert(limesurvey_sync_table('surveys_languagesettings'))
      ->fields(array(
      'surveyls_survey_id' => $sid,
      'surveyls_title' => format_string('Fake survey @sid from the drupal site @url', array('@sid' => '#' . ((string) $sid), '@url' => url(NULL, array('absolute' => TRUE)))),
      'surveyls_description' => format_string('This survey is not functionnal, it is generated by the LimeSurvey Sync Drupal module from the synchronisation setting page @url', array('@url' => url('admin/config/media/ls_sync', array('absolute' => TRUE)))),
    ))
      ->execute();
    // There is no schema database so no declared primary key : the returned
    // value from db_insert() is untrusted.
    $success1 = $success2 = TRUE;
  }
  else { // $action == 'delete')
    if (!empty($sid)) {
      $success1 = db_delete(limesurvey_sync_table('surveys'))
        ->condition('sid', $sid)
        ->execute();
      $success2 = db_delete(limesurvey_sync_table('surveys_languagesettings'))
        ->condition('surveyls_survey_id', $sid)
        ->execute();
    }
  }
  limesurvey_sync_db_close($previous_db);
  return ($success1 && $success2) ? ((string) $sid) : FALSE;
}

/**
 * Checks a database connection and returns any errors.
 */
function limesurvey_sync_ls_server_install_database_errors($database, $settings_file = '') {
  // Adapted from the install_database_errors() core function.
  global $databases;
  $errors = array();
  static $ls_test_id = 0;

  if (empty($settings_file)) {
    $settings_file = './' . conf_path() . '/settings.php';
  }

  // Check database type.
  $database_types = drupal_get_database_types();
  $driver = $database['driver'];
  if (!isset($database_types[$driver])) {
    $errors['driver'] = t("In your %settings_file file you have configured @drupal to use a %driver server, however your PHP installation currently does not support this database type.", array('%settings_file' => $settings_file, '@drupal' => t('Drupal'), '%driver' => $driver));
  }
  else {
    // Run driver specific validation
    $errors += $database_types[$driver]->validateDatabaseSettings($database);
    // Fix : the db prefix is not stored into the advanced tab anymore.
    // It seems that 'port' needs to be fixed to.
    foreach (array('db_prefix', 'port') as $property) {
      $prefix_key = $driver . '][advanced_options][' . $property;
      if (array_key_exists($prefix_key, $errors)) {
        $errors[$driver . '][' . $property] = $errors[$prefix_key];
        unset($errors[$prefix_key]);
      }
    }

    // Run tasks associated with the database type. Any errors are caught in the
    // calling function.

    $previous_db = limesurvey_sync_active_db_name();
    try {
      // Rewrite the db_run_tasks() core function.
      Database::loadDriverFile($driver, array('install.inc'));
      $task_class = 'DatabaseTasks_LimeSurveySync_' . $driver;
      $db_ogj = new $task_class();
      $db_ogj->ls_test_id = $ls_test_id;
      // Set a fake connexion on the fly.
      $conn_name = 'ls_test' . ((string) $ls_test_id);
      Database::addConnectionInfo($conn_name, 'default', $database);
      $db_ogj->runTasks();
    }
    catch (DatabaseTaskException $e) {
      // These are generic errors, so we do not have any specific key of the
      // database connection array to attach them to; therefore, we just put
      // them in the error array with standard numeric keys.
      $error_message = $e->getMessage();
      $modified_error_message = $error_message;
      // From DatabaseTasks::runTasks().
      $dp_install_message = '<p>In order for Drupal to work, and to continue with the installation process, you must resolve all issues reported below. For more help with configuring your database server, see the <a href="http://drupal.org/getting-started/install">installation handbook</a>. If you are unsure what any of this means you should probably contact your hosting provider.</p>';
      if (substr($error_message, 0, strlen($dp_install_message)) == $dp_install_message) {
        $modified_error_message = t('<p>In order for Drupal to connect with your LimeSurvey site, you must resolve all issues reported below. If you are unsure what any of this means you should probably contact your LimeSurvey site hosting provider.</p>') . substr($error_message, strlen($dp_install_message));
      }
      $errors[$driver . '][0'] = $modified_error_message;
    }
    // LS connexion was open into DatabaseTasks::connect().
    // Switch back to the Drupal connexion.
    db_set_active($previous_db);
  }

  $ls_test_id++;
  return $errors;
}

/**
 * Specifies installation tasks for MySQL and equivalent databases.
 */
abstract class DatabaseTasks_LimeSurveySync extends DatabaseTasks {

  public $ls_test_id = 0;

  protected $tasks = array(
    array(
      'function' => 'checkEngineVersion',
      'arguments' => array(),
    ),
    array(
      'arguments' => array(
        'CREATE TABLE {drupal_install_test} (id int NULL)',
        'Drupal can use CREATE TABLE database commands.',
        'Failed to <strong>CREATE</strong> a test table on your database server with the command %query. The server reports the following message: %error.<p>Are you sure the configured username has the necessary permissions to create tables in the database?</p>',
        TRUE,
      ),
    ),
    array(
      'arguments' => array(
        'INSERT INTO {drupal_install_test} (id) VALUES (1)',
        'Drupal can use INSERT database commands.',
        'Failed to <strong>INSERT</strong> a value into a test table on your database server. We tried inserting a value with the command %query and the server reported the following error: %error.',
      ),
    ),
    array(
      'arguments' => array(
        'UPDATE {drupal_install_test} SET id = 2',
        'Drupal can use UPDATE database commands.',
        'Failed to <strong>UPDATE</strong> a value in a test table on your database server. We tried updating a value with the command %query and the server reported the following error: %error.',
      ),
    ),
    array(
      'arguments' => array(
        'DELETE FROM {drupal_install_test}',
        'Drupal can use DELETE database commands.',
        'Failed to <strong>DELETE</strong> a value from a test table on your database server. We tried deleting a value with the command %query and the server reported the following error: %error.',
      ),
    ),
    array(
      'arguments' => array(
        'DROP TABLE {drupal_install_test}',
        'Drupal can use DROP TABLE database commands.',
        'Failed to <strong>DROP</strong> a test table from your database server. We tried dropping a table with the command %query and the server reported the following error %error.',
      ),
    ),
    // This one is specific to LimeSurvey.
    array(
      'arguments' => array(
        'SELECT 1 FROM {surveys}',
        'Drupal has found the LimeSurvey survey table',
        'Failed to <strong>found the LimeSurvey survey table</strong>. The server reports the following message: %error.<p>Are you sure the database prefix is correct or a LimeSurvey install does exist on the provided server ?</p>',
        TRUE,
      ),
    ),
  );

  /**
   * Check if we can connect to the LimeSurvey database.
   */
  protected function connect() {
    try {
      // This doesn't actually test the connection.
      // Switch to the LimeSurvey connexion.
      $conn_name = 'ls_test' . ((string) $this->ls_test_id);
      db_set_active($conn_name);
      // Now actually do a check.
      Database::getConnection();
      $this->pass('Drupal can CONNECT to the LimeSurvey database ok.');
    }
    catch (Exception $e) {
      // Switch back to the Drupal connexion.
      $this->fail(st('Failed to connect to your LimeSurvey database server. The server reports the following message: %error.<ul><li>Is the LimeSurvey database server running?</li><li>Does the LimeSurvey database exist, and have you entered the correct LimeSurvey database name?</li><li>Have you entered the correct username and password?</li><li>Have you entered the correct LimeSurvey database hostname?</li></ul>', array('%error' => $e->getMessage())));
      return FALSE;
    }
    // The connexion succeed : keep it open for the rest of the
    // DatabaseTasks::runTasks() process. it will be closed into the
    // limesurvey_sync_ls_server_install_database_errors() function.
    return TRUE;
  }
}

class DatabaseTasks_LimeSurveySync_mysql extends DatabaseTasks_LimeSurveySync {
  /**
   * The PDO driver name for MySQL and equivalent databases.
   *
   * @var string
   */
  protected $pdoDriver = 'mysql';

  /**
   * Returns a human-readable name string for MySQL and equivalent databases.
   */
  public function name() {
    return st('MySQL');
  }

  /**
   * Returns the minimum version for MySQL.
   */
  public function minimumVersion() {
    // From http://manual.limesurvey.org/Installation.
    return '4.1.0';
  }
}

/**
 * SQLite specific install functions
 */
class DatabaseTasks_LimeSurveySync_sqlite extends DatabaseTasks_LimeSurveySync {

  protected $pdoDriver = 'sqlite';

  public function name() {
    $dbt = new DatabaseTasks_sqlite();
    return $dbt->name();
  }

  /**
   * Minimum engine version.
   *
   * @todo: consider upping to 3.6.8 in Drupal 8 to get SAVEPOINT support.
   */
  public function minimumVersion() {
    $dbt = new DatabaseTasks_sqlite();
    return $dbt->minimumVersion();
  }
}

/**
 * PostgreSQL specific install functions
 */
class DatabaseTasks_LimeSurveySync_pgsql extends DatabaseTasks_LimeSurveySync {
  // Copy of the includes/database/pgsql/install.inc
  protected $pdoDriver = 'pgsql';

  public function __construct() {
    $this->tasks[] = array(
      'function' => 'checkEncoding',
      'arguments' => array(),
    );
    $this->tasks[] = array(
      'function' => 'checkPHPVersion',
      'arguments' => array(),
    );
    $this->tasks[] = array(
      'function' => 'checkBinaryOutput',
      'arguments' => array(),
    );
    $this->tasks[] = array(
      'function' => 'initializeDatabase',
      'arguments' => array(),
    );
  }

  public function name() {
    $dbt = new DatabaseTasks_pgsql();
    return $dbt->name();
  }

  public function minimumVersion() {
    return '8.3';
  }

  /**
   * Check encoding is UTF8.
   */
  protected function checkEncoding() {
    try {
      if (db_query('SHOW server_encoding')->fetchField() == 'UTF8') {
        $this->pass(st('Database is encoded in UTF-8'));
      }
      else {
        $replacements = array(
          '%encoding' => 'UTF8',
          '%driver' => $this->name(),
          '!link' => '<a href="INSTALL.pgsql.txt">INSTALL.pgsql.txt</a>',
        );
        $text  = 'The %driver database must use %encoding encoding to work with Drupal.';
        $text .= 'Recreate the database with %encoding encoding. See !link for more details.';
        $this->fail(st($text, $replacements));
      }
    }
    catch (Exception $e) {
      $this->fail(st('Drupal could not determine the encoding of the database was set to UTF-8'));
    }
  }

  /**
   * Check PHP version.
   *
   * There are two bugs in PDO_pgsql affecting Drupal:
   *
   * - in versions < 5.2.7, PDO_pgsql refuses to insert an empty string into
   *   a NOT NULL BLOB column. See: http://bugs.php.net/bug.php?id=46249
   * - in versions < 5.2.11 and < 5.3.1 that prevents inserting integer values
   *   into numeric columns that exceed the PHP_INT_MAX value.
   *   See: http://bugs.php.net/bug.php?id=48924
   */
  function checkPHPVersion() {
    if (!version_compare(PHP_VERSION, '5.2.11', '>=') || (version_compare(PHP_VERSION, '5.3.0', '>=') && !version_compare(PHP_VERSION, '5.3.1', '>='))) {
      $this->fail(st('The version of PHP you are using has known issues with PostgreSQL. You need to upgrade PHP to 5.2.11, 5.3.1 or greater.'));
    }
  }

  /**
   * Check Binary Output.
   *
   * Unserializing does not work on Postgresql 9 when bytea_output is 'hex'.
   */
  function checkBinaryOutput() {
    // PostgreSQL < 9 doesn't support bytea_output, so verify we are running
    // at least PostgreSQL 9.
    $database_connection = Database::getConnection();
    if (version_compare($database_connection->version(), '9') >= 0) {
      if (!$this->checkBinaryOutputSuccess()) {
        // First try to alter the database. If it fails, raise an error telling
        // the user to do it themselves.
        $connection_options = $database_connection->getConnectionOptions();
        // It is safe to include the database name directly here, because this
        // code is only called when a connection to the database is already
        // established, thus the database name is guaranteed to be a correct
        // value.
        $query = "ALTER DATABASE \"" . $connection_options['database'] . "\" SET bytea_output = 'escape';";
        try {
          db_query($query);
        }
        catch (Exception $e) {
          // Ignore possible errors when the user doesn't have the necessary
          // privileges to ALTER the database.
        }

        // Close the database connection so that the configuration parameter
        // is applied to the current connection.
        db_close();

        // Recheck, if it fails, finally just rely on the end user to do the
        // right thing.
        if (!$this->checkBinaryOutputSuccess()) {
          $replacements = array(
            '%setting' => 'bytea_output',
            '%current_value' => 'hex',
            '%needed_value' => 'escape',
            '!query' => "<code>" . $query . "</code>",
          );
          $this->fail(st("The %setting setting is currently set to '%current_value', but needs to be '%needed_value'. Change this by running the following query: !query", $replacements));
        }
      }
    }
  }

  /**
   * Verify that a binary data roundtrip returns the original string.
   */
  protected function checkBinaryOutputSuccess() {
    $dbt = new DatabaseTasks_pgsql();
    return $dbt->checkBinaryOutputSuccess();
  }

  /**
   * Make PostgreSQL Drupal friendly.
   */
  function initializeDatabase() {
    // We create some functions using global names instead of prefixing them
    // like we do with table names. This is so that we don't double up if more
    // than one instance of Drupal is running on a single database. We therefore
    // avoid trying to create them again in that case.

    try {
      // Create functions.
      db_query('CREATE OR REPLACE FUNCTION "greatest"(numeric, numeric) RETURNS numeric AS
        \'SELECT CASE WHEN (($1 > $2) OR ($2 IS NULL)) THEN $1 ELSE $2 END;\'
        LANGUAGE \'sql\''
      );
      db_query('CREATE OR REPLACE FUNCTION "greatest"(numeric, numeric, numeric) RETURNS numeric AS
        \'SELECT greatest($1, greatest($2, $3));\'
        LANGUAGE \'sql\''
      );
      // Don't use {} around pg_proc table.
      if (!db_query("SELECT COUNT(*) FROM pg_proc WHERE proname = 'rand'")->fetchField()) {
        db_query('CREATE OR REPLACE FUNCTION "rand"() RETURNS float AS
          \'SELECT random();\'
          LANGUAGE \'sql\''
        );
      }

      db_query('CREATE OR REPLACE FUNCTION "substring_index"(text, text, integer) RETURNS text AS
        \'SELECT array_to_string((string_to_array($1, $2)) [1:$3], $2);\'
        LANGUAGE \'sql\''
      );

      // Using || to concatenate in Drupal is not recommeneded because there are
      // database drivers for Drupal that do not support the syntax, however
      // they do support CONCAT(item1, item2) which we can replicate in
      // PostgreSQL. PostgreSQL requires the function to be defined for each
      // different argument variation the function can handle.
      db_query('CREATE OR REPLACE FUNCTION "concat"(anynonarray, anynonarray) RETURNS text AS
        \'SELECT CAST($1 AS text) || CAST($2 AS text);\'
        LANGUAGE \'sql\'
      ');
      db_query('CREATE OR REPLACE FUNCTION "concat"(text, anynonarray) RETURNS text AS
        \'SELECT $1 || CAST($2 AS text);\'
        LANGUAGE \'sql\'
      ');
      db_query('CREATE OR REPLACE FUNCTION "concat"(anynonarray, text) RETURNS text AS
        \'SELECT CAST($1 AS text) || $2;\'
        LANGUAGE \'sql\'
      ');
      db_query('CREATE OR REPLACE FUNCTION "concat"(text, text) RETURNS text AS
        \'SELECT $1 || $2;\'
        LANGUAGE \'sql\'
      ');

      $this->pass(st('PostgreSQL has initialized itself.'));
    }
    catch (Exception $e) {
      $this->fail(st('Drupal could not be correctly setup with the existing database. Revise any errors.'));
    }
  }
}
