<?php
define('DRUSH_MODULESMITH_SMART', 1);

/**
 * Implements hook_drush_command().
 *
 * Register the modulesmith command(s)
 */
function modulesmith_drush_command() {
  $commands = array();

  $commands['modulesmith-forge'] = array(
    'callback' => 'drush_modulesmith_forge',
    'description' => 'Create a new module.',
    'aliases' => array('msf'),
    'examples' => array(
      'drush modulesmith-forge module_name' => 'Create a new module in sites/all/modules.',
    ),
    'arguments' => array(
      'module_name' => 'The module name to create.',
      'path' => DRUSH_MODULESMITH_SMART,
    ),
    'required-arguments' => 1,
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_CONFIGURATION,
  );

  return $commands;
}

/**
 * Create a module
 */
function drush_modulesmith_forge($module_name, $path = DRUSH_MODULESMITH_SMART) {
  if ($path == DRUSH_MODULESMITH_SMART) {
    $path_preferences = array(
      DRUPAL_ROOT . '/sites/all/modules/custom',
      DRUPAL_ROOT . '/sites/all/modules',
    );

    $install_profile = variable_get('install_profile', FALSE);
    if ($install_profile) {
      $path_preferences += array(
        DRUPAL_ROOT . '/profiles/' . $install_profile . '/modules/custom',
        DRUPAL_ROOT . '/profiles/' . $install_profile . '/modules',
      );
    }

    foreach ($path_preferences as $this_path) {
      if (file_exists($this_path) && is_dir($this_path)) {
        $path = $this_path;
        break;
      }
    }
  }

  $module_path = $path . '/' . $module_name;

  if (file_exists($module_path)) {
    drush_log('Module exists at: ' . $module_path, 'error');
    return FALSE;
  }

  drush_log('Path: ' . $module_path, 'ok');
  if (drush_confirm('Create module ' . $module_name . '?')) {
    mkdir($module_path, 0775, TRUE);

    $info = file_get_contents(__DIR__ . '/templates/info.tpl');
    $info = str_replace('$module_name', $module_name, $info);

    $module = file_get_contents(__DIR__ . '/templates/module.tpl');
    $module = str_replace('$module_name', $module_name, $module);

    file_put_contents(
      $module_path . '/' . $module_name . '.info',
      $info
    );
    file_put_contents(
      $module_path . '/' . $module_name . '.module',
      $module
    );
  }
}
