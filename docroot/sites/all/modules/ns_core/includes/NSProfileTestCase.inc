<?php
/**
 * @file
 * This file contains a test case that can be extended if you want to test your profile.
 */

/**
 * This test case works exactly as the DruaplWebTestCase with one very important difference:
 * It does not install an individual install of Drupal, it copies the current installation
 * by cloning the existing tables in the current setup, and then runs the test on the cloned tables.
 * The test removes the tables after each test and clones the base tables again.
 * This means the setup is exactly as it is at the point when the test is run.
 * The idea is that you can install a particular profile of choice and then let your profile-specific tests
 * extend this class. You can then test any functionality provided by your current setup safely. This is very useful since:
 *  - It is hard to get tests to install complex installation profiles.
 *  - Installing complex installation profiles takes time.
 */
abstract class NSProfileTestCase extends DrupalWebTestCase {

  /**
   * This is a modified setUp(), mostly copying the things that is being done
   * by the the setUp() in DrupalWebTestCase, but it clones the tables instead of installing a new
   * drupal installation.
   */
  protected function setUp() {
    global $user, $language, $conf;
    // Generate a temporary prefixed database to ensure that tests have a clean starting point.
    $this->databasePrefix = 'simpletest' . mt_rand(1000, 1000000);
    db_update('simpletest_test_id')
      ->fields(array('last_prefix' => $this->databasePrefix))
      ->condition('test_id', $this->testId)
      ->execute();
    // Store a database dump of the current site, using the credentials from the current connection.
    $this->backupDB($this->databasePrefix);
    // Clone the current connection and replace the current prefix.
    $connection_info = Database::getConnectionInfo('default');
    Database::renameConnection('default', 'simpletest_original_default');
    foreach ($connection_info as $target => $value) {
      $connection_info[$target]['prefix'] = array(
        'default' => $value['prefix']['default'] . $this->databasePrefix,
      );
    }
    Database::addConnectionInfo('default', 'default', $connection_info['default']);
    // Store necessary current values before switching to prefixed database.
    $this->originalLanguage = $language;
    $this->originalLanguageDefault = variable_get('language_default');
    $this->originalFileDirectory = variable_get('file_public_path', conf_path() . '/files');
    $this->privatePath = variable_get('file_private_path', conf_path() . '/private/files');
    $this->originalProfile = drupal_get_profile();
    $clean_url_original = variable_get('clean_url', 0);

    $clean_url_original = variable_get('clean_url', 0);

    // Set to English to prevent exceptions from utf8_truncate() from t()
    // during install if the current language is not 'en'.
    // The following array/object conversion is copied from language_default().
    $language = (object) array('language' => 'en', 'name' => 'English', 'native' => 'English', 'direction' => 0, 'enabled' => 1, 'plurals' => 0, 'formula' => '', 'domain' => '', 'prefix' => '', 'weight' => 0, 'javascript' => '');

    // Save and clean shutdown callbacks array because it static cached and
    // will be changed by the test run. If we don't, then it will contain
    // callbacks from both environments. So testing environment will try
    // to call handlers from original environment.
    $callbacks = &drupal_register_shutdown_function();
    $this->originalShutdownCallbacks = $callbacks;
    $callbacks = array();

    // Create test directory ahead of installation so fatal errors and debug
    // information can be logged during installation process.
    // Use temporary files directory with the same prefix as the database.
    $public_files_directory  = $this->originalFileDirectory . '/simpletest/' . substr($this->databasePrefix, 10);
    $private_files_directory = $public_files_directory . '/private';
    $temp_files_directory    = $private_files_directory . '/temp';

    // Create the directories
    file_prepare_directory($public_files_directory, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);
    file_prepare_directory($private_files_directory, FILE_CREATE_DIRECTORY);
    file_prepare_directory($temp_files_directory, FILE_CREATE_DIRECTORY);
    $this->generatedTestFiles = FALSE;

    // Log fatal errors.
    ini_set('log_errors', 1);
    ini_set('error_log', $public_files_directory . '/error.log');

    // Reset all statics and variables to perform tests in a clean environment.
    $conf = array();
    drupal_static_reset();

    // Set the test information for use in other parts of Drupal.
    $test_info = &$GLOBALS['drupal_test_info'];
    $test_info['test_run_id'] = $this->databasePrefix;
    $test_info['in_child_site'] = FALSE;

    // Set path variables.
    variable_set('file_public_path', $public_files_directory);
    variable_set('file_private_path', $private_files_directory);
    variable_set('file_temporary_path', $temp_files_directory);

    // Log in with a clean $user.
    $this->originalUser = $user;
    drupal_save_session(FALSE);
    $user = user_load(1);

    // Restore necessary variables.
    variable_set('install_task', 'done');
    variable_set('clean_url', $clean_url_original);
    variable_set('site_mail', 'simpletest@example.com');
    variable_set('date_default_timezone', date_default_timezone_get());
    // Set up English language.
    unset($GLOBALS['conf']['language_default']);
    $language = language_default();

    // Use the test mail class instead of the default mail handler class.
    variable_set('mail_system', array('default-system' => 'TestingMailSystem'));

    drupal_set_time_limit($this->timeLimit);
    $this->setup = TRUE;
  }

  /**
   * Backup the current DB based on the current installed schema.
   */
  protected function backupDB($prefix) {
    // Get the current schema, order it by table name.
    $schema = drupal_get_schema();
    ksort($schema);
    // Export all the tables in the schema.
    foreach ($schema as $table => $data) {
      $new_table = $prefix . $table;
      db_create_table($new_table, $data);
      // Skip cache, sessions and watchdog.
      if (substr($table, 0, 5) == 'cache' || $table == 'sessions' || $table == 'watchdog') {
        continue;
      }
      // Copy table data.
      db_query("INSERT INTO {$new_table} SELECT * FROM {$table}");
    }
  }

}