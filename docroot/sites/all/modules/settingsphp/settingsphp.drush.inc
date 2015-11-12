<?php

/**
 * Implements hook_drush_command().
 */
function settingsphp_drush_command() {
  $items = array();
  $items['settingsphp-generate'] = array(
    'description' => 'Generates a settings.php file.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_ROOT,
    'options' => array(
      'db-url' => array(
        'description' => 'Database connection string.',
        'example-value' => 'mysql://user:pass@host/db',
      ),
      'db-prefix' => array(
        'description' => 'Database table prefix.',
        'example-value' => 'myprefix_',
      ),
      'db-collation' => array(
        'description' => 'Database collation. Defaults to utf8_general_ci.',
        'example-value' => 'utf8_general_ci',
      ),
    ),
  );
  return $items;
}

/**
 * Validation callback for 'settingsphp-generate' command.
 */
function drush_settingsphp_generate_validate() {
  global $databases;

  $db_url = drush_get_option('db-url', FALSE);
  if ($db_url) {
    $databases = drush_sitealias_convert_db_from_db_url($db_url);
  }
  else {
    $self = drush_sitealias_get_record('@self');
    // If not invoked on an alias and there's no db-url, we can't continue.
    if (!$self) {
      if (!$db_url) {
        return drush_set_error('SETTINGSPHP_MISSING_ARGS', dt('It is mandatory to specify an alias with a database record, or pass the --db-url option.'));
      }
    }
    // Otherwise, pick the databases from the alias, if available.
    elseif (empty($self['databases'])) {
      return drush_set_error('SETTINGSPHP_MISSING_ARGS', dt('Missing option --db-url and the site alias has no databases configured.'));
    }
    $databases = $self['databases'];
  }

  // Validate databases.
  require_once DRUPAL_ROOT . '/includes/file.inc';
  require_once DRUPAL_ROOT . '/includes/install.inc';

  $drivers = array_keys(drupal_get_database_types());
  foreach ($databases as $key => $targets) {
    foreach ($targets as $target => $db_spec) {
      if (!in_array($db_spec['driver'], $drivers)) {
        return drush_set_error('SETTINGSPHP_UNSUPPORTED_DB_DRIVER', dt('Unsupported db driver !driver.', array('!driver' => $db_spec['driver'])));
      }
      try {
        Database::setActiveConnection($key);
        Database::getConnection($target, $key);
      }
      catch (Exception $e) {
        return drush_set_error('SETTINGSPHP_CONNECTION_FAILED', dt('Failed to connect to database server. Error message: !error', array('!error' => $e->getMessage())));
      }
    }
  }
}

/**
 * Command callback for 'settingsphp-generate' command.
 */
function drush_settingsphp_generate() {
  // DRUSH_BOOTSTAP_DRUPAL_SITE phase validation does
  // fake the necessary HTTP headers that Drupal needs.
  drush_bootstrap_validate(DRUSH_BOOTSTRAP_DRUPAL_SITE);

  // Check for settings.php existence.
  $settingsphp_path = DRUPAL_ROOT . '/' . conf_path(FALSE, TRUE) . '/settings.php';
  if (file_exists($settingsphp_path)) {
    if (!drush_confirm(dt('!path already exists. Do you want to overwrite it?', array('!path' => $settingsphp_path)))) {
      drush_log(dt('Aborting'), 'ok');
      return;
    }
  }

  global $databases;

  // Prepare settings.
  $settings['databases'] = array(
    'value' => $databases,
    'required' => TRUE,
  );
  $settings['drupal_hash_salt'] = array(
    'value' => drupal_random_key(),
    'required' => TRUE,
  );
  // Allow other commandfiles to alter the settings.
  drush_command_invoke_all_ref('settingsphp_generate_alter', $settings);

  // Generate settings.php.
  require_once DRUPAL_ROOT . '/includes/install.inc';
  drupal_rewrite_settings($settings);

  drush_log(dt('!path generated. Go and set permissions!.', array('!path' => $settingsphp_path)), 'ok');
}

