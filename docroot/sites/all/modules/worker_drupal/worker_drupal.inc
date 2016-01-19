<?php
/**
 * @file
 * Common functions used by Drupal plugins.
 *
 * @author Jimmy Berry ("boombatower", http://drupal.org/user/214218)
 */

/**
 * Install Drupal on the primary database.
 *
 * @param $properties
 *   Associative array of properties defining the job.
 * @return
 *   TRUE if successful, otherwise FALSE.
 *
 * @see worker_perform() for a description of the properties array.
 */
function worker_drupal_install(array $properties) {
  worker_log('Prepare primary database.');
  if (!($connection = worker_database('drupal', $properties['database']))) {
    worker_log('Failed to prepare primary database.');
    return FALSE;
  }

  // Store database connection for later use.
  drupal_static('worker_drupal_database', $connection); // @TODO Remove once variable_set no longer needs it.

  worker_log('Install Drupal ' . $properties['version'] . '.');

  // Copy the default.settings.php file as settings.php
  $default_settings = JOB_ROOT . '/sites/default/default.settings.php';
  $settings = JOB_ROOT . '/sites/default/settings.php';
  copy($default_settings, $settings);

  // Invoke either the Drupal 6 installation function or the Drupal 7 and above
  // installation function and wait until after chmod() before checking result.
  $function = 'worker_drupal_install_' . min($properties['version'], 7);
  if (!$function($properties)) {
    return FALSE;
  }

  // Since Drupal installation has completed then go ahead and override/set the
  // specified list of variables.
  if (!worker_drupal_install_variables($properties['variables'])) {
    return FALSE;
  }

  return TRUE;
}

/**
 * Perform Drupal 7 installation using non-interactive script.
 *
 * For Drupal 7 and above we can use the non-interactive installation method
 * which requires a file to be generated with the site configuration
 * information and placed in the root of the Drupal install.
 *
 * @param $properties
 *   Associative array of properties defining the job.
 * @return
 *   TRUE if successful, otherwise FALSE.
 *
 * @see worker_perform() for a description of the properties array.
 */
function worker_drupal_install_7(array $properties) {
  // Generate array of settings information required for installation and dump
  // array in installation script template to a file in the root of job site.
  $settings = array(
    'parameters' => array(
      'profile' => 'minimal',
      'locale' => 'en',
    ),
    'forms' => array(
      'install_settings_form' => array(
        'mysql' => worker_database_info('drupal', $properties['database']),
      ),
      'install_configure_form' => array(
        'site_name' => 'Worker generated installation',
        'site_mail' => 'worker@example.com',
        'account' => array(
          'name' => 'admin',
          'mail' => 'admin@example.com',
          'pass' => array(
            'pass1' => worker_drupal_password(),
            'pass2' => worker_drupal_password(),
          ),
        ),
        'update_status_module' => array(
          1 => TRUE,
          2 => TRUE,
        ),
        'clean_url' => TRUE,
      ),
    ),
  );
  $out = <<<'EOT'
<?php
define('MAINTENANCE_MODE', 'install');
define('DRUPAL_ROOT', getcwd());
require DRUPAL_ROOT . '/includes/install.core.inc';
$settings = [settings];
install_drupal($settings);

EOT;
  $out = str_replace('[settings]', var_export($settings, TRUE), $out);
  file_put_contents(JOB_ROOT . '/install.worker.php', $out);

  // To compelete the installation the generated file needs to be invoked
  // using the URL through which the site will be accessed.
  $browser = new Browser();
  $result = $browser->get(JOB_URL . '/install.worker.php');
  if ($result['code'] != 200 || $result['content']) {
    worker_log('> Failed to install Drupal.');
    worker_drupal_install_error($browser);
    return FALSE;
  }
  return TRUE;
}

/**
 * Perform Drupal 6 installation using Browser class.
 *
 * @param $properties
 *   Associative array of properties defining the job.
 * @return
 *   TRUE if successful, otherwise FALSE.
 *
 * @see worker_perform() for a description of the properties array.
 */
function worker_drupal_install_6(array $properties) {
  $browser = new Browser();

  // Step: Select an installation profile.
  // Step: Choose language.
  $result = $browser->get(JOB_URL . '/install.php?profile=default&locale=en');
  if ($result['code'] != 200) {
    worker_log('> Failed to start installation process.');
    worker_drupal_install_error($browser);
    return FALSE;
  }

  // Step: Database configuration.
  $database = worker_database_info('drupal', $properties['database']);
  $post = array();
  if ($browser->getPage()->xpath('//input[@name="driver"]')) {
    $post['driver'] = $properties['database'];
  }
  $post['db_path'] = $database['database'];
  $post['db_user'] = $database['username'];
  $post['db_pass'] = $database['password'];
  $post['db_host'] = $database['host'];
  $result = $browser->post(NULL, $post, 'Save and continue');
  if ($result['code'] != 302) {
    worker_log('> Failed to submit database information.');
    worker_drupal_install_error($browser);
    return FALSE;
  }

  // Step: Site configuration.
  $post = array();
  $post['site_name'] = 'Conduit generated installation';
  $post['site_mail'] = 'worker@example.com';
  $post['account[name]'] = 'admin';
  $post['account[mail]'] = 'admin@example.com';
  $post['account[pass][pass1]'] = $post['account[pass][pass2]'] = worker_drupal_password();
  $post['update_status_module[1]'] = FALSE;
  $result = $browser->post(NULL, $post, 'Save and continue');
  if ($result['code'] != 200 || !$browser->getPage()->xpath('//h2[text()="Drupal installation complete"]')) {
    worker_log('> Failed to complete site configuration.');
    worker_drupal_install_error($browser);
    return FALSE;
  }
  return TRUE;
}

