<?php
/**
 * @file
 * Drush integration for Tadaa!
 */

/**
 * Implements hook_drush_command().
 */
function tadaa_drush_command() {
  $items = array();

  $items['tadaa-status'] = array(
    'description' => dt('Checks the status of the current environment.'),
    'drupal dependencies' => array('tadaa'),
    'aliases' => array('tadaa'),
  );
  $items['tadaa-fix'] = array(
    'description' => dt('Fixes the current environment.'),
    'drupal dependencies' => array('tadaa'),
    'aliases' => array('tf'),
  );
  $items['tadaa-switch'] = array(
    'description' => dt('Switch environment.'),
    'arguments' => array(
      'environment' => dt('The machine name of the environment to switch to.'),
    ),
    'examples' => array(dt('Switch to development environment') => 'drush tadaa-switch development'),
    'drupal dependencies' => array('tadaa'),
    'aliases' => array('ts'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function tadaa_drush_help($section) {
  switch ($section) {
    case 'drush:tadaa':
      return dt('Checks which environment is currently being used, and its status.');

    case 'drush:tadaa-fix':
      return dt('Fixes the configuration for the current environment, by enabling/disabling modules that are in the wrong state, and changing the values of variables that are wrong.');

    case 'drush:tadaa-switch':
      return dt('Switches environment to the one you specify. All caches will be flushed afterwards.');
  }
}

/**
 * Callback function for drush tadaa.
 */
function drush_tadaa_status() {
  // Get the environments.
  $environments = variable_get('tadaa_environments', array());
  if (empty($environments)) {
    drush_set_error(dt('No environments has been configured. Go to admin/config/development/tadaa/configure/environments/add and add your first one.'));
    return;
  }

  // Get the current environment.
  $environment = variable_get('tadaa_environment');
  if (!$environment) {
    drush_set_error(dt('No environment has been set. Execute drush tadaa-switch ENVIRONMENT_NAME to select an environment. See drush help tadaa-switch for more information.'));
    return;
  }

  // Print the name of the current environment.
  drush_print(dt("Current environment: !environment.\n", array('!environment' => $environments[$environment])));

  // Get the configured modules, and build a table with one row per module and
  // its status,
  $modules = tadaa_get_modules($environment);
  if (!empty($modules)) {
    $module_rows = array(
      array(dt('Module'), dt('Configuration'), dt('Status')),
    );
    foreach ($modules as $module => $configuration) {
      // Get the real module name.
      $module_info = drupal_parse_info_file(drupal_get_path('module', $module) . '/' . $module . '.info');
      // Set human readable texts for the configuration.
      $configuration_text = $configuration ? t('Enable') : t('Disable');
      // Get the real status of the module.
      $real_status = module_exists($module);
      // Build the row.
      $module_rows[] = array(
        $module_info['name'],
        $configuration_text,
        $configuration != $real_status ? dt('Error') : dt('OK'),
      );
    }
  }

  // Get the configured variables, and build a table with one row per variable
  // and its status,
  $variables = tadaa_get_variables($environment);
  if (!empty($variables)) {
    $variable_rows = array(
      array(dt('Variable'), dt('Configuration'), dt('Status')),
    );
    foreach ($variables as $variable => $configuration) {
      // Get the real variable value.
      $real_value = variable_get($variable);
      // Build the row.
      $variable_rows[] = array(
        $variable,
        wordwrap(serialize($configuration), 45, '<br />', TRUE),
        $configuration != $real_value ? dt('Error') : dt('OK'),
      );
    }
  }

  // Calculate the width for each row.
  $widths = array();
  foreach ($module_rows as $row) {
    foreach ($row as $col => $contents) {
      $widths[$col] = isset($widths[$col]) ? (strlen($contents) > $widths[$col] ? strlen($contents) : $widths[$col]) : strlen($contents);
    }
  }
  foreach ($variable_rows as $row) {
    foreach ($row as $col => $contents) {
      $widths[$col] = isset($widths[$col]) ? (strlen($contents) > $widths[$col] ? strlen($contents) : $widths[$col]) : strlen($contents);
    }
  }

  // Print the tables.
  if (!empty($module_rows)) {
    drush_print_table($module_rows, TRUE, $widths);
  }
  if (!empty($variable_rows)) {
    drush_print_table($variable_rows, TRUE, $widths);
  }
}

/**
 * Callback function for drush tadaa-fix.
 */
function drush_tadaa_fix() {
  // Get the environments.
  $environments = variable_get('tadaa_environments', array());
  if (empty($environments)) {
    drush_set_error(dt('No environments has been configured. Go to admin/config/development/tadaa/configure/environments/add and add your first one.'));
    return;
  }

  // Get the current environment.
  $environment = variable_get('tadaa_environment');
  if (!$environment) {
    drush_set_error(dt('No environment has been set. Execute drush tadaa-switch ENVIRONMENT_NAME to select an environment. See drush help tadaa-switch for more information.'));
    return;
  }

  // Load the modules for the active environment, and build one array for those
  // that we should enable, and one for those that we should disable.
  $modules = tadaa_get_modules($environment);
  $enable = array();
  $disable = array();
  foreach ($modules as $module => $configuration) {
    if ($configuration != module_exists($module)) {
      if ($configuration) {
        // This module should be enabled.
        $enable[] = $module;
      }
      else {
        // This module should be disabled.
        $disable[] = $module;
      }
    }
  }

  // Load the variables for the active environment, and build one array for
  // those that we should change.
  $variables = tadaa_get_variables($environment);
  $change = array();
  foreach ($variables as $variable => $configuration) {
    if ($configuration != variable_get($variable)) {
      $change[$variable] = $configuration;
    }
  }

  // Bail out if everything is OK.
  if (empty($enable) && empty($disable) && empty($change)) {
    drush_print('Everything is OK, nothing to do.');
    return;
  }

  // Enable and disable the modules that were wrong.
  if (!empty($enable)) {
    module_enable($enable);
    foreach ($enable as $module) {
      // Get the real module name, and print a helpful information text.
      $module_info = drupal_parse_info_file(drupal_get_path('module', $module) . '/' . $module . '.info');
      drush_print(dt('Enabled !module.', array('!module' => $module_info['name'])));
    }
  }
  if (!empty($disable)) {
    module_disable($disable);
    foreach ($disable as $module) {
      // Get the real module name, and print a helpful information text.
      $module_info = drupal_parse_info_file(drupal_get_path('module', $module) . '/' . $module . '.info');
      drush_print(dt('Disabled !module.', array('!module' => $module_info['name'])));
    }
  }

  // Change the values for the variables that were wrong.
  foreach ($change as $variable => $configuration) {
    variable_set($variable, $configuration);
    drush_print(dt('Changed the value of !variable.', array('!variable' => $variable)));
  }
}

/**
 * Callback function for drush tadaa-switch.
 */
function drush_tadaa_switch($environment = NULL) {
  // Get the enivorment that the user has switched from. The enivorment that is
  // to be swtiched to is the first argument in this function.
  $old = variable_get('tadaa_environment', '');
  $new = $environment;

  // Verify the supplied argument.
  if (!$environment) {
    drush_set_error('Missing environment. See drush help tadaa-switch for more information.');
    return;
  }

  // Get the environments.
  $environments = variable_get('tadaa_environments', array());
  if (empty($environments)) {
    drush_set_error(dt('No environments has been configured. Go to admin/config/development/tadaa/configure/environments/add and add your first one.'));
    return;
  }

  // Verify that the specified environment exists.
  if (!isset($environments[$environment])) {
    drush_set_error(dt("!environment doesn't exist.", array('!environment' => $environment)));
    return;
  }

  // Don't do anything if the specified environment is the current environment.
  if ($environment == variable_get('tadaa_environment')) {
    drush_set_error(dt("!environment is the current environment, can't switch.", array('!environment' => $environments[$environment])));
    return;
  }

  // Make it possible for hooks to act on swtich between environments before
  // the alterations by tadaa is made.
  module_invoke_all('tadaa_pre_switch', $old, $new);

  // Get the configured module and variables.
  $modules = tadaa_get_modules($environment);
  $variables = tadaa_get_variables($environment);

  // Build an array of the modules that we should enable and one for those that
  // should be disabled.
  $enable = array();
  $disable = array();
  foreach ($modules as $module => $configuration) {
    if ($configuration != module_exists($module)) {
      if ($configuration) {
        $enable[] = $module;
      }
      else {
        $disable[] = $module;
      }
    }
  }

  // Enable and disable the appropriate modules.
  if (!empty($enable)) {
    module_enable($enable);
    foreach ($enable as $module) {
      // Get the real module name, and print a helpful information text.
      $module_info = drupal_parse_info_file(drupal_get_path('module', $module) . '/' . $module . '.info');
      drush_print(dt('Enabled !module.', array('!module' => $module_info['name'])));
    }
  }
  if (!empty($disable)) {
    module_disable($disable);
    foreach ($disable as $module) {
      // Get the real module name, and print a helpful information text.
      $module_info = drupal_parse_info_file(drupal_get_path('module', $module) . '/' . $module . '.info');
      drush_print(dt('Disabled !module.', array('!module' => $module_info['name'])));
    }
  }

  // Change the values of the variables.
  foreach ($variables as $variable => $configuration) {
    if ($configuration != variable_get($variable)) {
      variable_set($variable, $configuration);
      drush_print(dt('Changed the value of !variable.', array('!variable' => $variable)));
    }
  }

  // Finally, set the new environment, flush the cache, and print a helpful
  // message.
  variable_set('tadaa_environment', $environment);
  drupal_flush_all_caches();

  // Make it possible for hooks to act on swtich between environments after
  // the alterations by tadaa is made.
  module_invoke_all('tadaa_post_switch', $old, $new);

  drush_print("\n" . dt('Successfully switched to !environment.', array('!environment' => $environments[$environment])));
}