/**
 * Attempt to scrape an error message from the current browser page.
 *
 * If found the message will be printed to the job log.
 *
 * @param $browser
 *   Browser class object after performing a request.
 */
function worker_drupal_install_error(Browser $browser) {
  if ($page = $browser->getPage()) {
    // Find the first list item in the error messages div or if only one error
    // and thus no list then just grab the only message.
    if (($error = current($page->xpath('//div[@class="messages error"]//li'))) ||
      ($error = current($page->xpath('//div[@class="messages error"]')))) {
      // Remove any HTML tags the message may contain.
      worker_log('> ' . trim(preg_replace('/^Error message/m', '', strip_tags((string) $error->asXML()))));
    }
    else {
      // Print the raw output to the log, usually a fatal error message.
      worker_log('> ' . trim($browser->getContent()));
    }
  }
}

/**
 * Set a list of Drupal variables.
 *
 * @param $variables
 *   List of Drupal variables.
 * @return
 *   TRUE if successful, otherwise FALSE.
 */
function worker_drupal_install_variables(array $variables) {
  worker_log('Set Drupal variable(s).');
  $success = TRUE;
  foreach ($variables as $name => $value) {
    // If the variable was not set successfully then log the event and set
    // success to false, but continue so that if the variable set had an error
    // for some reason other then database connectivity that may be noticed.
    if (!worker_drupal_variable_set($name, $value)) {
      worker_log('> Failed to set variable ' . $name . '.');
      $success = FALSE;
    }
  }
  if (!$success) worker_log('Failed to set Drupal variable(s).');
  return $success;
}

/**
 * Set a Drupal variable.
 *
 * @param $name
 *   Name of the variable.
 * @param $value
 *   Value of the variable.
 * @return
 *   TRUE if successful, otherwise FALSE.
 */
function worker_drupal_variable_set($name, $value) {
  // Once http://drupal.org/node/664452 has been fixed then remove this. Also
  // note this only clears the Drupal 7 style variables cache.
  if (!is_scalar($value)) {
    $connection = drupal_static('worker_drupal_database');
    $connection->delete('cache_bootstrap')->condition('cid', 'variables')->execute();
    return (bool) $connection->merge('variable')->key(array('name' => $name))->fields(array('value' => serialize($value)))->execute();
  }

  // Escape name and value arguments.
  $name = escapeshellarg($name);
  $value = escapeshellarg($value);

  // Invoke drush vset from within the job directory.
  worker_chdir();
  $success = worker_execute("drush vset --always-set $name $value");
  chdir(DRUPAL_ROOT);
  return $success;
}

/**
 * Get the admin password for the Drupal installation.
 *
 * @return
 *   The admin password for the Drupal installation.
 */
function worker_drupal_password() {
  if (!($password = &drupal_static(__FUNCTION__))) {
    $password = sha1(drupal_random_bytes(64));
  }
  return $password;
}

/**
 * Enable the module or modules on the job site.
 *
 * @param $modules
 *   A single module or list of modules.
 * @param $version
 *   Drupal core version.
 * @return
 *  TRUE if successful, otherwise FALSE.
 */
function worker_drupal_module_enable($modules, $version) {
  // If a single module was specified then create an array.
  if (!is_array($modules)) {
    $modules = array($modules);
  }

  worker_chdir();
  $modules = implode(' ', $modules);
  worker_log("Enable module(s): $modules.");
  if (!($success = worker_execute("drush --yes en $modules"))) {
    worker_log("Failed to enable module(s): $modules.");
  }
  chdir(DRUPAL_ROOT);
  return $success;
}

/**
 * Get the path to a module relative to the JOB_ROOT.
 *
 * @param $module
 *   Name of the module.
 * @return
 *   Relative path to the module, otherwise FALSE.
 */
function worker_drupal_module_path($module) {
  $modules = &drupal_static(__FUNCTION__);

  // If the module paths have not been loaded the scan the job directory for
  // all modules and store the directories in which they are found.
  if (!$modules) {
    worker_log('Scan for modules.');
    foreach (array('modules', 'sites/all', 'sites/default') as $directory) {
      $files = file_scan_directory(JOB_ROOT . '/' . $directory, '/\.module$/m');
      foreach ($files as $file) {
        $modules[$file->name] = str_replace(array(JOB_ROOT . '/', '/' . $file->filename), '', $file->uri);
      }
    }
    worker_log('> Found ' . count($modules) . '.');
  }

  return !empty($modules[$module]) ? $modules[$module] : FALSE;
}
